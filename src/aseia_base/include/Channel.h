#pragma once

#include <MetaEvent.h>
#include <EventID.h>

#include <TransformationFactory.h>

#include <iosfwd>
#include <memory>
#include <vector>

class EventType;
class Transformation;

class Channel{
  public:
    using TransID = TransformationFactory::TransID;
    using EventTypes = TransformationFactory::EventTypes;
  protected:
    const EventType& mOutType;
    const EventTypes mInTypes;
    const std::unique_ptr<Transformation> mTrans;
    std::map<EventID, std::vector<MetaEvent>> mBuffer;
    void handleEvent(const MetaEvent& e);
    virtual void publishEvent(MetaEvent e) const =0;
  public:
    Channel(const EventType& out, const EventTypes& in, TransID trans);
};

std::ostream& operator<<(std::ostream& o, const Channel& c);
