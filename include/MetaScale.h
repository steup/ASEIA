#pragma once

#include <ScaleType.h>

class MetaScale : public ScaleType {
  private:
    void normalize();
  public:
    MetaScale() = default;
    template<std::intmax_t num, std::intmax_t denom>
    MetaScale(const std::ratio<num, denom> scale) : ScaleType(scale) {}

    MetaScale operator*(const MetaScale& b) const;
    MetaScale operator/(const MetaScale& b) const;
};
