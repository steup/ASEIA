#pragma once

#include <Singleton.h>
#include <AbstractRegistry.h>
#include <EventType.h>

struct TypeRegistryImpl : public AbstractRegistry<EventType> {
  void registerType(const EventType& eT) {
    this->AbstractRegistry<EventType>::registerType(eT, eT);
  }
};

using TypeRegistry = Singleton<TypeRegistryImpl>;
