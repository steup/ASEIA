#include <Channel.h>

#include <FormatID.h>

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
  o << "[";
  for(const EventType* eT : c.mTrans->in())
    o << (EventID)*eT << "/" << (FormatID)*eT << ",";
  o << "] -> " << (EventID)c.mTrans->out() << "/" << (FormatID)c.mTrans->out() << ":" << std::endl << *c.mTrans;
  return o;
}
