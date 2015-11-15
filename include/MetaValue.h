#pragma once

#include <MetaValueBaseImplementation.h>
#include <ID.h>

#include <iosfwd>

class MetaScale;
class ValueType;

class MetaValue {
  private:
    using Ptr = MetaValueBaseImplementation::Ptr;
    Ptr mImpl;
public:
    explicit MetaValue(Ptr&& ref);
    Ptr& implementation() { return mImpl; }
    const Ptr& implementation() const { return mImpl; }
    
    void resize(std::size_t rows, std::size_t cols) { 
      return mImpl->resize(rows, cols);
    }
    
    void hasUncertainy(bool u) { 
      mImpl->hasUncertainty(u);
    }

  public:
    MetaValue() : mImpl(MetaValueBaseImplementation::sInstance.copy()) {}

    MetaValue(const MetaValue& copy);
    MetaValue(MetaValue&& copy);

    MetaValue& operator=(const MetaValue& b);
    MetaValue& operator=(MetaValue&& b);

    MetaValue operator+(const MetaValue& b) const;
		bool operator==(const MetaValue& b) const;

		MetaValue& scale(const MetaScale& b) { (*mImpl)*=b; return *this; }

    void set(std::size_t row, std::size_t col, ValueElement<double> value) {
      mImpl->set(row, col, value);
    }
    
    std::size_t size() const { 
      return mImpl->size();
    }

    std::size_t cols() const { 
      return mImpl->cols();
    }
    
    std::size_t rows() const { 
      return mImpl->rows();
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
  return v.mImpl->print(o);
}
