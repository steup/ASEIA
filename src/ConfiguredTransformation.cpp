#include <ConfiguredTransformation.h>

#include <IO.h>

#include <iostream>

using namespace std;

bool Transformer::operator==(const ConfiguredTransformation& t) const {
  return mTrans == *t.trans() && mOut == t.out() && mIn == t.in();
}

ostream& operator<<(ostream& o, const ConfiguredTransformation& t) {
  if(!t.mTrans)
    return o << "invalid configured transformation";

  o << *t.trans() << ": " << t.out() << " <- [";
  for(const EventType& eT : t.in())
    o << eT << ", ";
  return o << "]";
}
