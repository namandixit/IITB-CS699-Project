/** 
* @brief Function to log script information
*
* This function records the logs run by fucntion in c files.Lua script returns any error log occur during the run of script.  
*
* @param l ponter to lua function of type lua_State
* @return int
*/
internal_function
int scriptLog (lua_State *l)
{
    enum Log_Level level = luaL_checknumber(l, 1);
    enum Log_Channel channel = luaL_checknumber(l, 2);
    const char *text = luaL_checkstring(l, 3);

    logConsole(level, channel, text);

    return 0;
}
