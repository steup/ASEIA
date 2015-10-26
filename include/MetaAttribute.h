#pragma once

#include <MetaValue.h>
#include <MetaUnit.h>
#include <MetaScale.h>

class MetaAttribute { 
  private:

    id::attribute::ID mID    ;
    MetaValue         mValue ;
    MetaUnit          mUnit  ;
    MetaScale         mScale ;

  public:

  MetaAttribute() = default;

  MetaAttribute(id::attribute::ID id    = id::attribute::Base::value()) : mID(id) {}
	MetaAttribute& operator=(const MetaAttribute& copy);
	MetaAttribute& operator=(MetaAttribute&& copy);
	MetaAttribute& operator+=(const MetaAttribute& b);

  id::attribute::ID id() const { return mID; }

        MetaScale& scale()       { return mScale; }
  const MetaScale& scale() const { return mScale; }

        MetaUnit&  unit()        { return mUnit;  }
  const MetaUnit&  unit()  const { return mUnit;  }

        MetaValue& value()       { return mValue; }
  const MetaValue& value() const { return mValue; }
};

std::ostream& operator<<(std::ostream& o, const MetaAttribute& ma);
