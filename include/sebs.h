#pragma once

#include <attribute.h>
#include <tuple>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/distance.hpp>
#include <type_traits>

using namespace boost::mpl;

template<AttributeID i>
struct AttrIDCompare
{
  template<typename T>
  struct apply
  {
    using type = typename if_c<i==T::id(), true_, false_>::type;
  };
};


template<typename SchemeDefinition>
class SensorEvent
{
  public:
    using Scheme = SchemeDefinition;

  private:
    SensorEvent() = default;

    struct concat
    {
      template<typename tuple, typename argument>
      struct apply
      {
        using tupArg = std::tuple<argument>;
        using type  =  decltype(std::tuple_cat(tuple(), tupArg()));
      };
    };

    using AttrTuple = typename fold<typename Scheme::AttrList, std::tuple<>, concat>::type;

  private:
    AttrTuple data;

  public:
    template<AttributeID i>
    const typename Scheme::template attribute<i>::type& attribute() const
    {
      using n = typename Scheme::template attribute<i>::pos;
      return std::get<n::value>(data);
    }

    template<AttributeID i>
    typename Scheme::template attribute<i>::type& attribute()
    {
      using n = typename Scheme::template attribute<i>::pos;
      return std::get<n::value>(data);
    }

  template<typename...> friend class SensorEventScheme;
};

template<typename... Attrs>
class SensorEventScheme
{
  private:
    SensorEventScheme() = default;
    using AttrList = typename vector<Attrs...>::type;
  public:
    using EventType = SensorEvent<SensorEventScheme>;
    EventType createEvent() const {return EventType();}

    template<AttributeID i>
    struct attribute
    {
      using compare = AttrIDCompare<i>;
      using iter = typename boost::mpl::find_if<AttrList, compare>::type;
      using begin_iter = typename begin<AttrList>::type;
      using end_iter = typename end<AttrList>::type;
      static_assert(!std::is_same<iter, end_iter>::value, "No such attribute");
      using pos = typename distance<begin_iter, iter>::type;
      using type = typename deref<iter>::type;
    };
  template<typename> friend class SensorEvent;

  template<typename NewAttribute>
  using extent = SensorEventScheme<Attrs..., NewAttribute>;
};

template<typename T, std::size_t s>
using PositionAttribute    = Attribute<AttributeID::position   , T, s, boost::units::si::length>;
template<typename T, std::size_t s>
using TimeAttribute        = Attribute<AttributeID::time       , T, s, boost::units::si::time  >;
template<typename T, std::size_t s>
using PublisherIDAttribute = Attribute<AttributeID::publisherID, T, s>;
template<typename T, std::size_t s>
using ValidityAttribute    = Attribute<AttributeID::validity   , T, s>;

struct SEBSConfig
{
  using PositionType                  = float;
  using TimeType                      = float;
  using ProducerIDType                = uint16_t;
  using ValidityType                  = float;
  static const uint8_t PositionSize   = 3;
  static const uint8_t TimeSize       = 1;
  static const uint8_t ProducerIDSize = 1;
  static const uint8_t ValiditySize   = 1;
};

template<typename Config=SEBSConfig>
class SensorBaseEventScheme : public SensorEventScheme<
  PositionAttribute   < typename Config::PositionType,   Config::PositionSize   >,
  TimeAttribute       < typename Config::TimeType,       Config::TimeSize       >,
  PublisherIDAttribute< typename Config::ProducerIDType, Config::ProducerIDSize >,
  ValidityAttribute   < typename Config::ValidityType, Config::ValiditySize >
>
{
  public:
    SensorBaseEventScheme() = default;
};




template<typename Scheme>
std::ostream& operator<<(std::ostream& o, const SensorEvent<Scheme>& sbe)
{
  o << "Sensor Base Event: " << std::endl;
  o << "\t" <<  sbe.template attribute<AttributeID::position   >() << std::endl;
  o << "\t" <<  sbe.template attribute<AttributeID::time       >() << std::endl;
  o << "\t" <<  sbe.template attribute<AttributeID::publisherID>() << std::endl;
  o << "\t" <<  sbe.template attribute<AttributeID::validity   >() << std::endl;
  return o;
}

template<typename PacketBufferConstIterator, typename Scheme>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& i, SensorEvent<Scheme>& e)
{
  i >> e.template attribute<AttributeID::position>().value();
  i >> e.template attribute<AttributeID::time>().value();
  i >> e.template attribute<AttributeID::publisherID>().value();
  i >> e.template attribute<AttributeID::validity>().value();
  return i;
}

template<typename PacketBufferIterator, typename Scheme>
PacketBufferIterator& operator<<(PacketBufferIterator& i, const SensorEvent<Scheme>& e)
{
  i << e.template attribute<AttributeID::position>().value();
  i << e.template attribute<AttributeID::time>().value();
  i << e.template attribute<AttributeID::publisherID>().value();
  i << e.template attribute<AttributeID::validity>().value();
  return i;
}

template<typename PacketBufferConstIterator, typename AttrList>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& i, SensorEventScheme<AttrList>& e)
{
  i >> e.template attribute<AttributeID::position>();
  i >> e.template attribute<AttributeID::time>();
  i >> e.template attribute<AttributeID::publisherID>();
  i >> e.template attribute<AttributeID::validity>();
  return i;
}

template<typename PacketBufferIterator, typename AttrList>
PacketBufferIterator& operator<<(PacketBufferIterator& i, const SensorEventScheme<AttrList>& e)
{
  i << e.template attribute<AttributeID::position>();
  i << e.template attribute<AttributeID::time>();
  i << e.template attribute<AttributeID::publisherID>();
  i << e.template attribute<AttributeID::validity>();
  return i;
}
