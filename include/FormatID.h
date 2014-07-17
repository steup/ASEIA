#pragma once

#include <stdint.h>

class FormatID{
  private:
    static uint16_t sFormatNr;
   const uint32_t mValue;
  public:
    FormatID(uint16_t nodeID) : mValue(((uint32_t)(nodeID)<<sizeof(nodeID)*8) | sFormatNr++){}
    uint32_t value const { return mValue; } 
};

uint16_t FormatID::sFormatNr = 0;
