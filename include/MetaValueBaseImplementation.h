#pragma once

#include <ID.h>
#include <ValueElement.h>

#include <memory>
#include <iosfwd>
#include <cstddef>

class MetaScale;

class MetaValueBaseImplementation {
	public:
    using Interface = MetaValueBaseImplementation;

		enum class Attributes {
			HasUncertainty,
			Size,
			Rows,
			Cols,
			TypeID,
		};

		union Data {
			bool hasUncertainty;
			std::size_t  size;
			std::size_t  rows;
			std::size_t  cols;
			id::type::ID typeID;
		};

		enum class UnaryOp {
      Sin,
      Cos,
      Tan,
      ASin,
      ACos,
      ATan,
      Abs,
      Min,
      Max,
      Sqrt
    };

    enum class UnaryConstOp {
			Neg,
			Prod,
      Sum,
      Norm,
      Transpose,
      Zero,
      Ones,
      ZeroValue,
      Identity,
      Value,
      Uncertainty,
      ToUncertainty,
      Inverse,
      ArgMin
    };

		enum class BinaryOp {
			Add,
			Sub,
			Mul,
			Div,
      EMul
		};

		enum class BinaryConstOp {
			Smaller,
			Greater,
			SmallEqual,
			GreatEqual,
			Equal,
			NotEqual,
			ApproxEqual,
      CDot
		};
	public:
    using Ptr = std::unique_ptr<Interface>;

	protected:

		MetaValueBaseImplementation(const Interface& copy) = default;

		virtual const uint8_t* begin() const { return nullptr;}
		virtual uint8_t* begin() { return nullptr;}
		virtual const uint8_t* end() const {return nullptr;}
		virtual uint8_t* end() {return nullptr;}

    MetaValueBaseImplementation() = default;
  public:
    using ElemInitType = std::initializer_list<double>;
    static Ptr instance() { return Ptr(new MetaValueBaseImplementation()); }
    virtual ~MetaValueBaseImplementation() = default;

		Interface& operator=(const Interface& copy) = delete;

		virtual Interface& operator=( Interface&& movee);

		virtual Ptr copy() const;

    virtual Data get( Attributes a ) const;

    virtual ValueElement<double, true> get(std::size_t row, std::size_t col) const;

		virtual bool set(Attributes a, Data d);

    virtual bool set(std::size_t row, std::size_t col, ElemInitType elem);

		virtual bool unaryOp( UnaryOp op);

    virtual Ptr unaryConstOp( UnaryConstOp op) const;

		virtual bool binaryOp( BinaryOp op, const Interface& b);

		virtual Ptr binaryConstOp( BinaryConstOp op, const Interface& b ) const;

    virtual bool block(size_t i, size_t j, Ptr&&);
    virtual Ptr block(size_t i, size_t j, size_t numI, size_t numJ) const;
    virtual Ptr col(size_t col) const;
    virtual Ptr row(size_t row) const;

		virtual bool scale(const MetaScale& scale, bool invert = false);

    virtual std::ostream& print( std::ostream& o ) const;

    virtual explicit operator bool() const { return false; }

	friend class MetaFactoryImplementation;
  friend class MetaValue;
  template<typename T> friend Serializer<T> operator<<(Serializer<T>&, const MetaValueBaseImplementation&);
  template<typename T> friend DeSerializer<T> operator>>(DeSerializer<T>&, MetaValueBaseImplementation&);
};

std::ostream& operator<<(std::ostream& o, const MetaValueBaseImplementation& mvbi);

template<typename T>
Serializer<T> operator<<(Serializer<T>& s, const MetaValueBaseImplementation& mvbi) {
  for(const uint8_t dataElement: mvbi)
    s << dataElement;
  return s;
}

template<typename T>
DeSerializer<T> operator>>(DeSerializer<T>& d, MetaValueBaseImplementation& mvbi) {
  for(uint8_t& dataElement: mvbi)
    d >> dataElement;
  return d;
}
