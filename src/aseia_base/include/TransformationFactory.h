#pragma once

#include <Transformation.h>

#include <cstdint>

class TransformationFactory {
  public:
    using TransID = std::uint32_t;
    using TransPtr = Transformation::TransPtr;
    using EventTypes = Transformation::EventTypes;
  protected:
    using createFunc = TransPtr (*)(const EventType& out, const EventTypes& in);
    virtual TransID registerCreator(createFunc f) =0;

  public:
    static TransformationFactory& instance();
    virtual TransPtr create(const EventType&out, const EventTypes& in, TransID trans) const =0;

    template<class T>
    TransID registerTransformation() {
    createFunc f = &Transformation::create<T>;
      return registerCreator(f);
    }
};
