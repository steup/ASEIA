#pragma once

#include <cstdint>

namespace AttributeID
{
  struct Base
  {
    using Type = std::uint8_t;
    constexpr Type value(){return 0;}
    constexpr const char* name(){return "Unspecified";}
  };

  struct Position : public Base
  {
    constexpr Base::Type value(){return 1;}
    constexpr const char* name(){return "Position";}
  };
  
  struct Time : public Base
  {
    constexpr Base::Type value(){return 2;}
    constexpr const char* name(){return "Time";}
  };
  
  struct PublisherID : public Base
  {
    constexpr Base::Type value(){return 3;}
    constexpr const char* name(){return "Publisher ID";}
  };
  
  struct Validity : public Base
  {
    constexpr Base::Type value(){return 4;}
    constexpr const char* name(){return "Validity";}
  };

  struct Distance : public Base
  {
    constexpr Base::Type value(){return 5;}
    constexpr const char* name(){return "Distance";}
  };
}
