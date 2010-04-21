#include "..\include\Lua\LuaInterface.h"

using namespace WyvernsAssault;

bool LuaInterface::luaLoadScript(const char* name)
{
	/* run the script */
	int retval = lua_dofile(L, name);

	return (retval == 0);
}