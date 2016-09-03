#include <KnowledgeBase.h>
#include <TypeRegistry.h>

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
      if(&trans==&invalidTrans)
        return;
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
  public:
    using ImplData = TransformGenerator::ImplData;
    using EventIDs = ImplData::second_type;
    const EventID  mOut;
    const EventIDs mIn;
    TransformGeneratorImpl(EventID out, EventIDs in) : mOut(out), mIn(in) {}
    virtual ~TransformGeneratorImpl() {}
    virtual ImplData next() =0;
};

class GeneralTransformGenerator : public TransformGeneratorImpl {
  private:
    using TransIter = KB::GeneralTransformIter;
    using EventIter = EventIDs::const_iterator;
    const TransIter mTransEnd = KB::instance().generalTrans().end();
    TransIter mTrans = KB::instance().generalTrans().begin();
    EventIDs mCompatibleIn;
    EventIter mEvent;
  public:
    GeneralTransformGenerator(EventID out, EventIDs in)
      : TransformGeneratorImpl(out, in), mEvent(mCompatibleIn.begin())
    {}

    virtual ImplData next() {
      if(mEvent == mCompatibleIn.end())
        mTrans = find_if(mTrans, mTransEnd, [this](const Transformation* t){return t->out()>=mOut;});

      if(mTrans == mTransEnd)
        return ImplData(&invalidTrans, {});

      if(mEvent == mCompatibleIn.end()) {
        copy_if(mIn.begin(), mIn.end(), std::back_inserter(mCompatibleIn),
          [this](EventID id){return mOut <= id;}
        );
        mEvent = mCompatibleIn.begin();
      }

      if(mEvent == mCompatibleIn.end())
        return ImplData(&invalidTrans, {});

      ImplData res(*mTrans, {*mEvent++});

      if(mEvent == mCompatibleIn.end())
        mTrans++;

      return res;
    }
};

TransformGenerator::TransformGenerator(const EventType& out, const EventTypes& in)
  :  mImpl(nullptr), mOut(out), mIn(in)
{
  ImplData::second_type ids;
  std::transform(in.begin(), in.end(), std::back_inserter(ids), [](const EventType& e){return (EventID)e;});
  std::sort(ids.begin(), ids.end());
  ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
  mImpl= new GeneralTransformGenerator(out, ids);
}

TransformGenerator::~TransformGenerator() {
  if(mImpl)
    delete mImpl;
}

TransformGenerator::IterData TransformGenerator::next() {
  if(mImpl) {
    IterData i;
    do {
      //TODO hack for singular transforms
      if(i.first == nullptr || mType != mTypeEnd) {
        mImplData = mImpl->next();
        i.first = mImplData.first;
        if(mImplData.second.empty())
          continue;
        auto range = mIn.find(mImplData.second.front());
        mType = range.begin();
        mTypeEnd = range.end();
      }
      i.first = mImplData.first;
      i.second.push_back(mType++);
    } while(i.first != &invalidTrans && !(i.first->check(mOut, i.second)));
    return i;
  }else
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
