#pragma once

#include <MetaValueBaseImplementation.h>
#include <MetaValue.h>
#include <ValueElement.h>
#include <ID.h>
#include <IO.h>

#include <valarray>
#include <ostream>

class MetaValue;

template<typename T>
class MetaValueImplementation : public MetaValueBaseImplementation {
  private:
    using Base = MetaValueBaseImplementation;
    using Type = MetaValueImplementation;

    std::valarray<ValueElement<T>> mData;
    bool mHasUncertainty;

    static Base& factoryCreate(std::size_t n, bool u);

  protected:
    using ElementInitType = typename ValueElement<T>::InitType;
    using InitType = std::initializer_list<ElementInitType>;
    
    MetaValueImplementation(const Type&) = default;

    MetaValueImplementation(std::size_t n, bool u);

    MetaValueImplementation(T value);

    MetaValueImplementation(ElementInitType value);

    MetaValueImplementation(InitType values);

    virtual Base& copy() const;

    virtual void n( std::size_t n ) { 
      mData.resize(n);
    }

    virtual void hasUncertainty( bool u) { 
      mHasUncertainty = u;
    }

  public:
    using DataType = T;

    static MetaValue create();
    static MetaValue create(T value);
    static MetaValue create(InitType& i);

    virtual ~MetaValueImplementation() = default;

    virtual Type& operator=( const Type& b) = delete;

    virtual Base& operator+=(const Base& b);
   
    virtual id::type::ID typeId() const {
      return id::type::id(T());
    }

    virtual std::size_t n() const { 
      return mData.size();
    }

    virtual bool hasUncertainty() const { 
      return mHasUncertainty;
    }

    virtual std::size_t size() const { 
      return sizeof(Type) + sizeof(ValueElement<T>)*n();
    }

    virtual void print(std::ostream& o) const;

    friend class MetaFactoryImplementation;
};
