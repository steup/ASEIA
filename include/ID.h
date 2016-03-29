#pragma once

#include <cstdint>
#include <boost/mpl/int.hpp>

namespace id{
  namespace attribute{

    struct Tag{};

    using ID = std::uint8_t;

    struct Base : public Tag{
      static constexpr const ID value(){return 0;}
    };

    struct Position : public Base{
      static constexpr const ID value(){return 1;}
    };
    
    struct Time : public Base{
      static constexpr const ID value(){return 2;}
    };
    
    struct PublisherID : public Base{
      static constexpr const ID value(){return 3;}
    };
    
    struct Validity : public Base{
      static constexpr const ID value(){return 4;}
    };

    struct Distance : public Base{
      static constexpr const ID value(){return 5;}
    };

    struct Orientation : public Base{
      static constexpr const ID value(){return 6;}
    };

    struct Angle : public Base{
      static constexpr const ID value(){return 7;}
    };

    struct Reference : public Base{
      static constexpr const ID value(){return 8;}
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
      static constexpr const ID value(){return 0;}
    };

    struct UInt8 : public Base{
      using Type = std::uint8_t;
      static constexpr const ID value(){return 1;}
    };

    struct UInt16 : public Base{
      using Type = uint16_t;
      static constexpr const ID value(){return 2;}
    };

    struct UInt32 : public Base{
      using Type = uint32_t;
      static constexpr const ID value(){return 3;}
    };

    struct UInt64 : public Base{
      using Type = uint64_t;
      static constexpr const ID value(){return 4;}
    };

    struct Int8 : public Base{
      using Type = std::int8_t;
      static constexpr const ID value(){return 5;}
    };

    struct Int16 : public Base{
      using Type = int16_t;
      static constexpr const ID value(){return 6;}
    };

    struct Int32 : public Base{
      using Type = int32_t;
      static constexpr const ID value(){return 7;}
    };

    struct Int64 : public Base{
      using Type = int64_t;
      static constexpr const ID value(){return 8;}
    };

    struct Float : public Base{
      using Type = float;
      static constexpr const ID value(){return 9;}
    };

    struct Double : public Base{
      using Type = double;
      static constexpr const ID value(){return 10;}
    };
		
		struct Bool : public Base{
      using Type = bool;
      static constexpr const ID value(){return 11;}
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
    template<> struct id2Type< Bool  ::value() > { using type = Bool; };

    static constexpr const ID id( int8_t   ) { return Int8  ::value(); }
    static constexpr const ID id( int16_t  ) { return Int16 ::value(); }
    static constexpr const ID id( int32_t  ) { return Int32 ::value(); }
    static constexpr const ID id( int64_t  ) { return Int64 ::value(); }
    static constexpr const ID id( uint8_t  ) { return UInt8 ::value(); }
    static constexpr const ID id( uint16_t ) { return UInt16::value(); }
    static constexpr const ID id( uint32_t ) { return UInt32::value(); }
    static constexpr const ID id( uint64_t ) { return UInt64::value(); }
    static constexpr const ID id( float    ) { return Float ::value(); }
    static constexpr const ID id( double   ) { return Double::value(); }
    static constexpr const ID id( bool     ) { return Bool  ::value(); }

    template<typename T> struct t2Type { using type = typename id2Type< id( T() ) >::type; };
  }

  namespace unit{

    struct Tag{};

    using ID = std::uint8_t;

    template<ID id>
    struct Base : public Tag, boost::mpl::int_<id> { };

    using Steradian = Base<0>;
    using Radian    = Base<1>;
    using Candela   = Base<2>;
    using Mole      = Base<3>;
    using Kelvin    = Base<4>;
    using Ampere    = Base<5>;
    using Second    = Base<6>;
    using Kilogram  = Base<7>;
    using Meter     = Base<8>;

    using NumDim    = boost::mpl::int_<9>;

    template<ID id>
    struct id2Type;

    template<> struct id2Type< Steradian::value > { using type = Steradian; };
    template<> struct id2Type< Radian   ::value > { using type = Radian   ; };
    template<> struct id2Type< Candela  ::value > { using type = Candela  ; };
    template<> struct id2Type< Mole     ::value > { using type = Mole     ; };
    template<> struct id2Type< Kelvin   ::value > { using type = Kelvin   ; };
    template<> struct id2Type< Ampere   ::value > { using type = Ampere   ; };
    template<> struct id2Type< Second   ::value > { using type = Second   ; };
    template<> struct id2Type< Kilogram ::value > { using type = Kilogram ; };
    template<> struct id2Type< Meter    ::value > { using type = Meter    ; };
  }

	namespace filterOp {
		struct Tag{};

		using ID = std::uint8_t;

		template<ID id>
		struct Base : public Tag, boost::mpl::int_<id> { };

		using LE = Base<1>;
		using GE = Base<2>;
		using LT = Base<3>;
		using GT = Base<4>;
		using EQ = Base<5>;
		using NE = Base<6>;
		using AE = Base<7>;
		using NA = Base<8>;
	}

	namespace logicOp {
		struct Tag{};

		using ID = std::uint8_t;

		template<ID id>
		struct Base : public Tag, boost::mpl::int_<id> { };

		using AND = Base<1>;
		using OR  = Base<2>;
		using NOT = Base<3>;
	}
}
