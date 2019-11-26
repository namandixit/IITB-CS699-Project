/** 
* @brief Function to render text on OpenGL rendered screen
*
* This function is used to render text in openGL game console screen.It uses shaders to render text and different font and color in game console. 
*

* @param vao Gluint variable for 3D rendering
* @param vbo Gluint variable for 3D rendering
* @param baked_char stbtt_bakedchar library variable to bake the character
* @param program Gluint variable for 3D rendering for lua program
* @param texture Gluint variable for 3D rendering for texture
* @param bitmap_width bitmap width in unsigned32 
* @param bitmap_height bitmap height in unsigned32 
* @param char_first first charecter in unsigned32
* @param char_num number of chareter in unsigned32
* @param text character pointer to the content to be rendered
* @param screen_pos position on the screen at which the text to be rendered in Vec3
* @param color color of the text to be rendered in Vec3
* @param scale_factor scale factor for the text size in float32
* @param x_scaling scaling in x direction in float32
* @param x_ret length of rendered text in x direction
* @param y_min_ret length of rendered text below in the y direction 
* @param y_max_ret length of rendered text up in the y direction
*
* @return bool
*/
internal_function
B32 renderText (GLuint vao, GLuint vbo, GLuint texture, GLuint program,
                char char_first, char char_num,
                U32 bitmap_width, U32 bitmap_height,
                stbtt_bakedchar *baked_char,
                const char *text,
                Vec3 screen_pos, Vec3 color,
                F32 scale_factor, F32 x_scaling,
                F32 *x_ret, F32 *y_min_ret, F32 *y_max_ret)
{

    Size text_length = strlen(text);
    Size vertices_size = text_length * 6 * 4;
    GLfloat *vertices = malloc(sizeof(*vertices) * vertices_size);

    U32 i = 0;
    F32 x = 0, y_min = 0, y_max = 0;

    while (*text) {
        if ((*text >= char_first) &&
            (*text < (char_first + char_num))) {

            stbtt_aligned_quad q;
            stbtt_bakedchar *b = baked_char + ((*text) - char_first);
            F32 ibw = 1.0f / (bitmap_width);
            F32 ibh = 1.0f / (bitmap_height);

            q.x0 = (x + b->xoff) * x_scaling;
            q.y0 = -(b->yoff + (b->y1 - b->y0));
            q.x1 = (x + b->xoff + (b->x1 - b->x0)) * x_scaling;
            q.y1 = -b->yoff;

            q.s0 = b->x0 * ibw;
            q.t0 = b->y0 * ibh;
            q.s1 = b->x1 * ibw;
            q.t1 = b->y1 * ibh;

            F32 y_low = -(b->yoff + (b->y1 - b->y0));
            if (y_low < y_min) {
                y_min = y_low;
            }

            F32 y_high = -b->yoff;
            if (y_high > y_max) {
                y_max = y_high;
            }

            x += (b->xadvance);

            vertices[i + 0] = q.x0;
            vertices[i + 1] = q.y0;
            vertices[i + 2] = q.s0;
            vertices[i + 3] = q.t1;
            vertices[i + 4] = q.x1;
            vertices[i + 5] = q.y0;
            vertices[i + 6] = q.s1;
            vertices[i + 7] = q.t1;
            vertices[i + 8] = q.x1;
            vertices[i + 9] = q.y1;
            vertices[i + 10] = q.s1;
            vertices[i + 11] = q.t0;
            vertices[i + 12] = q.x1;
            vertices[i + 13] = q.y1;
            vertices[i + 14] = q.s1;
            vertices[i + 15] = q.t0;
            vertices[i + 16] = q.x0;
            vertices[i + 17] = q.y1;
            vertices[i + 18] = q.s0;
            vertices[i + 19] = q.t0;
            vertices[i + 20] = q.x0;
            vertices[i + 21] = q.y0;
            vertices[i + 22] = q.s0;
            vertices[i + 23] = q.t1;

            i = i + 24;
        }
        ++text;
    }

    *x_ret = x;
    *y_min_ret = y_min;
    *y_max_ret = y_max;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(sizeof(*vertices) * vertices_size),
                 vertices,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    free(vertices);

    glBindVertexArray(vao);
    glUseProgram(program);
    glBindTexture(GL_TEXTURE_2D, texture);

    Mat4 translation = mat4Translate2D(vec2New(screen_pos.x,
                                               screen_pos.y));

    Vec2 scaling_factor = vec2New(scale_factor,
                                  scale_factor);
    Mat4 scaling = mat4Scale2D(scaling_factor);

    Mat4 transform = mat4Mul(translation, scaling);

    glUniformMatrix4fv(glGetUniformLocation(program,
                                            "transform"),
                       1,
                       GL_FALSE,
                       transform.elem);

    glUniform1f(glGetUniformLocation(program,
                                     "depth"),
                screen_pos.z);

    glUniform3fv(glGetUniformLocation(program,
                                      "color"),
                 1,
                 (GLfloat*)(&color));

    // NOTE: 4 floats per vertex
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices_size/4);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}
