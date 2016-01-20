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
  private:
    ID         mID    ;
    MetaValue  mValue ;
    MetaUnit   mUnit  ;
    MetaScale  mScale ;

  public:
    MetaAttribute() = default;

    MetaAttribute(ID id = id::attribute::Base::value()) : mID(id) {}
    MetaAttribute(const MetaAttribute& copy);
    MetaAttribute(MetaAttribute&& movee);
    MetaAttribute& operator=(const MetaAttribute& copy);
    MetaAttribute& operator=(MetaAttribute&& movee);
    MetaAttribute& operator+=(const MetaAttribute& b);
    MetaAttribute operator*(const MetaScale& scale) const;
    bool operator==(const MetaAttribute& b) const;
    bool operator!=(const MetaAttribute& b) const { return !(*this==b); }

          ID            id() const { return mID;    }

          MetaScale& scale()       { return mScale; }
    const MetaScale& scale() const { return mScale; }

          MetaUnit&  unit()        { return mUnit;  }
    const MetaUnit&  unit()  const { return mUnit;  }

          MetaValue& value()       { return mValue; }
    const MetaValue& value() const { return mValue; }

    explicit operator AttributeType() const;


  template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, const MetaValue&);
};

std::ostream& operator<<(std::ostream& o, const MetaAttribute& ma);

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaAttribute& me){
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, const MetaAttribute& me){
  return d;
}
