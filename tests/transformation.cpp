#include <KnowledgeBase.h>
#include <MetaFactory.h>

namespace test {

using namespace ::id::attribute;
using namespace ::id::type;

using TransPtr    = Transformation::TransPtr;
using Events      = Transformer::Events;
using EventTypes  = Transformer::EventTypes;

struct TransformTestSuite : public ::testing::Test{
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
	KnowledgeBase::registerEventType(inET);
  outT = (EventType)out;
  in   = { &inE };
  auto transList=KnowledgeBase::findTransforms(outT);
  ASSERT_GT(transList.size(), 0) << "No Transform found!";
  std::ostringstream os;
  for(const ConfiguredTransformation& t : transList)
    os << t;
  ASSERT_LT(transList.size(), 2) << "Too many Transforms found: " << os.str();
  TransPtr scaleT = transList.front().create();
	EXPECT_TRUE(scaleT->check(in)) << "MetaValue is not supported by ScaleTransform";
	MetaEvent temp = (*scaleT)(in);
	EXPECT_EQ(temp, out) << "Events not transformed correctly";
}

TEST_F(TransformTestSuite, typeTransformBasicTest) {
  MetaEvent inE;
  EventType  inET;
  MetaAttribute fs(Time::value());
  fs.unit() = Second();
  fs.value() = f.create({{{1234.5, 0}}}, Float::value());
  inE.add(fs);
  MetaAttribute us64 = fs;
  us64.value() = f.create({{{1234, 1}}}, UInt64::value());
  us64.scale() = MetaScale();
  out.add(us64);
  inET = (EventType)inE;
	KnowledgeBase::registerEventType(inET);
  outT = (EventType)out;
  in   = { &inE };
  auto transList = KnowledgeBase::findTransforms(outT);
  ASSERT_GT(transList.size(), 0) << "No Transform found!";
  std::ostringstream os;
  for(const ConfiguredTransformation& t : transList)
    os << t;
  ASSERT_LT(transList.size(), 2) << "Too many Transforms found: " << os.str();
  TransPtr typeT = transList.front().create();
	EXPECT_TRUE(typeT->check(in)) << "MetaValue is not supported by TypeTransform";
	MetaEvent temp = (*typeT)(in);
	EXPECT_EQ(temp, out) << "Events not transformed correctly";
}

}
