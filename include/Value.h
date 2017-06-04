#pragma once

#include <Eigen/Core>

#include <ratio>
#include <cmath>

#include <ValueElement.h>
#include <ValueType.h>


namespace Eigen {
	template<typename T, bool U>
	struct NumTraits<ValueElement<T, U>> : public NumTraits<T> {
		enum {
			ReadCost = U?2:1 * NumTraits<T>::ReadCost,
			AddCost  = U?3:1 * NumTraits<T>::AddCost,
		  MulCost = U?5:1 * NumTraits<T>::MulCost
		};
	};
}

template<typename T, bool U>
const ValueElement<T, U>& conj(const ValueElement<T, U>& x)  { return x; }

template<typename T, bool U>
const ValueElement<T, U>& real(const ValueElement<T, U>& x)  { return x; }

template<typename T, bool U>
ValueElement<T, U> imag(const ValueElement<T, U>&)    { return 0.; }

template<typename T, bool U>
ValueElement<T, U> abs(const ValueElement<T, U>&  x)  { return ValueElement<T, U>({{std::abs(x.value()), x.uncertainty()}}); }

template<typename T, bool U>
ValueElement<T, U> abs2(const ValueElement<T, U>& x)  { return x*x; }

template<typename T, bool U>
ValueElement<T, U> ceil(const ValueElement<T, U>& x)  { return ValueElement<T, U>({{ceil(x.value()), x.uncertainty()}}); }

template<typename T, bool U>
ValueElement<T, U> log(const ValueElement<T, U>& x)  { return ValueElement<T, U>({{log(x.value()), log(x.uncertainty())}}); }


template<typename T, int32_t R, int32_t C = 1, bool U=true>
class Value {
  private:
    Eigen::Matrix<ValueElement<T, U>, R, C> mData;
  public:
		using BaseType      = ValueElement<T, U>;
		using RowInitType   = std::initializer_list<BaseType>;
		using InitType      = std::initializer_list<RowInitType>;
		using TypeID        = typename BaseType::TypeID;
    using DataType      = Eigen::Matrix<BaseType, R, C>;
		using Bool          = Eigen::Matrix<ValueElement<bool, U>, R, C>;
    using Iterator      = BaseType*;
    using ConstIterator = const BaseType*;

		Iterator begin() { return Iterator(this->data().data()); }

		Iterator end()   { return Iterator(this->data().data()+this->cols()*this->rows()); }

		ConstIterator begin() const { return ConstIterator(this->data().data()); }

		ConstIterator end() const { return ConstIterator(this->data().data()+this->cols()*this->rows()); }

    void resize(std::size_t rows, std::size_t cols=1) { mData.resize(rows, cols); }
    void conservativeResize(std::size_t rows, std::size_t cols=1) { mData.conservativeResize(rows, cols); }

    Value() = default;
    Value(std::size_t rows, std::size_t cols) : mData(rows, cols) {};
    Value(const Eigen::Matrix<ValueElement<T, U>, R, C>& copy) : mData(copy) {}

		Value(InitType l) : mData(l.size(), l.begin()->size()){
      unsigned int i=0;
      for(const auto& row : l) {
				unsigned int j=0;
        for(const auto& elem : row)
          mData(i, j++)=elem;
        i++;
			}
    }
    
    Value identity() const { return Value(DataType::Identity(mData.rows(), mData.cols())); }
    Value zero() const    { return Value(DataType::Zero(mData.rows(), mData.cols())); }

    auto block(size_t i, size_t j, size_t p, size_t q) -> decltype(mData.block(i, j, p, q)) {
      return mData.block(i,j,p,q);
    }

    auto block(size_t i, size_t j, size_t p, size_t q) const -> decltype(mData.block(i, j, p, q)) {
      return mData.block(i,j,p,q);
    }

    auto col(size_t c) -> decltype(mData.col(c)) {
      return mData.col(c);
    }

    auto col(size_t c) const -> decltype(mData.col(c)) {
      return mData.col(c);
    }

    auto row(size_t r) -> decltype(mData.row(r)) {
      return mData.row(r);
    }

    auto row(size_t r) const -> decltype(mData.row(r)){
      return mData.row(r);
    }

    auto transpose() const -> decltype(mData.transpose()) {
      return mData.transpose();
    }

    Value& cwiseProduct(const Value& b) {
      mData=mData.cwiseProduct(b.mData);
      return *this;
    }

    Value& sin() { mData.array().sin(); return *this; }
    Value& cos() { mData.array().cos(); return *this; }
    Value& tan() { mData.array().tan(); return *this; }
    Value& asin() { mData.array().asin(); return *this; }
    Value& acos() { mData.array().acos(); return *this; }
    Value& atan() { mData.array().atan(); return *this; }
    Value& abs() { mData.array().abs(); return *this; }

    std::size_t rows() const { return R==-1?mData.rows():R; }
    std::size_t cols() const { return C==-1?mData.cols():C; }

    BaseType prod() const { return mData.prod(); }
    BaseType sum() const { return mData.sum(); }
    BaseType norm() const { return mData.norm(); }
    const DataType& data() const { return mData; }
    DataType& data() { return mData; }

    template<std::intmax_t num, std::intmax_t den>
    Value operator*(std::ratio<num, den> r) {
      Value res = *this;
      res *= BaseType(r.num);
      res /= BaseType(r.den);
      return res;
    }

    template<std::intmax_t num, std::intmax_t den>
    Value& operator*=(std::ratio<num, den> r) {
      mData *= BaseType(r.num);
      mData /= BaseType(r.den);
      return *this;
    }

    template<std::intmax_t num, std::intmax_t den>
    Value operator/(std::ratio<num, den> r) const {
      Value res = *this;
      res *= BaseType(r.den);
      res /= BaseType(r.num);
      return res;
    }

    template<std::intmax_t num, std::intmax_t den>
    Value& operator/=(std::ratio<num, den> r) {
      mData *= BaseType(r.den);
      mData /= BaseType(r.num);
      return *this;
    }

    Value& operator+=(const Value& b) {
      mData+=b.mData;
      return *this;
    }

    Value& operator-=(const Value& b) {
      mData-=b.mData;
      return *this;
    }

    template<int32_t R2, int32_t C2>
    Value& operator*=(const Value<T, R2, C2, U>& b) {
      mData*=b.mData;
      return *this;
    }

    Value& operator*=(const BaseType& s) {
      mData*=s;
      return *this;
    }

    Value& operator/=(const BaseType& s) {
      mData/=s;
      return *this;
    }
    
    Value& operator/=(const Value& b) {
      mData.cwiseQuotient(b.mData);
      return *this;
    }

    BaseType& operator()(int row, int col=0) {
      return mData(row, col);
    }

    Value operator-() const {
      Value temp=*this;
      temp.mData = -temp.mData;
      return temp;
    }

    const BaseType& operator()(int row, int col=0) const {
      return mData(row, col);
    }

		explicit operator ValueType() const {
			return ValueType(BaseType::TypeID::value(),
											 rows(), cols(),
											 hasUncertainty());
		}

		Bool operator<( const Value& b ) const {
			Bool res(mData.rows(), mData.cols());
			for(unsigned int i=0;i<mData.rows();i++)
				for(unsigned int j=0;j<mData.cols();j++)
					res(i, j) = mData(i, j) < b(i, j);
			return res;
    }

		Bool operator>( const Value& b ) const {
			Bool res(mData.rows(), mData.cols());
			for(unsigned int i=0;i<mData.rows();i++)
				for(unsigned int j=0;j<mData.cols();j++)
					res(i, j) = mData(i, j) > b(i, j);
			return res;
    }

		Bool operator<=(const Value& b) const {
			Bool res(mData.rows(), mData.cols());
			for(unsigned int i=0;i<mData.rows();i++)
				for(unsigned int j=0;j<mData.cols();j++)
					res(i, j) = mData(i, j) <= b(i, j);
			return res;
		}

		Bool operator>=(const Value& b) const {
			Bool res(mData.rows(), mData.cols());
			for(unsigned int i=0;i<mData.rows();i++)
				for(unsigned int j=0;j<mData.cols();j++)
					res(i, j) = mData(i, j) >= b(i, j);
			return res;
		}

		Bool operator==(const Value& b) const {
			Bool res(mData.rows(), mData.cols());
			for(unsigned int i=0;i<mData.rows();i++)
				for(unsigned int j=0;j<mData.cols();j++)
					res(i, j) = mData(i, j) == b(i, j);
			return res;
		}

		Bool operator!=(const Value& b) const {
			Bool res(mData.rows(), mData.cols());
			for(unsigned int i=0;i<mData.rows();i++)
				for(unsigned int j=0;j<mData.cols();j++)
					res(i, j) = mData(i, j) != b(i, j);
			return res;
		}

    Bool approxEqual(const Value& b) const {
      if(rows() != b.rows() || cols() != b.cols())
        return Bool::Zero(rows(), cols());
      Bool res(rows(), cols());
      for(unsigned int i=0;i<rows();i++)
        for(unsigned int j=0;j<cols();j++)
          res(i, j) = ::approxEqual(mData(i, j), b.mData(i, j));
      return res;
    }

    template<typename VE>
    Value<typename VE::VType, R, C, VE::U::value> cast() const {
      return Value<typename VE::VType, R, C, VE::U::value>(mData.template cast<VE>());
    }

    Value<T, R, C, false> value() const {
      return cast<ValueElement<T, false>>();
    }

    Value<T, R, C, false> uncertainty() const {
      Value<T, R, C, false> temp;
      temp.resize(rows(), cols());
      for(size_t i=0; i<temp.rows();i++)
        for(size_t j=0; j<temp.cols(); j++)
          temp.mData(i,j).value(mData(i,j).uncertainty());
      return temp;
    }

		constexpr const bool hasUncertainty() const noexcept {return BaseType().hasUncertainty();}

    static constexpr const std::size_t staticSize() { return R * C * BaseType::size(); }
    std::size_t dynamicSize() const { return mData.rows() * mData.cols() * BaseType::size(); }

    template<typename TF, int32_t RF, int32_t CF, bool UF, int32_t C2F>
    friend Value<TF, RF, C2F, UF> operator*(const Value<TF, RF, CF, UF>&, const Value<TF, CF, C2F, UF>&);
    template<typename T2, int32_t R2, int32_t C2, bool U2> friend class Value;

};

template<typename T, int32_t R, int32_t C, bool U>
Value<T, R, C, U> operator+(const Value<T, R, C, U>& a, const Value<T, R, C, U>& b) {
  Value<T, R, C, U> temp=a;
  return temp+=b;
}

template<typename T, int32_t R, int32_t C, bool U>
Value<T, R, C, U> operator-(const Value<T, R, C, U>& a, const Value<T, R, C, U>& b) {
  Value<T, R, C, U> temp=a;
  return temp-=b;
}

template<typename T, int32_t R, int32_t C, bool U>
Value<T, R, C, U> operator*(T a, const Value<T, R, C, U>& b) {
  return b*a;
}

template<typename T, int32_t R, int32_t C, bool U>
Value<T, R, C, U> operator*(const ValueElement<T, U>& a, const Value<T, R, C, U>& b) {
  Value<T, R, C, U> temp=b;
  return temp*=a;
}

template<typename T, int32_t R, int32_t C, bool U>
Value<T, R, C, U> operator*(const Value<T, R, C, U>& a, const ValueElement<T, U>& b) {
  return b*a;
}

template<typename T, int32_t R, int32_t C, bool U>
Value<T, R, C, U> operator*(const Value<T, R, C, U>& a, T b) {
  return ValueElement<T,U>(b)*a;
}

template<typename T, int32_t R, int32_t C, bool U, int32_t C2>
Value<T, R, C2, U> operator*(const Value<T, R, C, U>& a, const Value<T, C, C2, U>& b) {
  return b.mData*a.mData;
}

template<typename T, int32_t R, int32_t C, bool U>
Value<T, R, C, U> operator/(const Value<T, R, C, U>& a, const ValueElement<T, U>& b) {
  Value<T, R, C, U> temp=a;
  return a/=b;
}

template<typename T, int32_t rows, int32_t cols, bool u>
auto approxEqual(const Value<T, rows, cols, u>& a, const Value<T, rows, cols, u>& b) -> typename Value<T, rows, cols, u>::Bool {
  return a.approxEqual(b);
}



template<typename PB, typename T, int32_t rows, int32_t cols, bool u>
Serializer<PB>& operator<<(Serializer<PB>& s, const Value<T,rows,cols,u>& value){
  for(const auto& v : value)
		s << v;
  return s;
}

template<typename PB, typename T, int32_t rows, int32_t cols, bool u>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, Value<T,rows,cols,u>& value){
  for(auto& v : value)
    s >> v;
  return s;
}
