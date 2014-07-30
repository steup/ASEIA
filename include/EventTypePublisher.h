#pragma once

#include <Serializer.h>
#include <EventType.h>

#include <mw/api/PublisherEventChannel.h>

template<typename ECH>
class EventTypePublisher : public famouso::mw::api::PublisherEventChannel<ECH>{
  private:
    using Base = famouso::mw::api::PublisherEventChannel<ECH>;

  public:
    EventTypePublisher() : Base("!SENSORS"){
      Base::announce();
    }

    void publish(const famouso::mw::Subject& sub, uint32_t id, const EventType& e){
      famouso::mw::Event fE(this->subject());
      uint8_t buffer[e.size()+12];
      Serializer<uint8_t*> s(buffer, buffer+sizeof(buffer));
      s << sub.value() << id << e;
      fE.data=buffer;
      fE.length=sizeof(buffer);
      Base::publish(fE);
    }
};
