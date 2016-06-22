#include <TypeRegistry.h>

#include <EventType.h>


struct KeyImpl  : public TypeRegistry::Key {
	KeyImpl(const Key& k) : TypeRegistry::Key(k) {}
	KeyImpl(const EventType& eT) : TypeRegistry::Key(eT) {} 
	KeyImpl(EventID eID, FormatID fID) : TypeRegistry::Key(eID, fID) {}

	operator uint64_t() const {
		uint64_t temp = first;
		temp <<= sizeof(second);
		temp |=  second;
		return temp;
	}

};

#ifndef HASHED_REGISTRY
#include <map>
using Map = std::map<const KeyImpl, const EventType>;
#else
#include <unordered_map>
using Map = std::unordered_map<KeyImpl, const EventType, std::hash<uint64_t>>;
#endif

class TypeRegistryImpl : public TypeRegistry {
	private:
		Map mStorage;
	public:
		virtual bool registerType(const EventType& eT) {
			if(contains(eT))
				return false;
			mStorage.emplace(eT, eT);
			return true;
		}

		virtual const EventType* operator[](const Key& k) const {
			auto i = mStorage.find(k);
			if(i!=mStorage.end())
				return &i->second;
			else
				return nullptr;
		}

		virtual bool contains(const Key& k) const {
			return mStorage.find(k)!=mStorage.end();
		}
} typeRegistry;

TypeRegistry& TypeRegistry::instance() {
	return typeRegistry;
}
