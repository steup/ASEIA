#include <KnowledgeBase.h>
#include <MetaFactory.h>
#include <TypeRegistry.h>

namespace test {

using namespace ::id::attribute;

using TransPtr    = Transformation::TransPtr;
using Events      = Transformer::Events;
using EventTypes  = Transformer::EventTypes;

struct TransformTestSuite : public ::testing::Test{
	MetaFactory&   f  = MetaFactory::instance();
	AbstractRegistry<EventType> t;
	Events       in;
	EventTypes   inT;
	MetaEvent    out;
	EventType    outT;
	TransPtr     trans;
};

TEST_F(TransformTestSuite, scaleTransformBasicTest) {
  MetaEvent inE;
  EventType  inET;
  MetaAttribute ms(Time::value());
  ms.scale() = MetaScale(std::ratio<1, 1000>());
  ms.unit() = Second();
  ms.value() = f.create({{{1234, 0}}});
  inE.add(ms);
  MetaAttribute s = ms;
  s.value() = f.create({{{1, 1}}});
  s.scale() = MetaScale();
  out.add(s);
  inET = (EventType)inE;
	t.registerType(inET, inET);
  outT = (EventType)out;
  in   = { &inE };
  std::list<TransPtr> transList;
  TransformGenerator g(outT, t);
  std::move(g.begin(), g.end(), std::back_inserter(transList));
  ASSERT_GT(transList.size(), 0) << "No Transform found!";
  std::ostringstream os;
  for(const TransPtr& ptr : transList)
    ptr->print(os);
  ASSERT_LT(transList.size(), 2) << "Too many Transforms found: " << os.str();
	EXPECT_TRUE(transList.front()->check(in)) << "MetaValue is not supported by ScaleTransform";
	MetaEvent temp = (*transList.front())(in);
	EXPECT_EQ(temp, out) << "Events not transformed correctly";
}

}
