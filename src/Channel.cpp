#include <Channel.h>

Channel::Channel(Channel::TransPtr&& trans)
  : mTrans(std::move(trans)),
		mStore(mTrans->in().size())
{ }

Channel::Channel(Channel&& movee)
  : mTrans(std::move(movee.mTrans)),
    mStore(movee.mStore)
{}

void Channel::handleEvent(const MetaEvent& e) {
  mStore.addEvent(e);
	MetaEvent newE = mStore.executeTransform(*mTrans);
	if(newE != MetaEvent())
		publishEvent(newE);
}

/* TODO: implement */
std::ostream& operator<<(std::ostream& o, const Channel& c) {
  return o;
}
