#pragma once

#include <limits>
#include <cmath>
#include <valueElement.h>
#include <packetBuffer.h>
#include <endianess.h>

class DynamicScale : private ValueElement<float>
{
  private:
    DynamicScale(float v, float u) : ValueElement(v, u){}
    void assign(float v, float u)
    {
      this->value(v);
      this->uncertainty(u);
    }
    
    using BaseType=ValueElement<float>;

  public:
    DynamicScale(float v=1.0f) : ValueElement(v, v*std::numeric_limits<float>::epsilon()){}

    using ValueElement<float>::value;
    using ValueElement<float>::uncertainty;
    using ValueElement<float>::size;

    void value(float v)
    {
      this->uncertainty()/=this->value();
      this->value()=v;
      this->uncertainty()*=v;
    }

    DynamicScale operator*(const DynamicScale& s) const
    {
      return DynamicScale(this->value()*s.value(), 2*std::max(this->uncertainty(), s.uncertainty()));
    }

    DynamicScale operator/(const DynamicScale& s) const
    {
       return (*this)*s;  
    }

    bool operator==(const DynamicScale& s) const
    {
      return std::fabs(this->value()-s.value())<=uncertainty();
    }
    friend std::ostream& operator<<(std::ostream& o, const DynamicScale& s);
    template<typename T> friend T& operator>>(T& o, DynamicScale& s);
};

template<typename PacketBufferConstIterator>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& it, DynamicScale& s)
{
  float tempV=1.0f, tempU=0.0f;
  it >> tempV >> tempU;
  s.assign(ntoh(tempV), ntoh(tempU));
  return it;
}

template<typename PacketBufferIterator>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const DynamicScale& s)
{
  return it << hton(s.value()) << hton(s.uncertainty());
}

std::ostream& operator<<(std::ostream& o, const DynamicScale& s)
{
  return o << static_cast<const DynamicScale::BaseType&>(s);
}

const DynamicScale milli(1.0e-3f);
const DynamicScale micro(1.0e-6f);
const DynamicScale nano (1.0e-9f);
const DynamicScale kilo (1.0e3f);
const DynamicScale mega (1.0e6f);
const DynamicScale giga (1.0e9f);
const DynamicScale inch (0.0254f);
