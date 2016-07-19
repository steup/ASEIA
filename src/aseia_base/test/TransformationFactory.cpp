#include <TransformationFactory.h>

#include <MetaEvent.h>

#include <algorithm>

namespace test {

struct TransformationFactoryTestSuite : public ::testing::Test {
	using TransPtr = TransformationFactory::TransPtr;
	using TransID = TransformationFactory::TransID;
	using EventTypes = TransformationFactory::EventTypes;
	using Events = Transformation::Events;
	TransformationFactory& f = TransformationFactory::instance();
	struct ExampleTransform : public Transformation {
		ExampleTransform(const EventType& out, const EventTypes& in) { mValid = true; }
		MOCK_CONST_METHOD1(check, bool(const Events& events));
		MOCK_METHOD1(call, MetaEvent(const Events& events));
		virtual MetaEvent operator()(const Events& events) { return call(events); }
		MOCK_CONST_METHOD1(print, void(std::ostream& o));
	};
	struct MultiTrans0 : public ExampleTransform {
		MultiTrans0(const EventType& out, const EventTypes& in) : ExampleTransform(out, in) {}
	};
	struct MultiTrans1 : public ExampleTransform {
		MultiTrans1(const EventType& out, const EventTypes& in) : ExampleTransform(out, in) {}
	};
	struct InvalidTransform : public ExampleTransform {
		InvalidTransform(const EventType& out, const EventTypes& in)
			: ExampleTransform(out, in)
		{
			mValid=false;
		}
	};
	EventType outT;
	EventType inT;
	EventTypes inTList = { &inT };
};

TEST_F(TransformationFactoryTestSuite, basicTest) {
	TransID example = f.registerTransformation<ExampleTransform>();
	EXPECT_GE(example, 0) << "Invalid transformation id returned";
	TransPtr t = f.create(outT, inTList, example);
	ASSERT_TRUE((bool)t) << "Registered transformation not found!";
	EXPECT_NO_THROW(ExampleTransform& trans = dynamic_cast<ExampleTransform&>(*t);) << "Wrong transformation returned for transformation id " << example;
}

TEST_F(TransformationFactoryTestSuite, multiCreateTest) {
	TransID example = f.registerTransformation<ExampleTransform>();
	std::vector<TransPtr> trans;
	for(unsigned int i=0;i<1000;i++) {
		trans.push_back(f.create(outT, inTList, example));
		ASSERT_TRUE((bool)trans.back()) << "Transformation " << i << " not correctly created!";
	}
	std::sort(trans.begin(), trans.end());
	EXPECT_EQ(std::unique(trans.begin(), trans.end()), trans.end()) <<  "Dynamically created transformations are not independent";
}

TEST_F(TransformationFactoryTestSuite, multiTransTest) {
	TransID mT0 = f.registerTransformation<MultiTrans0>();
	TransID mT1 = f.registerTransformation<MultiTrans1>();
	TransID mT2 = f.registerTransformation<MultiTrans1>();
	ASSERT_NE(mT0, mT1) << "Registered transformations have non-unique IDs";
	EXPECT_EQ(mT1, mT2) << "Multi-registered transformations have different IDs";
	TransPtr t0Ptr = f.create(outT, inTList, mT0);
	TransPtr t1Ptr = f.create(outT, inTList, mT1);
	ASSERT_TRUE((bool)t0Ptr)  << "Transformation MultiTrans0 not correctly created";
	ASSERT_TRUE((bool)t1Ptr)  << "Transformation MultiTrans1 not correctly created";
	EXPECT_NE(t0Ptr, t1Ptr) << "Created transformations are identical";
	EXPECT_NO_THROW(dynamic_cast<MultiTrans0&>(*t0Ptr)) << "Wrong type of MultiTrans0";
	EXPECT_NO_THROW(dynamic_cast<MultiTrans1&>(*t1Ptr)) << "Wrong type of MultiTrans1";
	EXPECT_NO_THROW(dynamic_cast<ExampleTransform&>(*t0Ptr)) << "Wrong inheritance of MultiTrans0 (Missig ExampleTransformation)";
	EXPECT_NO_THROW(dynamic_cast<ExampleTransform&>(*t1Ptr)) << "Wrong inheritance of MultiTrans1 (Missig ExampleTransformation)";
	EXPECT_THROW(dynamic_cast<MultiTrans1&>(*t0Ptr), std::bad_cast) << "Wrong inheritance of MultiTrans0 (Also MultiTrans1)";
	EXPECT_THROW(dynamic_cast<MultiTrans0&>(*t1Ptr), std::bad_cast) << "Wrong inheritance of MultiTrans1 (Also MultiTrans0)";
}

TEST_F(TransformationFactoryTestSuite, invalidIDTest) {
	TransPtr t = f.create(outT, inTList, 0);
	ASSERT_FALSE((bool)t)  << "Invalid id not correctly handled";
	TransID iT = f.registerTransformation<InvalidTransform>();
	TransPtr iTPtr = f.create(outT, inTList, iT);
	ASSERT_FALSE((bool)iTPtr)  << "Invalid Transformation somehow correctly created";
}
}
