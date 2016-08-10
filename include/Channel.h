#pragma once

#include <Transformation.h>
#include <EventStorage.h>

#include <iosfwd>

class EventType;
class MetaEvent;

class Channel{
  public:
    using TransPtr   = Transformation::TransPtr;
    using EventTypes = Transformation::EventTypes;
  protected:
    TransPtr    mTrans;
    EventStorage      mStore;

    void handleEvent(const MetaEvent& e);
    virtual void publishEvent(const MetaEvent& e) const =0;
  public:
    Channel() = default;
    Channel(TransPtr&& trans);
    Channel(Channel&& movee);

  friend std::ostream& operator<<(std::ostream&, const Channel&);
};

std::ostream& operator<<(std::ostream& o, const Channel& c);
