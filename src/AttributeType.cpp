#include <AttributeType.h>

#include <limits>

using namespace std;

AttributeType::ID AttributeType::   id() const { return mId   ; }
const UnitType&   AttributeType:: unit() const { return mUnit ; }
const ScaleType&  AttributeType::scale() const { return mScale; }
const ValueType&  AttributeType::value() const { return mValue; }
      UnitType&   AttributeType:: unit()       { return mUnit ; }
      ScaleType&  AttributeType::scale()       { return mScale; }
      ValueType&  AttributeType::value()       { return mValue; }

bool AttributeType::operator==(const AttributeType& b) const {
  return mId    == b.mId    &&
         mValue == b.mValue &&
         mScale == b.mScale &&
         mUnit  == b.mUnit  ;
}

size_t AttributeType::operator-(const AttributeType& b) const {
  size_t value=0;
  if(mId != b.mId)
    return numeric_limits<size_t>::max();
  value+=mValue-b.mValue;
  value+=mScale-b.mScale;
  value+=mUnit-b.mUnit;
  return value;
}
