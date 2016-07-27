#include <EventID.h>

#include <Prime.h>
#include <EventType.h>

uint32_t EventID::idGen(const EventType& eT) {
  uint32_t id=1;
  for(const AttributeType& aT : eT)
    id *= PrimeGenerator::prime(aT.id());
  return id;
}

const EventID EventID::any;
