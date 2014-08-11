#pragma once

#include <stdint.h>
#include <Serializer.h>
#include <DeSerializer.h>
#include <Endianess.h>

class FormatID{
  public:
    enum class Direction : bool{
      publisher = 0,
      subscriber
    };

  private:
   union Data{
     uint32_t value;
     struct{
       uint16_t  nr    : 15;
       Direction dir   :  1;
       uint16_t  node  : 16;
     };
   };

   Data mData;

   static uint16_t sSubNr;
   static uint16_t sPubNr;

  public:
    FormatID() = default;
    FormatID(uint16_t nodeID, Direction dir);
    uint16_t  node() const;
    Direction direction() const;
    uint16_t  nr() const;
    bool operator==(const FormatID& b) const;
    bool operator<(const FormatID& b) const;
    static constexpr std::size_t size() noexcept { return sizeof(mData); }

template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, FormatID&);
template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, const FormatID&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const FormatID& id){
  return s << hton(id.mData.value);
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, FormatID& id){
  d >> id.mData.value;
  id.mData.value=ntoh(id.mData.value);
  return d;
}
