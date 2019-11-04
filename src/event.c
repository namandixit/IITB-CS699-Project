internal_function
void eventKeyboard (lua_State *l,
                    char *key, char *state)
{
    // ... Events
    lua_newtable(l);
    // ... Events <TABLE>

    lua_pushstring(l, "Keyboard");
    // ... Events <TABLE> "Keyboard"
    lua_setfield(l, -2, "Device");

    // ... Events <TABLE>

    lua_pushstring(l, key);
    // ... Events <TABLE> key
    lua_setfield(l, -2, "Key");

    // ... Events <Table>

    lua_pushstring(l, state);
    // ... Events <TABLE> state
    lua_setfield(l, -2, "State");

    // ... Events <Table>

    Size events_size = lua_objlen(l, -2);
    lua_rawseti(l, -2, (Sint)events_size + 1);
}

internal_function
void eventText (lua_State *l,
                const char *const text)
{
    // ... Events
    lua_newtable(l);
    // ... Events <TABLE>

    lua_pushstring(l, "Text");
    // ... Events <TABLE> "Text"
    lua_setfield(l, -2, "Device");
    // ... Events <TABLE>

    lua_pushstring(l, "Text");
    // ... Events <TABLE> "Text"
    lua_setfield(l, -2, "Type");
    // ... Events <Table>

    lua_pushstring(l, text);
    // ... Events <TABLE> text
    lua_setfield(l, -2, "Text");
    // ... Events <Table>

    Size events_size = lua_objlen(l, -2);
    lua_rawseti(l, -2, (Sint)events_size + 1);
}

internal_function
void eventTextControl (lua_State *l,
                       const char *const control)
{
    // ... Events
    lua_newtable(l);
    // ... Events <TABLE>

    lua_pushstring(l, "Text");
    // ... Events <TABLE> "Text"
    lua_setfield(l, -2, "Device");
    // ... Events <TABLE>

    lua_pushstring(l, "Control");
    // ... Events <TABLE> "Control"
    lua_setfield(l, -2, "Type");
    // ... Events <Table>

    lua_pushstring(l, control);
    // ... Events <TABLE> control
    lua_setfield(l, -2, "Control");
    // ... Events <Table>

    Size events_size = lua_objlen(l, -2);
    lua_rawseti(l, -2, (Sint)events_size + 1);
}
