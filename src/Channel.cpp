#include <Channel.h>

#include <MetaEvent.h>
#include <MetaFactory.h>
#include <FormatID.h>
#include <IO.h>

Channel::Channel(TransPtr&& trans) : mTrans(std::move(trans)) {}

//Channel::Channel(Channel&& movee) : mTrans(std::move(movee.mTrans)) {}

void Channel::fixType(MetaEvent& e) const {
  if(!e.valid()) {
    error(Errors::InvalidEvent, e);
    e=MetaEvent();
    return;
  }

  ::id::attribute::ID toRemove[e.length()];
  unsigned int removeCount=0;

  for(const AttributeType& aT : mTrans->out())
    if(!e.attribute(aT.id())) {
      e=MetaEvent();
      error(Errors::MissingAttribute, e);
      return;
    }

  for(MetaAttribute& attr : e) {
    if(!mTrans->out().attribute(attr.id())) {
      toRemove[removeCount++]=attr.id();
      continue;
    }
    const AttributeType& eT=mTrans->out()[attr.id()];

    if(AttributeType(attr) == eT)
      continue;

    AttributeType aT(attr);
    if(aT.scale().reference() != eT.scale().reference() ||
       aT.unit()              != eT.unit()              ||
       aT.value().cols()      <  eT.value().cols()      ||
       aT.value().rows()      <  eT.value().rows()      ){
      error(Errors::IncompatibleType, e);
      e=MetaEvent();
      break;
    }
    if(aT.scale() != eT.scale())
      attr*=MetaScale(eT.scale())/aT.scale();

    if(aT.value().rows() > eT.value().rows() ||
       aT.value().cols() > eT.value().cols() )
      attr.value() = attr.value().block(0,0,eT.value().rows(), eT.value().cols());

    if(aT.value().typeId() != eT.value().typeId()                 ||
       aT.value().hasUncertainty() != eT.value().hasUncertainty() )
      attr.value() = MetaFactory::instance().convert(eT.value(), attr.value());
  }

  for(unsigned int i=0;i<removeCount;i++)
    e.remove(toRemove[i]);
}

void Channel::handleEvent(const MetaEvent& e) {
  for(MetaEvent& gen : (*mTrans)(e)) {
    if(mTrans->out() != EventType(gen))
      fixType(gen);
    if(gen.valid())
      publishEvent(gen);
    else
      error(Errors::InvalidEvent, e);
  }
}

std::ostream& operator<<(std::ostream& o, const Channel& c) {
  o << "[";
  for(const EventType& eT : c.mTrans->in())
    o << (EventID)eT << "/" << (FormatID)eT << ",";
  o << "] -> " << (EventID)c.mTrans->out() << "/" << (FormatID)c.mTrans->out() << ":" << std::endl << *c.mTrans;
  return o;
}
