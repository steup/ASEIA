#include <MetaValueBaseImplementation.h>
#include <ostream>

using namespace std;
using MVBI      = MetaValueBaseImplementation;
using Interface = MVBI::Interface;
using Ptr       = MVBI::Ptr;
using Data      = MVBI::Data;

Interface& MVBI::operator=( Interface&& movee) { 
	return *this; 
}

Ptr MVBI::copy() const {
	return Ptr(new MVBI());
}

Data MVBI::get( Attributes a ) const {
	Data res;
	switch(a) {
		case(Attributes::HasUncertainty): res.hasUncertainty = false;
																			break;
		case(Attributes::Size)          : res.size = 0;
																			break;
		case(Attributes::Rows)          : res.size = 0;
																			break;
		case(Attributes::Cols)          : res.size = 0;
																			break;
		case(Attributes::TypeID)        : res.size = id::attribute::Base::value();
																			break;
	}
	return res;
}

bool MVBI::set(Attributes a, Data d) {
	return false;
}

ValueElement<double, true> MetaValueBaseImplementation::get(std::size_t row, std::size_t col) const {
    return ValueElement<double, true>();
}

bool MetaValueBaseImplementation::set(std::size_t row, std::size_t col, ElemInitType elem) {
  return false;
}

Interface& MVBI::unaryOp( UnaryOp op) {
	return *this; 
}

Ptr MVBI::unaryConstOp( UnaryConstOp op ) const { 
	return copy();
} 

Interface& MVBI::binaryOp( BinaryOp op, const Interface& b) { 
	return *this;	
}

Ptr MVBI::binaryConstOp( BinaryConstOp op, const Interface& b ) const { 
	return copy();
} 

MVBI::Ptr MVBI::block(size_t i, size_t j, size_t numI, size_t numJ) const {
  return copy();
}

bool MVBI::block(size_t i, size_t j, Ptr&&){
  return false;
}
MVBI::Ptr MVBI::col(size_t col) const {
  return copy();
}
MVBI::Ptr MVBI::row(size_t row) const {
  return copy();
}
Interface& MVBI::scale(const MetaScale& scale, bool invert){
	return *this;
}
		
ostream& MVBI::print( ostream& o ) const { 
      return o << "void"; 
}

ostream& operator<<( ostream& o, const MVBI& mvbi) {
  return mvbi.print(o);
}
