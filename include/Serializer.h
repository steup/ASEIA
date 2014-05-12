#pragma once

#include <PODConverter.h>

template<typename PacketBufferIterator>
class Serializer{
  private:
    using Iterator = PacketBufferIterator;
    Iterator mI;
    Iterator mEnd;
    bool mError;
    bool copyBytes(const uint8_t* bytes, std::size_t n){
      while(n--){
        if(mI==mEnd){
          mError=true;
          return false;
        }
        *mI++=*bytes++;
      }
      return true;
    }
  public:
    Serializer(Iterator start, Iterator end) : mI(start), mEnd(end), mError(false){}

    const Iterator& iterator() const{return mI;}
    bool error() const{return mError;}

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
  PODConverter c;
  c.uint8=value;
  s.copyBytes(c.bytes, sizeof(uint8_t));
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::uint16_t value){
  PODConverter c;
  c.uint16=value;
  s.copyBytes(c.bytes, sizeof(uint16_t));
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::uint32_t value){
  PODConverter c;
  c.uint32=value;
  s.copyBytes(c.bytes, sizeof(uint32_t));
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::uint64_t value){
  PODConverter c;
  c.uint64=value;
  s.copyBytes(c.bytes, sizeof(uint64_t));
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::int8_t value){
  PODConverter c;
  c.int8=value;
  s.copyBytes(c.bytes, sizeof(int8_t));
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::int16_t value){
  PODConverter c;
  c.int16=value;
  s.copyBytes(c.bytes, sizeof(int16_t));
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::int32_t value){
  PODConverter c;
  c.int32=value;
  s.copyBytes(c.bytes, sizeof(int32_t));
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, std::int64_t value){
  PODConverter c;
  c.int64=value;
  s.copyBytes(c.bytes, sizeof(int64_t));
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, float value){
  PODConverter c;
  c._float=value;
  s.copyBytes(c.bytes, sizeof(float));
  return s;
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, double value){
  PODConverter c;
  c._double=value;
  s.copyBytes(c.bytes, sizeof(double));
  return s;
}
