#include <IDIO.h>

namespace id {
  namespace attribute {
    const char* name(ID id) {
      switch(id){
        case(Position::value())      : return "Position      ";
        case(Time::value())          : return "Time          ";
        case(PublisherID::value())   : return "Publisher ID  ";
        case(Validity::value())      : return "Validity      ";
        case(Distance::value())      : return "Distance      ";
        case(Orientation::value())   : return "Orientation   ";
        case(Angle::value())         : return "Angle         ";
        case(Reference::value())     : return "Reference     ";
        case(ObjectID::value())      : return "ObjectID      ";
        case(ObjectType::value())    : return "ObjectType    ";
        case(OccupancyGrid::value()) : return "OccupancyGrid ";
        default                      : return "Unspecified   ";
      }
    }
  }
  
  namespace type {
    const char* name(ID id) {
      switch(id){
        case( UInt8 ::value() ) : return "uint8 ";
        case( UInt16::value() ) : return "uint16";
        case( UInt32::value() ) : return "uint32";
        case( UInt64::value() ) : return "uint64";
        case( Int8  ::value() ) : return "int8  ";
        case( Int16 ::value() ) : return "int16 ";
        case( Int32 ::value() ) : return "int32 ";
        case( Int64 ::value() ) : return "int64 ";
        case( Float ::value() ) : return "float ";
        case( Double::value() ) : return "double";
        default                 : return "void  ";
      }
    }
  }

  namespace unit {
    const char* name(ID id) {
      switch(id){
        case(Steradian::value): return "steradian";
        case(Radian   ::value): return "radian";
        case(Candela  ::value): return "candela";
        case(Mole     ::value): return "mole";
        case(Kelvin   ::value): return "kelvin";
        case(Ampere   ::value): return "ampere";
        case(Second   ::value): return "second";
        case(Kilogram ::value): return "kilogram";
        case(Meter    ::value): return "meter";
        default                 : return "none";
      };
    }

    const char* shortName(ID id) {
      switch(id){
        case(Steradian::value): return "srad";
        case(Radian   ::value): return "rad";
        case(Candela  ::value): return "cd";
        case(Mole     ::value): return "mol";
        case(Kelvin   ::value): return "K";
        case(Ampere   ::value): return "A";
        case(Second   ::value): return "s";
        case(Kilogram ::value): return "kg";
        case(Meter    ::value): return "m";
        default                 : return "";
      };
    }
  }
}
