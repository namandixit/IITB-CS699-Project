internal_function
int scriptLog (lua_State *l)
{
    enum Log_Level level = luaL_checknumber(l, 1);
    enum Log_Channel channel = luaL_checknumber(l, 2);
    const char *text = luaL_checkstring(l, 3);

    logConsole(level, channel, text);

    return 0;
}
