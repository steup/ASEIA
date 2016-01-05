#include <cstdint>
#include <cstdlib>

class PrimeGenerator {
	public:
		using Prime=uint32_t;
		static Prime prime(std::size_t i);
		static std::size_t size();
};
