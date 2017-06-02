extern "C" {
#include <TH.h>
#include <luaT.h>
}

#define torch_(NAME) TH_CONCAT_3(torch_, Real, NAME)
#define torch_Tensor TH_CONCAT_STRING_3(torch., Real, Tensor)
#define libfilter_(NAME) TH_CONCAT_3(libfilter_, Real, NAME)

#include "generic/filter.cpp"
#include "THGenerateAllTypes.h"

extern "C" { int luaopen_libfilter(lua_State *L)
{
  libfilter_FloatMain_init(L);
  libfilter_DoubleMain_init(L);

  lua_newtable(L);
  lua_pushvalue(L, -1);
  lua_setglobal(L, "libfilter");

  lua_newtable(L);
  luaT_setfuncs(L, libfilter_DoubleMain__, 0);
  lua_setfield(L, -2, "double");

  lua_newtable(L);
  luaT_setfuncs(L, libfilter_FloatMain__, 0);
  lua_setfield(L, -2, "float");

  return 1;
}
}
