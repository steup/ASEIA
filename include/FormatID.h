#pragma once

#include <stdint.h>

class FormatID{
  private:
   static uint16_t sFormatNr;
   const uint32_t mValue;
  public:
    FormatID(uint16_t nodeID);
    uint32_t value() const;
};
