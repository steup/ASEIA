#include <gtest/gtest.h>

#include <id.test>
#include <serializer.test>

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}	
