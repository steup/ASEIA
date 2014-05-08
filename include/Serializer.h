#pragma once

#include <cstdint>

template<typename PacketBufferIterator>
class Serializer{
  private:
    using Iterator = PacketBufferIterator;
    Iterator mI;
    union Converter{
      std::uint8_t bytes[8];
      std::uint8_t uint8;
      std::uint16_t uint16;
      std::uint32_t uint32;
      std::uint64_t uint64;
      std::int8_t int8;
      std::int16_t int16;
      std::int32_t int32;
      std::int64_t int64;
      float _float;
      double _double;
    };
  public:
    Serializer(Iterator i) : mI(i){}

    const Iterator& iterator() const{return mI;}

    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, std::uint8_t value);
    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, std::uint16_t value);
    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, std::uint32_t value);
    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, std::uint64_t value);
    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, std::int8_t value);
    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, std::int16_t value);
    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, std::int32_t value);
    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, std::int64_t value);
    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, float value);
    template<typename PB> friend Serializer<PB>& operator<<(Serializer<PB>&, double value);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::uint8_t value){
  typename Serializer<PB>::Converter c;
  c.uint8=value;
  for(unsigned int j=0;j<sizeof(std::uint8_t);j++)
    *s.mI++=c.bytes[j];
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::uint16_t value){
  typename Serializer<PB>::Converter c;
  c.uint16=value;
  for(unsigned int j=0;j<sizeof(std::uint16_t);j++)
    *s.mI++=c.bytes[j];
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::uint32_t value){
  typename Serializer<PB>::Converter c;
  c.uint32=value;
  for(unsigned int j=0;j<sizeof(std::uint32_t);j++)
    *s.mI++=c.bytes[j];
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::uint64_t value){
  typename Serializer<PB>::Converter c;
  c.uint64=value;
  for(unsigned int j=0;j<sizeof(std::uint64_t);j++)
    *s.mI++=c.bytes[j];
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::int8_t value){
  typename Serializer<PB>::Converter c;
  c.int8=value;
  for(unsigned int j=0;j<sizeof(std::int8_t);j++)
    *s.mI++=c.bytes[j];
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::int16_t value){
  typename Serializer<PB>::Converter c;
  c.int16=value;
  for(uint8_t j=0;j<sizeof(std::int16_t);j++)
    *s.mI++=c.bytes[j];
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::int32_t value){
  typename Serializer<PB>::Converter c;
  c.int32=value;
  for(unsigned int j=0;j<sizeof(std::int32_t);j++)
    *s.mI++=c.bytes[j];
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::int64_t value){
  typename Serializer<PB>::Converter c;
  c.int64=value;
  for(unsigned int j=0;j<sizeof(std::int64_t);j++)
    *s.mI++=c.bytes[j];
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, float value){
  typename Serializer<PB>::Converter c;
  c._float=value;
  for(unsigned int j=0;j<sizeof(float);j++)
    *s.mI++=c.bytes[j];
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, double value){
  typename Serializer<PB>::Converter c;
  c._double=value;
  for(unsigned int j=0;j<sizeof(double);j++)
    *s.mI++=c.bytes[j];
  return s;
}
