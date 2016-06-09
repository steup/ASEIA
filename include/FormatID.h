#pragma once

#include <cstdint.h>
#include <Serializer.h>
#include <DeSerializer.h>
#include <Endianess.h>

class FormatID{
	private:
		uint32_t mHash;
  public:
    FormatID() = default;
    FormatID(const EventType& eT);
    uint32_t value() const { return mHash; }
    bool operator==(const FormatID& b) const { return mHash == b.mHash; }
    bool operator<(const FormatID& b) const { return mHash < b.mHash; }
    static constexpr std::size_t size() noexcept { return sizeof(mHash); }

	template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, FormatID&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const FormatID& id){
  return s << htonl(id.value());
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, FormatID& id){
  d >> id.mHash;
  id.mHash=ntohl(id.mHash);
  return d;
}
