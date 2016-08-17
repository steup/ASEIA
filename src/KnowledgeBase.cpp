#include <KnowledgeBase.h>

#include <Singleton.h>

#include <list>
#include <algorithm>

using EventTypes = KnowledgeBase::EventTypes;
using EventIDs = Transformation::EventIDs;

using namespace std;

class InvalidTransformation : public Transformation {
	public:
		InvalidTransformation() : Transformation(EventID::any, {}) {}
    virtual bool check(const EventType& out, const EventTypes& in) const {
			return false;
		}
    virtual TransPtr create(const EventType& out, const EventTypes& in) const {
			return TransPtr();
		}
    virtual void print(std::ostream& o) const {
			o << "invalid";
		}
};

static InvalidTransformation invalidTrans;

TransformGenerator::Iterator TransformGenerator::end() { 
	return TransformGenerator::Iterator(*this, IterData(&invalidTrans, {}));
}

class KBImpl {
  public:
    using GeneralTransformStorage = std::list<const Transformation*>;
  private:
    GeneralTransformStorage mGeneralTrans;
  public:
    using GeneralTransformIter = GeneralTransformStorage::const_iterator;
    /*TODO: Implement registering of Transformation */
    void regTrans(const Transformation& trans) {
      if(trans.out() == EventID::any)
        mGeneralTrans.push_back(&trans);
    }
    /*TODO: Implement unregistering of Transformation */
    void unregTrans(const Transformation& trans) {

    }
    const GeneralTransformStorage& generalTrans() const { return mGeneralTrans; }
};

using KB = Singleton<KBImpl>;


class TransformGeneratorImpl {
	protected:
		using IterData = TransformGenerator::IterData;
  public:
    virtual ~TransformGeneratorImpl() {}
    virtual IterData next() =0;
};

class GeneralTransformGenerator : public TransformGeneratorImpl {
  private:
		using Iter = KB::GeneralTransformIter;
    Iter mStart, mEnd;
    EventID mIn, mOut;
  public:
    GeneralTransformGenerator(EventID out, EventID in)
      : mIn(in), mOut(out)
    {
      const KB::GeneralTransformStorage& trans = KB::instance().generalTrans();
      mStart = trans.begin();
      mEnd = trans.end();
    }
		/** \todo implement type search **/
    virtual IterData next() {
      auto comp = [this](const Transformation* t){return t && t->out()>=mOut && t->in().front()>=mIn;};
      Iter i = find_if(mStart, mEnd, comp);
      mStart = i;
      mStart++;
      if(i!=mEnd)
        return IterData(*i, {});
      else
        return IterData(&invalidTrans, {});
    }
};

class GeneratorTransformGenerator : public TransformGeneratorImpl {
  public:
    GeneratorTransformGenerator(EventID out) { }

    virtual IterData next() {
      return IterData(&invalidTrans, {});
    }
};

class SpecialTransformGenerator : public TransformGeneratorImpl {
  public:
    SpecialTransformGenerator(EventID out, EventIDs in) { }

    virtual IterData next() {
      return IterData(&invalidTrans, {});
    }
};


TransformGenerator::TransformGenerator(const EventType& out, const EventTypes& in) :
  mImpl(nullptr), mOut(out), mIn(in)
{
  switch(in.size()) {
    case(0): mImpl = new GeneratorTransformGenerator(out);
             break;
    case(1): if(in.front())
              mImpl = new GeneralTransformGenerator(out, *in.front());
             break;
    default: if(!count(in.begin(), in.end(), nullptr)){
              EventIDs ids;
              auto cast = [](const EventType* t){return (EventID)(*t);};
              transform(in.begin(), in.end(), back_inserter(ids), cast);
              mImpl = new SpecialTransformGenerator(out, ids);
             }
  }
}

TransformGenerator::~TransformGenerator() {
  if(mImpl)
    delete mImpl;
}

/* TODO: Implement transformation list generation */
TransformGenerator::IterData TransformGenerator::next() {
  if(mImpl)
    return mImpl->next();
  else
    return IterData(&invalidTrans, {});
}

bool TransformGenerator::Iterator::operator==(const TransformGenerator::Iterator& b) const {
  return mData.first == b.mData.first && std::equal(mData.second.begin(), mData.second.end(), b.mData.second.begin());
}


void KnowledgeBase::registerTransformation(const Transformation& trans) {
  KB::instance().regTrans(trans);
}

void KnowledgeBase::unregisterTransformation(const Transformation& trans) {
  KB::instance().unregTrans(trans);
}
