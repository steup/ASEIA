#pragma once

#include <PODConverter.h>

template<typename ConstPacketBufferIterator>
class DeSerializer{
  private:
    using Iterator = ConstPacketBufferIterator;
    Iterator mI;
    Iterator mEnd;
    bool mError;
    bool copyBytes(uint8_t* bytes, std::size_t n){
      while(n--){
        if(mI==mEnd){
          mError=true;
          return false;
        }
        *bytes++=*mI++;
      }
      return true;
    }
  public:
    DeSerializer(Iterator start, Iterator end) : mI(start), mEnd(end), mError(false){}

    const Iterator& iterator() const{return mI;}
    bool error() const{return mError;}

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
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(std::uint8_t));
  value=c.uint8;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::uint16_t& value){
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(std::uint16_t));
  value=c.uint16;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::uint32_t& value){
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(std::uint32_t));
  value=c.uint32;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::uint64_t& value){
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(std::uint64_t));
  value=c.uint64;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::int8_t& value){
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(std::int8_t));
  value=c.int8;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::int16_t& value){
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(std::int16_t));
  value=c.int16;;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::int32_t& value){
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(std::int32_t));
  value=c.int32;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, std::int64_t& value){
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(std::int64_t));
  value=c.int64;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, float& value){
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(float));
  value=c._float;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, double& value){
  PODConverter c;
  s.copyBytes(c.bytes, sizeof(double));
  value=c._double;
  return s;
}
