#pragma once

#include <ID.h>
#include <ostream>
#include <valarray>

class BaseValue {
  public:
    virtual ~BaseValue() = default;
    virtual id::type::ID id() const = 0;
    virtual BaseValue& operator+=(const BaseValue& b) = 0;
    virtual std::size_t size() const = 0;
    virtual void print(std::ostream& o) const = 0;
    virtual std::size_t n() const = 0;
    virtual BaseValue* copy() const = 0;
};

template<typename T>
class Value : public BaseValue{
  private:
    std::valarray<T> mData;
  public:
    Value(const std::initializer_list<T>& values) : mData(values) { }
    virtual id::type::ID id() const {return id::type::id(T()); }
    virtual BaseValue& operator+=(const BaseValue& b) {
      mData += reinterpret_cast<const Value&>(b).mData;
      return *this;
    }
    virtual std::size_t size() const { return sizeof(Value)+sizeof(T)*n(); }
    virtual std::size_t n() const { return mData.size(); }
    virtual void print(std::ostream& o) const {
      o << "[";
      for(const auto& e : mData)
        o << e <<", ";
      o << "]";
    }
    virtual BaseValue* copy() const {
      return new Value(*this);
    }
};

class MetaValue {
  private:
    BaseValue* mImpl = nullptr;
  public:
    MetaValue() = default;
    ~MetaValue() { delete mImpl; }

    template<typename T>
    MetaValue(const std::initializer_list<T>& i) : mImpl(new Value<T>(i)) {}

    MetaValue(const MetaValue& value) {
      if(value.valid())
        mImpl = value.mImpl->copy();
    }

    MetaValue operator+(const MetaValue& b) {
      MetaValue temp(*this);
      if(temp.compatible(b)) {
        *temp.mImpl+=*b.mImpl;
        return temp;
      } else
        return MetaValue();
    }

    std::size_t size() const { 
      if(valid())
        return sizeof(MetaValue) + mImpl->size();
      else
        return sizeof(MetaValue);
    }

    std::size_t n() const { 
      if(valid())
        return mImpl->n();
      else
        return 0;
    }

    id::type::ID id() const { 
      if(valid()) 
        return mImpl->id(); 
      else 
        return id::type::Base::value();
    }

    bool valid() const {
      return mImpl;
    }

    bool compatible(const MetaValue& b) const {
      return valid() && b.valid() && mImpl->id() == b.mImpl->id() && mImpl->n() == b.mImpl->n();
    }

    bool hasUncertainty() const { return false; }

    friend std::ostream& operator<<(std::ostream&, const MetaValue&);
};

std::ostream& operator<<(std::ostream& o, const MetaValue& v) {
  if(v.valid())
      v.mImpl->print(o);
  else
    o << "invalid";
  return o;
}
