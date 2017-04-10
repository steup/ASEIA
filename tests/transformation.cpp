
#include <gtest/gtest.h>
#include <KnowledgeBase.h>
#include <MetaFactory.h>
#include <Transformations.h>

namespace test {

using namespace ::id::attribute;
using namespace ::id::type;

using TransPtr    = Transformation::TransPtr;
using Events      = Transformer::Events;
using EventTypes  = Transformer::EventTypes;

struct TransformationTestSuite : public ::testing::Test{
	MetaFactory& f  = MetaFactory::instance();
  MetaEvent    in, out;
	EventType    inT, outT;

  TransformationTestSuite()  {
    MetaAttribute s(Time::value());
    s.unit() = Second();
    in.add(s);
    out.add(s);
    KnowledgeBase::clear();
    KnowledgeBase::registerTransformation(cast);
    KnowledgeBase::registerTransformation(rescale);
  }

  void registerTypes() {
    inT = (EventType)in;
    outT = (EventType)out;
    KnowledgeBase::registerEventType(inT);
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
  for(const CompositeTransformation& t : transList)
    os << t;
  ASSERT_LE(transList.size(), 1U) << "Too many Transforms found: " << os.str();
  TransPtr scaleT = transList.front().create(AbstractPolicy());
  ASSERT_NE(scaleT, nullptr);
	EXPECT_TRUE(scaleT->check(in)) << "MetaValue is not supported by ScaleTransform";
  Events result = (*scaleT)(in);
  ASSERT_GE(result.size(), 1U) << "No Events generated!";
	EXPECT_EQ(result.front(), out) << "Events not transformed correctly";
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
  for(const CompositeTransformation& t : transList)
    os << t;
  ASSERT_LE(transList.size(), 1U) << "Too many Transforms found: " << os.str();
  TransPtr typeT = transList.front().create(AbstractPolicy());
  ASSERT_NE(typeT, nullptr);
	EXPECT_TRUE(typeT->check(in)) << "MetaValue is not supported by TypeTransform";
  Events result = (*typeT)(in);
  ASSERT_GE(result.size(), 1U) << "No Events generated!";
	EXPECT_EQ(result.front(), out) << "Events not transformed correctly";
}

TEST_F(TransformationTestSuite, castedRescaleTest) {
  MetaAttribute& inA  = *in.attribute(Time::value());
  MetaAttribute& outA = *out.attribute(Time::value());
  inA.scale() = MetaScale(Scale<std::milli>());
  inA.value() = f.create({{{1234.5, 0}}}, Float::value());
  outA.value() = f.create({{{1234, 1}}}, UInt64::value());
  registerTypes();
  auto transList = KnowledgeBase::findTransforms(outT);
  EXPECT_GE(transList.size(), 1U) << "Wrong amount of Transforms found!";
  std::ostringstream os;
  for(const CompositeTransformation& t : transList) {
    os << t;
    TransPtr typeT = t.create(AbstractPolicy());
    ASSERT_NE(typeT, nullptr);
	  EXPECT_TRUE(typeT->check(in)) << "MetaValue is not supported by TypeTransform";
    Events result = (*typeT)(in);
    ASSERT_GE(result.size(), 1U) << "No Events generated!";
	  EXPECT_EQ(result.front(), out) << "Events not transformed correctly";
  }
}

}
