/** 
* @brief Function to record the key being pressed.
*
* This fucntion records the key pressed events and call neccessary lua function to perform corresponding action. 
*
* @param l lua_State pointer to keep track of the game state
* @param key charater pointer which records the key being pressed.
* @param state keep track of state of the other fields.
*
* @return void
*/
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

/** 
* @brief Function to record the text after key have been pressed.
*
* This fucntion records the text which is typed after key pressed events and call neccessary lua function to perform corresponding action. 
*
* @param l lua_State pointer to keep track of the game state
* @param text character pointer which points to the text typed
*
* @return void
*/
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
/** 
* @brief Function to record the control character beeing pressed.
*
* This fucntion records the control character e.g. ENTER ,BACKSPACE which is used to perform special action in gameplay. 
*
* @param l lua_State pointer to keep track of the game state
* @param control character pointer which points to the control character typed
*
* @return void
*/
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
