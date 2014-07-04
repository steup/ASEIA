#pragma once

#include <cstdint>
#include <boost/units/systems/si.hpp>
#include <boost/units/physical_dimensions/length.hpp>

namespace id{
  namespace attribute{

    using ID = std::uint8_t;

    struct Base{
      static constexpr ID value(){return 0;}
      static constexpr const char* name(){return "Unspecified   ";}
    };

    struct Position : public Base{
      static constexpr ID value(){return 1;}
      static constexpr const char* name(){return "Position      ";}
    };
    
    struct Time : public Base{
      static constexpr ID value(){return 2;}
      static constexpr const char* name(){return "Time          ";}
    };
    
    struct PublisherID : public Base{
      static constexpr ID value(){return 3;}
      static constexpr const char* name(){return "Publisher ID  ";}
    };
    
    struct Validity : public Base{
      static constexpr ID value(){return 4;}
      static constexpr const char* name(){return "Validity      ";}
    };

    struct Distance : public Base{
      static constexpr ID value(){return 5;}
      static constexpr const char* name(){return "Distance      ";}
    };

    struct Orientation : public Base{
      static constexpr ID value(){return 6;}
      static constexpr const char* name(){return "Orientation   ";}
    };

    struct Angle : public Base{
      static constexpr ID value(){return 7;}
      static constexpr const char* name(){return "Angle         ";}
    };

    struct Reference : public Base{
      static constexpr ID value(){return 8;}
      static constexpr const char* name(){return "Reference";}
    };

    const char* name(ID id){
      switch(id){
        case(Position::value())    : return Position::name();
        case(Time::value())        : return Time::name();
        case(PublisherID::value()) : return PublisherID::name();
        case(Validity::value())    : return Validity::name();
        case(Distance::value())    : return Distance::name();
        case(Orientation::value()) : return Orientation::name();
        case(Angle::value())       : return Angle::name();
        case(Reference::value())   : return Reference::name();
        default                    : return Base::name();
      }
    }

    template<ID id>
    struct attribute;

    template<> struct attribute<Position::value()>    { using type = Position;    };
    template<> struct attribute<Time::value()>        { using type = Time;        };
    template<> struct attribute<PublisherID::value()> { using type = PublisherID; };
    template<> struct attribute<Validity::value()>    { using type = Validity;    };
    template<> struct attribute<Distance::value()>    { using type = Distance;    };
    template<> struct attribute<Orientation::value()> { using type = Orientation; };
    template<> struct attribute<Angle::value()>       { using type = Angle;       };
    template<> struct attribute<Reference::value()>   { using type = Reference;   };
  }

  namespace type{

    using ID = std::uint8_t;

    struct Base{
      using Type = void;
      static constexpr ID value(){return 0;}
      static constexpr const char* name(){return "void  ";}
    };

    struct Int8 : public Base{
      using Type = std::int8_t;
      static constexpr ID value(){return 1;}
      static constexpr const char* name(){return "int8  ";}
    };

    struct Int16 : public Base{
      using Type = int16_t;
      static constexpr ID value(){return 2;}
      static constexpr const char* name(){return "int16 ";}
    };

    struct Int32 : public Base{
      using Type = int32_t;
      static constexpr ID value(){return 3;}
      static constexpr const char* name(){return "int32 ";}
    };

    struct Int64 : public Base{
      using Type = int64_t;
      static constexpr ID value(){return 4;}
      static constexpr const char* name(){return "int64 ";}
    };

    struct UInt8 : public Base{
      using Type = std::uint8_t;
      static constexpr ID value(){return 5;}
      static constexpr const char* name(){return "uint8 ";}
    };

    struct UInt16 : public Base{
      using Type = uint16_t;
      static constexpr ID value(){return 6;}
      static constexpr const char* name(){return "uint16";}
    };

    struct UInt32 : public Base{
      using Type = uint32_t;
      static constexpr ID value(){return 7;}
      static constexpr const char* name(){return "uint32";}
    };

    struct UInt64 : public Base{
      using Type = uint64_t;
      static constexpr ID value(){return 8;}
      static constexpr const char* name(){return "uint64";}
    };

    struct Float : public Base{
      using Type = float;
      static constexpr ID value(){return 9;}
      static constexpr const char* name(){return "float ";}
    };

    struct Double : public Base{
      using Type = double;
      static constexpr ID value(){return 10;}
      static constexpr const char* name(){return "double";}
    };

    struct Char : public Base{
      using Type = char;
      static constexpr ID value(){return 11;}
      static constexpr const char* name(){return "char  ";}
    };

    template<ID id>
    struct id2Type;

    template<> struct id2Type< Int8  ::value() > { using type = Int8  ; };
    template<> struct id2Type< Int16 ::value() > { using type = Int16 ; };
    template<> struct id2Type< Int32 ::value() > { using type = Int32 ; };
    template<> struct id2Type< Int64 ::value() > { using type = Int64 ; };
    template<> struct id2Type< UInt8 ::value() > { using type = UInt8 ; };
    template<> struct id2Type< UInt16::value() > { using type = UInt16; };
    template<> struct id2Type< UInt32::value() > { using type = UInt32; };
    template<> struct id2Type< UInt64::value() > { using type = UInt64; };
    template<> struct id2Type< Float ::value() > { using type = Float ; };
    template<> struct id2Type< Double::value() > { using type = Double; };

    constexpr ID id( int8_t   ) { return Int8  ::value(); }
    constexpr ID id( int16_t  ) { return Int16 ::value(); }
    constexpr ID id( int32_t  ) { return Int32 ::value(); }
    constexpr ID id( int64_t  ) { return Int64 ::value(); }
    constexpr ID id( uint8_t  ) { return UInt8 ::value(); }
    constexpr ID id( uint16_t ) { return UInt16::value(); }
    constexpr ID id( uint32_t ) { return UInt32::value(); }
    constexpr ID id( uint64_t ) { return UInt64::value(); }
    constexpr ID id( float    ) { return Float ::value(); }
    constexpr ID id( double   ) { return Double::value(); }

    template<typename T> struct t2Type { using type = typename id2Type< id( T() ) >::type; };

    const char* name(ID id){
      switch(id){
        case( UInt8 ::value() ) : return UInt8 ::name();
        case( UInt16::value() ) : return UInt16::name();
        case( UInt32::value() ) : return UInt32::name();
        case( UInt64::value() ) : return UInt64::name();
        case( Int8  ::value() ) : return Int8  ::name();
        case( Int16 ::value() ) : return Int16 ::name();
        case( Int32 ::value() ) : return Int32 ::name();
        case( Int64 ::value() ) : return Int64 ::name();
        case( Float ::value() ) : return Float ::name();
        case( Double::value() ) : return Double::name();
        default                 : return Base  ::name();
      }
    }
  }

  namespace unit{

    using ID = std::uint8_t;

    struct Base{
      using Unit   = boost::units::si::dimensionless;
      static constexpr ID value(){return 9;}
      static constexpr const char* name(){return "none";}
      static constexpr const char* shortName(){return "";}
    };
    struct Steradian : public Base{
      using Unit = boost::units::si::solid_angle;
      using Dim  = boost::units::solid_angle_base_dimension;
      static constexpr ID value(){return 0;}
      static constexpr const char* name(){return "steradian";}
      static constexpr const char* shortName(){return "srad";}
    };
    struct Radian : public Base{
      using Unit = boost::units::si::plane_angle;
      using Dim  = boost::units::plane_angle_base_dimension;
      static constexpr ID value(){return 1;}
      static constexpr const char* name(){return "radian";}
      static constexpr const char* shortName(){return "rad";}
    };
    struct Candela : public Base{
      using Unit = boost::units::si::luminous_intensity;
      using Dim  = boost::units::luminous_intensity_base_dimension;
      static constexpr ID value(){return 2;}
      static constexpr const char* name(){return "candela";}
      static constexpr const char* shortName(){return "cd";}
    };
    struct Mole : public Base{
      using Unit = boost::units::si::amount;
      static constexpr ID value(){return 3;}
      static constexpr const char* name(){return "mole";}
      static constexpr const char* shortName(){return "mol";}
    };
    struct Kelvin : public Base{
      using Unit = boost::units::si::temperature;
      using Dim  = boost::units::temperature_base_dimension;
      static constexpr ID value(){return 4;}
      static constexpr const char* name(){return "kelvin";}
      static constexpr const char* shortName(){return "K";}
    };
    struct Ampere : public Base{
      using Unit = boost::units::si::current;
      using Dim  = boost::units::current_base_dimension;
      static constexpr ID value(){return 5;}
      static constexpr const char* name(){return "ampere";}
      static constexpr const char* shortName(){return "A";}
    };
    struct Second : public Base{
      using Unit = boost::units::si::time;
      using Dim  = boost::units::time_base_dimension;
      static constexpr ID value(){return 6;}
      static constexpr const char* name(){return "second";}
      static constexpr const char* shortName(){return "s";}
    };
    struct Kilogram : public Base{
      using Unit = boost::units::si::mass;
      using Dim  = boost::units::mass_base_dimension;
      static constexpr ID value(){return 7;}
      static constexpr const char* name(){return "kilogram";}
      static constexpr const char* shortName(){return "kg";}
    };
    struct Meter : public Base{
      using Unit = boost::units::si::length;
      using Dim  = boost::units::length_base_dimension;
      static constexpr ID value(){return 8;}
      static constexpr const char* name(){return "meter";}
      static constexpr const char* shortName(){return "m";}
    };

    constexpr ID id( boost::units::solid_angle_base_dimension   ) { return Steradian::value(); }
    constexpr ID id( boost::units::plane_angle_base_dimension   ) { return Radian   ::value(); }
    constexpr ID id( boost::units::luminous_intensity_dimension ) { return Candela  ::value(); }
    constexpr ID id( boost::units::amount_base_dimension        ) { return Mole     ::value(); }
    constexpr ID id( boost::units::temperature_base_dimension   ) { return Kelvin   ::value(); }
    constexpr ID id( boost::units::current_base_dimension       ) { return Ampere   ::value(); }
    constexpr ID id( boost::units::time_base_dimension          ) { return Second   ::value(); }
    constexpr ID id( boost::units::mass_base_dimension          ) { return Kilogram ::value(); }
    constexpr ID id( boost::units::length_base_dimension        ) { return Meter    ::value(); }

    template<ID id>
    struct id2Type;

    template<> struct id2Type< Steradian::value() > { using type = Steradian; };
    template<> struct id2Type< Radian   ::value() > { using type = Radian   ; };
    template<> struct id2Type< Candela  ::value() > { using type = Candela  ; };
    template<> struct id2Type< Mole     ::value() > { using type = Mole     ; };
    template<> struct id2Type< Kelvin   ::value() > { using type = Kelvin   ; };
    template<> struct id2Type< Ampere   ::value() > { using type = Ampere   ; };
    template<> struct id2Type< Second   ::value() > { using type = Second   ; };
    template<> struct id2Type< Kilogram ::value() > { using type = Kilogram ; };
    template<> struct id2Type< Meter    ::value() > { using type = Meter    ; };

    template<typename Tag> struct Tag2Type { using type = typename id2Type< id( Tag() ) >::type; };

    const char* name(ID id){
      switch(id){
        case(Steradian::value()): return Steradian::name();
        case(Radian   ::value()): return Radian   ::name();
        case(Candela  ::value()): return Candela  ::name();
        case(Mole     ::value()): return Mole     ::name();
        case(Kelvin   ::value()): return Kelvin   ::name();
        case(Ampere   ::value()): return Ampere   ::name();
        case(Second   ::value()): return Second   ::name();
        case(Kilogram ::value()): return Kilogram ::name();
        case(Meter    ::value()): return Meter    ::name();
        default                 : return Base     ::name();
      };
    }

    const char* shortName(ID id){
      switch(id){
        case(Steradian::value()): return Steradian::shortName();
        case(Radian   ::value()): return Radian   ::shortName();
        case(Candela  ::value()): return Candela  ::shortName();
        case(Mole     ::value()): return Mole     ::shortName();
        case(Kelvin   ::value()): return Kelvin   ::shortName();
        case(Ampere   ::value()): return Ampere   ::shortName();
        case(Second   ::value()): return Second   ::shortName();
        case(Kilogram ::value()): return Kilogram ::shortName();
        case(Meter    ::value()): return Meter    ::shortName();
        default                 : return Base     ::shortName();
      };
    }
  }
}
