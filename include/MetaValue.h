#pragma once

#include <ID.h>
#include <IO.h>
#include <ostream>
#include <valarray>
#include <ValueElement.h>
#include <ValueType.h>

namespace implementation {
  class BaseValue {
    public:
      static BaseValue sInstance;
      virtual ~BaseValue() = default;
      virtual id::type::ID typeId() const { return id::type::Base::value(); }
      virtual BaseValue& operator+=(const BaseValue& b) { return *this; }
      virtual std::size_t size() const { return sizeof(BaseValue); }
      virtual void print(std::ostream& o) const { o << "void"; }
      virtual std::size_t n() const { return 0; }
      virtual BaseValue* copy() const { return &sInstance; }
      virtual bool hasUncertainty() const { return false; }
  };

  template<typename T>
  class Value : public BaseValue{
    private:
      std::valarray<ValueElement<T>> mData;
      bool mHasUncertainty;
    public:
      using DataType = T;
      using ElementInitType = typename ValueElement<T>::InitType;
      using InitType = std::initializer_list<ElementInitType>;

      Value(std::size_t n, bool u) : mHasUncertainty(u) {
        mData.resize(n);
      }

      Value(T value) : mHasUncertainty(false) {
        mData.resize(1);
        mData[0]=ValueElement<T>(value);
      }

      Value(ElementInitType value) : mHasUncertainty(true) {
        mData.resize(1);
        mData[0]=ValueElement<T>(*value.begin(), *std::next(value.begin()));
      }

      Value(InitType values) : mHasUncertainty(true) {
        mData.resize(values.end()-values.begin());
        std::size_t i = 0;
        for( const auto& v : values ) {
          mData[i++]=ValueElement<T>(*v.begin(), *std::next(v.begin()));
        }
      }

      virtual id::type::ID typeId() const {return id::type::id(T()); }
      virtual BaseValue& operator+=(const BaseValue& b) {
        mData += reinterpret_cast<const Value&>(b).mData;
        return *this;
      }
      virtual std::size_t size() const { return sizeof(Value)+sizeof(ValueElement<T>)*n(); }
      virtual std::size_t n() const { return mData.size(); }
      virtual bool hasUncertainty() const { return mHasUncertainty; }
      virtual void print(std::ostream& o) const {
        o << "[";
        for(const auto& e : mData)
          if(mHasUncertainty)
            o << e <<", ";
          else
            o << e.value() <<", ";
        o << "]";
      }
      virtual BaseValue* copy() const {
        return new Value(*this);
      }
  };

}
class MetaValue {
  private:
    implementation::BaseValue* mImpl = &implementation::BaseValue::sInstance;
    bool mHasUncertainty;
  public:
    MetaValue() = default;
    ~MetaValue();

    MetaValue(implementation::BaseValue* ptr) : mImpl(ptr) {}

    template<typename T>
    MetaValue(T i) : mImpl(new implementation::Value<T>(i)) {}

    template<typename T>
    MetaValue(std::initializer_list<T> i) : mImpl(new implementation::Value<T>(i)) {}

    template<typename T>
    MetaValue(std::initializer_list<std::initializer_list<T>> i) : mImpl(new implementation::Value<T>(i)) {}

    MetaValue(const MetaValue& value) : mImpl(value.mImpl->copy()) {}

    MetaValue& operator=(const MetaValue& b);

    MetaValue operator+(const MetaValue& b) const;

    std::size_t size() const { return sizeof(MetaValue) + mImpl->size(); }

    std::size_t n() const { return mImpl->n(); }

    id::type::ID typeId() const { return mImpl->typeId(); }

    bool valid() const;

    bool compatible(const MetaValue& b) const;

    bool hasUncertainty() const { return mImpl->hasUncertainty(); }

    operator ValueType();

    friend std::ostream& operator<<(std::ostream&, const MetaValue&);
};

inline std::ostream& operator<<(std::ostream& o, const MetaValue& v) {
  v.mImpl->print(o);
  return o;
}
