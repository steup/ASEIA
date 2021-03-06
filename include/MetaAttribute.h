#pragma once

#include <MetaValue.h>
#include <MetaUnit.h>
#include <MetaScale.h>
#include <Serializer.h>
#include <DeSerializer.h>

#include <iosfwd>

class AttributeType;

class MetaAttribute {
  public:
    using ID = id::attribute::ID;
  protected:
    ID         mID    ;
    MetaValue  mValue ;
    MetaUnit   mUnit  ;
    MetaScale  mScale ;

    bool check(const MetaAttribute& b) const;

  public:
    MetaAttribute(ID id = id::attribute::Base::value()) : mID(id) {}
    MetaAttribute(const AttributeType& at);
    MetaAttribute(const MetaAttribute& b);
    MetaAttribute(MetaAttribute&& b);
    MetaAttribute& operator=(const MetaAttribute& b);
    MetaAttribute& operator=(MetaAttribute&& b);
    MetaAttribute& operator+=(const MetaAttribute& b);
    MetaAttribute& operator-=(const MetaAttribute& b);
    MetaAttribute& operator*=(const MetaAttribute& b);
    MetaAttribute& operator/=(const MetaAttribute& b);
    MetaAttribute& operator*=(const MetaScale& scale);
    MetaAttribute& operator/=(const MetaScale& b);
    MetaAttribute& operator*=(const MetaValue& b);
    MetaAttribute& operator/=(const MetaValue& b);
    MetaAttribute norm() const;
    MetaAttribute inverse() const;
    MetaAttribute sqrt() const;
    MetaValue operator<(const MetaAttribute& b) const;
    MetaValue operator>(const MetaAttribute& b) const;
    MetaValue operator<=(const MetaAttribute& b) const;
    MetaValue operator>=(const MetaAttribute& b) const;
    MetaValue operator==(const MetaAttribute& b) const;
    MetaValue operator!=(const MetaAttribute& b) const;
    MetaAttribute uncertainty() const;
    MetaAttribute valueOnly() const;
    MetaAttribute toUncertainty() const;

          ID            id() const { return mID;    }

          MetaScale& scale()       { return mScale; }
    const MetaScale& scale() const { return mScale; }

          MetaUnit&  unit()        { return mUnit;  }
    const MetaUnit&  unit()  const { return mUnit;  }

          MetaValue& value()       { return mValue; }
    const MetaValue& value() const { return mValue; }

    bool valid() const { return mValue.valid(); }

    explicit operator AttributeType() const;

	//friend class MetaFactory;
  template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, const MetaValue&);
};

inline MetaAttribute operator+(const MetaAttribute& a, const MetaAttribute& b) {return MetaAttribute(a)+=b; };
inline MetaAttribute operator-(const MetaAttribute& a, const MetaAttribute& b) {return MetaAttribute(a)-=b; };
inline MetaAttribute operator*(const MetaAttribute& a, const MetaAttribute& b) {return MetaAttribute(a)*=b; };
inline MetaAttribute operator/(const MetaAttribute& a, const MetaAttribute& b) {return MetaAttribute(a)/=b; };

inline MetaAttribute operator*(const MetaAttribute& a, const MetaScale& scale) {return MetaAttribute(a)*=scale; };
inline MetaAttribute operator/(const MetaAttribute& a, const MetaScale& scale) {return MetaAttribute(a)/=scale; };
inline MetaAttribute operator*(const MetaScale& scale, const MetaAttribute& a) {return MetaAttribute(a)*=scale; };

inline MetaAttribute operator*(const MetaAttribute& a, const MetaValue& b)     {return MetaAttribute(a)*=b; };
inline MetaAttribute operator/(const MetaAttribute& a, const MetaValue& b)     {return MetaAttribute(a)/=b; };
MetaAttribute operator*(const MetaValue& a    , const MetaAttribute& b);



std::ostream& operator<<(std::ostream& o, const MetaAttribute& ma);

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaAttribute& me){
	return s << me.value();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaAttribute& me){
	return d >> me.value();
}
