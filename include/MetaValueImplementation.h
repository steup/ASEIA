#pragma once

#include <MetaValueBaseImplementation.h>
#include <MetaFactory.h>
#include <Value.h>

template<typename T>
class MetaValueImplementation : public MetaValueBaseImplementation {
  private:
    using Impl = MetaValueImplementation;
		using Base = Value<T, Eigen::Dynamic, Eigen::Dynamic>;
		using Elem = typename Base::BaseType;

    Base mData;

    static Ptr factoryCreate(std::size_t rows, std::size_t cols, bool u);
		
		MetaValueImplementation(const MetaValueImplementation& copy) = default;
		MetaValueImplementation(const Base& copy);

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

		virtual Interface& binaryOp( BinaryOp op, const Interface& b);

		virtual Ptr binaryConstOp( BinaryConstOp op, const Interface& b ) const;

		virtual Interface& scale(const MetaScale& scale, bool invert = false);
		
		std::ostream& print(std::ostream& o) const;

    friend class MetaFactoryImplementation;
    template<typename T0>  friend class MetaValueImplementation;
    template<typename T1, typename T2> friend struct Converter;
};

extern template class MetaValueImplementation<uint8_t>;
extern template class MetaValueImplementation<uint16_t>;
extern template class MetaValueImplementation<uint32_t>;
extern template class MetaValueImplementation<uint64_t>;
extern template class MetaValueImplementation<int8_t>;
extern template class MetaValueImplementation<int16_t>;
extern template class MetaValueImplementation<int32_t>;
extern template class MetaValueImplementation<int64_t>;
extern template class MetaValueImplementation<float>;
extern template class MetaValueImplementation<double>;
extern template class MetaValueImplementation<bool>;

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
