#pragma once

#include <MetaValueBaseImplementation.h>
#include <MetaFactory.h>
#include <Value.h>

template<typename T, bool U>
class MetaValueImplementation : public MetaValueBaseImplementation {
  private:
    using Impl = MetaValueImplementation;
		using Base = Value<T, Eigen::Dynamic, Eigen::Dynamic, U>;
		using Elem = typename Base::BaseType;

    Base mData;

    static Ptr factoryCreate(std::size_t rows, std::size_t cols, bool u);
		
		MetaValueImplementation(const MetaValueImplementation& copy) = default;
		MetaValueImplementation(const Base& copy);
		virtual const uint8_t* begin() const;
		virtual uint8_t* begin();
		virtual const uint8_t* end() const;
		virtual uint8_t* end();

  public:
    MetaValueImplementation(std::size_t rows, std::size_t cols);

    MetaValueImplementation(typename Base::InitType values);
    
		virtual ~MetaValueImplementation() = default;

		virtual Interface& operator=( Interface&& movee);
	
    virtual Ptr copy() const;

    virtual Data get( Attributes a ) const;
    
    virtual ValueElement<double, true> get(std::size_t row, std::size_t col) const;

		virtual bool set(Attributes a, Data d);

    virtual bool set(std::size_t row, std::size_t col, const ValueElement<double, true>& v); 

		virtual Interface& unaryOp( UnaryOp op);

    virtual Ptr unaryConstOp( UnaryConstOp op) const;

		virtual Interface& binaryOp( BinaryOp op, const Interface& b);

		virtual Ptr binaryConstOp( BinaryConstOp op, const Interface& b ) const;

		virtual Interface& scale(const MetaScale& scale, bool invert = false);
    
    virtual explicit operator bool() const { return (bool)mData.prod(); }
		
		std::ostream& print(std::ostream& o) const;

    friend class MetaFactoryImplementation;
    template<typename T0, bool U0>  friend class MetaValueImplementation;
    template<typename T1, bool U1, typename T2, bool U2> friend struct Converter;
};

extern template class MetaValueImplementation<uint8_t, true>;
extern template class MetaValueImplementation<uint16_t, true>;
extern template class MetaValueImplementation<uint32_t, true>;
extern template class MetaValueImplementation<uint64_t, true>;
extern template class MetaValueImplementation<int8_t, true>;
extern template class MetaValueImplementation<int16_t, true>;
extern template class MetaValueImplementation<int32_t, true>;
extern template class MetaValueImplementation<int64_t, true>;
extern template class MetaValueImplementation<float, true>;
extern template class MetaValueImplementation<double, true>;
extern template class MetaValueImplementation<bool, true>;
extern template class MetaValueImplementation<uint8_t, false>;
extern template class MetaValueImplementation<uint16_t, false>;
extern template class MetaValueImplementation<uint32_t, false>;
extern template class MetaValueImplementation<uint64_t, false>;
extern template class MetaValueImplementation<int8_t, false>;
extern template class MetaValueImplementation<int16_t, false>;
extern template class MetaValueImplementation<int32_t, false>;
extern template class MetaValueImplementation<int64_t, false>;
extern template class MetaValueImplementation<float, false>;
extern template class MetaValueImplementation<double, false>;
extern template class MetaValueImplementation<bool, false>;

template<typename T0, bool U0, typename T1, bool U1>
struct Converter{
  using Base = MetaValueBaseImplementation;
  using T0Impl = MetaValueImplementation<T0, U0>;
  using T1Impl = MetaValueImplementation<T1, U1>;

  static void convert(const Base& in, Base& out){
    const T0Impl& inC = reinterpret_cast<const T0Impl&>(in);
    T1Impl& outC = reinterpret_cast<T1Impl&>(out);
    outC.mData = inC.mData.template cast< typename T1Impl::Base::BaseType >();
  }
  
  operator MetaFactory::Converter(){
    return {{id::type::id(T0()), id::type::id(T1())}, &Converter::convert};
  }
};
