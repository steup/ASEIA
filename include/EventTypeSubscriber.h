#pragma once

#include <DeSerializer.h>
#include <EventType.h>

#include <mw/api/SubscriberEventChannel.h>

template<typename ECH>
class EventTypeSubscriber : public famouso::mw::api::SubscriberEventChannel<ECH>{
  private:
    using Base = famouso::mw::api::SubscriberEventChannel<ECH>;

  public:
    struct CallbackData{
      famouso::mw::Subject sub;
      uint32_t formatID;
      EventType eT;
    };

  private:
    void handle(famouso::mw::api::SECCallBackData& cbd){
      CallbackData data;
      DeSerializer<uint8_t*> d(cbd.data, cbd.data+cbd.length);
      uint64_t subject;
      d >> subject >> data.formatID >> data.eT;
      data.sub=famouso::mw::Subject(subject);
      callback(data);
    }
  public:

    using Callback = famouso::util::Delegate<const CallbackData&>;

    Callback callback;

    EventTypeSubscriber() : Base("!SENSORS"){
      Base::subscribe();
      Base::callback.template bind<EventTypeSubscriber, &EventTypeSubscriber::handle>(this);
    }
};
