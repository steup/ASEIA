#include <EventID.h>

#include <Prime.h>
#include <EventType.h>

uint32_t EventID::idGen(const EventType& eT) {
  uint32_t id=1;
  for(const AttributeType& aT : eT)
    id *= PrimeGenerator::prime(aT.id());
  return id;
}

uint32_t EventID::idGen(const initializer_list<uint8_t>& attrs) {
  uint32_t id=1;
  for(uint8_t attr : attrs)
    id *= PrimeGenerator::prime(attr);
  return id;
}

const EventID EventID::any;
