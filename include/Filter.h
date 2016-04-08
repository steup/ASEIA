#pragma once

#include <boost/phoenix/core/argument.hpp>
#include <boost/phoenix/operator.hpp>

namespace filter {
  const boost::phoenix::expression::argument<1>::type e0 = {};
  const boost::phoenix::expression::argument<2>::type e1 = {};
  const boost::phoenix::expression::argument<3>::type e2 = {};
}

struct PseudoAttr {};

template<typename Serializer>
struct FilterEvent {
  Serializer* mS = nullptr;
  union Event{
    struct {
      uint8_t attr : 5;
      uint8_t num  : 3;
    };
    uint8_t data;
  } mEvent;


  FilterEvent(uint8_t num) {
    mEvent.num=num;
  }

  FilterEvent(uint8_t num, Serializer& s) : mS(&s) {
    mEvent.num=num;
  }

  template<typename AttrID>
  FilterEvent& operator[](AttrID id){
    mEvent.attr = AttrID::value();
    return *this;
  }
};

template<typename Serializer, typename Attr = PseudoAttr>
struct FilterPredicate {
  Serializer* mS = nullptr;
  union Op{
    struct {
      uint8_t code      : 7;
      uint8_t constArg  : 1;
    };
    uint8_t data;
  } mOp;
  using Event = FilterEvent<Serializer>;
  const Event& mE0;
  const Event* mE1 = nullptr;
  const Attr* mAttr = nullptr;
  FilterPredicate(id::filterOp::ID op, Event e0, Event e1) : mE0(e0), mE1(&e1) {
    mOp.code = op;
    mOp.constArg = 0;
    mS = e0.mS;
  }

  FilterPredicate(id::filterOp::ID op, Event e0, const Attr& attr) : mE0(e0), mAttr(&attr) {
    mOp.code = op;
    mOp.constArg = 1;
    mS = e0.mS;
  }
};

template<typename T, typename A0, typename A1>
FilterPredicate<T, A1> operator&&(const FilterPredicate<T, A0>& a, const FilterPredicate<T, A1>& b) {
  (*a.mS) << a << (id::filterOp::ID)id::filterOp::AND::value;
  auto temp = b;
  temp.mS = a.mS;
  return temp;
}

template<typename T, typename A0, typename A1>
FilterPredicate<T, A1> operator||(const FilterPredicate<T, A0>& a, const FilterPredicate<T, A1>& b) {
  (*a.mS) << a << (id::filterOp::ID)id::filterOp::OR::value;
  auto temp = b;
  temp.mS = a.mS;
  return temp;
}


template<typename Serializer>
FilterPredicate<Serializer> operator>(FilterEvent<Serializer> a, FilterEvent<Serializer> b){
  return FilterPredicate<Serializer>(id::filterOp::GT::value, a, b);
}

template<typename Serializer>
FilterPredicate<Serializer> operator>=(FilterEvent<Serializer> a, FilterEvent<Serializer> b){
  return FilterPredicate<Serializer>(id::filterOp::GE::value, a, b);
}

template<typename Serializer>
FilterPredicate<Serializer> operator<(FilterEvent<Serializer> a, FilterEvent<Serializer> b){
  return FilterPredicate<Serializer>(id::filterOp::LT::value, a, b);
}

template<typename Serializer>
FilterPredicate<Serializer> operator<=(FilterEvent<Serializer> a, FilterEvent<Serializer> b){
  return FilterPredicate<Serializer>(id::filterOp::LE::value, a, b);
}

template<typename Serializer>
FilterPredicate<Serializer> operator==(FilterEvent<Serializer> a, FilterEvent<Serializer> b){
  return FilterPredicate<Serializer>(id::filterOp::EQ::value, a, b);
}

template<typename Serializer>
FilterPredicate<Serializer> operator!=(FilterEvent<Serializer> a, FilterEvent<Serializer> b){
  return FilterPredicate<Serializer>(id::filterOp::NE::value, a, b);
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

template<typename Serializer>
Serializer& operator<<(Serializer& s, const FilterPredicate<Serializer, PseudoAttr>& p) {
    return s << p.mE0 << p.mOp.data << (*p.mE1);
}

template<typename Serializer, typename Attr>
Serializer& operator<<(Serializer& s, const FilterPredicate<Serializer, Attr>& p) {
    return s << p.mE0 << p.mOp.data << (*p.mAttr);
}
