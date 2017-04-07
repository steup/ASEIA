#include <EventStorage.h>

#include <MetaEvent.h>

using StoragePtr = EventStorage::StoragePtr;

class SimpleStorage : public EventStorage {
	private:
		MetaEvent mE;

	public:
		virtual void addEvent(const MetaEvent& e) {
			mE = e;
		}

		virtual void purge() {
			mE = MetaEvent();
		}

		virtual MetaEvent executeTransform(Transformer& t) const {
			if(t.check({&mE}))
				return t({&mE});
			else
				return MetaEvent();
		}
};

StoragePtr EventStorage::create(Type type, Policy policy) {
  if(type == Type::simple)
    return StoragePtr(new SimpleStorage());
  return StoragePtr();
}
