#pragma once

#include <MetaValueBaseImplementation.h>
#include <MetaScale.h>
#include <ValueType.h>

class MetaValue {
  private:
    using Ptr = MetaValueBaseImplementation::Ptr;
    Ptr mImpl;

    explicit MetaValue(Ptr&& ref);
    Ptr& implementation() { return mImpl; }
    const Ptr& implementation() const { return mImpl; }

  public:
    MetaValue() : mImpl(MetaValueBaseImplementation::sInstance.copy()) {}

    MetaValue(const MetaValue& copy);
    MetaValue(MetaValue&& copy);

    MetaValue& operator=(const MetaValue& b);
    MetaValue& operator=(MetaValue&& b);

    MetaValue operator+(const MetaValue& b) const;

		MetaValue& scale(const MetaScale& b) { mImpl->scale(b); return *this; }

    std::size_t size() const { 
      return sizeof(MetaValue) + mImpl->size();
    }

    std::size_t n() const { 
      return mImpl->n();
    }

    id::type::ID typeId() const { 
      return mImpl->typeId();
    }

    bool valid() const;

    bool compatible(const MetaValue& b) const;

    bool hasUncertainty() const { 
      return mImpl->hasUncertainty();
    }

    operator ValueType();

    friend std::ostream& operator<<(std::ostream&, const MetaValue&);
    friend class MetaFactoryImplementation;
};

inline std::ostream& operator<<(std::ostream& o, const MetaValue& v) {
  v.mImpl->print(o);
  return o;
}
