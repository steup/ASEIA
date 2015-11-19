#include <MetaEvent.h>
#include <MetaFactory.h>
#include <ID.h>
#include <ratio>

namespace tests {

namespace metaEventSuite {

using namespace ::id::attribute;
using namespace std;

	class MetaEventSuite : public ::testing::Test {
		public:
			ID posID = Position::value();
			MetaFactory& f=MetaFactory::instance();
			MetaEvent e;
			MetaEventSuite(){
				MetaAttribute position = posID;
				position.value() = f.create({{{0,1}, {1,1}, {2,1}}});
				position.unit()  = Meter();
				position.scale() = ratio<1, 1000>();
        e.add(position);
			}
	};

	TEST_F(MetaEventSuite, findAttributeTest) {
		MetaAttribute test = posID;
		test.value() = f.create({{{0,1}, {1,1}, {2,1}}});
		test.unit()  = Meter();
		test.scale() = ratio<1, 1000>();
    const MetaAttribute* posPtr = e.attribute(posID);
    ASSERT_NE(posPtr, nullptr) << "Position attribute could not be retrieved";
    const MetaAttribute& pos = *posPtr;
		EXPECT_EQ(pos, test) << "Position attribute not stored correctly";
	}
	
	TEST_F(MetaEventSuite, typeTest) {
		EventType eT;
		eT.add(AttributeType(posID, ValueType(id::type::Double::value(), 1, 3, true), ratio<1,1000>(), Meter()));
		EXPECT_EQ((EventType)e, eT) << "Inferred MetaEvent-Type is wrong";
	}
}}
