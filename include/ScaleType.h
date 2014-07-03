#pragma once

#include <Serializer.h>
#include <DeSerializer.h>

#include <ratio>

class ScaleType{
  private:
    uint32_t mNum = 0, mDenom = 0;
  public:
    ScaleType() = default;
    
    template<std::intmax_t num, std::intmax_t denom>
    ScaleType(const std::ratio<num, denom> scale): mNum(num), mDenom(denom){}

    uint32_t num() const {return mNum;}
    void num(uint32_t num) {mNum=num;}

    uint32_t denom() const {return mDenom;}
    void denom(uint32_t denom) {mDenom=denom;}

    bool operator==(const ScaleType& b){
      return mNum==b.mNum && mDenom == b.mDenom;
    }

    constexpr static std::size_t size() noexcept {return sizeof(mNum) + sizeof(mDenom);}

    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, ScaleType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const ScaleType& scale){
  return s << scale.num() << scale.denom();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, ScaleType& scale){
  return d >> scale.mNum >> scale.mDenom;
}
