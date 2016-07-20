#include <Channel.h>

Channel::Channel(const EventType& out, const Channel::EventTypes& in, TransID trans)
  : mOutType(out),
    mInTypes(in),
    mTrans(TransformationFactory::instance().create(out, in, trans)),
		mStore(mInTypes.size())
{ }

void Channel::handleEvent(const MetaEvent& e) {
  mStore.addEvent(e);
	MetaEvent newE = mStore.executeTransform(*mTrans);
	if(newE != MetaEvent())
		publishEvent(newE);
}

std::ostream& operator<<(std::ostream& o, const Channel& c) {
  return o;
}
