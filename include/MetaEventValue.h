#pragma once

#include <ValueElement.h>

class MetaValueElement{
  private:
    char mStorage[sizeof(ValueElement<uint64_t>)];

  public:
    MetaValueElement() = default;
    
    template<typename T, bool u>
    operator ValueElement<T,u>(){ return *reinterpret_cast<ValueElement<T,u>*>(mStorage); }
};
