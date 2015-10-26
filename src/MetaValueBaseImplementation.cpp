#include <MetaValueBaseImplementation.h>
#include <ostream>

MetaValueBaseImplementation MetaValueBaseImplementation::sInstance;

std::ostream& MetaValueBaseImplementation::print( std::ostream& o ) const { 
      return o << "void"; 
}

std::ostream& operator<<(std::ostream& o, const MetaValueBaseImplementation& mvbi) {
  return mvbi.print(o);
}
