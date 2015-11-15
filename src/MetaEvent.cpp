#include <MetaEvent.h>

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
			
MetaAttribute& MetaEvent::attribute(id::attribute::ID id) {
	Storage::iterator iter = mStorage.find(id);
	if(iter == mStorage.end()) {
		auto res = mStorage.emplace(id, id);
		iter = res.first;
	}
	return iter->second;
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
