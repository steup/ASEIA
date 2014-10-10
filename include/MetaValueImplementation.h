#pragma once

#include <MetaValueBaseImplementation.h>
#include <MetaValue.h>
#include <MetaFactory.h>
#include <ValueElement.h>
#include <Value.h>
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
    
    MetaValueImplementation(const Type&) = default;

    MetaValueImplementation(std::size_t n, bool u);

    virtual Base& copy() const;

    virtual void n( std::size_t n ) { 
      mData.resize(n);
    }

    virtual void hasUncertainty( bool u) { 
      mHasUncertainty = u;
    }

  public:
    using DataType = T;

    /*MetaValueImplementation(T value) : mData(value), mHasUncertainty(false){     
    }

    MetaValueImplementation(T value, T uncertainty)
      : mData({value, uncertanity}), mHasUncertainty(true)
      {
    }*/

    /*MetaValueImplementation(const std::initializer_list<T>& i) : mData(i), mHasUncertainty(false){     
    }*/

    MetaValueImplementation(std::initializer_list<std::initializer_list<T>> values)
      : mHasUncertainty(true)
    {
      {
        auto temp = values.begin();
        std::size_t n=0;
        while(temp!=values.end()){n++;temp=std::next(temp);}
        mData.resize(n);
        n=0;
      }
      std::size_t i=0;
      for(const std::initializer_list<T>& elem : values){
        auto uI = std::next(elem.begin());
        T v = *elem.begin();
        if(uI!=elem.end()) {
          T u = *uI;
          mData[i++]=ValueElement<T>(v,u);
        }else
          mData[i++]=ValueElement<T>(v);
      }
    }

    MetaValueImplementation(std::initializer_list<T> i)
      : mHasUncertainty(false)
    {
      {
        auto temp = i.begin();
        std::size_t n=0;
        while(temp!=i.end()){n++;temp=std::next(temp);}
        mData.resize(n);
      }
      std::size_t n=0;
      for(const auto& elem : i)
          mData[n++]=ValueElement<T>(elem);
    }

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
