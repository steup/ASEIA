#include <Transformation.h>

#include <KnowledgeBase.h>

Transformation::Transformation() {
  KnowledgeBase::instance().registerTransformation(*this);
}
