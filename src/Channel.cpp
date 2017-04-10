#include <Channel.h>

#include <MetaEvent.h>
#include <FormatID.h>
#include <IO.h>

Channel::Channel(TransPtr&& trans) : mTrans(std::move(trans)) {}

//Channel::Channel(Channel&& movee) : mTrans(std::move(movee.mTrans)) {}

void Channel::handleEvent(const MetaEvent& e) {
  for(MetaEvent& gen : (*mTrans)(e))
		  publishEvent(gen);
}

std::ostream& operator<<(std::ostream& o, const Channel& c) {
  o << "[";
  for(const EventType& eT : c.mTrans->in())
    o << (EventID)eT << "/" << (FormatID)eT << ",";
  o << "] -> " << (EventID)c.mTrans->out() << "/" << (FormatID)c.mTrans->out() << ":" << std::endl << *c.mTrans;
  return o;
}
