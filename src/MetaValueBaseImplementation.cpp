#include <MetaValueBaseImplementation.h>
#include <ostream>

using namespace std;
using MVBI      = MetaValueBaseImplementation;
using Interface = MVBI::Interface;
using Ptr       = MVBI::Ptr;
using Data      = MVBI::Data;

MVBI MVBI::sInstance;

Interface& MVBI::operator=( Interface&& movee) { 
	return *this; 
}

Ptr MVBI::copy() const {
	return Ptr(&sInstance, MVBI::Deleter());
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

Interface& MVBI::unaryOp( UnaryOp op) {
	return *this; 
}

Interface& MVBI::binaryOp( BinaryOp op, const Interface& b) { 
	return *this;	
}

Ptr MVBI::binaryConstOp( BinaryConstOp op, const Interface& b ) const { 
	return copy();
} 
		
Interface& MVBI::scale(const MetaScale& scale){
	return *this;
}
		
ostream& MVBI::print( ostream& o ) const { 
      return o << "void"; 
}

ostream& operator<<( ostream& o, const MVBI& mvbi) {
  return mvbi.print(o);
}
