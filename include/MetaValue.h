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
    MetaValue();

    MetaValue(const MetaValue& copy);
    MetaValue(MetaValue&& copy);

    MetaValue& operator=(const MetaValue& b);
    MetaValue& operator=(MetaValue&& b);

    MetaValue operator+(const MetaValue& b) const;
		MetaValue operator==(const MetaValue& b) const;
		MetaValue operator!=(const MetaValue& b) const;
		MetaValue operator<=(const MetaValue& b) const;
		MetaValue operator>=(const MetaValue& b) const;
		MetaValue operator<(const MetaValue& b) const;
		MetaValue operator>(const MetaValue& b) const;
		MetaValue& operator*=(const MetaScale& b);
		MetaValue& operator/=(const MetaScale& b);
    ValueElement<double, true> get(std::size_t row, std::size_t col) const;
    bool set(std::size_t row, std::size_t col, const ValueElement<double, true>& v);

    MetaValue prod() const;
    MetaValue sum() const;

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
