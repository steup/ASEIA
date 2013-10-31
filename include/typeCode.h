#pragma once

#include <ostream>
#include <cstdint>

enum class TypeCode : std::uint8_t
{
  int8,
  int16,
  int32,
  int64,
  uint8,
  uint16,
  uint32,
  uint64,
  float32,
  float64,
  char8,
  unknown
};

template<typename T>
struct getCode;

template<>
struct getCode<int8_t>{static const TypeCode code=TypeCode::int8;};

template<>
struct getCode<int16_t>{static const TypeCode code=TypeCode::int16;};

template<>
struct getCode<int32_t>{static const TypeCode code=TypeCode::int32;};

template<>
struct getCode<int64_t>{static const TypeCode code=TypeCode::int64;};

template<>
struct getCode<uint8_t>{static const TypeCode code=TypeCode::uint8;};

template<>
struct getCode<uint16_t>{static const TypeCode code=TypeCode::uint16;};

template<>
struct getCode<uint32_t>{static const TypeCode code=TypeCode::uint32;};

template<>
struct getCode<uint64_t>{static const TypeCode code=TypeCode::uint64;};

template<>
struct getCode<float>{static const TypeCode code=TypeCode::float32;};

template<>
struct getCode<double>{static const TypeCode code=TypeCode::float64;};

template<>
struct getCode<char>{static const TypeCode code=TypeCode::char8;};

std::ostream& operator<<(std::ostream& o, const TypeCode c)
{
  switch(c)
  {
    case(TypeCode::int8)   :return o << "int8";
    case(TypeCode::int16)  :return o << "int16";
    case(TypeCode::int32)  :return o << "int32";
    case(TypeCode::int64)  :return o << "int64";
    case(TypeCode::uint8)  :return o << "uint8";
    case(TypeCode::uint16) :return o << "uint16";
    case(TypeCode::uint32) :return o << "uint32";
    case(TypeCode::uint64) :return o << "uint64";
    case(TypeCode::float32):return o << "float32";
    case(TypeCode::float64):return o << "float64";
    case(TypeCode::char8)  :return o << "char8";
    case(TypeCode::unknown):return o << "unknown";
  }
  return o;
}
