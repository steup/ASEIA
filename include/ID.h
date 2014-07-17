#pragma once

#include <cstdint>
#include <boost/units/systems/si.hpp>
#include <boost/units/physical_dimensions/length.hpp>

namespace id{
  namespace attribute{

    struct Tag{};

    using ID = std::uint8_t;

    struct Base : public Tag{
      static constexpr ID value(){return 0;}
    };

    struct Position : public Base{
      static constexpr ID value(){return 1;}
    };
    
    struct Time : public Base{
      static constexpr ID value(){return 2;}
    };
    
    struct PublisherID : public Base{
      static constexpr ID value(){return 3;}
    };
    
    struct Validity : public Base{
      static constexpr ID value(){return 4;}
    };

    struct Distance : public Base{
      static constexpr ID value(){return 5;}
    };

    struct Orientation : public Base{
      static constexpr ID value(){return 6;}
    };

    struct Angle : public Base{
      static constexpr ID value(){return 7;}
    };

    struct Reference : public Base{
      static constexpr ID value(){return 8;}
    };

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

    struct Tag{};

    using ID = std::uint8_t;

    struct Base : public Tag{
      using Type = void;
      static constexpr ID value(){return 0;}
    };

    struct Int8 : public Base{
      using Type = std::int8_t;
      static constexpr ID value(){return 1;}
    };

    struct Int16 : public Base{
      using Type = int16_t;
      static constexpr ID value(){return 2;}
    };

    struct Int32 : public Base{
      using Type = int32_t;
      static constexpr ID value(){return 3;}
    };

    struct Int64 : public Base{
      using Type = int64_t;
      static constexpr ID value(){return 4;}
    };

    struct UInt8 : public Base{
      using Type = std::uint8_t;
      static constexpr ID value(){return 5;}
    };

    struct UInt16 : public Base{
      using Type = uint16_t;
      static constexpr ID value(){return 6;}
    };

    struct UInt32 : public Base{
      using Type = uint32_t;
      static constexpr ID value(){return 7;}
    };

    struct UInt64 : public Base{
      using Type = uint64_t;
      static constexpr ID value(){return 8;}
    };

    struct Float : public Base{
      using Type = float;
      static constexpr ID value(){return 9;}
    };

    struct Double : public Base{
      using Type = double;
      static constexpr ID value(){return 10;}
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

    static constexpr ID id( int8_t   ) { return Int8  ::value(); }
    static constexpr ID id( int16_t  ) { return Int16 ::value(); }
    static constexpr ID id( int32_t  ) { return Int32 ::value(); }
    static constexpr ID id( int64_t  ) { return Int64 ::value(); }
    static constexpr ID id( uint8_t  ) { return UInt8 ::value(); }
    static constexpr ID id( uint16_t ) { return UInt16::value(); }
    static constexpr ID id( uint32_t ) { return UInt32::value(); }
    static constexpr ID id( uint64_t ) { return UInt64::value(); }
    static constexpr ID id( float    ) { return Float ::value(); }
    static constexpr ID id( double   ) { return Double::value(); }

    template<typename T> struct t2Type { using type = typename id2Type< id( T() ) >::type; };
  }

  namespace unit{

    struct Tag{};

    using ID = std::uint8_t;

    struct Base : public Tag{
      using Unit   = boost::units::si::dimensionless;
      static constexpr ID value(){return 9;}
    };
    struct Steradian : public Base{
      using Unit = boost::units::si::solid_angle;
      using Dim  = boost::units::solid_angle_base_dimension;
      static constexpr ID value(){return 0;}
    };
    struct Radian : public Base{
      using Unit = boost::units::si::plane_angle;
      using Dim  = boost::units::plane_angle_base_dimension;
      static constexpr ID value(){return 1;}
    };
    struct Candela : public Base{
      using Unit = boost::units::si::luminous_intensity;
      using Dim  = boost::units::luminous_intensity_base_dimension;
      static constexpr ID value(){return 2;}
    };
    struct Mole : public Base{
      using Unit = boost::units::si::amount;
      static constexpr ID value(){return 3;}
    };
    struct Kelvin : public Base{
      using Unit = boost::units::si::temperature;
      using Dim  = boost::units::temperature_base_dimension;
      static constexpr ID value(){return 4;}
    };
    struct Ampere : public Base{
      using Unit = boost::units::si::current;
      using Dim  = boost::units::current_base_dimension;
      static constexpr ID value(){return 5;}
    };
    struct Second : public Base{
      using Unit = boost::units::si::time;
      using Dim  = boost::units::time_base_dimension;
      static constexpr ID value(){return 6;}
    };
    struct Kilogram : public Base{
      using Unit = boost::units::si::mass;
      using Dim  = boost::units::mass_base_dimension;
      static constexpr ID value(){return 7;}
    };
    struct Meter : public Base{
      using Unit = boost::units::si::length;
      using Dim  = boost::units::length_base_dimension;
      static constexpr ID value(){return 8;}
    };

    static constexpr ID id( boost::units::solid_angle_base_dimension   ) { return Steradian::value(); }
    static constexpr ID id( boost::units::plane_angle_base_dimension   ) { return Radian   ::value(); }
    static constexpr ID id( boost::units::luminous_intensity_dimension ) { return Candela  ::value(); }
    static constexpr ID id( boost::units::amount_base_dimension        ) { return Mole     ::value(); }
    static constexpr ID id( boost::units::temperature_base_dimension   ) { return Kelvin   ::value(); }
    static constexpr ID id( boost::units::current_base_dimension       ) { return Ampere   ::value(); }
    static constexpr ID id( boost::units::time_base_dimension          ) { return Second   ::value(); }
    static constexpr ID id( boost::units::mass_base_dimension          ) { return Kilogram ::value(); }
    static constexpr ID id( boost::units::length_base_dimension        ) { return Meter    ::value(); }

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
  }
}
