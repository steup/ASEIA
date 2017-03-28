#pragma once

#include <Transformation.h>

#include <EventID.h>
#include <EventType.h>

#include <iosfwd>
#include <initializer_list>
#include <vector>

class MetaEvent;
class EventType;
class ConfiguredTransformation;

class ConfiguredTransformation {
  public:
  using EventTypes = Transformer::EventTypes;
  using TransPtr   = Transformation::TransPtr;
  using EventIDs   = Transformation::EventIDs;
  protected:
  TransformationPtr mTrans = nullptr;
  EventType mOut;
  EventTypes mIn;
  public:
  ConfiguredTransformation() = default;
  ConfiguredTransformation(TransformationPtr t, const EventType& out, const EventTypes in)
    : mTrans(t), mOut(out), mIn(in)
  {}
  TransPtr create() const {
    if(!check())
      return TransPtr();
    else
      return mTrans->create(mOut, mIn);
  }
  bool  check() const {
    return mTrans && mIn.size() == mTrans->arity();
  }
  TransformationPtr trans() const { return mTrans; }
  void trans(TransformationPtr t) { mTrans = t; }
  const EventType& out() const { return mOut; }
  void out(const EventType& eT) {mOut = eT; }
  const EventTypes& in() const { return mIn; }
  EventIDs inIDs() const { return (mTrans)?mTrans->in(EventID(mOut)):EventIDs(); }
  void in(const EventTypes& eTs) { mIn = eTs; }
  bool operator==(const Transformer& t) const { return t==*this; }
  friend std::ostream& operator<<(std::ostream&, const ConfiguredTransformation&);
};

std::ostream& operator<<(std::ostream& o, const ConfiguredTransformation& t);
