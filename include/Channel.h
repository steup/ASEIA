#pragma once

#include <Transformation.h>

#include <iosfwd>

class MetaEvent;

class Channel{
  public:
    using TransPtr   = Transformation::TransPtr;
  protected:
    enum class Errors {
      InvalidEvent,
      MissingAttribute,
      IncompatibleType
    };
    TransPtr   mTrans;
    virtual void fixType(MetaEvent& e) const;
    virtual void error(Errors error, const MetaEvent& e) const {}
    void handleEvent(const MetaEvent& e);
    virtual void publishEvent(MetaEvent& e) const =0;
  public:
    Channel() = default;
    Channel(TransPtr&& trans);
    const Transformer* trans() const {return mTrans.get();}
  friend std::ostream& operator<<(std::ostream&, const Channel&);
};

std::ostream& operator<<(std::ostream& o, const Channel& c);
