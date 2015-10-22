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
    using Ptr  = Base::Ptr;
    using Type = MetaValueImplementation;

    std::valarray<ValueElement<T>> mData;
    bool mHasUncertainty;

    static Ptr factoryCreate(std::size_t n, bool u);

  public:
    
    MetaValueImplementation(const Type&) = default;

    MetaValueImplementation(std::size_t n, bool u);

    virtual Ptr copy() const{
      return Ptr(new MetaValueImplementation(*this), deleter);
    }

    virtual void n( std::size_t n ) { 
      mData.resize(n);
    }

    virtual void hasUncertainty( bool u) { 
      mHasUncertainty = u;
    }

  public:
    using DataType = T;

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

		virtual void scale(const MetaScale& b) {
			for(auto& v : mData) 
				v = v * (T)b.num() / (T)b.denom();
		}

    virtual void set(uint8_t i, double value, double uncertainty){
      mData[i].value(value);
      mData[i].uncertainty(uncertainty);
    }
   
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
    template<typename T1, typename T2> friend class Converter;
};

template<typename T0, typename T1>
struct Converter{
  using Base = MetaValueBaseImplementation;

  static void convert(const Base& in, Base& out){
    auto i=std::begin(reinterpret_cast<MetaValueImplementation<T1>&>(out).mData);
    auto end=std::end(reinterpret_cast<MetaValueImplementation<T1>&>(out).mData);
    for(const auto& elem : reinterpret_cast<const MetaValueImplementation<T0>&>(in).mData){
      if(i==end)
        break;
      *i++=elem;
    }
  }
  
  operator MetaFactory::Converter(){
    return {{id::type::id(T0()), id::type::id(T1())}, &Converter::convert};
  }
};
