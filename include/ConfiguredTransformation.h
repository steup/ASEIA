#include <Transformation.h>

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
  using TransPtr   = std::unique_ptr<Transformer>;
  using EventIDs   = Transformation::EventIDs;
  private:
  TransformationPtr mTrans = nullptr;
  const EventType* mOut = nullptr;
  EventTypes mIn;
  public:
  ConfiguredTransformation() = default;
  ConfiguredTransformation(TransformationPtr t, const EventType& out, const EventTypes in)
    : mTrans(t), mOut(&out), mIn(in)
  {}
  TransPtr create() const {
    if(!check())
      return TransPtr();
    else
      return mTrans->create(*mOut, mIn);
  }
  bool  check() const {
    return mTrans && mOut && mIn.size() == mTrans->arity();
  }
  TransformationPtr trans() const { return mTrans; }
  const EventType& out() const { return *mOut; }
  const EventTypes& in() const { return mIn; }
  EventIDs inIDs() const { return (mTrans && mOut)?mTrans->in(EventID(*mOut)):EventIDs(); }
  void in(const EventTypes& eTs) { mIn = eTs; }
  bool operator==(const Transformer& t) const { return t==*this; }
  friend std::ostream& operator<<(std::ostream&, const ConfiguredTransformation&);
};

std::ostream& operator<<(std::ostream& o, const ConfiguredTransformation& t);