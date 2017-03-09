#pragma once

#include <Scale.h>

#include <Serializer.h>
#include <DeSerializer.h>

#include <ratio>

class ScaleType{
  protected:
    int32_t mNum = 1;
    uint32_t mDenom = 1;
    uint32_t mRef = 0;
    public:
      ScaleType() = default;

    template<typename ratio, uint32_t ref>
    ScaleType(const Scale<ratio, ref> scale): mNum(scale.num), mDenom(scale.den), mRef(ref) {}

    int32_t num() const { return mNum; }
    uint32_t denom() const { return mDenom; }
    uint32_t reference() const { return mRef; }

    bool operator==(const ScaleType& b) const {
      return mNum == b.mNum && mDenom == b.mDenom && mRef == b.mRef;
    }
    bool operator!=(const ScaleType& b) const { return !(*this==b); }

    constexpr static std::size_t size() noexcept {return sizeof(mNum) + sizeof(mDenom) + sizeof(mRef);}

    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, ScaleType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const ScaleType& scale){
  return s << scale.num() << scale.denom() << scale.reference();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, ScaleType& scale){
  return d >> scale.mNum >> scale.mDenom >> scale.mRef;
}
