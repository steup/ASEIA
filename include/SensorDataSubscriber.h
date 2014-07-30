#pragma once

#include <DeSerializer.h>
#include <EventTypePublisher.h>
#include <FormatID.h>

#include <mw/api/SubscriberEventChannel.h>
#include <case/Delegate.h>

template<typename ECH, typename SensorEvent>
class SensorDataSubscriber : public famouso::mw::api::SubscriberEventChannel<ECH>{
  public:
    using SensorEventType = SensorEvent;

  private:
    using Base = famouso::mw::api::SubscriberEventChannel<ECH>;
    using Subject = famouso::mw::Subject;
    using Callback = famouso::util::Delegate<const SensorEvent&>;

    EventTypePublisher<ECH> etp;
    FormatID id;

    void handle(famouso::mw::api::SECCallBackData& cbd){
      SensorEvent e;
      DeSerializer<const uint8_t*> d(cbd.data, cbd.data+cbd.length);
      d >> e;
      if(!d.error() && callback)
        callback(e);
    }

  public:

    Callback callback;

    SensorDataSubscriber(const famouso::mw::Subject& s, uint16_t nodeID) : Base(s), id(nodeID){
      Base::callback.template bind<SensorDataSubscriber, &SensorDataSubscriber::handle>(this);
    }

    void subscribe(){
      etp.publish(Base::subject(), id.value(), SensorEvent());
      Base::subscribe();
    }
};

