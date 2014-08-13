#pragma once

#include <DeSerializer.h>
#include <EventTypePublisher.h>
#include <FormatID.h>

#include <mw/api/SubscriberEventChannel.h>
#include <case/Delegate.h>
#include <IO.h>

template<typename ECH, typename SensorEvent>
class SensorDataSubscriber : public famouso::mw::api::SubscriberEventChannel<ECH>{
  public:
    using SensorEventType = SensorEvent;

  private:
    using Base = famouso::mw::api::SubscriberEventChannel<ECH>;
    using Subject = famouso::mw::Subject;
    using Callback = famouso::util::Delegate<const SensorEvent&>;

    EventTypePublisher<ECH> mEtp;
    FormatID mId;

    void handle(famouso::mw::api::SECCallBackData& cbd){
      SensorEvent e;
      FormatID id;
      DeSerializer<const uint8_t*> d(cbd.data, cbd.data+cbd.length);
      d >> id >> e;
      if(!(id == mId)){
        std::cout << "Invalid FormatID [" << id << "] should be [" << mId << "] dropping event" << std::endl;
        return;
      }

      if(!d.error() && callback)
        callback(e);
    }

  public:

    Callback callback;

    SensorDataSubscriber(const famouso::mw::Subject& s, uint16_t nodeID) : Base(s), mId(nodeID, FormatID::Direction::subscriber){
      Base::callback.template bind<SensorDataSubscriber, &SensorDataSubscriber::handle>(this);
    }

    void subscribe(){
      mEtp.publish(Base::subject(), mId, SensorEvent());
      Base::subscribe();
    }
};

