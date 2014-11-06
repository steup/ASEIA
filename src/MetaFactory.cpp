#include <MetaFactory.h>
#include <MetaValueImplementation.h>

#include <boost/mpl/list.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/for_each.hpp>
#include <type_traits>


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
    Conversions<uint8_t,
                uint16_t,
                uint32_t,
                uint64_t,
                int8_t,
                int16_t,
                int32_t,
                int64_t,
                float,
                double,
                bool>::insert(*this);
}

MetaFactoryImplementation::~MetaFactoryImplementation() {
}

MetaValue MetaFactoryImplementation::create(const ValueType& type) const {
  auto iter = creators.find(type.typeId());
  if(iter!=creators.end())
    return MetaValue(iter->second(type.n(), type.hasUncertainty()));
  else
    return MetaValue();
}

MetaValue MetaFactoryImplementation::create(id::type::ID id, std::size_t n, bool u) const {
  auto iter = creators.find(id);
  if(iter!=creators.end())
    return MetaValue(iter->second(n, u));
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
  MetaValueBaseImplementation& converted = converter->second(value.implementation());
 return MetaValue(converted);
}

template class Singleton<MetaFactoryImplementation>;
