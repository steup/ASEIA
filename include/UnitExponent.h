#pragma once

#include <ostream>
#include <cstdint>

class UnitExponent
{
  using int8_t=std::int8_t;
  using uint8_t=std::uint8_t;

  private:
    union
    {
      uint8_t value;
      struct
      {
        int8_t  n : 6;
        uint8_t d : 2;
      };
    };
  public:
    UnitExponent(int8_t n=0, uint8_t d=1) : n(n), d(d-1){}
    void numerator(int8_t n)                      {this->n=n;}
    int8_t numerator() const                      {return n;}
    void denominator(uint8_t d)                   {this->d=d-1;}
    uint8_t denominator() const                   {return d+1;}
    template<typename PacketBufferConstIterator>
    void deserialize(PacketBufferConstIterator& i){value=*i++;}
    template<typename PacketBufferIterator>
    void serialize(PacketBufferIterator& i) const {*i++=value;}
    bool operator==(const UnitExponent r) const   {return value==r.value;}
    bool operator!=(const UnitExponent r) const   {return value!=r.value;}
    constexpr static std::size_t size(){return 1;}
};

std::ostream& operator<<(std::ostream& o, const UnitExponent r)
{
  if(r.denominator()==0)
    return o << (int16_t)r.numerator();
  else
    return o << "(" << (int16_t)r.numerator() << "/" << (uint16_t)r.denominator() << ")";
}
