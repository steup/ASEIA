
#include <gtest/gtest.h>
#include <KnowledgeBase.h>
#include <MetaFactory.h>
#include <Transformations.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace test {

 namespace fs = boost::filesystem;
using namespace ::id::attribute;
using namespace ::id::type;

using std::to_string;

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
  inA.value() = MetaValue({{{1234, 0}}});
  outA.value() = MetaValue({{{1.234, 0.0001}}});
  registerTypes();
  auto transList=KnowledgeBase::findTransforms(outT);
  ASSERT_GT(transList.size(), 0U) << "No Transform found!";
  size_t i=0;
  for(const CompositeTransformation& t : transList) {
    fs::path file = fs::current_path()/"doc"/("basicScale"+to_string(i++)+".dot");
    fs::ofstream out(file);
    out << t;
  }
  ASSERT_LE(transList.size(), 1U);
  TransPtr scaleT = transList.front().create(AbstractPolicy());
  ASSERT_NE(scaleT, nullptr);
  EXPECT_TRUE(scaleT->check(in)) << "MetaValue is not supported by ScaleTransform";
  Events result = (*scaleT)(in);
  ASSERT_GE(result.size(), 1U) << "No Events generated!";
  MetaAttribute* resA = result.front().attribute(Time::value());
  ASSERT_NE(resA, nullptr);
  EXPECT_EQ(resA->value().get(0,0).value(), outA.value().get(0,0).value()) <<  resA->value() << " != " << outA.value();
  EXPECT_LE(resA->value().get(0,0).uncertainty(), outA.value().get(0,0).uncertainty()) <<  resA->value() << " != " << outA.value();
}

TEST_F(TransformationTestSuite, typeTransformBasicTest) {
  MetaAttribute& inA  = *in.attribute(Time::value());
  MetaAttribute& outA = *out.attribute(Time::value());
  inA.value() = MetaValue({{{1234.5, 0}}}, Float::value());
  outA.value() = MetaValue({{{1234, 1}}}, UInt32::value());
  registerTypes();
  auto transList = KnowledgeBase::findTransforms(outT);
  ASSERT_GT(transList.size(), 0U) << "No Transform found!";
  size_t i=0;
  for(const CompositeTransformation& t : transList) {
    fs::path file = fs::current_path()/"doc"/("basicType"+to_string(i++)+".dot");
    fs::ofstream out(file);
    out << t;
  }

  ASSERT_LE(transList.size(), 1U);
  TransPtr typeT = transList.front().create(AbstractPolicy());
  ASSERT_NE(typeT, nullptr);
  EXPECT_TRUE(typeT->check(in)) << "MetaValue is not supported by TypeTransform";
  Events result = (*typeT)(in);
  ASSERT_GE(result.size(), 1U) << "No Events generated!";
  MetaAttribute* resA = result.front().attribute(Time::value());
  ASSERT_NE(resA, nullptr);
  EXPECT_EQ(resA->value().get(0,0).value(), outA.value().get(0,0).value()) <<  resA->value() << " != " << outA.value();
  EXPECT_EQ(resA->value().get(0,0).uncertainty(), outA.value().get(0,0).uncertainty()) <<  resA->value() << " != " << outA.value();
}

TEST_F(TransformationTestSuite, castedRescaleTest) {
  MetaAttribute& inA  = *in.attribute(Time::value());
  MetaAttribute& outA = *out.attribute(Time::value());
  inA.scale() = MetaScale(Scale<std::milli>());
  inA.value() = MetaValue({{{1234.5, 0}}}, Float::value());
  outA.value() = MetaValue({{{1, 1}}}, UInt32::value());
  registerTypes();
  auto transList = KnowledgeBase::findTransforms(outT);
  EXPECT_GE(transList.size(), 1U);
  size_t i=0;
  for(const CompositeTransformation& t : transList) {
    fs::path file = fs::current_path()/"doc"/("scaleType"+to_string(i++)+".dot");
    fs::ofstream out(file);
    out << t;
    TransPtr typeT = t.create(AbstractPolicy());
    ASSERT_NE(typeT, nullptr);
    EXPECT_TRUE(typeT->check(in)) << "MetaValue is not supported by TypeTransform";
    Events result = (*typeT)(in);
    ASSERT_GE(result.size(), 1U) << "No Events generated!";
    MetaAttribute* resA = result.front().attribute(Time::value());
    ASSERT_NE(resA, nullptr);
    EXPECT_EQ(resA->value().rows(), 1U);
    EXPECT_EQ(resA->value().cols(), 1U);
    EXPECT_EQ(resA->value().get(0,0).value(), outA.value().get(0,0).value()) <<  resA->value() << " != " << outA.value();
    EXPECT_EQ(resA->value().get(0,0).uncertainty(), outA.value().get(0,0).uncertainty()) <<  resA->value() << " != " << outA.value();
  }
}

}
