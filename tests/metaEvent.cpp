#include <MetaEvent.h>
#include <MetaFactory.h>
#include <ratio>

namespace test {

namespace metaEventSuite {

using namespace id::attribute;
using namespace std;

	class MetaEventSuite : public ::testing::Test {
		public:
			ID posID = Position::value();
			MetaFactory& f=MetaFactory::instance();
			MetaEvent e;
			MetaEventSuite(){
				MetaAttribute& position = e.attribute(posID);
				position.value() = f.create({{0,1}, {1,1}, {2,1}});
				position.unit()  = Meter();
				position.scale() = ratio<1, 1000>();
			}
	};

	TEST_F(MetaEventSuite, findAttributeTest) {
		MetaAttribute test = posID;
		test.value() = f.create({{0,1}, {1,1}, {2,1}});
		test.unit()  = Meter();
		test.scale() = ratio<1, 1000>();

		EXPECT_EQ(e.attribute(posID), test) << "Position attribute not stored correctly";
	}
}}
