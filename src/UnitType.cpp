#include <UnitType.h>

UnitType::DimensionConverter::DimensionConverter(StorageType& storage) : mStorage(storage) {}

UnitType::iterator UnitType::begin() { 
  return mStorage.begin();
}

UnitType::iterator UnitType::end() { 
  return mStorage.end();
}

int8_t UnitType::operator[](uint8_t i) const{
  if(i<mStorage.size())
    return mStorage[i];
  return 0;
}

UnitType::const_iterator UnitType::begin() const { 
  return mStorage.cbegin();
}

UnitType::const_iterator UnitType::end() const { 
  return mStorage.cend();
}

bool UnitType::operator==(const UnitType& b) const{
  for(uint8_t i=0;i<mStorage.size();i++)
    if(mStorage[i]!=b.mStorage[i])
        return false;
  return true;
}
