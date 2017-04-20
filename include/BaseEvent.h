#pragma once

#include <Attribute.h>
#include <Unit.h>
#include <ID.h>
#include <Event.h>
#include <Value.h>

struct BaseConfig
{
  static const Endianess end = hostEndianess;
  using PositionValueType    = Value<float   , 3       >;
  using TimeValueType        = Value<uint32_t, 1       >;
  using PublisherIDValueType = Value<uint32_t, 1, 1, false>;
  using PositionScale        = Scale<>;
  using TimeScale            = Scale<>;
  using PublisherIDScale     = Scale<>;
};

template<typename Config = BaseConfig>
using BaseEvent = Event< Config::end,
                         Attribute < id::attribute::Position,
                                     typename Config::PositionValueType,
                                     Meter,
                                     typename Config::PositionScale
                         >,
                         Attribute < id::attribute::Time,
                                     typename Config::TimeValueType, 
                                     Second,
                                     typename Config::TimeScale
                         >,
                         Attribute < id::attribute::PublisherID,
                                     typename Config::PublisherIDValueType,
                                     Dimensionless,
                                     typename Config::PublisherIDScale
                         >
                  >;

