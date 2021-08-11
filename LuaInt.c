#include <stdio.h>
#include <string.h>
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

void bail(lua_State *L, char *msg){
	fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n",
		msg, lua_tostring(L, -1));
	exit(1);
}

int main(void)
{
    lua_State *L=luaL_newstate();                        /* Create Lua state variable */

    luaL_openlibs(L);
    //luaopen_base(L);   /* opens the basic library */
    //luaopen_table(L);  /* opens the table library */
    //luaopen_io(L);     /* opens the I/O library */
    //lua_setglobal(L,"io");
    //luaopen_string(L); /* opens the string lib. */
    //luaopen_math(L);   /* opens the math lib. */
    
    if (luaL_loadfile(L, "luascript.lua"))    /* Load but don't run the Lua script */
    {
        bail(L, "luaL_loadfile() failed");/* Error out if file can't be read */
    }      

    if (lua_pcall(L, 0, 0, 0))                  /* PRIMING RUN. FORGET THIS AND YOU'RE TOAST */
        bail(L, "lua_pcall() failed");          /* Error out if Lua file has an error */

    printf("In C, calling Lua->tellme()\n");

    lua_getglobal(L, "tellme");                 /* Tell it to run callfuncscript.lua->tellme() */
    if (lua_pcall(L, 0, 0, 0))                  /* Run the function */
        bail(L, "lua_pcall() failed");          /* Error out if Lua file has an error */

    printf("Back in C again\n");
    printf("In C, calling Lua->square(6)\n");

    lua_getglobal(L, "square");                 /* Tell it to run callfuncscript.lua->square() */
    lua_pushnumber(L, 6);                       /* Submit 6 as the argument to square() */
    if (lua_pcall(L, 1, 1, 0))                  /* Run function, !!! NRETURN=1 !!! */
        bail(L, "lua_pcall() failed"); 

    printf("Back in C again\n");
    int mynumber = lua_tonumber(L, -1);
    printf("Returned number=%d\n", mynumber);
    printf("In C, calling Lua->tweaktable()\n");
    lua_getglobal(L, "tweaktable");             /* Tell it to run callfuncscript.lua->tweaktable() */
    lua_newtable(L);                            /* Push empty table onto stack table now at -1 */
    lua_pushliteral(L, "fname");                /* Push a key onto the stack, table now at -2 */
    lua_pushliteral(L, "Margie");               /* Push a value onto the stack, table now at -3 */
    lua_settable(L, -3);                        /* Take key and value, put into table at -3, */
    /*  then pop key and value so table again at -1 */

    lua_pushliteral(L, "lname");                /* Push a key onto the stack, table now at -2 */    
    lua_pushliteral(L, "Martinez");             /* Push a value onto the stack, table now at -3 */
    lua_settable(L, -3);                        /* Take key and value, put into table at -3, */
    /*  then pop key and value so table again at -1 */

    if (lua_pcall(L, 1, 1, 0))                  /* Run function, !!! NRETURN=1 !!! */
        bail(L, "lua_pcall() failed"); 

    printf("============ Back in C again, Iterating thru returned table ============\n");

    /* table is in the stack at index 't' */
    lua_pushnil(L);  /* Make sure lua_next starts at beginning */
    const char *k, *v;
    while (lua_next(L, -2)) {                    /* TABLE LOCATED AT -2 IN STACK */
        v = lua_tostring(L, -1);                 /* Value at stacktop */
        lua_pop(L,1);                            /* Remove value */
        k = lua_tostring(L, -1);                 /* Read key at stacktop, */
        /* leave in place to guide next lua_next() */
        printf("Fromc k=>%s<, v=>%s<\n", k, v);
    }

    lua_close(L);                               /* Clean up, free the Lua state var */
    return 0;
}






//int main(void) {
//  char buff[256];
//  int error;
//  lua_State *L = luaL_newstate();
//  luaopen_base(L);   /* opens the basic library */
//  luaopen_table(L);  /* opens the table library */
//  luaopen_io(L);     /* opens the I/O library */
//  luaopen_string(L); /* opens the string lib. */
//  luaopen_math(L);   /* opens the math lib. */
//
//  while (fgets(buff, sizeof(buff), stdin) != NULL) {
//    error =
//        luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
//    if (error) {
//      fprintf(stderr, "%s", lua_tostring(L, -1));
//      lua_pop(L, 1); /* pop error message from the stack */
//    }
//  }
//
//  lua_close(L);
//  return 0;
//}
