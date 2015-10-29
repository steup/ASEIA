#include <DeSerializer.h>

#include <array>

/** \class DeSerializer
 *  \test POD DeSerialization Test Suite\see tests::deserialize
 **/

namespace tests {

/** \brief POD DeSerializion Test Suite **/
namespace deserialize {
	using namespace std;
	
	/** \brief Used Forward Iterator **/
	using Iter = array<uint8_t,  8>::const_iterator;

	/** \brief Fixture class providing data storage
	 *
	 *  This class provides the necessary data storage to fed raw data to the DeSerializer
	 **/
	class DeSerializeSuite : public ::testing::Test, public DeSerializer<Iter> {
		private:
			/** \brief Fixed-size data storage **/
			array<uint8_t, 8> mBuffer;
		public:
			/** \brief Data setting function
			 *  \tparam T the type of the input data
			 *  \param v the input data to be put in the storage
			 *
			 *  This function copies the provided input data to the fixed-size storage
			 **/
			template<typename T>
			void data(T v) { *(T*)mBuffer.data()=v;}
			/** \brief Get size of deserialized data
			 *  \return number of bytes read from the fixed-size storage
			 **/
			size_t size() const { return (size_t)(iterator() - mBuffer.begin()); }
			/** \brief Construct a DeSerializer with a fixed-size storage **/
			DeSerializeSuite() : DeSerializer<Iter>(mBuffer.begin(), mBuffer.end()){}
			
	};

	/** \brief uint8_t deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw uint8_t value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, uint8Test) {
		uint8_t value;
		data((uint8_t)13);
		*this >> value;
		EXPECT_EQ(value, (uint8_t)13) << "uint8_t was not correctly deserialized";
		EXPECT_EQ(size(), 1) << "uint8_t was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize uint8_t";
	}

	/** \brief uint16_t deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw uint16_t value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, uint16Test) {
		uint16_t value;
		data((uint16_t)1337);
		*this >> value;
		EXPECT_EQ(value, (uint16_t)1337) << "uint16_t was not correctly deserialized";
		EXPECT_EQ(size(), 2) << "uint16 was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize uint16";
	}
	
	/** \brief uint32_t deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw uint32_t value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, uint32Test) {
		uint32_t value;
		data((uint32_t)13371337);
		*this >> value;
		EXPECT_EQ(value, (uint32_t)13371337) << "uint32_t was not correctly deserialized";
		EXPECT_EQ(size(), 4) << "uint32 was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize uint32";
	}
	
	/** \brief uint64_t deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw uint64_t value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, uint64Test) {
		uint64_t value;
		data((uint64_t)1337133713371337);
		*this >> value;
		EXPECT_EQ(value, (uint64_t)1337133713371337) << "uint64_t was not correctly deserialized";
		EXPECT_EQ(size(), 8) << "uint64 was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize uint64";
	}
	
	/** \brief int8_t deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw int8_t value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, int8Test) {
		int8_t value;
		data((int8_t)-13);
		*this >> value;
		EXPECT_EQ(value, (int8_t)-13) << "int8_t was not correctly deserialized";
		EXPECT_EQ(size(), 1) << "int8 was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize int8";
	}

	/** \brief int16_t deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw int16_t value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, int16Test) {
		int16_t value;
		data((int16_t)-1337);
		*this >> value;
		EXPECT_EQ(value, (int16_t)-1337) << "int16_t was not correctly deserialized";
		EXPECT_EQ(size(), 2) << "int16 was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize int16";
	}
	
	/** \brief int32_t deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw int32_t value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, int32Test) {
		int32_t value;
		data((int32_t)-13371337);
		*this >> value;
		EXPECT_EQ(value, (int32_t)-13371337) << "int32_t was not correctly deserialized";
		EXPECT_EQ(size(), 4) << "int32 was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize int32";
	}
	
	/** \brief int64_t deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw int64_t value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, int64Test) {
		int64_t value;
		data((int64_t)-1337133713371337);
		*this >> value;
		EXPECT_EQ(value, (int64_t)-1337133713371337) << "int64_t was not correctly deserialized";
		EXPECT_EQ(size(), 8) << "int64 was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize int64";
	}

	/** \brief float deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw float value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, floatTest) {
		float value;
		data((float)-1337.1337);
		*this >> value;
		EXPECT_EQ(value, (float)-1337.1337) << "float was not correctly deserialized";
		EXPECT_EQ(size(), 4) << "float was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize float";
	}
	
	/** \brief double deserialization test
	 *
	 *  \test Unit test checking the correct deserialization of a raw double value, the correct amount of read bytes. Additionally check for errors caused by underflow of data storage.
	 **/
	TEST_F(DeSerializeSuite, doubleTest) {
		double value;
		data((double)-13371337.13371337);
		*this >> value;
		EXPECT_EQ(value, (double)-13371337.13371337) << "double was not correctly deserialized";
		EXPECT_EQ(size(), 8) << "double was deserialized with wrong size";
		EXPECT_FALSE(error()) << "Not enough data to deserialize double";
	}
	
	/** \brief Input buffer underrun detection test
	 *
	 *  \test Unit test checking the correct detection of an input buffer underrun.
	 **/
	TEST_F(DeSerializeSuite, shortInputTest) {
		double value;
		*this >> value >> value;
		EXPECT_TRUE(error()) << "Incomplete input not correctly detected";
	}
}}