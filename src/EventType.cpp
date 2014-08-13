#include <EventType.h>

EventType::iterator::iterator(StorageType::iterator i) : i(i) {}

EventType::iterator& EventType::iterator::operator++() { 
  i++; 
  return *this;
}

EventType::ValueType& EventType::iterator::operator*() {
  return i->second;
}

bool EventType::iterator::operator==(const iterator& b) { 
  return i == b.i;
}

bool EventType::iterator::operator!=(const iterator& b) { 
  return !(*this== b);
}

EventType::iterator EventType::begin() { 
  return iterator(mStorage.begin());
}

EventType::iterator EventType::end() { 
  return iterator(mStorage.end());
}

EventType::Parser::Parser(StorageType& storage) : mStorage(storage) {}

EventType::const_iterator::const_iterator(StorageType::const_iterator i) : i(i) {}

EventType::const_iterator& EventType::const_iterator::operator++() { 
  i++; 
  return *this;
}

const EventType::ValueType& EventType::const_iterator::operator*() const { 
  return i->second;
}

bool EventType::const_iterator::operator==(const const_iterator& b) { 
  return i == b.i;
}

bool EventType::const_iterator::operator!=(const const_iterator& b) { 
  return !(*this== b);
}

EventType::const_iterator EventType::begin() const { 
  return const_iterator(mStorage.cbegin());
}

EventType::const_iterator EventType::end() const { 
  return const_iterator(mStorage.cend()); 
}

const AttributeType* EventType::attribute(KeyType key) const{
  auto i = mStorage.find(key);
  if(i == mStorage.end())
    return NULL;
  else
    return &i->second;
}

bool EventType::operator==(const EventType& b) const{
  if(mStorage.size() != b.mStorage.size())
    return false;

  for( const auto& p : mStorage ){
    auto i = b.mStorage.find(p.first);
    if( i == b.mStorage.end())
      return false;
    if( !(p.second == i->second) )
      return false;
  }
  return true;
}

uint8_t EventType::length() const { 
  return mStorage.size();
}

std::size_t EventType::size() const throw() {
  return AttributeType::size()*length()+1;
}
