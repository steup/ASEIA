#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <EventStorage.h>
#include <ID.h>
#include <algorithm>
#include <iterator>

namespace test {

 using ::testing::Contains;
 using ::testing::Pointee;
 using ::testing::ElementsAre;
 using ::testing::Each;
 using ::testing::SizeIs;
 using ::testing::UnorderedElementsAre;
 using EventTypes = EventStorage::EventTypes;
 using namespace std;

  struct EventStorageTestSuite : public ::testing::Test {
    EventType eT0, eT1, eT2;
    AbstractPolicy policy;
    EventStorageTestSuite() {
      ValueType v(id::type::UInt8::value(), 1, 1, false);
      AttributeType aT0(251, v, Scale<>(), Dimensionless());
      AttributeType aT1(252, v, Scale<>(), Dimensionless());
      AttributeType aT2(253, v, Scale<>(), Dimensionless());
      eT0.add(aT0);
      eT1.add(aT1);
      eT2.add(aT2);
    }

};

TEST_F(EventStorageTestSuite, singularStorage) {
  EventTypes in = {eT0};
  EventStorage s(in, policy);
  MetaEvent e(eT0);
  MetaAttribute* a = e.attribute(251);
  ASSERT_NE(a, nullptr);
  a->value().set(0,0,{1});
  s.addEvent(e);
  vector<vector<const MetaEvent*>> result;
  copy(s.begin(), s.end(), back_inserter(result));
  EXPECT_THAT(result,
    UnorderedElementsAre(
      ElementsAre(Pointee(e))
    )
  );
}

TEST_F(EventStorageTestSuite, homogeneusDualStorage) {
  EventTypes in = {eT0, eT0};
  EventStorage s(in, policy);
  MetaEvent e0(eT0);
  MetaAttribute* a = e0.attribute(251);
  ASSERT_NE(a, nullptr);
  a->value().set(0,0,{1});
  s.addEvent(e0);
  MetaEvent e1(e0);
  a = e1.attribute(251);
  ASSERT_NE(a, nullptr);
  a->value().set(0,0,{2});
  s.addEvent(e1);
  vector<vector<const MetaEvent*>> result;
  copy(s.begin(), s.end(), back_inserter(result));
  EXPECT_THAT(result,
    UnorderedElementsAre(
      ElementsAre(Pointee(e0), Pointee(e0)),
      ElementsAre(Pointee(e0), Pointee(e1)),
      ElementsAre(Pointee(e1), Pointee(e0)),
      ElementsAre(Pointee(e1), Pointee(e1))
    )
  );
}

TEST_F(EventStorageTestSuite, heterogeneusDualStorage) {
  EventTypes in = {eT0, eT1};
  EventStorage s(in, policy);
  MetaEvent e0(eT0);
  MetaAttribute* a = e0.attribute(251);
  ASSERT_NE(a, nullptr);
  a->value().set(0,0,{1});
  s.addEvent(e0);
  MetaEvent e1(eT1);
  a = e1.attribute(252);
  ASSERT_NE(a, nullptr);
  a->value().set(0,0,{2});
  s.addEvent(e1);
  vector<vector<const MetaEvent*>> result;
  copy(s.begin(), s.end(), back_inserter(result));
  EXPECT_THAT(result,
    UnorderedElementsAre(
      ElementsAre(Pointee(e0), Pointee(e1))
    )
  );

}

TEST_F(EventStorageTestSuite, largeStorage) {
  EventTypes in = {eT0, eT1, eT2};
  EventStorage s(in, policy);
  for(const EventType& eT: in)
    for(size_t i=0;i<policy.bufferSize;i++) {
      MetaEvent e(eT);
      ASSERT_GT(eT.size(), 0U);
      MetaAttribute* a = e.attribute(eT.begin()->id());
      ASSERT_NE(a, nullptr);
      a->value().set(0,0,{(double)i});
      s.addEvent(e);
    }
  vector<vector<const MetaEvent*>> result;
  copy(s.begin(), s.end(), back_inserter(result));
  EXPECT_THAT(result, SizeIs(pow(policy.bufferSize, 3U)));
  EXPECT_THAT(result, Each(SizeIs(3)));
}

}
