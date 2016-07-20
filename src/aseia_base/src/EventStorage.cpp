#include <EventStorage.h>

#include <MetaEvent.h>

class EventStorageImpl {
	public:
		virtual ~EventStorageImpl() = default;
		virtual void addEvent(const MetaEvent& e) =0;
		virtual void purge() =0;
		virtual MetaEvent executeTransform(Transformation& t) const =0;
};

class SimpleStorage : public EventStorageImpl {
	private:
		MetaEvent mE;

	public:
		virtual void addEvent(const MetaEvent& e) {
			mE = e;
		}

		virtual void purge() {
			mE = MetaEvent();
		}

		virtual MetaEvent executeTransform(Transformation& t) const {
			if(t.check({&mE}))
				return t({&mE});
			else
				return MetaEvent();
		}
};


EventStorage::EventStorage(std::size_t inputSize, Policy policy) {
	if(inputSize==1)
		mImpl= new SimpleStorage();
	/*if(inputSize>1)
		switch(policy) {
			case(Policy::recent):             mImpl.reset(new RecentStorage(inputSize));
													            	break;
			case(Policy::minimalUncertainty): mImpl.reset(new MinimalUncertaintyStorage(inputSize));
														            break;
			case(Policy::performance):        mImpl.reset(new PerformanceStorage(inputSize));
																				break;
		}*/
}

EventStorage::~EventStorage() { if(mImpl) delete mImpl; }
void EventStorage::addEvent(const MetaEvent& e) { mImpl->addEvent(e); }
void EventStorage::purge() { mImpl->purge(); }
MetaEvent EventStorage::executeTransform(Transformation& t) const { return mImpl->executeTransform(t); }

