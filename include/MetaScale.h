#pragma once

#include <ScaleType.h>

class MetaScale : public ScaleType {
  private:
    void normalize();
  public:
    MetaScale() = default;
		MetaScale(const ScaleType& copy) : ScaleType(copy) {}
    template<typename ratio, uint32_t ref>
    MetaScale(const Scale<ratio, ref>& scale) : ScaleType(scale) {}

    MetaScale operator*(const MetaScale& b) const;
    MetaScale operator/(const MetaScale& b) const;
    MetaScale& operator*=(const MetaScale& b);
    MetaScale& operator/=(const MetaScale& b);
};
