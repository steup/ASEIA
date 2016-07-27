#pragma once

#include <Transformation.h>

#include <list>
#include <iterator>

class TransformGeneratorImpl;

class TransformGenerator {
  public:
    using TransList = std::list<const Transformation*>;
    using EventTypes = Transformer::EventTypes;
  private:
    TransformGeneratorImpl* mImpl;
    const EventType& mOut;
    const EventTypes mIn;
    TransList next();
  public:
    TransformGenerator(const EventType& out, const EventTypes& in);
    ~TransformGenerator();
    class Iterator
      : std::iterator<
        std::input_iterator_tag,
        Transformation::TransPtr>
    {
      private:
        TransformGenerator& mGen;
        TransList mTrans;
        Iterator(TransformGenerator& tG, TransList l)
          : mGen(tG), mTrans(l) { }
      public:
        /* TODO:implement compound Transformations */
        Transformation::TransPtr operator*() const {
          if(mTrans.size()==1 && mTrans.front())
            return mTrans.front()->create(mGen.mOut, mGen.mIn);

          return Transformation::TransPtr();
        }
        Iterator& operator++() {
          mTrans = mGen.next();
          return *this;
        }
        bool operator==(const Iterator& b) const;
        bool operator!=(const Iterator& b) const { return !(*this==b); }
       friend class TransformGenerator;
    };
    Iterator begin() { return Iterator(*this, next()); }
    Iterator end() { return Iterator(*this, TransList()); }
};

class KnowledgeBase {
  public:
    using EventTypes = Transformer::EventTypes;
    static void registerTransformation(const Transformation& trans);
    static void unregisterTransformation(const Transformation& trans);
};

