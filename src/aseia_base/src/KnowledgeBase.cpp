#include <KnowledgeBase.h>

using EventTypes = KnowledgeBase::EventTypes;

/*TODO: Implement registering of Transformation */
void KnowledgeBaseImpl::registerTransformation(const Transformation& trans) {

}

/*TODO: Implement unregistering of Transformation */
void KnowledgeBaseImpl::unregisterTransformation(const Transformation& trans) {

}

TransformGenerator KnowledgeBaseImpl::generate(const EventType& out, const EventTypes& in) const {
    return TransformGenerator(out, in);
}


/* TODO: Implement transformation list generation */
const TransformGenerator::TransList* TransformGenerator::next() {
  return &mCurrTrans;
}

TransformIterator::TransformIterator(TransformGenerator& tG, bool end)
  : mGen(tG)
{
  if(!end)
    mTrans = mGen.next();
  else
    mTrans = nullptr;
}

/* TODO:implement compound Transformations */
Transformation::TransPtr TransformIterator::operator*() const {
  return Transformation::TransPtr();
}

TransformIterator& TransformIterator::operator++() {
  mTrans = mGen.next();
  return *this;
}

bool TransformIterator::operator==(const TransformIterator& b) const {
  if(mTrans == b.mTrans)
    return true;
  if(!mTrans || !b.mTrans)
    return false;
  return std::equal(mTrans->begin(), mTrans->end(), b.mTrans->begin());
}
