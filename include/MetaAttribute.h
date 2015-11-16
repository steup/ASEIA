#pragma once

#include <MetaValue.h>
#include <MetaUnit.h>
#include <MetaScale.h>

#include <iosfwd>

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
    bool operator==(const MetaAttribute& b) const;
    bool operator!=(const MetaAttribute& b) const { return !(*this==b); }

          ID            id() const { return mID;    }

          MetaScale& scale()       { return mScale; }
    const MetaScale& scale() const { return mScale; }

          MetaUnit&  unit()        { return mUnit;  }
    const MetaUnit&  unit()  const { return mUnit;  }

          MetaValue& value()       { return mValue; }
    const MetaValue& value() const { return mValue; }

};

std::ostream& operator<<(std::ostream& o, const MetaAttribute& ma);
