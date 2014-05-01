#pragma once

#include <cstdint>
#include <boost/units/systems/si/base.hpp>

namespace id{
  namespace attribute{
    struct Base{
      using IDType = std::uint8_t;
      constexpr IDType value(){return 0;}
      constexpr const char* name(){return "Unspecified";}
    };

    struct Position : public Base{
      constexpr Base::IDType value(){return 1;}
      constexpr const char* name(){return "Position";}
    };
    
    struct Time : public Base{
      constexpr Base::IDType value(){return 2;}
      constexpr const char* name(){return "Time";}
    };
    
    struct PublisherID : public Base{
      constexpr Base::IDType value(){return 3;}
      constexpr const char* name(){return "Publisher ID";}
    };
    
    struct Validity : public Base{
      constexpr Base::IDType value(){return 4;}
      constexpr const char* name(){return "Validity";}
    };

    struct Distance : public Base{
      constexpr Base::IDType value(){return 5;}
      constexpr const char* name(){return "Distance";}
    };
  }

  namespace type{
    struct Base{
      using IDType = std::uint8_t;
      using Type = void;
      constexpr IDType value(){return 0;}
      constexpr const char* name(){return "void";}
    };

    struct Int8 : public Base{
      using Type = std::int8_t;
      constexpr IDType value(){return 1;}
      constexpr const char* name(){return "int8";}
    };

    struct Int16 : public Base{
      using Type = int16_t;
      constexpr Base::IDType value(){return 2;}
      constexpr const char* name(){return "int16";}
    };

    struct Int32 : public Base{
      using Type = int32_t;
      constexpr Base::IDType value(){return 3;}
      constexpr const char* name(){return "int32";}
    };

    struct Int64 : public Base{
      using Type = int64_t;
      constexpr Base::IDType value(){return 4;}
      constexpr const char* name(){return "int64";}
    };

    struct UInt8 : public Base{
      using Type = std::uint8_t;
      constexpr Base::IDType value(){return 5;}
      constexpr const char* name(){return "uint8";}
    };

    struct UInt16 : public Base{
      using Type = uint16_t;
      constexpr Base::IDType value(){return 6;}
      constexpr const char* name(){return "uint16";}
    };

    struct UInt32 : public Base{
      using Type = uint32_t;
      constexpr Base::IDType value(){return 7;}
      constexpr const char* name(){return "uint32";}
    };

    struct UInt64 : public Base{
      using Type = uint64_t;
      constexpr Base::IDType value(){return 8;}
      constexpr const char* name(){return "uint64";}
    };

    struct Float : public Base{
      using Type = float;
      constexpr Base::IDType value(){return 9;}
      constexpr const char* name(){return "float";}
    };

    struct Double : public Base{
      using Type = double;
      constexpr Base::IDType value(){return 10;}
      constexpr const char* name(){return "double";}
    };

    struct Char : public Base{
      using Type = char;
      constexpr Base::IDType value(){return 11;}
      constexpr const char* name(){return "char";}
    };

    namespace helpers{
      template<typename T>
      struct getTypeID;

      template<>
      struct getTypeID<int8_t>{using type=Int8;};
      template<>
      struct getTypeID<int16_t>{using type=Int16;};
      template<>
      struct getTypeID<int32_t>{using type=Int32;};
      template<>
      struct getTypeID<int64_t>{using type=Int64;};
      template<>
      struct getTypeID<uint8_t>{using type=UInt8;};
      template<>
      struct getTypeID<uint16_t>{using type=UInt16;};
      template<>
      struct getTypeID<uint32_t>{using type=UInt32;};
      template<>
      struct getTypeID<uint64_t>{using type=UInt64;};
      template<>
      struct getTypeID<float>{using type=Float;};
      template<>
      struct getTypeID<double>{using type=Double;};
      template<>
      struct getTypeID<char>{using type=Char;};
    }
    template<typename T>
    struct getTypeID : public helpers::getTypeID<T>{};
  }

  namespace unit{
    struct Base{
      using IDType = std::uint8_t;
      using Type = boost::units::si::dimensionless;
      constexpr IDType value(){return 11;}
      constexpr const char* name(){return "none";}
      constexpr const char* shortName(){return "";}
    };
    struct Steradian : public Base{
      using Type = boost::units::angle::steradian_base_unit;
      constexpr Base::IDType value(){return 0;}
      constexpr const char* name(){return "steradian";}
      constexpr const char* shortName(){return "srad";}
    };
    struct Radian : public Base{
      using Type = boost::units::angle::radian_base_unit;
      constexpr Base::IDType value(){return 1;}
      constexpr const char* name(){return "radian";}
      constexpr const char* shortName(){return "rad";}
    };
    struct Candela : public Base{
      using Type = boost::units::si::candela_base_unit;
      constexpr Base::IDType value(){return 2;}
      constexpr const char* name(){return "candela";}
      constexpr const char* shortName(){return "cd";}
    };
    struct Mole : public Base{
      using Type = boost::units::si::mole_base_unit;
      constexpr Base::IDType value(){return 3;}
      constexpr const char* name(){return "mole";}
      constexpr const char* shortName(){return "mol";}
    };
    struct Kelvin : public Base{
      using Type = boost::units::si::kelvin_base_unit;
      constexpr Base::IDType value(){return 4;}
      constexpr const char* name(){return "kelvin";}
      constexpr const char* shortName(){return "K";}
    };
    struct Ampere : public Base{
      using Type = boost::units::si::ampere_base_unit;
      constexpr Base::IDType value(){return 5;}
      constexpr const char* name(){return "ampere";}
      constexpr const char* shortName(){return "A";}
    };
    struct Second : public Base{
      using Type = boost::units::si::second_base_unit;
      constexpr Base::IDType value(){return 6;}
      constexpr const char* name(){return "second";}
      constexpr const char* shortName(){return "s";}
    };
    struct Kilogram : public Base{
      using Type = boost::units::si::kilogram_base_unit;
      constexpr Base::IDType value(){return 7;}
      constexpr const char* name(){return "kilogram";}
      constexpr const char* shortName(){return "kg";}
    };
    struct Meter : public Base{
      using Type = boost::units::si::meter_base_unit;
      constexpr Base::IDType value(){return 8;}
      constexpr const char* name(){return "meter";}
      constexpr const char* shortName(){return "m";}
    };
    namespace helpers{
      template<typename BoostUnit>
      struct getUnitID;

      template<>
      struct getUnitID<boost::units::si::dimensionless>{using type = Base;};
      template<>
      struct getUnitID<boost::units::angle::steradian_base_unit>{using type = Steradian;};
      template<>
      struct getUnitID<boost::units::angle::radian_base_unit>{using type = Radian;};
      template<>
      struct getUnitID<boost::units::si::candela_base_unit>{using type = Candela;};
      template<>
      struct getUnitID<boost::units::si::mole_base_unit>{using type = Mole;};
      template<>
      struct getUnitID<boost::units::si::kelvin_base_unit>{using type = Kelvin;};
      template<>
      struct getUnitID<boost::units::si::ampere_base_unit>{using type = Ampere;};
      template<>
      struct getUnitID<boost::units::si::second_base_unit>{using type = Second;};
      template<>
      struct getUnitID<boost::units::si::kilogram_base_unit>{using type = Kilogram;};
      template<>
      struct getUnitID<boost::units::si::meter_base_unit>{using type = Meter;};
    }
    template<typename BoostUnit>
    struct getUnitID : public helpers::getUnitID<BoostUnit> {};
  }
}
