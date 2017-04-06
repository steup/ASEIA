
#include <gtest/gtest.h>
#include <Serializer.h>

#include <vector>
#include <iterator>

namespace tests {

/** \brief POD Serialization Test Suite **/
namespace serialize {
	using namespace std;
	
	/** \class Serializer
	 *  \test POD Serialization Test Suite \see tests::serialize
	 **/

	/** \brief Used Serialization Output Iterator **/
	using Iter = back_insert_iterator<vector<uint8_t>>;

	/** \brief Serialization Test Fixture class
	 * 	
	 * 	Provides basic data storage to test correct serialization of POD values to binary packet buffers.
	 **/
	class SerializeSuite : public ::testing::Test, public Serializer<Iter> {
		private:
			/** \brief Fixed size data storage **/
			vector<uint8_t> mBuffer;
		public:
			/** \brief Data access method to enable comparision
			 * 	\tparam T the data type contained in mBuffer
			 * 	\param dummy a dummy parameter feeding the type T
			 * 	\return a copy of the  data storage casted to type T
			 **/
			template<typename T>
			const T data(T dummy) const { return *(const T*)mBuffer.data(); }
			/** \brief Gets the current size of the data storage
			 *  \return the current size of mBuffer
			 **/
			size_t size() const { return mBuffer.size(); }
			/** \brief Constructor initializing data storage **/
			SerializeSuite() : Serializer<Iter>(back_inserter(mBuffer)){}
			
	};

	/** \brief uint8_t serialization test
	 *
	 *  \test Unit test checking the correct serialization of uint8_t and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, uint8Test) {
		uint8_t value = 13;
		*this << value;
		EXPECT_EQ(value, 13) << "uint8_t was not correctly serialized";
		EXPECT_EQ(size(), 1U) << "serialized uint8_t had wrong size";
	}

	/** \brief uint16_t serialization test
	 *
	 *  \test Unit test checking the correct serialization of uint16_t and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, uint16Test) {
		uint16_t value = 1337;
		*this << value;
		EXPECT_EQ(value, data(uint16_t())) << "uint16_t was not correctly serialized";
		EXPECT_EQ(size(), 2U) << "serialized uint16_t had wrong size";
	}
	
	/** \brief uint32_t serialization test
	 *
	 *  \test Unit test checking the correct serialization of uint32_t and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, uint32Test) {
		uint32_t value = 13371337;
		*this << value;
		EXPECT_EQ(value, data(uint32_t())) << "uint32_t was not correctly serialized";
		EXPECT_EQ(size(), 4U) << "serialized uint32_t had wrong size";
	}
	
	/** \brief uint64_t serialization test
	 *
	 *  \test Unit test checking the correct serialization of uint64_t and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, uint64Test) {
		uint64_t value = 1337133713371337;
		*this << value;
		EXPECT_EQ(value, data(uint64_t())) << "uint64_t was not correctly serialized";
		EXPECT_EQ(size(), 8U) << "serialized uint64_t had wrong size";
	}
	
	/** \brief int8_t serialization test
	 *
	 *  \test Unit test checking the correct serialization of int8_t and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, int8Test) {
		int8_t value = -13;
		*this << value;
		EXPECT_EQ(value, data(int8_t())) << "int8_t was not correctly serialized";
		EXPECT_EQ(size(), 1U) << "serialized int8_t had wrong size";

	}

	/** \brief int16_t serialization test
	 *
	 *  \test Unit test checking the correct serialization of int16_t and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, int16Test) {
		int16_t value = -1337;
		*this << value;
		EXPECT_EQ(value, data(int16_t())) << "int16_t was not correctly serialized";
		EXPECT_EQ(size(), 2U) << "serialized int16_t had wrong size";
	}
	
	/** \brief int32_t serialization test
	 *
	 *  \test Unit test checking the correct serialization of int32_t and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, int32Test) {
		int32_t value = -13371337;
		*this << value;
		EXPECT_EQ(value, data(int32_t())) << "int32_t was not correctly serialized";
		EXPECT_EQ(size(), 4U) << "serialized int32_t had wrong size";
	}
	
	/** \brief int64_t serialization test
	 *
	 *  \test Unit test checking the correct serialization of int64_t and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, int64Test) {
		int64_t value = -1337133713371337;
		*this << value;
		EXPECT_EQ(value, data(int64_t())) << "int64_t was not correctly serialized";
		EXPECT_EQ(size(), 8U) << "serialized int64_t had wrong size";
	}

	/** \brief float serialization test
	 *
	 *  \test Unit test checking the correct serialization of float and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, floatTest) {
		float value = -1337.1337;
		*this << value;
		EXPECT_EQ(value, data(float())) << "float was not correctly serialized";
		EXPECT_EQ(size(), 4U) << "serialized float had wrong size";
	}
	
	/** \brief double serialization test
	 *
	 *  \test Unit test checking the correct serialization of double and the correct size of the data fed to the data storage
	 **/
	TEST_F(SerializeSuite, doubleTest) {
		double value = -13371337.13371337;
		*this << value;
		EXPECT_EQ(value, data(double())) << "double was not correctly serialized";
		EXPECT_EQ(size(), 8U) << "serialized double had wrong size";
	}
}}
