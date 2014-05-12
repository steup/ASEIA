#pragma once

#include <Serializer.h>

#include "mw/api/PublisherEventChannel.h"

template<typename ECH, typename SensorEvent>
class SensorDataPublisher : public famouso::mw::api::PublisherEventChannel<ECH>{
  public:
    using SensorEventType = SensorEvent;

  private:
    using Base = famouso::mw::api::PublisherEventChannel<ECH>;
    using Subject = famouso::mw::Subject;
  public:
    SensorDataPublisher(const Subject& s) : Base(s){}

    void publish(const SensorEvent& e){
      using Wrapper = famouso::mw::Event;
      using Buffer = std::array<uint8_t, SensorEvent::size()>;

      Wrapper wrapper(this->subject());
      Buffer buffer;

      wrapper.data=buffer.data();
      wrapper.length=buffer.size();

      Serializer<typename Buffer::iterator> s(buffer.begin(), buffer.end());
      s << e;

      Base::publish(wrapper);
    }
};
