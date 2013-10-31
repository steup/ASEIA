#pragma once

#include <vector>
#include <cstdint>
#include <array>

using DynamicPacketBuffer=std::vector<std::uint8_t>;

template<std::size_t n>
using StaticPacketBuffer=std::array<std::uint8_t, n>;

template<std::size_t n>
inline std::ostream& operator<<(std::ostream& o, StaticPacketBuffer<n>& p)
{
  auto flagSave=o.flags();
  o << std::hex;
  for(auto i : p)
    o << "0x" << (uint16_t)i << " ";
  o.flags(flagSave);
  return o;
}

inline std::ostream& operator<<(std::ostream& o, DynamicPacketBuffer& p)
{
  auto flagSave=o.flags();
  o << std::hex;
  for(auto i : p)
    o << "0x" << (uint16_t)i << " ";
  o.flags(flagSave);
  return o;
}

template<typename PacketBufferIterator, typename T>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const T v)
{
    for(unsigned int i=0;i<sizeof(T);i++)
      *it++=reinterpret_cast<const uint8_t*>(&v)[i];
    return it;
}

template<typename PacketBufferConstIterator, typename T>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& it, T& v)
{
  for(unsigned int i=0;i<sizeof(T);i++)
    reinterpret_cast<uint8_t*>(&v)[i]=*it++;
  return it;
}

template<typename PacketBufferIterator, typename T, std::size_t n>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const std::array<T,n>& a)
{
    for(auto v : a)
      it << v;
    return it;
}

template<typename PacketBufferConstIterator, typename T, std::size_t n>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& it, std::array<T,n>& a)
{
    for(auto& v : a)
      it >> v;
    return it;
}
