#include <TransformationFactory.h>
#include <MetaFactory.h>
#include <ScaleTransform.h>

namespace test {

struct TransformTestSuite : public ::testing::Test{
	using TransPtr = Transformation::TransPtr;
	using Events = Transformation::Events;
	MetaFactory& f= MetaFactory::instance();
	MetaEvent in;
	EventType inT;
	MetaEvent out;
	EventType outT;
	TransPtr sT;
	TransformTestSuite() {
		MetaAttribute ms(id::attribute::Time::value());
		ms.scale() = MetaScale(std::ratio<1, 1000>());
		ms.unit() = Second();
		ms.value() = f.create({{{1234, 0}}});
		in.add(ms);
		MetaAttribute s = ms;
		s.value() = f.create({{{1, 1}}});
		s.scale() = MetaScale();
		out.add(s);
		inT = (EventType)in;
		outT = (EventType)out;
		sT.reset(new ScaleTransform(outT, {inT}));
	}
};

TEST_F(TransformTestSuite, scaleTransformBasicTest) {
	EXPECT_TRUE(sT->valid()) << "ScaleTransform is not valid!";
	EXPECT_TRUE(sT->check(Events({in}))) << "MetaValue is not supported by ScaleTransform";
	MetaValue temp = (*sT)(Events({in}));
	EXPECT_EQ(temp, out);
}

}
