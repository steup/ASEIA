#include <MetaEvent.h>
#include <memory>

using namespace std;

MetaEvent::iterator::iterator(Storage::iterator iter)
	: Storage::iterator(iter) {}

MetaAttribute& MetaEvent::iterator::operator*() { 
	return this->Storage::iterator::operator*().second; 
}

MetaEvent::const_iterator::const_iterator(Storage::const_iterator iter)
	: Storage::const_iterator(iter) {}

const MetaAttribute& MetaEvent::const_iterator::operator*() const { 
	return this->Storage::const_iterator::operator*().second; 
}
			
const MetaAttribute* MetaEvent::attribute(id::attribute::ID id) const {
	Storage::const_iterator iter = mStorage.find(id);
	if(iter != mStorage.end())
    return &(iter->second);
  else
    return nullptr;
}

MetaAttribute* MetaEvent::attribute(id::attribute::ID id){
	Storage::iterator iter = mStorage.find(id);
	if(iter != mStorage.end())
    return &(iter->second);
  else
    return nullptr;
}

bool MetaEvent::add(const MetaAttribute& mA) {
  auto res = mStorage.emplace(mA.id(), mA.id());
  if(res.second) {
    res.first->second = mA;
    return true;
  } else
    return false;
}

bool MetaEvent::add(MetaAttribute&& mA) {
  auto res = mStorage.emplace(mA.id(), mA.id());
  if(res.second) {
    res.first->second = move(mA);
    return true;
  } else
    return false;
}

bool MetaEvent::operator==(const MetaEvent& b) const {
  if(mStorage.size() != b.mStorage.size())
    return false;
  for(const MetaAttribute& mA : *this) {
    const MetaAttribute* bPtr = b.attribute(mA.id());
    if(! bPtr || mA != *bPtr)
      return false;
  }
  return true;
}

MetaEvent::iterator MetaEvent::begin() noexcept { 
	return iterator(mStorage.begin()); 
}

MetaEvent::const_iterator MetaEvent::begin() const noexcept { 
	return const_iterator(mStorage.begin()); 
}

MetaEvent::iterator MetaEvent::end() noexcept { 
	return iterator(mStorage.end()); 
}

MetaEvent::const_iterator MetaEvent::end() const noexcept { 
	return const_iterator(mStorage.end()); 
}

ostream& operator<<(ostream& o, const MetaEvent& me) {
	o << "Event: " << endl;
	for(const MetaAttribute& ma : me)
		o << "\t" << ma << endl;
	return o;
}	
