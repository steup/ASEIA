
#include <gtest/gtest.h>
#include <ID.h>

/** \namespace ::id::type::attribute
 *  \test Unit-Tests regarding unique number assignment and id::attribute::lookup of id::attribute::AttrID in ::tests::id::attr
 **/

/** \namespace ::id::type
 *  \test Unit-Tests regarding unique number assignment and id::type::lookup of id::type::TypeID in ::tests::id::type
 **/

/** \namespace ::id::unit
 *  \test Unit-Tests regarding unique number assignment and id::unit::lookup of id::unit::UnitID in ::tests::id::unit
 **/

namespace tests {

/** \brief ID Test Suites **/
namespace id {

/** \brief AttrID Test Suite **/
namespace attr {
	using namespace ::id::attribute;
	/** \brief AttrID number assignment test case
   *
   *	unit test checking the correct assignment of unqiue integers to static Attribute IDs
	 **/
	TEST(AttrIdTestSuite, valueTest) {
		EXPECT_EQ(Base::value()       , 0) << "ID of attribute \"Base\" does not match ";
		EXPECT_EQ(Position::value()   , 1) << "ID of attribute \"Position\" does not match ";
		EXPECT_EQ(Time::value()       , 2) << "ID of attribute \"Time\" does not match ";
		EXPECT_EQ(PublisherID::value(), 3) << "ID of attribute \"PublisherID\" does not match ";
		EXPECT_EQ(Validity::value()   , 4) << "ID of attribute \"Validity\" does not match ";
		EXPECT_EQ(Distance::value()   , 5) << "ID of attribute \"Distance\" does not match ";
		EXPECT_EQ(Orientation::value(), 6) << "ID of attribute \"Orientation\" does not match ";
		EXPECT_EQ(Angle::value()      , 7) << "ID of attribute \"Angle\" does not match ";
		EXPECT_EQ(Reference::value()  , 8) << "ID of attribute \"Reference\" does not match ";
	}

	/** \brief AttrID lookup test case
   *
   *	Unit-Test checking the correct lookup of AttributeIDs from integers
	 **/
	TEST(AttrIdTestSuite, lookUpTest) {
		EXPECT_EQ(attribute<1>::type::value(), 1) << "ID of attribute \"Position\" does not match ";
		EXPECT_EQ(attribute<2>::type::value(), 2) << "ID of attribute \"Time\" does not match ";
		EXPECT_EQ(attribute<3>::type::value(), 3) << "ID of attribute \"PublisherID\" does not match ";
		EXPECT_EQ(attribute<4>::type::value(), 4) << "ID of attribute \"Validity\" does not match ";
		EXPECT_EQ(attribute<5>::type::value(), 5) << "ID of attribute \"Distance\" does not match ";
		EXPECT_EQ(attribute<6>::type::value(), 6) << "ID of attribute \"Orientation\" does not match ";
		EXPECT_EQ(attribute<7>::type::value(), 7) << "ID of attribute \"Angle\" does not match ";
		EXPECT_EQ(attribute<8>::type::value(), 8) << "ID of attribute \"Reference\" does not match ";
	}
}

/** \brief TypeID Test Suites **/
namespace type {
	using namespace ::id::type;

	/** \brief Type ID assignment test case
	 *	\test %Unit test checking the correct assignment of integers to Type IDs
	 **/
	TEST(IdTypeSuite, valueTest) {
		EXPECT_EQ(Base::value()  ,  0) << "ID of type \"Base\" does not match ";
		EXPECT_EQ(UInt8::value() ,  1) << "ID of type \"UInt8\" does not match ";
		EXPECT_EQ(UInt16::value(),  2) << "ID of type \"UInt16\" does not match ";
		EXPECT_EQ(UInt32::value(),  3) << "ID of type \"UInt32\" does not match ";
		EXPECT_EQ(Int8::value()  ,  4) << "ID of type \"Int8\" does not match ";
		EXPECT_EQ(Int16::value() ,  5) << "ID of type \"Int16\" does not match ";
		EXPECT_EQ(Int32::value() ,  6) << "ID of type \"Int32\" does not match ";
		EXPECT_EQ(Float::value() ,  7) << "ID of type \"Float\" does not match ";
		EXPECT_EQ(Double::value(),  8) << "ID of type \"Double\" does not match ";
	}

	/** \brief Type ID lookup from integer test case
	 *	\test %Unit test checking the correct lookup of Type IDs from integers
	 **/
	TEST(IdTypeSuite, lookUpTest) {
		EXPECT_EQ(id2Type< 1>::type::value(),  1) << "ID of type \"UInt8\" does not match ";
		EXPECT_EQ(id2Type< 2>::type::value(),  2) << "ID of type \"UInt16\" does not match ";
		EXPECT_EQ(id2Type< 3>::type::value(),  3) << "ID of type \"UInt32\" does not match ";
		EXPECT_EQ(id2Type< 4>::type::value(),  4) << "ID of type \"Int8\" does not match ";
		EXPECT_EQ(id2Type< 5>::type::value(),  5) << "ID of type \"Int16\" does not match ";
		EXPECT_EQ(id2Type< 6>::type::value(),  6) << "ID of type \"Int32\" does not match ";
		EXPECT_EQ(id2Type< 7>::type::value(),  7) << "ID of type \"Float\" does not match ";
		EXPECT_EQ(id2Type< 8>::type::value(),  8) << "ID of type \"Double\" does not match ";
	}
	
	/** \brief Type integer lookup from type test case
	 *	\test %Unit test checking the correct lookup of Type integers from types
	 **/
	TEST(IdTypeSuite, getIdTest) {
		EXPECT_EQ(id::type::id(uint8_t()) ,  1) << "ID of type \"UInt8\" does not match ";
		EXPECT_EQ(id::type::id(uint16_t()),  2) << "ID of type \"UInt16\" does not match ";
		EXPECT_EQ(id::type::id(uint32_t()),  3) << "ID of type \"UInt32\" does not match ";
		EXPECT_EQ(id::type::id(int8_t())  ,  4) << "ID of type \"Int8\" does not match ";
		EXPECT_EQ(id::type::id(int16_t()) ,  5) << "ID of type \"Int16\" does not match ";
		EXPECT_EQ(id::type::id(int32_t()) ,  6) << "ID of type \"Int32\" does not match ";
		EXPECT_EQ(id::type::id(float())   ,  7) << "ID of type \"Float\" does not match ";
		EXPECT_EQ(id::type::id(double())  ,  8) << "ID of type \"Double\" does not match ";
	}
}
/** \brief UnitID Test Suites **/
namespace unit {
	using namespace ::id::unit;

	/** \brief %Unit ID assignment test case
	 *	\test %Unit test checking the correct assignment of integers to %Attribute IDs
	 **/
	TEST(IdUnitSuite, valueTest) {
		EXPECT_EQ(Steradian::value , 0) << "ID of unit \"Steradian\" does not match ";
		EXPECT_EQ(Radian::value    , 1) << "ID of unit \"Radian\" does not match ";
		EXPECT_EQ(Candela::value   , 2) << "ID of unit \"Candela\" does not match ";
		EXPECT_EQ(Mole::value      , 3) << "ID of unit \"Mole\" does not match ";
		EXPECT_EQ(Kelvin::value    , 4) << "ID of unit \"Kelvin\" does not match ";
		EXPECT_EQ(Ampere::value    , 5) << "ID of unit \"Ampere\" does not match ";
		EXPECT_EQ(Second::value    , 6) << "ID of unit \"Second\" does not match ";
		EXPECT_EQ(Kilogram::value  , 7) << "ID of unit \"Kilogram\" does not match ";
		EXPECT_EQ(Meter::value     , 8) << "ID of unit \"Meter\" does not match ";
	}
	
	/** \brief %Unit ID lookup test case
	 *	\test %Unit test checking the correct lookup of %Unit IDs from integers
	 **/
	TEST(IdUnitSuite, lookUpTest) {
		EXPECT_EQ(id2Type<0>::type::value, 0) << "ID of unit \"Steradian\" does not match ";
		EXPECT_EQ(id2Type<1>::type::value, 1) << "ID of unit \"Radian\" does not match ";
		EXPECT_EQ(id2Type<2>::type::value, 2) << "ID of unit \"Candela\" does not match ";
		EXPECT_EQ(id2Type<3>::type::value, 3) << "ID of unit \"Mole\" does not match ";
		EXPECT_EQ(id2Type<4>::type::value, 4) << "ID of unit \"Kelvin\" does not match ";
		EXPECT_EQ(id2Type<5>::type::value, 5) << "ID of unit \"Ampere\" does not match ";
		EXPECT_EQ(id2Type<6>::type::value, 6) << "ID of unit \"Second\" does not match ";
		EXPECT_EQ(id2Type<7>::type::value, 7) << "ID of unit \"Kilogram\" does not match ";
		EXPECT_EQ(id2Type<8>::type::value, 8) << "ID of unit \"Meter\" does not match ";
	}
}}}
