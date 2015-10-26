#include <MetaValueBaseImplementation.h>

MetaValueBaseImplementation MetaValueBaseImplementation::sInstance;

std::ostream& operator<<(std::ostream& o, const MetaValueBaseImplementation& mvbi){
	mvbi.print(o);
	return o;
}
