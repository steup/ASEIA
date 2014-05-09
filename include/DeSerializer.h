#pragma once

#include <cstdint>

template<typename ConstPacketBufferIterator>
class DeSerializer{
  private:
    using Iterator = ConstPacketBufferIterator;
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
    DeSerializer(Iterator i) : mI(i){}

    const Iterator& iterator() const{return mI;}

    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, std::uint8_t& value);
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, std::uint16_t& value);
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, std::uint32_t& value);
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, std::uint64_t& value);
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, std::int8_t& value);
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, std::int16_t& value);
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, std::int32_t& value);
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, std::int64_t& value);
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, float& value);
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, double& value);
};

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::uint8_t& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(std::uint8_t);j++)
    c.bytes[j]=*s.mI++;
  value=c.uint8;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::uint16_t& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(std::uint16_t);j++)
    c.bytes[j]=*s.mI++;
  value=c.uint16;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::uint32_t& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(std::uint32_t);j++)
    c.bytes[j]=*s.mI++;
  value=c.uint32;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::uint64_t& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(std::uint64_t);j++)
    c.bytes[j]=*s.mI++;
  value=c.uint64;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::int8_t& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(std::int8_t);j++)
    c.bytes[j]=*s.mI++;
  value=c.int8;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::int16_t& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(std::int16_t);j++)
    c.bytes[j]=*s.mI++;
  value=c.int16;;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::int32_t& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(std::int32_t);j++)
    c.bytes[j]=*s.mI++;
  value=c.int32;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::int64_t& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(std::int64_t);j++)
    c.bytes[j]=*s.mI++;
  value=c.int64;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, float& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(float);j++)
    c.bytes[j]=*s.mI++;
  value=c._float;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, double& value){
  typename DeSerializer<PB>::Converter c;
  for(unsigned int j=0;j<sizeof(double);j++)
    c.bytes[j]=*s.mI++;
  value=c._double;
  return s;
}

