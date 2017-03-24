#include <Channel.h>

#include <FormatID.h>
#include <IO.h>

Channel::Channel(Channel::TransPtr&& trans)
  : mTrans(std::move(trans))
{
  if(mTrans && mTrans->in().size() == 1)
    mStore = EventStorage::create(EventStorage::Type::simple, EventStorage::Policy::recent);
}

Channel::Channel(Channel&& movee)
  : mTrans(std::move(movee.mTrans)),
    mStore(std::move(movee.mStore))
{}

void Channel::handleEvent(const MetaEvent& e) {
  if(mTrans && mStore) {
    mStore->addEvent(e);
	  MetaEvent newE = mStore->executeTransform(*mTrans);
	  if(newE != MetaEvent())
		  publishEvent(newE);
  }
}

/* TODO: implement */
std::ostream& operator<<(std::ostream& o, const Channel& c) {
  o << "[";
  for(const EventType& eT : c.mTrans->in())
    o << (EventID)eT << "/" << (FormatID)eT << ",";
  o << "] -> " << (EventID)c.mTrans->out() << "/" << (FormatID)c.mTrans->out() << ":" << std::endl << *c.mTrans;
  return o;
}
