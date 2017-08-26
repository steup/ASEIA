#include <EventID.h>

#include <Prime.h>
#include <EventType.h>

EventID::IDType EventID::idGen(const EventType& eT) {
  IDType id=1;
  for(const AttributeType& aT : eT)
    id *= PrimeGenerator::prime(aT.id());
  return id;
}

EventID::IDType EventID::idGen(const EventID::InitType& attrs) {
  IDType id=1;
  for(uint8_t attr : attrs)
    id *= PrimeGenerator::prime(attr);
  return id;
}

EventID& EventID::operator/=(id::attribute::ID attr) {
  IDType attrID = PrimeGenerator::prime(attr);
  if(mID % attrID == 0)
    mID /= attrID;
  return *this;
}

EventID& EventID::operator*=(id::attribute::ID attr){
  IDType attrID = PrimeGenerator::prime(attr);
  if(mID % attrID != 0)
    mID *= attrID;
  return *this;
}

const EventID EventID::any;
