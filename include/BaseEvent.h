#pragma once

#include <Attribute.h>
#include <ID.h>
#include <Event.h>
#include <Value.h>

struct BaseConfig
{
  static const Endianess end = hostEndianess;
  using PositionValueType    = Value<float   , 3       >;
  using TimeValueType        = Value<uint64_t, 1       >;
  using PublisherIDValueType = Value<uint64_t, 1, false>;
  using ValidityValueType    = Value<float   , 1, false>;
  using PositionScale        = std::ratio<1>;
  using TimeScale            = std::ratio<1>;
  using PublisherIDScale     = std::ratio<1>;
  using ValidityScale        = std::ratio<1>;
};

template<typename Config = BaseConfig>
using BaseEvent = Event< Config::end,
  Attribute < id::attribute::Position   , typename Config::PositionValueType
              , boost::units::si::length       ,typename Config::PositionScale >,

  Attribute < id::attribute::Time       , typename Config::TimeValueType
              , boost::units::si::time         ,typename Config::TimeScale     >,

  Attribute < id::attribute::PublisherID, typename Config::PublisherIDValueType
              , boost::units::si::dimensionless,typename Config::PublisherIDScale >,

  Attribute < id::attribute::Validity   , typename Config::ValidityValueType
              , boost::units::si::dimensionless,typename Config::ValidityScale >
>;
