#pragma once

#include <MetaValueBaseImplementation.h>
#include <ID.h>
#include <Serializer.h>
#include <DeSerializer.h>
#include <ValueElement.h>

#include <iosfwd>
#include <memory>

class MetaScale;
class ValueType;

class MetaValue {
  public:
    using Ptr = MetaValueBaseImplementation::Ptr;
		using ID  = id::type::ID;

	private:
    Ptr mImpl;
    
	public:
    explicit MetaValue(Ptr&& ref);
    Ptr& implementation() { return mImpl; }
    const Ptr& implementation() const { return mImpl; }

    bool resize(std::size_t rows, std::size_t cols);
    bool hasUncertainy(bool u);

  public:
    using ElemInitType = MetaValueBaseImplementation::ElemInitType;
    using RowInitType = std::initializer_list<ElemInitType>;
    using InitType = std::initializer_list<RowInitType>;
    MetaValue();

    MetaValue(id::type::ID typeID, size_t rows, size_t cols=1, bool u=false);
    MetaValue(double v, id::type::ID typeIDid=id::type::Double::value());
    //MetaValue(ElemInitType l, id::type::ID typeIDid=id::type::Double::value(), bool u=false);
    //MetaValue(RowInitType l, id::type::ID typeIDid=id::type::Double::value(),
    //          size_t rows=0, bool u=false);
    MetaValue(InitType l, id::type::ID typeIDid=id::type::Double::value(),
              size_t rows=0, size_t cols=0, bool u=false);

    MetaValue(const MetaValue& copy);

    MetaValue& operator=(const MetaValue& b);

    MetaValue& operator+=(const MetaValue& b);
    MetaValue& operator-=(const MetaValue& b);
    MetaValue& operator*=(const MetaValue& b);
    MetaValue& operator/=(const MetaValue& b);
		MetaValue operator==(const MetaValue& b) const;
		MetaValue operator!=(const MetaValue& b) const;
		MetaValue operator<=(const MetaValue& b) const;
		MetaValue operator>=(const MetaValue& b) const;
		MetaValue operator<(const MetaValue& b) const;
		MetaValue operator>(const MetaValue& b) const;
		MetaValue& operator*=(const MetaScale& b);
		MetaValue& operator/=(const MetaScale& b);
    ValueElement<double, true> get(std::size_t row, std::size_t col) const;
    MetaValue block(size_t i, size_t j, size_t numI, size_t numJ) const;
    MetaValue operator()(size_t row, size_t col) const { return block(row, col, 1, 1); }
    bool set(std::size_t row, std::size_t col, ElemInitType elem);
    bool set(std::size_t row, std::size_t col, double elem);
    MetaValue& block(size_t i, size_t j, const MetaValue& v);
    MetaValue& block(size_t i, size_t j, MetaValue&& v);
    MetaValue col(size_t col) const;
    MetaValue row(size_t row) const;

    MetaValue& sin();
    MetaValue& cos();
    MetaValue& tan();
    MetaValue& asin();
    MetaValue& acos();
    MetaValue& atan();
    MetaValue& abs();
    MetaValue& min();
    MetaValue& max();

    MetaValue identity() const;
    MetaValue zero() const;
    MetaValue prod() const;
    MetaValue sum() const;
    MetaValue norm() const;
    MetaValue transpose() const;

    std::size_t size()   const;
    std::size_t cols()   const;
    std::size_t rows()   const;
    ID          typeId() const;
    bool hasUncertainty() const;

    bool valid() const;
    bool compatible(const MetaValue& b) const;

    explicit operator ValueType() const;
    explicit operator bool() const { return (bool)(*mImpl);}

    std::ostream& print(std::ostream& o) const;

  template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, const MetaValue&);
  template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, const MetaValue&);
  friend class MetaFactoryImplementation;
};

inline MetaValue operator+(const MetaValue& a, const MetaValue& b) {
  return MetaValue(a)+=b;
}

inline MetaValue operator-(const MetaValue& a, const MetaValue& b) {
  return MetaValue(a)-=b;
}

inline MetaValue operator*(const MetaValue& a, const MetaValue& b) {
  return MetaValue(a)*=b;
}

inline MetaValue operator/(const MetaValue& a, const MetaValue& b) {
  return MetaValue(a)/=b;
}

inline std::ostream& operator<<(std::ostream& o, const MetaValue& v) {
  return v.print(o);
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaValue& me){
	if(me.implementation())
    s << *me.implementation();
	return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaValue& me){
	if(me.implementation())
    d >> *me.implementation();
	return d;
}
