#pragma once

#include <MetaValueBaseImplementation.h>
#include <MetaFactory.h>
#include <MetaScale.h>
#include <Value.h>
#include <IO.h>

template<typename T>
class MetaValueImplementation : public MetaValueBaseImplementation {
  private:
    using Impl = MetaValueImplementation;
		using Base = Value<T, Eigen::Dynamic, Eigen::Dynamic>;
		using Elem = typename Base::BaseType;

    Base mData;

    static Ptr factoryCreate(std::size_t rows, std::size_t cols, bool u) {
      return Ptr(new MetaValueImplementation(rows, cols, u));
    }

  public:
    
    MetaValueImplementation(const MetaValueImplementation& copy) = default;

    virtual Ptr copy() const{
      return Ptr(new MetaValueImplementation(*this), deleter);
    }

    virtual void resize( std::size_t rows, std::size_t cols ) { 
      mData.resize(rows, cols);
    }
    
  public:
    using DataType = T;

    MetaValueImplementation(std::size_t rows, std::size_t cols, bool u=true) : mData(rows, cols) { }

    MetaValueImplementation(std::initializer_list<Elem> values) : mData(values) { }

    virtual ~MetaValueImplementation() = default;

    virtual MetaValueImplementation& operator=( const MetaValueImplementation& b) = delete;

    Interface& operator+=(const Interface& b) {
      mData += reinterpret_cast<const MetaValueImplementation&>(b).mData;
      return *this;
    }

		virtual Interface& operator*=(const MetaScale& b) {
			mData  *= b.num();
      mData  /= b.denom();
      return *this;
		}

    virtual void set(std::size_t row, std::size_t col, ValueElement<double> value) {
      mData(row, col)=value;
    }
   
    virtual id::type::ID typeId() const {
      return id::type::id(T());
    }

    virtual std::size_t cols() const { 
      return mData.cols();
    }
    
    virtual std::size_t rows() const { 
      return mData.rows();
    }

    virtual std::size_t size() const { 
      return Elem::size()*cols()*rows();
    }

    virtual std::ostream& print(std::ostream& o) const {
      return o << mData;
    }

    friend class MetaFactoryImplementation;
    template<typename T1, typename T2> friend class Converter;
};

template<typename T0, typename T1>
struct Converter{
  using Base = MetaValueBaseImplementation;
  using T0Impl = MetaValueImplementation<T0>;
  using T1Impl = MetaValueImplementation<T1>;

  static void convert(const Base& in, Base& out){
    const T0Impl& inC = reinterpret_cast<const T0Impl&>(in);
    T1Impl& outC = reinterpret_cast<T1Impl&>(out);
    outC.mData = inC.mData.template cast< typename T1Impl::Base::BaseType >();
  }
  
  operator MetaFactory::Converter(){
    return {{id::type::id(T0()), id::type::id(T1())}, &Converter::convert};
  }
};
