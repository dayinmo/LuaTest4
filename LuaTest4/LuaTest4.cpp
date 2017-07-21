#include "stdafx.h"
#include <iostream>  
#include <string.h>  
using namespace std;

extern "C"
{
#include "lua.h"  
#include "lauxlib.h"  
#include "lualib.h"  
}
static void ReadTableFromItem(lua_State *L, const char* lpszTableName, const char* lpszTableItem)
{
	printf("it vvv= %d\n", lua_gettop(L));
	lua_getglobal(L, lpszTableName);
	printf("it1 = %d\n", lua_gettop(L));
	lua_pushstring(L, lpszTableItem);
	lua_gettable(L, -2);
	printf("%s.%s=%d\n", lpszTableName, lpszTableItem, (int)lua_tonumber(L, -1));
	lua_pop(L, 2);
}

static void ReadTableFromIndex(lua_State *L, const char* lpszTableName, int index)
{
	lua_getglobal(L, lpszTableName);
	printf("it2 = %d\n", lua_gettop(L));
	lua_rawgeti(L, -1, index);
	printf("%s[%d]=%d\n", lpszTableName, index, (int)lua_tonumber(L, -1));
	lua_pop(L, 2);
}

static void EnumTableItem(lua_State *L, const char* lpszTableName)
{
	lua_getglobal(L, lpszTableName);
	int it = lua_gettop(L);
	printf("it = %d\n", it);
	lua_pushnil(L);
	printf("Enum %s:", lpszTableName);
	while (lua_next(L, it))
	{
		printf("  %d", (int)lua_tonumber(L, -1));
		lua_pop(L, 1);
	}
	printf("\n");
	lua_pop(L, 1);
}

int main(int argc, char* argv[])
{
	lua_State *L = lua_open();
	printf("LuaTable = %d\n", lua_gettop(L));
	luaopen_base(L);
	printf("LuaTable = %d\n", lua_gettop(L));
	luaL_dofile(L, "LuaTable.lua");
	printf("LuaTable = %d\n", lua_gettop(L));
	ReadTableFromItem(L, "luat_Test1", "a");  // 等价与lua代码：print(luat_Test1.a)
	ReadTableFromItem(L, "luat_Test1", "b");
	ReadTableFromItem(L, "luat_Test1", "c");
	EnumTableItem(L, "luat_Test1");    // 枚举Table

	ReadTableFromIndex(L, "luat_Test2", 1);  // 等价与lua代码：print(luat_Test1[1])
	ReadTableFromIndex(L, "luat_Test2", 2);
	ReadTableFromIndex(L, "luat_Test2", 3);
	EnumTableItem(L, "luat_Test2");
	lua_close(L);
	return 0;
}