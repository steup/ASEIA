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
		using IterData = std::pair<const Transformation*, std::list<const EventType*>>;
    TransformGeneratorImpl* mImpl;
    const EventType& mOut;
    const EventTypes mIn;
    IterData next();
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
				IterData mData;
        Iterator(TransformGenerator& tG, IterData data)
          : mGen(tG), mData(data) { }
      public:
        /* TODO:implement compound Transformations */
        Transformation::TransPtr operator*() const {
        	return mData.first->create(mGen.mOut, mData.second);
        }
        Iterator& operator++() {
          mData = mGen.next();
          return *this;
        }
        bool operator==(const Iterator& b) const;
        bool operator!=(const Iterator& b) const { return !(*this==b); }
       friend class TransformGenerator;
    };
    Iterator begin() { return Iterator(*this, next()); }
		Iterator end();
	friend class TransformGeneratorImpl;
};

class KnowledgeBase {
  public:
    using EventTypes = Transformer::EventTypes;
    static void registerTransformation(const Transformation& trans);
    static void unregisterTransformation(const Transformation& trans);
};

