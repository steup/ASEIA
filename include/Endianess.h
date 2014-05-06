#pragma once

#include <cstdint>
#include <endian.h>

enum class Endianess : uint8_t
{
  big,
  little
};

#if __BYTE_ORDER == __LITTLE_ENDIAN
  const Endianess hostEndianess=Endianess::little;
#else
  const Endianess hostEndianess=Endianess::big;
#endif

template<std::size_t n>
void byteSwap(std::uint8_t* s)
{
  static_assert(n==2||n==4||n==8, "Only storages of size 2, 4 or 8 can be byte swapped");
  for(std::uint8_t i=0;i<n/2;i++)
  {
    std::uint8_t t=s[i];
    s[i]=s[n-i-1];
    s[n-i-1]=t;
  }
}

template<typename T>
inline T betole(T v)
{
  byteSwap<sizeof(T)>(reinterpret_cast<uint8_t*>(&v));
  return v;
}

template<typename T>
inline T letobe(T v)
{
  return betole(v);
}

template<typename T>
inline T hton(T v)
{
  if(hostEndianess==Endianess::little)
    return letobe(v);
  else
    return v;
}

template<typename T>
inline T ntoh(T v)
{
  if(hostEndianess==Endianess::little)
    return betole(v);
  else
    return v;
}
