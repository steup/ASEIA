#include <KnowledgeBase.h>

#include <Singleton.h>

#include <list>
#include <algorithm>

using EventTypes = KnowledgeBase::EventTypes;
using EventIDs = Transformation::EventIDs;

using namespace std;

class KBImpl {
  public:
    using UnaryStorage = std::list<const Transformation*>;
  private:
    UnaryStorage mUnary;
  public:
    using UnaryIter = UnaryStorage::const_iterator;
    /*TODO: Implement registering of Transformation */
    void regTrans(const Transformation& trans) {
      if(trans.in().size()==1)
        mUnary.push_back(&trans);
    }
    /*TODO: Implement unregistering of Transformation */
    void unregTrans(const Transformation& trans) {

    }
    const UnaryStorage& unaryTrans() const { return mUnary; }
};

using KB = Singleton<KBImpl>;

using TransList = TransformGenerator::TransList;

class TransformGeneratorImpl {
  public:
    virtual ~TransformGeneratorImpl() {}
    virtual TransList next() =0;
};

class TransformGeneratorUnary : public TransformGeneratorImpl {
  private:
    KB::UnaryIter mStart, mEnd;
    EventID mIn, mOut;
  public:
    TransformGeneratorUnary(EventID out, EventID in)
      : mIn(in), mOut(out)
    {
      const KB::UnaryStorage& trans = KB::instance().unaryTrans();
      mStart = trans.begin();
      mEnd = trans.end();
    }

    virtual TransList next() {
      auto comp = [this](const Transformation* t){return t && t->out()>=mOut && t->in().front()>=mIn;};
      KB::UnaryIter i = find_if(mStart, mEnd, comp);
      mStart = i;
      mStart++;
      if(i!=mEnd)
        return {*i};
      else
        return TransList();
    }
};

class TransformGeneratorGen : public TransformGeneratorImpl {
  public:
    TransformGeneratorGen(EventID out) { }

    virtual TransList next() {
      return TransList();
    }
};

class TransformGeneratorNary : public TransformGeneratorImpl {
  public:
    TransformGeneratorNary(EventID out, EventIDs in) { }

    virtual TransList next() {
      return TransList();
    }
};


TransformGenerator::TransformGenerator(const EventType& out, const EventTypes& in) :
  mImpl(nullptr), mOut(out), mIn(in)
{
  switch(in.size()) {
    case(0): mImpl = new TransformGeneratorGen(out);
             break;
    case(1): if(in.front())
              mImpl = new TransformGeneratorUnary(out, *in.front());
             break;
    default: if(!count(in.begin(), in.end(), nullptr)){
              EventIDs ids;
              auto cast = [](const EventType* t){return (EventID)(*t);};
              transform(in.begin(), in.end(), back_inserter(ids), cast);
              mImpl = new TransformGeneratorNary(out, ids);
             }
  }
}

TransformGenerator::~TransformGenerator() {
  if(mImpl)
    delete mImpl;
}

/* TODO: Implement transformation list generation */
TransformGenerator::TransList TransformGenerator::next() {
  if(mImpl)
    return mImpl->next();
  else
    return TransList();
}

bool TransformGenerator::Iterator::operator==(const TransformGenerator::Iterator& b) const {
  return std::equal(mTrans.begin(), mTrans.end(), b.mTrans.begin());
}


void KnowledgeBase::registerTransformation(const Transformation& trans) {
  KB::instance().regTrans(trans);
}

void KnowledgeBase::unregisterTransformation(const Transformation& trans) {
  KB::instance().unregTrans(trans);
}
