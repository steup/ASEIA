#include <Transformation.h>

#include <KnowledgeBase.h>

Transformation::Transformation(const EventID& out, const EventIDs& in)
  : mOut(out), mIn(in)
{
  KnowledgeBase::instance().registerTransformation(*this);
}
