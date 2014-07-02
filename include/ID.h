#pragma once

#include <cstdint>
#include <boost/units/systems/si.hpp>

namespace id{
  namespace attribute{
    struct Base{
      using IDType = std::uint8_t;
      static constexpr IDType value(){return 0;}
      static constexpr const char* name(){return "Unspecified   ";}
    };

    struct Position : public Base{
      static constexpr Base::IDType value(){return 1;}
      static constexpr const char* name(){return "Position      ";}
    };
    
    struct Time : public Base{
      static constexpr Base::IDType value(){return 2;}
      static constexpr const char* name(){return "Time          ";}
    };
    
    struct PublisherID : public Base{
      static constexpr Base::IDType value(){return 3;}
      static constexpr const char* name(){return "Publisher ID  ";}
    };
    
    struct Validity : public Base{
      static constexpr Base::IDType value(){return 4;}
      static constexpr const char* name(){return "Validity      ";}
    };

    struct Distance : public Base{
      static constexpr Base::IDType value(){return 5;}
      static constexpr const char* name(){return "Distance      ";}
    };

    struct Orientation : public Base{
      static constexpr Base::IDType value(){return 6;}
      static constexpr const char* name(){return "Orientation   ";}
    };

    struct PolarAngle : public Base{
      static constexpr Base::IDType value(){return 7;}
      static constexpr const char* name(){return "PolarAngle    ";}
    };

    struct PoseReference : public Base{
      static constexpr Base::IDType value(){return 8;}
      static constexpr const char* name(){return "Pose Reference";}
    };

    const char* getName(Base::IDType id){
      switch(id){
        case(Position::value())      : return Position::name();
        case(Time::value())          : return Time::name();
        case(PublisherID::value())   : return PublisherID::name();
        case(Validity::value())      : return Validity::name();
        case(Distance::value())      : return Distance::name();
        case(Orientation::value())   : return Orientation::name();
        case(PolarAngle::value())    : return PolarAngle::name();
        case(PoseReference::value()) : return PoseReference::name();
        default                      : return Base::name();
      }
    }
  }

  namespace type{
    struct Base{
      using IDType = std::uint8_t;
      using Type = void;
      static constexpr IDType value(){return 0;}
      static constexpr const char* name(){return "void  ";}
    };

    struct Int8 : public Base{
      using Type = std::int8_t;
      static constexpr IDType value(){return 1;}
      static constexpr const char* name(){return "int8  ";}
    };

    struct Int16 : public Base{
      using Type = int16_t;
      static constexpr Base::IDType value(){return 2;}
      static constexpr const char* name(){return "int16 ";}
    };

    struct Int32 : public Base{
      using Type = int32_t;
      static constexpr Base::IDType value(){return 3;}
      static constexpr const char* name(){return "int32 ";}
    };

    struct Int64 : public Base{
      using Type = int64_t;
      static constexpr Base::IDType value(){return 4;}
      static constexpr const char* name(){return "int64 ";}
    };

    struct UInt8 : public Base{
      using Type = std::uint8_t;
      static constexpr Base::IDType value(){return 5;}
      static constexpr const char* name(){return "uint8 ";}
    };

    struct UInt16 : public Base{
      using Type = uint16_t;
      static constexpr Base::IDType value(){return 6;}
      static constexpr const char* name(){return "uint16";}
    };

    struct UInt32 : public Base{
      using Type = uint32_t;
      static constexpr Base::IDType value(){return 7;}
      static constexpr const char* name(){return "uint32";}
    };

    struct UInt64 : public Base{
      using Type = uint64_t;
      static constexpr Base::IDType value(){return 8;}
      static constexpr const char* name(){return "uint64";}
    };

    struct Float : public Base{
      using Type = float;
      static constexpr Base::IDType value(){return 9;}
      static constexpr const char* name(){return "float ";}
    };

    struct Double : public Base{
      using Type = double;
      static constexpr Base::IDType value(){return 10;}
      static constexpr const char* name(){return "double";}
    };

    struct Char : public Base{
      using Type = char;
      static constexpr Base::IDType value(){return 11;}
      static constexpr const char* name(){return "char  ";}
    };

    template<typename T>
    struct getTypeID{
      static constexpr Int8   get(int8_t)  {return Int8();}
      static constexpr Int16  get(int16_t) {return Int16();}
      static constexpr Int32  get(int32_t) {return Int32();}
      static constexpr Int64  get(int64_t) {return Int64();}
      static constexpr UInt8  get(uint8_t) {return UInt8();}
      static constexpr UInt16 get(uint16_t){return UInt16();}
      static constexpr UInt32 get(uint32_t){return UInt32();}
      static constexpr UInt64 get(uint64_t){return UInt64();}
      static constexpr Float  get(float)   {return Float();}
      static constexpr Double get(double)  {return Double();}
      using type = decltype(get(T()));
    };

    const char* getName(Base::IDType id){
      switch(id){
        case(UInt8::value())  : return UInt8::name();
        case(UInt16::value()) : return UInt16::name();
        case(UInt32::value()) : return UInt32::name();
        case(UInt64::value()) : return UInt64::name();
        case(Int8::value())   : return Int8::name();
        case(Int16::value())  : return Int16::name();
        case(Int32::value())  : return Int32::name();
        case(Int64::value())  : return Int64::name();
        case(Float::value())  : return Float::name();
        case(Double::value()) : return Double::name();
        default               : return Base::name();
      }
    }
  }

  namespace unit{
    struct Base{
      using IDType = std::uint8_t;
      using Unit = boost::units::si::dimensionless;
      static constexpr IDType value(){return 11;}
      static constexpr const char* name(){return "none";}
      static constexpr const char* shortName(){return "";}
    };
    struct Steradian : public Base{
      using Unit = boost::units::si::solid_angle;
      static constexpr Base::IDType value(){return 0;}
      static constexpr const char* name(){return "steradian";}
      static constexpr const char* shortName(){return "srad";}
    };
    struct Radian : public Base{
      using Unit = boost::units::si::plane_angle;
      static constexpr Base::IDType value(){return 1;}
      static constexpr const char* name(){return "radian";}
      static constexpr const char* shortName(){return "rad";}
    };
    struct Candela : public Base{
      using Unit = boost::units::si::luminous_intensity;
      static constexpr Base::IDType value(){return 2;}
      static constexpr const char* name(){return "candela";}
      static constexpr const char* shortName(){return "cd";}
    };
    struct Mole : public Base{
      using Unit = boost::units::si::amount;
      static constexpr Base::IDType value(){return 3;}
      static constexpr const char* name(){return "mole";}
      static constexpr const char* shortName(){return "mol";}
    };
    struct Kelvin : public Base{
      using Unit = boost::units::si::temperature;
      static constexpr Base::IDType value(){return 4;}
      static constexpr const char* name(){return "kelvin";}
      static constexpr const char* shortName(){return "K";}
    };
    struct Ampere : public Base{
      using Unit = boost::units::si::current;
      static constexpr Base::IDType value(){return 5;}
      static constexpr const char* name(){return "ampere";}
      static constexpr const char* shortName(){return "A";}
    };
    struct Second : public Base{
      using Unit = boost::units::si::time;
      static constexpr Base::IDType value(){return 6;}
      static constexpr const char* name(){return "second";}
      static constexpr const char* shortName(){return "s";}
    };
    struct Kilogram : public Base{
      using Unit = boost::units::si::mass;
      static constexpr Base::IDType value(){return 7;}
      static constexpr const char* name(){return "kilogram";}
      static constexpr const char* shortName(){return "kg";}
    };
    struct Meter : public Base{
      using Unit = boost::units::si::length;
      static constexpr Base::IDType value(){return 8;}
      static constexpr const char* name(){return "meter";}
      static constexpr const char* shortName(){return "m";}
    };

    template<typename BoostUnit>
    struct getUnitID{
      Base      get(boost::units::si::dimensionless)     {return Base();}
      Steradian get(boost::units::si::solid_angle)       {return Steradian();}
      Radian    get(boost::units::si::plane_angle)       {return Radian();}
      Candela   get(boost::units::si::luminous_intensity){return Candela();}
      Mole      get(boost::units::si::amount)            {return Mole();}
      Kelvin    get(boost::units::si::temperature)       {return Kelvin();}
      Ampere    get(boost::units::si::current)           {return Ampere();}
      Second    get(boost::units::si::time)              {return Second();}
      Kilogram  get(boost::units::si::mass)              {return Kilogram();}
      Meter     get(boost::units::si::length)            {return Meter();}
      using type = decltype(get(BoostUnit()));
    };
  }
}
