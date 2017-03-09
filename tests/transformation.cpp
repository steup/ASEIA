#include <KnowledgeBase.h>
#include <MetaFactory.h>

namespace test {

using namespace ::id::attribute;
using namespace ::id::type;

using TransPtr    = Transformation::TransPtr;
using Events      = Transformer::Events;
using EventTypes  = Transformer::EventTypes;

struct TransformationTestSuite : public ::testing::Test{
	MetaFactory& f  = MetaFactory::instance();
  MetaEvent    in, out;
	Events       inL = { &in };
	EventType    inT, outT;

  TransformationTestSuite()  {
    MetaAttribute s(Time::value());
    s.unit() = Second();
    in.add(s);
    out.add(s);
  }

  void registerTypes() {
    inT = (EventType)in;
    outT = (EventType)out;
    for(const MetaEvent* ePtr : inL)
      KnowledgeBase::registerEventType((EventType)*ePtr);
  }
};

TEST_F(TransformationTestSuite, scaleTransformBasicTest) {
  MetaAttribute& inA  = *in.attribute(Time::value());
  MetaAttribute& outA = *out.attribute(Time::value());
  inA.scale() = MetaScale(Scale<std::milli>());
  inA.value() = f.create({{{1234, 0}}});
  outA.value() = f.create({{{1, 1}}});
  registerTypes();
  auto transList=KnowledgeBase::findTransforms(outT);
  ASSERT_GT(transList.size(), 0U) << "No Transform found!";
  std::ostringstream os;
  for(const ConfiguredTransformation& t : transList)
    os << t;
  ASSERT_LE(transList.size(), 1U) << "Too many Transforms found: " << os.str();
  TransPtr scaleT = transList.front().create();
	EXPECT_TRUE(scaleT->check(inL)) << "MetaValue is not supported by ScaleTransform";
	EXPECT_EQ((*scaleT)(inL), out) << "Events not transformed correctly";
  KnowledgeBase::unregisterEventType((EventType)in);
}

TEST_F(TransformationTestSuite, typeTransformBasicTest) {
  MetaAttribute& inA  = *in.attribute(Time::value());
  MetaAttribute& outA = *out.attribute(Time::value());
  inA.value() = f.create({{{1234.5, 0}}}, Float::value());
  outA.value() = f.create({{{1234, 1}}}, UInt64::value());
  registerTypes();
  auto transList = KnowledgeBase::findTransforms(outT);
  ASSERT_GT(transList.size(), 0U) << "No Transform found!";
  std::ostringstream os;
  for(const ConfiguredTransformation& t : transList)
    os << t;
  ASSERT_LE(transList.size(), 1U) << "Too many Transforms found: " << os.str();
  TransPtr typeT = transList.front().create();
	EXPECT_TRUE(typeT->check(inL)) << "MetaValue is not supported by TypeTransform";
	EXPECT_EQ((*typeT)(inL), out) << "Events not transformed correctly";
  KnowledgeBase::unregisterEventType((EventType)in);
}

}
