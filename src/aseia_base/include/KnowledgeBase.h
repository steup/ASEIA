#pragma once

#include <EventID.h>
#include <Transformation.h>
#include <Singleton.h>

#include <cstdint>
#include <list>

class TransformGenerator;

class TransformIterator
  : std::iterator<
    std::input_iterator_tag,
    Transformation::TransPtr>
{
  friend class TransformGenerator;
  private:
    using TransList = std::list<const Transformation*>;
    TransformGenerator& mGen;
    const TransList* mTrans;
    TransformIterator(TransformGenerator& tG, bool end = false);
  public:
    Transformation::TransPtr operator*() const;
    TransformIterator& operator++();
    bool operator==(const TransformIterator& b) const;
    bool operator!=(const TransformIterator& b) const { return !(*this==b); }
};

class TransformGenerator {
  friend class KnowledgeBaseImpl;
  friend class TransformIterator;
  private:
    using EventTypes = Transformer::EventTypes;
    using TransList  = TransformIterator::TransList;
    const EventType& mOut;
    const EventTypes& mIn;
    TransList mCurrTrans;
    TransformGenerator(const EventType& out, const EventTypes& in)
      : mOut(out),
        mIn(in)
    { }
    const TransList* next();
  public:
    TransformIterator begin() { return TransformIterator(*this); }
    TransformIterator end() { return TransformIterator(*this, true); }
};

class KnowledgeBaseImpl {
  public:
    using EventTypes = Transformer::EventTypes;
    void registerTransformation(const Transformation& trans);
    void unregisterTransformation(const Transformation& trans);
    TransformGenerator generate(const EventType& out, const EventTypes& in) const;
};

using KnowledgeBase = Singleton<KnowledgeBaseImpl>;
