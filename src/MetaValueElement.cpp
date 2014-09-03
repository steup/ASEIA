#include <MetaValueElement.h>

MetaValueElement::MetaValueElement(const MetaValueElement& mve) : mId(mve.mId) {

  switch(mve.mId){

      case(id::type::Int8::value()) : 
      case(id::type::Int16::value()):
      case(id::type::Int32::value()):
      case(id::type::Int64::value()):  _int = mve._int;
                                       break;
      case(id::type::UInt8::value()) : 
      case(id::type::UInt16::value()):
      case(id::type::UInt32::value()):
      case(id::type::UInt64::value()): _uint = mve._uint;
                                       break;

      case(id::type::Float::value()) : _float = mve._float;
                                       break;

      case(id::type::Double::value()): _double = mve._double;
                                       break;
  }
}

void MetaValueElement::convert(id::type::ID newType) {
  ValueElement<uint64_t, true> tempU;
  ValueElement<int64_t , true> tempI;
  ValueElement<float   , true> tempF;
  ValueElement<double  , true> tempD;

  switch(mId){
      case(id::type::Int8::value()) : 
      case(id::type::Int16::value()):
      case(id::type::Int32::value()):
      case(id::type::Int64::value()):  tempU = _int;
                                       tempI = _int;
                                       tempF = _int;
                                       tempD = _int;
                                       break;
      case(id::type::UInt8::value()) : 
      case(id::type::UInt16::value()):
      case(id::type::UInt32::value()):
      case(id::type::UInt64::value()): tempU = _uint;
                                       tempI = _uint;
                                       tempF = _uint;
                                       tempD = _uint;
                                       break;

      case(id::type::Float::value()) : tempU = _float;
                                       tempI = _float;
                                       tempF = _float;
                                       tempD = _float;
                                       break;

      case(id::type::Double::value()): tempU = _double;
                                       tempI = _double;
                                       tempF = _double;
                                       tempD = _double;
                                       break;
  }

  switch(newType){
      case(id::type::Int8::value()) : 
      case(id::type::Int16::value()):
      case(id::type::Int32::value()):
      case(id::type::Int64::value()):  _int = tempI;
                                       break;
      case(id::type::UInt8::value()) : 
      case(id::type::UInt16::value()):
      case(id::type::UInt32::value()):
      case(id::type::UInt64::value()): _uint = tempU;
                                       break;

      case(id::type::Float::value()) : _float = tempF;
                                       break;

      case(id::type::Double::value()): _double= tempD;
                                       break;
  }
  mId=newType;
}

void MetaValueElement::equalize(MetaValueElement& b){
  if(mId == b.mId)
    return;
  id::type::ID common = std::max(mId, b.mId);
  convert(common);
  b.convert(common);
}

MetaValueElement& MetaValueElement::MetaValueElement::operator+=(const MetaValueElement& b){
  MetaValueElement temp(b);
  equalize(temp);
  switch( mId ){
    case(id::type::UInt8::value()):  
    case(id::type::UInt16::value()): 
    case(id::type::UInt32::value()): 
    case(id::type::UInt64::value()): _uint += temp._uint;
                                     break;

    case(id::type::Int8::value())  : 
    case(id::type::Int16::value()) : 
    case(id::type::Int32::value()) : 
    case(id::type::Int64::value()) : _int += temp._int;
                                     break;

    case(id::type::Float::value()) : _float += temp._float;
                                     break;

    case(id::type::Double::value()): _double += temp._double;
                                     break;
  }
  return *this;
}

MetaValueElement& MetaValueElement::operator-=(const MetaValueElement& b){
  MetaValueElement temp(b);
  equalize(temp);
  switch( mId ){
    case(id::type::UInt8::value()):  
    case(id::type::UInt16::value()): 
    case(id::type::UInt32::value()): 
    case(id::type::UInt64::value()): _uint -= temp._uint;
                                     break;

    case(id::type::Int8::value())  : 
    case(id::type::Int16::value()) : 
    case(id::type::Int32::value()) : 
    case(id::type::Int64::value()) : _int -= temp._int;
                                     break;

    case(id::type::Float::value()) : _float -= temp._float;
                                     break;

    case(id::type::Double::value()): _double -= temp._double;
                                     break;
  }
  return *this;
}

MetaValueElement& MetaValueElement::operator*=(const MetaValueElement& b){
  MetaValueElement temp(b);
  equalize(temp);
  switch( mId ){
    case(id::type::UInt8::value()):  
    case(id::type::UInt16::value()): 
    case(id::type::UInt32::value()): 
    case(id::type::UInt64::value()): _uint *= temp._uint;
                                     break;

    case(id::type::Int8::value())  : 
    case(id::type::Int16::value()) : 
    case(id::type::Int32::value()) : 
    case(id::type::Int64::value()) : _int *= temp._int;
                                     break;

    case(id::type::Float::value()) : _float *= temp._float;
                                     break;

    case(id::type::Double::value()): _double *= temp._double;
                                     break;
  }
  return *this;
}

MetaValueElement& MetaValueElement::operator/=(const MetaValueElement& b){
  MetaValueElement temp(b);
  equalize(temp);
  switch( mId ){
    case(id::type::UInt8::value()):  
    case(id::type::UInt16::value()): 
    case(id::type::UInt32::value()): 
    case(id::type::UInt64::value()): _uint /= temp._uint;
                                     break;

    case(id::type::Int8::value())  : 
    case(id::type::Int16::value()) : 
    case(id::type::Int32::value()) : 
    case(id::type::Int64::value()) : _int /= temp._int;
                                     break;

    case(id::type::Float::value()) : _float /= temp._float;
                                     break;

    case(id::type::Double::value()): _double /= temp._double;
                                     break;
  }
  return *this;
}

MetaValueElement MetaValueElement::operator+(const MetaValueElement& b) const{
  MetaValueElement a(*this);
  MetaValueElement temp(b);
  a.equalize(temp);
  switch( mId ){
    case(id::type::UInt8::value()):  
    case(id::type::UInt16::value()): 
    case(id::type::UInt32::value()): 
    case(id::type::UInt64::value()): a._uint += temp._uint;
                                     break;

    case(id::type::Int8::value())  : 
    case(id::type::Int16::value()) : 
    case(id::type::Int32::value()) : 
    case(id::type::Int64::value()) : a._int += temp._int;
                                     break;

    case(id::type::Float::value()) : a._float += temp._float;
                                     break;

    case(id::type::Double::value()): a._double += temp._double;
                                     break;
  }
  return a;
}

MetaValueElement MetaValueElement::operator-(const MetaValueElement& b) const{
  MetaValueElement a(*this);
  MetaValueElement temp(b);
  a.equalize(temp);
  switch( a.mId ){
    case(id::type::UInt8::value()):  
    case(id::type::UInt16::value()): 
    case(id::type::UInt32::value()): 
    case(id::type::UInt64::value()): a._uint -= temp._uint;
                                     break;

    case(id::type::Int8::value())  : 
    case(id::type::Int16::value()) : 
    case(id::type::Int32::value()) : 
    case(id::type::Int64::value()) : a._int -= temp._int;
                                     break;

    case(id::type::Float::value()) : a._float -= temp._float;
                                     break;

    case(id::type::Double::value()): a._double -= temp._double;
                                     break;
  }
  return a;
}

MetaValueElement MetaValueElement::operator*(const MetaValueElement& b) const{
  MetaValueElement a(*this);
  MetaValueElement temp(b);
  a.equalize(temp);
  switch( a.mId ){
    case(id::type::UInt8::value()):  
    case(id::type::UInt16::value()): 
    case(id::type::UInt32::value()): 
    case(id::type::UInt64::value()): a._uint *= temp._uint;
                                     break;

    case(id::type::Int8::value())  : 
    case(id::type::Int16::value()) : 
    case(id::type::Int32::value()) : 
    case(id::type::Int64::value()) : a._int *= temp._int;
                                     break;

    case(id::type::Float::value()) : a._float *= temp._float;
                                     break;

    case(id::type::Double::value()): a._double *= temp._double;
                                     break;
  }
  return a;
}

MetaValueElement MetaValueElement::operator/(const MetaValueElement& b) const{
  MetaValueElement a(*this);
  MetaValueElement temp(b);
  a.equalize(temp);
  switch( a.mId ){
    case(id::type::UInt8::value()):  
    case(id::type::UInt16::value()): 
    case(id::type::UInt32::value()): 
    case(id::type::UInt64::value()): a._uint /= temp._uint;
                                     break;

    case(id::type::Int8::value())  : 
    case(id::type::Int16::value()) : 
    case(id::type::Int32::value()) : 
    case(id::type::Int64::value()) : a._int /= temp._int;
                                     break;

    case(id::type::Float::value()) : a._float /= temp._float;
                                     break;

    case(id::type::Double::value()): a._double /= temp._double;
                                     break;
  }
  return a;
}
