#include <MetaFactory.h>
#include <MetaValueImplementation.h>
#include <ValueElement.h>

#include <boost/mpl/list.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/for_each.hpp>
#include <type_traits>
#include <utility>

using namespace std;

template<typename... Types>
class Conversions {
  private:
    template<typename A>
    struct appendPair{
      template<typename List, typename B>
      struct apply{
        using converter = Converter<A,B>;
        using extendedList = typename boost::mpl::push_front<List, converter>::type;
        using same = typename std::is_same<A,B>::type;
        using type = typename boost::mpl::if_<same, List, extendedList>::type;
      };
    };

    template<typename Input>
    struct appendList{
      template<typename List, typename A>
        struct apply{
          using func = appendPair<A>;
          using type = typename boost::mpl::fold<Input, List, func>::type;
        };
    };

    class InsertionHelper{
      private:
        MetaFactoryImplementation& mFactory;
      public:
        InsertionHelper(MetaFactoryImplementation& factory) : mFactory(factory) {}
    
        template<typename Conversion>
        void operator()(Conversion& c){
          mFactory.insert(c);
        }
    };

  public:

    using Input = typename boost::mpl::list<Types...>::type;
    using func = appendList<Input>;
    using type = typename boost::mpl::fold<Input, boost::mpl::list<>, func>::type;

    static void insert(MetaFactoryImplementation& factory){
      InsertionHelper h(factory);
      boost::mpl::for_each< type >(h);
    }
};

MetaFactoryImplementation::MetaFactoryImplementation() : 
  creators{
    {id::type::Bool  ::value(), &MetaValueImplementation<bool    >::factoryCreate},
    {id::type::UInt8 ::value(), &MetaValueImplementation<uint8_t >::factoryCreate},
    {id::type::UInt16::value(), &MetaValueImplementation<uint16_t>::factoryCreate},
    {id::type::UInt32::value(), &MetaValueImplementation<uint32_t>::factoryCreate},
    {id::type::UInt64::value(), &MetaValueImplementation<uint64_t>::factoryCreate},
    {id::type::Int8  ::value(), &MetaValueImplementation<int8_t  >::factoryCreate},
    {id::type::Int16 ::value(), &MetaValueImplementation<int16_t >::factoryCreate},
    {id::type::Int32 ::value(), &MetaValueImplementation<int32_t >::factoryCreate},
    {id::type::Int64 ::value(), &MetaValueImplementation<int64_t >::factoryCreate},
    {id::type::Float ::value(), &MetaValueImplementation<float   >::factoryCreate},
    {id::type::Double::value(), &MetaValueImplementation<double  >::factoryCreate},
  }{ 
    Conversions<bool,
                uint8_t,
                uint16_t,
                uint32_t,
                uint64_t,
                int8_t,
                int16_t,
                int32_t,
                int64_t,
                float,
                double>::insert(*this);
}

MetaFactoryImplementation::~MetaFactoryImplementation() {
}

MetaValue MetaFactoryImplementation::create(const ValueType& type) const {
  auto iter = creators.find(type.typeId());
  if(iter!=creators.end())
    return MetaValue(iter->second(type.rows(), type.cols(), type.hasUncertainty()));
  else
    return MetaValue();
}

MetaValue MetaFactoryImplementation::create(id::type::ID id, std::size_t rows, std::size_t cols, bool u) const {
  auto iter = creators.find(id);
  if(iter!=creators.end())
    return MetaValue(iter->second(rows, cols, u));
  else
    return MetaValue();
}

MetaValue MetaFactoryImplementation::convert(const ValueType& type, const MetaValue& value) const {
  if(value.typeId() == type.typeId())
    return value;
  ConverterKey key = {value.typeId(), type.typeId()};
  auto converter = converters.find(key);
  if(converter==converters.end())
    return MetaValue();
  MetaValue temp = create(type);
  converter->second(*value.implementation(), *temp.implementation());
  return temp;
}

MetaValue MetaFactoryImplementation::create(
	initializer_list<initializer_list<ValueElement<double>>> l, id::type::ID id) const{
	if(l.size()==0 || l.begin()->size()==0)
		return MetaValue();

  MetaValue temp = create(id, l.size(), l.begin()->size(), true);
  
	unsigned int i=0;
  for(const auto& col : l) {
		unsigned int j=0;
		for(const auto& elem : col)
    	temp.set(i, j++, elem);
		i++;
	}
   return temp;
}

MetaAttribute MetaFactoryImplementation::create(const AttributeType& type) const {
	MetaAttribute attr(type.id());
	attr.scale() = type.scale();
	attr.unit()  = type.unit();
	attr.value() = create(type.value());
	return attr;
}

MetaEvent MetaFactoryImplementation::create(const EventType& type) const {
	MetaEvent event;
	for(const auto& attrType : type)
		event.add(create(attrType));
	return event;
}

MetaAttribute MetaFactoryImplementation::convert(const AttributeType& type, const MetaAttribute& attr) const {
	if(type.unit() != attr.unit() || type.id() != attr.id())
		return MetaAttribute();
	MetaAttribute newAttr(attr.id());
	newAttr.unit()  = attr.unit();
	newAttr.value() = convert(type.value(), attr.value());
	newAttr.scale() = type.scale();
	newAttr.value() *= newAttr.scale() / attr.scale();
	return newAttr;
}

MetaEvent MetaFactoryImplementation::convert(const EventType& type, const MetaEvent& event) const {
	MetaEvent newEvent;
	EventType::const_iterator typeIter = type.begin();
	for(const MetaAttribute& attr : event) {
		MetaAttribute newAttr = convert(*typeIter++, attr);
		if(newAttr != MetaAttribute())
			newEvent.add(newAttr);
		else
			return MetaEvent();
	}
	return newEvent;
}

template class Singleton<MetaFactoryImplementation>;
