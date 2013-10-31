#pragma once

#include <iostream>

enum class AttributeID : uint8_t
{
  distance,
  position,
  time,
  publisherID,
  validity,
  unspecified
};

std::ostream& operator<<(std::ostream& o, const AttributeID i)
{
  switch(i)
  {
    case(AttributeID::distance)   : return o << "Distance   ";
    case(AttributeID::position)   : return o << "Position   ";
    case(AttributeID::time)       : return o << "Time       ";
    case(AttributeID::publisherID): return o << "PublisherID";
    case(AttributeID::validity)   : return o << "Validity   ";
    default                       : return o << "Unspecified";
  }
}
