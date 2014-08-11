#pragma once

#include <Serializer.h>
#include <EventTypePublisher.h>
#include <FormatID.h>

#include "mw/api/PublisherEventChannel.h"

template<typename ECH, typename SensorEvent>
class SensorDataPublisher : public famouso::mw::api::PublisherEventChannel<ECH>{
  public:
    using SensorEventType = SensorEvent;

  private:
    using Base = famouso::mw::api::PublisherEventChannel<ECH>;
    using Subject = famouso::mw::Subject;

    EventTypePublisher<ECH> mEtp;
    FormatID mId;
  public:
    SensorDataPublisher(const Subject& s, uint16_t nodeID) : Base(s), mId(nodeID, FormatID::Direction::publisher){}

    void announce(){
      mEtp.publish(Base::subject(), mId, SensorEvent());
      Base::announce();
    }

    void publish(const SensorEvent& e){
      using Wrapper = famouso::mw::Event;
      using Buffer = std::array<uint8_t, FormatID::size()+SensorEvent::size()>;

      Wrapper wrapper(this->subject());
      Buffer buffer;

      wrapper.data=buffer.data();
      wrapper.length=buffer.size();

      Serializer<typename Buffer::iterator> s(buffer.begin(), buffer.end());
      s << mId << e;

      Base::publish(wrapper);
    }
};
