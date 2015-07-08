#pragma once

#include <cstdint>

struct PODConverter{
	PODConverter(){
		for(uint8_t& byte : bytes)
			byte=0;
	}
	union{
    std::uint8_t bytes[8];
    std::uint8_t uint8;
    std::uint16_t uint16;
    std::uint32_t uint32;
    std::uint64_t uint64;
    std::int8_t int8;
    std::int16_t int16;
    std::int32_t int32;
    std::int64_t int64;
    float _float;
    double _double;
	};
};
