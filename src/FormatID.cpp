#include <FormatID.h>

FormatID::FormatID(uint16_t nodeID, FormatID::Direction dir) {
  if(dir==Direction::publisher)
    mData.nr = sPubNr++;
  else
    mData.nr = sSubNr++;
  mData.dir = dir;
  mData.node = nodeID;
}

uint16_t FormatID::node() const{
  return mData.node;
}

FormatID::Direction FormatID::direction() const{
  return mData.dir;
}

uint16_t FormatID::nr() const{
  return mData.nr;
}

bool FormatID::operator==(const FormatID& b) const{
  return mData.node == b.mData.node && mData.dir == b.mData.dir && mData.nr == b.mData.nr;
}

bool FormatID::operator<(const FormatID& b) const{
  if( mData.node < b.mData.node )
    return true;
  if( mData.node > b.mData.node )
    return false;
  if( mData.nr < b.mData.nr )
    return true;
  if( mData.nr > b.mData.nr )
    return false;
  return mData.dir < b.mData.dir;
}

uint16_t FormatID::sPubNr = 0;
uint16_t FormatID::sSubNr = 0;
