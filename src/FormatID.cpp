#include <FormatID.h>
#include <EventType.h>
#include <Serializer.h>

#include <MurmurHash2.h>

FormatID::FormatID(const EventType&  eT) {
  uint8_t buffer[eT.size()];
  Serializer<uint8_t*> s(buffer);
  s << eT;
  mHash = MurmurHash2(buffer, sizeof(buffer), 0);
}
