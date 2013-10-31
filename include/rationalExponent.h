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
    void assign(int8_t n, uint8_t d=1)
    {
      this->n=n;
      this->d=d-1;
    }
    int8_t numerator() const                      {return n;}
    uint8_t denominator() const                   {return d+1;}
    double value() const                          {return (double)n/(d+1);}
    template<typename PacketBufferConstIterator>
    void deserialize(PacketBufferConstIterator& i){value=*i++;}
    template<typename PacketBufferIterator>
    void serialize(PacketBufferIterator& i) const {*i++=value;}
    bool operator==(const UnitExponent r) const   {return value==r.value;}
    bool operator!=(const UnitExponent r) const   {return value!=r.value;}
};

std::ostream& operator<<(std::ostream& o, const UnitExponent r)
{
  if(r.denominator()==0)
    return o << (int16_t)r.numerator();
  else
    return o << "(" << (int16_t)r.numerator() << "/" << (uint16_t)r.denominator() << ")";
}
