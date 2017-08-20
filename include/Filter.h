#pragma once

#include <boost/phoenix/core/argument.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/function.hpp>
#include <type_traits>
#include <functional>

#include <ID.h>

namespace filter {
  const boost::phoenix::expression::argument<1>::type e0 = {};
  const boost::phoenix::expression::argument<2>::type e1 = {};
  const boost::phoenix::expression::argument<3>::type e2 = {};
  const boost::phoenix::expression::argument<4>::type e3 = {};
  const boost::phoenix::expression::argument<5>::type e4 = {};
  const boost::phoenix::expression::argument<6>::type e5 = {};
  const boost::phoenix::expression::argument<7>::type e6 = {};
  const boost::phoenix::expression::argument<8>::type e7 = {};
  
  struct uncertainty_impl {
    
    template<typename Attribute>
    auto operator()(const Attribute& attr) const -> decltype(attr.uncertainty()){
      return attr.uncertainty();
    }
  };

  extern boost::phoenix::function<uncertainty_impl> uncertainty;

  struct value_impl {
    
    template<typename Attribute>
    auto operator()(const Attribute& attr) const -> decltype(attr.value()){
      return attr.value();
    }
  };

  extern boost::phoenix::function<value_impl> value;
  
  struct norm_impl {
    
    template<typename Attribute>
    auto operator()(const Attribute& attr) const -> decltype(attr.norm()){
      return attr.norm();
    }
  };

  extern boost::phoenix::function<norm_impl> norm;
}

struct PseudoAttr {};

union EventPlaceholder{
	struct {
  	uint8_t attr : 8;
    uint8_t num  : 3;
 	};
  uint16_t data;
};

union FilterOp{
	struct {
		uint8_t code      : 7;
		uint8_t constArg  : 1;
	};
	uint8_t data;
};

template<typename Serializer>
struct FilterEvent {
  Serializer& mS;
	EventPlaceholder mEvent;

  FilterEvent(uint8_t num, Serializer& s) : mS(s) {
    mEvent.num=num;
  }

  template<typename AttrID>
  FilterEvent& operator[](AttrID id){
    mEvent.attr = AttrID::value();
    return *this;
  }

  /** \todo implement **/
  FilterEvent& uncertainty() { }
  /** \todo implement **/
  FilterEvent& value() { }
  /** \todo implement **/
  FilterEvent& norm() { }
};

template<typename Serializer, typename Attr = FilterEvent<Serializer>>
struct FilterPredicate {
  using Event = FilterEvent<Serializer>;
  Serializer& mS;
  FilterOp mOp;
  const Event mE0;
  const Attr mAttr;
  FilterPredicate(id::filterOp::ID op, const Event& e0, const Attr& attr) 
    : mS(e0.mS), mE0(e0), mAttr(attr) {

    mOp.code = op;
    mOp.constArg = std::is_same<Attr, Event>::type::value?0:1;
  }
};


template<typename T, typename A0, typename A1>
FilterPredicate<T, A1> operator&&(const FilterPredicate<T, A0>& a, const FilterPredicate<T, A1>& b) {
  a.mS << a << (id::filterOp::ID)id::filterOp::AND::value;
  return b;
}

template<typename T, typename A0, typename A1>
FilterPredicate<T, A1> operator||(const FilterPredicate<T, A0>& a, const FilterPredicate<T, A1>& b) {
  a.mS << a << (id::filterOp::ID)id::filterOp::OR::value;
  return b;
}

template<typename Serializer, typename Attribute>
FilterPredicate<Serializer, Attribute> operator>(FilterEvent<Serializer> a, const Attribute& attr){
  return FilterPredicate<Serializer, Attribute>(id::filterOp::GT::value, a, attr);
}

template<typename Serializer, typename Attribute>
FilterPredicate<Serializer, Attribute> operator>=(FilterEvent<Serializer> a, const Attribute& b){
  return FilterPredicate<Serializer, Attribute>(id::filterOp::GE::value, a, b);
}

template<typename Serializer, typename Attribute>
FilterPredicate<Serializer, Attribute> operator<(FilterEvent<Serializer> a, const Attribute& b){
  return FilterPredicate<Serializer, Attribute>(id::filterOp::LT::value, a, b);
}

template<typename Serializer, typename Attribute>
FilterPredicate<Serializer, Attribute> operator<=(FilterEvent<Serializer> a, const Attribute& b){
  return FilterPredicate<Serializer, Attribute>(id::filterOp::LE::value, a, b);
}

template<typename Serializer, typename Attribute>
FilterPredicate<Serializer, Attribute> operator==(FilterEvent<Serializer> a, const Attribute& b){
  return FilterPredicate<Serializer, Attribute>(id::filterOp::EQ::value, a, b);
}

template<typename Serializer, typename Attribute>
FilterPredicate<Serializer, Attribute> operator!=(FilterEvent<Serializer> a, const Attribute& b){
  return FilterPredicate<Serializer, Attribute>(id::filterOp::NE::value, a, b);
}

template<typename Serializer>
Serializer& operator<<(Serializer& s, const FilterEvent<Serializer>& e) {
  return s << e.mEvent.data;
}

template<typename Serializer, typename  T>
Serializer& operator<<(Serializer& s, const FilterPredicate<Serializer, T>& p) {
    return s << p.mE0 << p.mOp.data << p.mAttr;
}
