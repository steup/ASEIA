#pragma once

#include <ID.h>

namespace id{
  namespace attribute{
    const char* name(ID id);
  }

  namespace type{
    const char* name(ID id);
  }

  namespace unit{
    const char* name(ID id);
    const char* shortName(ID id);
  }
}
