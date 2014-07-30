#include <FormatID.h>

FormatID::FormatID(uint16_t nodeID) : mValue(((uint32_t)(nodeID)<<sizeof(nodeID)*8) | sFormatNr++){

}

uint32_t FormatID::value() const { 
  return mValue; 
} 

uint16_t FormatID::sFormatNr = 0;

