#include <UnitType.h>

class MetaUnit : public UnitType{
  public:
    MetaUnit() = default;

    template<typename Dimensions>
    MetaUnit(const Unit<Dimensions>& copy) : UnitType(copy) {}

    MetaUnit(const UnitType& copy) : UnitType(copy) {}

    MetaUnit& operator=(const UnitType& copy) {
      return *this = MetaUnit(copy);
    }
    template<typename Dimensions>
    MetaUnit& operator=(const Unit<Dimensions>& copy) {
      return *this = MetaUnit(copy);
    }

    MetaUnit& operator*=(const MetaUnit& b);
    MetaUnit& operator/=(const MetaUnit& b);
    MetaUnit operator*(const MetaUnit& b) const { return MetaUnit(*this)*=b; }
    MetaUnit operator/(const MetaUnit& b) const { return MetaUnit(*this)/=b; }
    MetaUnit& inverse();
    MetaUnit& sqrt();
};
