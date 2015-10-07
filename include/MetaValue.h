#pragma once

#include <MetaValueBaseImplementation.h>
#include <ValueType.h>
#include <memory>

class MetaValue {
  private:
    using Ptr = MetaValueBaseImplementation::Ptr;
    Ptr mImpl;

  public:
    MetaValue() : mImpl(MetaValueBaseImplementation::sInstance.copy()) {}
    explicit MetaValue(Ptr&& ref) : mImpl(std::move(ref)) {}

    MetaValue(const MetaValue& value) : MetaValue(value.mImpl->copy()){
    }

    MetaValue& operator=(const MetaValue& b) { mImpl=b.mImpl->copy(); return *this;}
    MetaValue& operator=(MetaValue&& b) { mImpl=std::move(b.mImpl); return *this;}
    MetaValue& operator=(Ptr&& b) { mImpl=std::move(b); return *this;}

    MetaValue operator+(const MetaValue& b) const;

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

    Ptr& implementation() { return mImpl; }
    const Ptr& implementation() const { return mImpl; }

    friend std::ostream& operator<<(std::ostream&, const MetaValue&);
    friend class MetaFactoryImplementation;
};

inline std::ostream& operator<<(std::ostream& o, const MetaValue& v) {
  v.mImpl->print(o);
  return o;
}
