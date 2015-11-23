#pragma once

#include <ID.h>

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
			Neg,
			Not
		};

		enum class BinaryOp {
			Add,
			Sub,
			Mul,
			Div
		};

		enum class BinaryConstOp {
			Smaller,
			Greater,
			SmallEqual,
			GreatEqual,
			Equal,
			NotEqual,
			ApproxEqual
		};

  protected:
    struct Deleter{
      void operator()(Interface* ptr){
        if( ptr != &sInstance )
          delete ptr;
      }
    };

	public:
    using Ptr = std::unique_ptr<Interface, Deleter>;

	protected:

    static Interface sInstance;

    MetaValueBaseImplementation() = default;
		
		MetaValueBaseImplementation(const Interface& copy) = default;

  public:

    virtual ~MetaValueBaseImplementation() = default;

		Interface& operator=(const Interface& copy) = delete;

		virtual Interface& operator=( Interface&& movee);
	
		virtual Ptr copy() const;

    virtual Data get( Attributes a ) const;

		virtual bool set(Attributes a, Data d);

		virtual Interface& unaryOp( UnaryOp op);

		virtual Interface& binaryOp( BinaryOp op, const Interface& b);

		virtual Ptr binaryConstOp( BinaryConstOp op, const Interface& b ) const;

		virtual Interface& scale(const MetaScale& scale);
		
    virtual std::ostream& print( std::ostream& o ) const;

	friend class MetaFactoryImplementation;
  friend class MetaValue;
};

std::ostream& operator<<(std::ostream& o, const MetaValueBaseImplementation& mvbi);
