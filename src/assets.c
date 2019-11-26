/** 
* @brief This function load the shader assets for gameplay.
*
* It reads the file at vertex path and fragement path and caste it into character array.
* Before exiting it frees up the vertex and fragment source.
* 
* @param vertex_path character array containing path for vertex
* @param fragment_path character array containing path for fragment
*
* @return bool
*/
internal_function
B32 assetLoadShader (char* vertex_path,
                     char* fragment_path,
                     GLuint *program)
{
    char *vertex_src = (Char*)fileRead(vertex_path, NULL);
    if (vertex_src == NULL) {
        return false;
    }

    char *fragment_src = (Char*)fileRead(fragment_path, NULL);
    if (fragment_src == NULL) {
        free(vertex_src);
        return false;
    }

    S32 program_temp = 0;
    if ((program_temp = openglShaderCreate(vertex_src, fragment_src)) < 0) {
        logConsole(LOG_LEVEL_CRITICAL,
                   LOG_CHANNEL_SCRIPT,
                   "Couldn't load shaders %s and %s",
                   vertex_path, fragment_path);

        free(vertex_src);
        free(fragment_src);

        return false;
    }

    *program = (GLuint)program_temp;

    free(vertex_src);
    free(fragment_src);

    return true;

}

/** 
* @brief This function load the script assets for gameplay.
*
* It reads the file at script path caste it into character array.
* Check the lua load script , lua game call and log console. 
* Finally, free up the resources used by script. 
*
* @param game lua_State pointer used to load assets 
* @param script_path character array containing path for script
*
* @return bool
*/

internal_function
B32 assetLoadScript (lua_State *game,
                     char *script_path)
{
    char *script_src = (Char*)fileRead(script_path, NULL);
    if (script_path == NULL) return false;

    if (luaL_loadstring(game, script_src) ||
        lua_pcall(game, 0, 0, 0)) {
        logConsole(LOG_LEVEL_CRITICAL,
                   LOG_CHANNEL_SCRIPT,
                   "Couldn't load script %s: %s",
                   script_path,
                   lua_tostring((lua_State*)game, -1));
        lua_pop((lua_State*)game, 1);
        free(script_src);

        return false;
    }

    free(script_src);

    return true;
}

/** 
* @brief This function load the true type font used in gameplay.
*
* It load the font for all the text and graphics rendering in the game.
* It load different set of fonts for OpenGL rendering. 
*
* @param font_path character array for the path of font 
* @param unsigned32 number for font size 
* @param vert_path charater array for verticle path
* @param frag_path charater array for fragment path
* @param bitmap_width bitmap width in unsigned32 
* @param bitmap_height bitmap height in unsigned32 
* @param char_first first charecter in unsigned32
* @param char_num number of chareter in unsigned32
* @param vao Gluint variable for 3D rendering
* @param vbo Gluint variable for 3D rendering
* @param baked_char stbtt_bakedchar library variable to bake the character
* @param program Gluint variable for 3D rendering for lua program
* @param texture Gluint variable for 3D rendering for texture
*
* @return bool
*/

internal_function
B32 assetLoadTrueTypeFont (Char *font_path, U32 font_size,
                           Char *vert_path, Char *frag_path,
                           U32 bitmap_width, U32 bitmap_height,
                           U32 char_first, U32 char_num,
                           GLuint vao, GLuint vbo,
                           stbtt_bakedchar **baked_char,
                           GLuint *program, GLuint *texture)
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    assetLoadShader(vert_path, frag_path, program);
    glUseProgram(*program);

    GLint pos_attrib = glGetAttribLocation(*program, "position");
    glVertexAttribPointer((GLuint)pos_attrib,
                          2,
                          GL_FLOAT, GL_FALSE,
                          4 * sizeof(float),
                          0);
    glEnableVertexAttribArray((GLuint)pos_attrib);

    GLint tex_attrib = glGetAttribLocation(*program, "tex_coord");
    glVertexAttribPointer((GLuint)tex_attrib,
                          2,
                          GL_FLOAT, GL_FALSE,
                          4 * sizeof(float),
                          (void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray((GLuint)tex_attrib);

    U8 *ttf_buffer = fileRead(font_path, NULL);

    U8 *bitmap = malloc(sizeof(*bitmap) * bitmap_width * bitmap_height);
    *baked_char = malloc(sizeof(**baked_char) * char_num);

    stbtt_BakeFontBitmap(ttf_buffer, 0,
                         font_size,
                         bitmap,
                         (int)bitmap_width, (int)bitmap_height,
                         (int)char_first, (int)char_num,
                         *baked_char);

    free(ttf_buffer);

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
                 (GLsizei)bitmap_width, (GLsizei)bitmap_height,
                 0, GL_RED, GL_UNSIGNED_BYTE,
                 bitmap);
    glGenerateMipmap(GL_TEXTURE_2D);

    glUniform1i(glGetUniformLocation(*program, "font_bitmap"), 0);
    free(bitmap);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}
