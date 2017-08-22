#pragma once

#include <cstdint>
#include <boost/mpl/int.hpp>

namespace id{
  namespace attribute{

    struct Tag{};

    using ID = std::uint8_t;

    template<uint8_t id>
    struct AttrID : public Tag {
      static constexpr const ID value(){ return id; }
      operator ID() const { return id; }
    };

    using Base        = AttrID<0>;
    using Position    = AttrID<1>;
    using Time        = AttrID<2>;
    using PublisherID = AttrID<3>;
    using Validity    = AttrID<4>;
    using Distance    = AttrID<5>;
    using Orientation = AttrID<6>;
    using Angle       = AttrID<7>;
    using Reference   = AttrID<8>;
    using Object      = AttrID<9>;
    using Speed       = AttrID<10>;

    template<ID id>
    struct attribute {
      using type = AttrID<id>;
    };
  }

  namespace type{

    struct Tag{};

    using ID = std::uint8_t;

    template<uint8_t id, typename T>
    struct TypeID : public Tag {
      static constexpr const ID value(){ return id; }
      operator ID() const { return id; }
      using Type = T;
    };

    using Base   = TypeID<0, void>;
    using UInt8  = TypeID<1, uint8_t>;
    using UInt16 = TypeID<2, uint16_t>;
    using UInt32 = TypeID<3, uint32_t>;
    using Int8   = TypeID<4, int8_t>;
    using Int16  = TypeID<5, int16_t>;
    using Int32  = TypeID<6, int32_t>;
    using Float  = TypeID<7, float>;
    using Double = TypeID<8, double>;
    using Bool   = TypeID<9, bool>;

/*  doesn work because of uncertainty computation and numeric_limits errors
    using Base = TypeID<10, void>;
 */
/*  doesn work because of uncertainty computation and numeric_limits errors
    using Base = TypeID<11, void>;
 */

    template<ID id>
    struct id2Type;

    template<> struct id2Type< Int8  ::value() > { using type = Int8  ; };
    template<> struct id2Type< Int16 ::value() > { using type = Int16 ; };
    template<> struct id2Type< Int32 ::value() > { using type = Int32 ; };
//    template<> struct id2Type< Int64 ::value() > { using type = Int64 ; };
    template<> struct id2Type< UInt8 ::value() > { using type = UInt8 ; };
    template<> struct id2Type< UInt16::value() > { using type = UInt16; };
    template<> struct id2Type< UInt32::value() > { using type = UInt32; };
//    template<> struct id2Type< UInt64::value() > { using type = UInt64; };
    template<> struct id2Type< Float ::value() > { using type = Float ; };
    template<> struct id2Type< Double::value() > { using type = Double; };
    template<> struct id2Type< Bool  ::value() > { using type = Bool; };

    static constexpr const ID id( int8_t   ) { return Int8  ::value(); }
    static constexpr const ID id( int16_t  ) { return Int16 ::value(); }
    static constexpr const ID id( int32_t  ) { return Int32 ::value(); }
//    static constexpr const ID id( int64_t  ) { return Int64 ::value(); }
    static constexpr const ID id( uint8_t  ) { return UInt8 ::value(); }
    static constexpr const ID id( uint16_t ) { return UInt16::value(); }
    static constexpr const ID id( uint32_t ) { return UInt32::value(); }
//    static constexpr const ID id( uint64_t ) { return UInt64::value(); }
    static constexpr const ID id( float    ) { return Float ::value(); }
    static constexpr const ID id( double   ) { return Double::value(); }
    static constexpr const ID id( bool     ) { return Bool  ::value(); }

    template<typename T> struct t2Type { using type = typename id2Type< id( T() ) >::type; };
    inline bool smaller(ID a, ID b) {
      switch(a) {
        case(UInt8::value()):
          switch(b) {
            case(UInt8::value()): return false;
            default    : return true;
          }
        case(Int8::value()):
          switch(b) {
            case(UInt8::value()): return false;
            case(Int8::value()) : return false;
            default    : return true;
          }
        case(UInt16::value()):
          switch(b) {
            case(UInt8::value()): return false;
            case(Int8::value()) : return false;
            default    : return true;
          }
        case(Int16::value()):
          switch(b) {
            case(UInt8::value()): return false;
            case(Int8::value()) : return false;
            case(UInt16::value()): return false;
            default    : return true;
          }
        case(UInt32::value()):
          switch(b) {
            case(UInt8::value()): return false;
            case(Int8::value()) : return false;
            case(UInt16::value()): return false;
            case(Int16::value()): return false;
            default    : return true;
          }
        case(Int32::value()):
          switch(b) {
            case(UInt8::value()): return false;
            case(Int8::value()) : return false;
            case(UInt16::value()): return false;
            case(Int16::value()): return false;
            case(UInt32::value()): return false;
            default    : return true;
          }
        case(Float::value()):
          switch(b) {
            case(Double::value()) : return true;
            default               : return false;
          }
        default             : return false;
      }
    }
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
		struct Base : public Tag, boost::mpl::int_<id> {
      operator ID() const { return id; }
    };

		using NOP = Base< 0>;
		using LE  = Base< 1>;
		using GE  = Base< 2>;
		using LT  = Base< 3>;
		using GT  = Base< 4>;
		using EQ  = Base< 5>;
		using NE  = Base< 6>;
		using AE  = Base< 7>;
		using NA  = Base< 8>;
		using AND = Base< 9>;
		using OR  = Base< 10>;
		using NOT = Base< 11>;

    using UNC = Base< 16>;
    using CER = Base< 17>;
    using NOR = Base< 18>;

		bool isLogical(ID id);
	}
}
