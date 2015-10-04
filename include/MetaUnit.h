#include <UnitType.h>

class MetaUnit : public UnitType{
  public:
    MetaUnit() = default;

    template<typename Dimensions>
    MetaUnit(const Unit<Dimensions>& copy) : UnitType(copy) {}

    MetaUnit(const UnitType& copy) : UnitType(copy) {}

    MetaUnit& operator=(const UnitType& copy) {
      return *this = copy;
    }

    MetaUnit operator*(const MetaUnit& b) const;
    MetaUnit operator/(const MetaUnit& b) const;
};
