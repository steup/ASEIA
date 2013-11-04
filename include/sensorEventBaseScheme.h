#pragma once

#include <attribute.h>
#include <attributeID.h>
#include <sensorEventScheme.h>

using namespace boost::mpl;

template<typename T, std::size_t s>
using PositionAttribute    = Attribute<AttributeID::Position   , T, s, boost::units::si::length>;
template<typename T, std::size_t s>
using TimeAttribute        = Attribute<AttributeID::Time       , T, s, boost::units::si::time  >;
template<typename T, std::size_t s>
using PublisherIDAttribute = Attribute<AttributeID::PublisherID, T, s>;
template<typename T, std::size_t s>
using ValidityAttribute    = Attribute<AttributeID::Validity   , T, s>;

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
  ValidityAttribute   < typename Config::ValidityType, Config::ValiditySize     >
>
{
  public:
    SensorBaseEventScheme() = default;
};
