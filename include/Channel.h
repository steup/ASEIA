#pragma once

#include <Transformation.h>

#include <iosfwd>

class MetaEvent;

class Channel{
  public:
    using TransPtr   = Transformation::TransPtr;
  protected:
    TransPtr   mTrans;

    void handleEvent(const MetaEvent& e);
    virtual void publishEvent(const MetaEvent& e) const =0;
  public:
    Channel() = default;
    Channel(TransPtr&& trans);
    const Transformer* trans() const {return mTrans.get();}
  friend std::ostream& operator<<(std::ostream&, const Channel&);
};

std::ostream& operator<<(std::ostream& o, const Channel& c);
