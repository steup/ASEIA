#pragma once

#include <EventID.h>
#include <EventStorage.h>

#include <TransformationFactory.h>

#include <iosfwd>
#include <memory>
#include <vector>

class EventType;
class MetaEvent;

class Channel{
  public:
    using TransID = TransformationFactory::TransID;
    using TransPtr = TransformationFactory::TransPtr;
    using EventTypes = TransformationFactory::EventTypes;
  protected:
    const EventType& mOutType;
    const EventTypes mInTypes;
    const TransPtr   mTrans;
    EventStorage     mStore;
		
    void handleEvent(const MetaEvent& e);
    virtual void publishEvent(const MetaEvent& e) const =0;
  public:
    Channel(const EventType& out, const EventTypes& in, TransID trans);
};

std::ostream& operator<<(std::ostream& o, const Channel& c);
