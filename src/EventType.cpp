#include <EventType.h>

#include <EventID.h>
#include <FormatID.h>

EventType::const_iterator::const_iterator(StorageType::const_iterator i)
  : mI(i) {}

const EventType::ValueType& EventType::const_iterator::operator*() const { 
  return mI->second;
}

const EventType::ValueType* EventType::const_iterator::operator->() const { 
  return &mI->second;
}

EventType::const_iterator& EventType::const_iterator::operator++() {
	++mI;
	return *this;
}

EventType::const_iterator EventType::const_iterator::operator++(int) {
	const_iterator temp = *this;
	mI++;
	return temp;
}

EventType::const_iterator EventType::begin() const { 
  return const_iterator(mStorage.cbegin());
}

EventType::const_iterator EventType::end() const {
  return const_iterator(mStorage.cend());
}

bool EventType::const_iterator::operator==(const const_iterator& b) const {
	return mI==b.mI;
}

bool EventType::const_iterator::operator!=(const const_iterator& b) const {
	return mI!=b.mI;
}

const AttributeType* EventType::attribute(KeyType key) const{
  auto i = mStorage.find(key);
  if(i == mStorage.end())
    return NULL;
  else
    return &i->second;
}

AttributeType* EventType::attribute(KeyType key) {
  auto i = mStorage.find(key);
  if(i == mStorage.end())
    return NULL;
  else
    return &i->second;
}

bool EventType::operator<(const EventType& b) const {
  EventID aEID = (EventID)*this;
  EventID bEID = (EventID)b;
  FormatID aFID = (FormatID)*this;
  FormatID bFID = (FormatID)b;
  if(aEID<bEID)
    return true;
  if(aEID>bEID)
    return false;
  return aFID<bFID;
}

bool EventType::operator==(const EventType& b) const{
  EventID aEID = (EventID)*this;
  EventID bEID = (EventID)b;
  FormatID aFID = (FormatID)*this;
  FormatID bFID = (FormatID)b;
  return aEID == bEID && aFID == bFID;
}

size_t EventType::operator-(const EventType& b) const{
  size_t value=0;
  for( const auto& p : mStorage ){
    auto i = b.mStorage.find(p.first);
    if( i == b.mStorage.end())
      continue;
    value+=p.second - i->second;
  }
  return value;
}

bool EventType::isCompatible(const EventType& b) const {
  EventID aEID = (EventID)*this;
  EventID bEID = (EventID)b;
  FormatID aFID = (FormatID)*this;
  FormatID bFID = (FormatID)b;
  if (aEID == bEID && aFID == bFID)
    return true;
  if(!aEID.isCompatible(bEID))
    return false;
  for(const AttributeType& aT : b)
    if(aT != (*this)[aT.id()])
      return false;
  return true;
}

uint8_t EventType::length() const {
  return mStorage.size();
}

std::size_t EventType::size() const throw() {
  return AttributeType::size()*length()+1;
}
