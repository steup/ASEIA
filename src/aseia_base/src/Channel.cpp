#include <Channel.h>

Channel::Channel(const EventType& out, const Channel::EventTypes& in, TransID trans)
  : mOutType(out),
    mInTypes(in),
    mTrans(TransformationFactory::instance().create(out, in, trans))
{ }

void Channel::handleEvent(const MetaEvent& e) {
  EventID eID(e);
  if(mBuffer.find(eID)==mBuffer.end())
    mBuffer.insert(std::make_pair(eID, std::vector<MetaEvent>()));
  mBuffer[eID].push_back(e);
  //TODO: check transform on all event combinations
}

std::ostream& operator<<(std::ostream& o, const Channel& c) {
  return o;
}
