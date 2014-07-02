#pragma once

#include <Serializer.h>
#include <DeSerializer.h>

#include <ratio>

class MetaScaleType{
  private:
    uint32_t mNum = 0, mDenom = 0;
  public:
    MetaScaleType() = default;
    
    template<std::intmax_t num, std::intmax_t denom>
    MetaScaleType(const std::ratio<num, denom> scale): mNum(num), mDenom(denom){}

    uint32_t num() const {return mNum;}
    void num(uint32_t num) {mNum=num;}

    uint32_t denom() const {return mDenom;}
    void denom(uint32_t denom) {mDenom=denom;}

    bool operator==(const MetaScaleType& b){
      return mNum==b.mNum && mDenom == b.mDenom;
    }

    constexpr static std::size_t size() noexcept {return sizeof(mNum) + sizeof(mDenom);}

    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, MetaScaleType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaScaleType& scale){
  return s << scale.num() << scale.denom();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaScaleType& scale){
  return d >> scale.mNum >> scale.mDenom;
}
