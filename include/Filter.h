#pragma once

#include <Attribute.h>
#include <boost/phoenix/core/argument.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/function.hpp>
#include <type_traits>
#include <functional>

#include <ID.h>

template<typename Serializer>
class FilterEvent;

namespace filter {
  using namespace ::id;
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
    auto operator()(Attribute& attr) const -> decltype(attr.uncertainty()){
      return attr.uncertainty();
    }
  };

  const boost::phoenix::function<uncertainty_impl> uncertainty;

  struct certain_impl {
    template<typename Attribute>
    auto operator()(Attribute& attr) const -> decltype(attr.certain()){
      return attr.valueOnly();
    }
  };

  const boost::phoenix::function<certain_impl> certain;

  struct norm_impl {

    template<typename Attribute>
    auto operator()(Attribute& attr) const -> decltype(attr.norm()){
      return attr.norm();
    }
  };

  const boost::phoenix::function<norm_impl> norm;

  union Op{
    struct {
      uint8_t code      : 5;
      uint8_t unary     : 1;
      uint8_t comp      : 1;
      uint8_t constArg  : 1;
    };
    uint8_t data;
  };

  template<typename SubExp0, typename SubExp1>
  struct Expression {
    Op op;
    SubExp0 exp0;
    SubExp1 exp1;
    Expression(filterOp::ID op, SubExp0 exp0, SubExp1 exp1)
      : exp0(exp0), exp1(exp1) {
      this->op.data=0;
      this->op.code=op;
    }
    template< typename Serializer>
    Serializer& operator()(Serializer& s) const {
      return exp1(exp0(s) << op.data);
    }
    static constexpr uint8_t size() { return SubExp0::size() + SubExp1::size() + 1; }
  };

  struct AttributeTag {};

  template<typename ID, typename E>
  struct Attribute;

  template<typename Arg0, typename Arg1>
  struct Predicate {
    Op op;
    Arg0 arg0;
    Arg1 arg1;

    Predicate(filterOp::ID op, Arg0 arg0, Arg1 arg1) : arg0(arg0), arg1(arg1) {
      this->op.data=0;
      this->op.code=op;
      this->op.comp=true;
      this->op.constArg=!std::is_base_of<AttributeTag, Arg1>::type::value;
    }

    template<typename Serializer, typename ID, typename V, typename S, typename U>
    Serializer& help(Serializer& s, const ::Attribute<ID, V, S, U>& v) const {
      return s << v;
    }

    template<typename Serializer, typename T, int Rows, int Cols, bool U>
    Serializer& help(Serializer& s, const ::Value<T, Rows, Cols, U>& v) const {
      return s << v;
    }

    template<typename Serializer, typename ID, typename Event>
    Serializer& help(Serializer& s, const Attribute<ID, Event>& a) const {
      return a(s);
    }

    template<typename Serializer>
    Serializer& operator()(Serializer& s) const {
      return help(arg0(s) << op.data, arg1);
    }

    template<typename Arg10, typename Arg11>
    auto operator&&(Predicate<Arg10, Arg11> b)
      -> Expression<Predicate, Predicate<Arg10, Arg11>> {
      return Expression<Predicate, Predicate<Arg10, Arg11>>(filterOp::AND(), *this, b);
    }

    template<typename Arg10, typename Arg11>
    auto operator||(Predicate<Arg10, Arg11> b)
      -> Expression<Predicate, Predicate<Arg10, Arg11>> {
      return Expression<Predicate, Predicate<Arg10, Arg11>>(filterOp::OR(), *this, b);
    }

    static constexpr uint8_t size() { return Arg0::size() + Arg1::size() + 1; }
  };

  template<typename Arg0>
  struct UnaryFunctor;

  template<typename T>
  struct Ops {
    template<typename Arg1>
    auto operator<(Arg1 arg1) const
      -> Predicate<T, Arg1> {
        return Predicate<T, Arg1>(filterOp::LT(), *(T*)this, arg1);
    }

    template<typename Arg1>
    auto operator<=(Arg1 arg1) const
      -> Predicate<T, Arg1> {
        return Predicate<T, Arg1>(filterOp::LE(), *(T*)this, arg1);
    }

    template<typename Arg1>
    auto operator>(Arg1 arg1) const
      -> Predicate<T, Arg1> {
        return Predicate<T, Arg1>(filterOp::GT(), *(T*)this, arg1);
    }

    template<typename Arg1>
    auto operator>=(Arg1 arg1) const
      -> Predicate<T, Arg1> {
        return Predicate<T, Arg1>(filterOp::GE(), *(T*)this, arg1);
    }

    template<typename Arg1>
    auto operator==(Arg1 arg1) const
      -> Predicate<T, Arg1> {
        return Predicate<T, Arg1>(filterOp::EQ(), *(T*)this, arg1);
    }

    template<typename Arg1>
    auto operator!=(Arg1 arg1) const
      -> Predicate<T, Arg1> {
        return Predicate<T, Arg1>(filterOp::NE(), *(T*)this, arg1);
    }

    UnaryFunctor<T> uncertainty() const {
      return UnaryFunctor<T>(filterOp::UNC(), *(T*)this);
    }
    UnaryFunctor<T> norm() const {
      return UnaryFunctor<T>(filterOp::NOR(), *(T*)this);
    }
    UnaryFunctor<T> valueOnly() const {
      return UnaryFunctor<T>(filterOp::CER(), *(T*)this);
    }
  };

  template<typename Arg0>
  struct UnaryFunctor : public Ops<UnaryFunctor<Arg0>> {
    Op op;
    Arg0 arg0;
    UnaryFunctor(filterOp::ID op, Arg0 arg0) : arg0(arg0) {
      this->op.data = 0;
      this->op.unary = true;
      this->op.code = op;
    }
    template<typename Serializer>
    Serializer& operator()(Serializer& s) const {
      return arg0(s) << op.data;
    }
    using Ops<UnaryFunctor>::operator<;
    using Ops<UnaryFunctor>::operator<=;
    using Ops<UnaryFunctor>::operator>;
    using Ops<UnaryFunctor>::operator>=;
    using Ops<UnaryFunctor>::operator==;
    using Ops<UnaryFunctor>::operator!=;
    using Ops<UnaryFunctor>::uncertainty;
    using Ops<UnaryFunctor>::valueOnly;
    using Ops<UnaryFunctor>::norm;
    static constexpr uint8_t size() { return Arg0::size() + 1; }
  };


  template<typename ID, typename E>
  struct Attribute  : public Ops<Attribute<ID, E>>, public AttributeTag {
    E e;
    Attribute(E e) : e(e) {}
    template<typename Serializer>
    Serializer& operator()(Serializer& s) const {
      return e(s) << ID();
    }
    using Ops<Attribute>::operator<;
    using Ops<Attribute>::operator<=;
    using Ops<Attribute>::operator>;
    using Ops<Attribute>::operator>=;
    using Ops<Attribute>::operator==;
    using Ops<Attribute>::operator!=;
    using Ops<Attribute>::uncertainty;
    using Ops<Attribute>::valueOnly;
    using Ops<Attribute>::norm;
    static constexpr uint8_t size() { return E::size() + 1; }
  };

  struct Event {
    uint8_t num;

    Event(uint8_t num) : num(num) {}

    template<typename Serializer>
    Serializer& operator()(Serializer& s) const {
      return s << num;
    };

    template<typename AttrID>
    Attribute<AttrID, Event> operator[](AttrID id) const {
      return Attribute<AttrID, Event>(*this);
    }

    static constexpr uint8_t size() { return 1; }
  };

  const Event s0(0);
  const Event s1(1);
  const Event s2(2);
  const Event s3(3);
  const Event s4(4);
  const Event s5(5);
  const Event s6(6);
  const Event s7(7);
}

template<typename Serializer, typename Arg0, typename Arg1>
Serializer& operator<<(Serializer& s, const filter::Predicate<Arg0, Arg1>& p) {
  return p(s) << (uint8_t)::id::filterOp::NOP();
}

template<typename Serializer, typename SubExp0, typename SubExp1>
Serializer& operator<<(Serializer& s, const filter::Expression<SubExp0, SubExp1>& e) {
  return e(s) << (uint8_t)::id::filterOp::NOP();
}
