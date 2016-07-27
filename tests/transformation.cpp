#include <KnowledgeBase.h>
#include <MetaFactory.h>

namespace test {

using namespace ::id::attribute;

struct TransformTestSuite : public ::testing::Test{
	using TransPtr    = Transformation::TransPtr;
	using Events      = Transformer::Events;
	using EventTypes  = Transformer::EventTypes;
	MetaFactory&   f  = MetaFactory::instance();
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
  inT  = { &inET };
  outT = (EventType)out;
  in   = { &inE };
  unsigned int i=0;
  for(TransPtr p : TransformGenerator(outT, {inT})) {
    trans = std::move(p);
    i++;
  }
  ASSERT_GT(i, 0) << "No Transform found!";
  ASSERT_LT(i, 2) << "Too many Transforms found!";
	EXPECT_TRUE(trans->check(in)) << "MetaValue is not supported by ScaleTransform";
	MetaEvent temp = (*trans)(in);
	EXPECT_EQ(temp, out) << "Events not transformed correctly";
}

}
