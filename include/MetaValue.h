#pragma once

#include <MetaValueBaseImplementation.h>
#include <ValueType.h>

class MetaValue {
  private:
    MetaValueBaseImplementation* mImpl = &MetaValueBaseImplementation::sInstance;

  public:
    MetaValue() = default;
    ~MetaValue();
    
    MetaValue(const MetaValueBaseImplementation& ref) : mImpl(&ref.copy()) {}

    MetaValue(const MetaValue& value) : mImpl(&value.mImpl->copy()){
    }

    MetaValue& operator=(const MetaValue& b);
    MetaValue& operator=(const MetaValueBaseImplementation& b);

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

    MetaValueBaseImplementation& implementation() { return *mImpl; }
    const MetaValueBaseImplementation& implementation() const { return *mImpl; }

    friend std::ostream& operator<<(std::ostream&, const MetaValue&);
    friend class MetaFactoryImplementation;
};

inline std::ostream& operator<<(std::ostream& o, const MetaValue& v) {
  v.mImpl->print(o);
  return o;
}
