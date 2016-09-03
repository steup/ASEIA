#pragma once

#include <Singleton.h>
#include <AbstractRegistry.h>
#include <EventType.h>

struct TypeRegistryImpl : public AbstractRegistry<EventType> {
	using Base = AbstractRegistry<EventType>;
	using Base::value_type;
	using Base::const_iterator;
	using Base::const_local_iterator;
	using Base::Range;
	using Base::LocalRange;
  void registerType(const EventType& eT) {
    this->AbstractRegistry<EventType>::registerType(eT, eT);
  }
};

using TypeRegistry = Singleton<TypeRegistryImpl>;
