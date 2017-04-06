
#include <gtest/gtest.h>
#include <EventID.h>
#include <BaseEvent.h>
#include <ID.h>
#include <IO.h>
#include <FormatID.h>

#include <vector>
#include <tuple>
#include <boost/mpl/list.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/mpl/for_each.hpp>

namespace tests {

namespace eventTestSuite {

using namespace ::id::attribute;
using namespace ::boost::mpl;

struct ScaleTestSuite : public ::testing::Test{
  struct createEvent  {
    template<typename Scale>
    struct apply {
      struct EventConfig : public BaseConfig {
        using PositionScale = Scale;
      };
      using type = std::tuple<Position, Scale, BaseEvent<EventConfig>>;
    };
  };

  struct createExtendedEvent  {
    template<typename Scale>
    struct apply {
      using ValueType = Value<float, 1>;
      using Attr = Attribute<Angle, ValueType, Radian, Scale>;
      using ExtE = typename BaseEvent<>::append<Attr>::type;
      using type = std::tuple<Angle, Scale, ExtE>;
    };
  };
  
  struct FormatTester {
    template<typename Tuple>
    void operator()(Tuple t){
      using A = typename std::tuple_element<0, Tuple>::type;
      using S = typename std::tuple_element<1, Tuple>::type;
      using E = typename std::tuple_element<2, Tuple>::type;
      EventType type  = (EventType)E();
      EventType comp = type;
      comp.attribute(A::value())->scale() = (ScaleType)S();
      EXPECT_EQ(type, comp) << "Scale Types are not equivalent " << type.attribute(A::value())->scale() 
                            << " <-> " << comp.attribute(A::value())->scale() ;
      EXPECT_EQ(FormatID(type), FormatID(comp)) << "FormatIDs are not eqivalent";
    }
  };
  template<typename T0>
  struct FormatInEqHelper {
    template<typename T1>
    void operator()(T1 t){
      using E0 = typename std::tuple_element<2, T0>::type;
      using E1 = typename std::tuple_element<2, T1>::type;
      EventType t0  = (EventType)E0();
      EventType t1  = (EventType)E1();
      EXPECT_NE(t0, t1) << "Scale Types are equivalent " << t0 << " <-> " << t1;
      EXPECT_NE(FormatID(t0), FormatID(t1)) << "FormatIDs are eqivalent";
    }
  };
  template<typename List>
  struct FormatInEqTester {
    template<typename Tuple>
    void operator()(Tuple t){
      using CleanList = typename boost::mpl::remove<List, Tuple>::type;
      for_each<CleanList>(FormatInEqHelper<Tuple>());
    }
  };

  using ScaleList = list< Scale<>,
                          Scale<std::milli>,
                          Scale<std::ratio<1>, 1>,
                          Scale<std::milli, 1>
                    >::type;
  using BasicEventList = transform<ScaleList, createEvent>::type;
  using ExtendedEventList = transform<ScaleList, createExtendedEvent>::type;
};

TEST_F(ScaleTestSuite, equalityTest) {
  for_each< BasicEventList >(FormatTester());
  for_each< ExtendedEventList >(ScaleTestSuite::FormatTester());
}
TEST_F(ScaleTestSuite, inEqualityTest) {
  for_each< BasicEventList >(FormatInEqTester<BasicEventList>());
  for_each< ExtendedEventList >(FormatInEqTester<ExtendedEventList>());
}

}}
