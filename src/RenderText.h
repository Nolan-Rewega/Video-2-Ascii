#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include<iostream>
#include<string>
#include<map>

#include<glad/gl.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<ft2build.h>
#include FT_FREETYPE_H

using namespace std;

/* The structure for holding a Character */
struct Character{
    unsigned int texture_id;
    unsigned int advance;
    glm::ivec2 size;
    glm::ivec2 bearing;
};

class DisplayGL{
public:
    /* Screen size attributes*/
    unsigned int WIDTH;
    unsigned int HEIGHT;
    unsigned int TEXT_SIZE;
    float* RGB_PTR;

    /* Character map */
    map<GLchar, Character> char_table;

    /* Error flag*/
    bool INIT_ERROR;

    /* Buffers */
    unsigned int VBO;
    unsigned int VAO;
    
    /* shaders*/
    const char* vertex_shader_src;
    const char* fragment_shader_src;
    unsigned int vertex;
    unsigned int fragment;
   
    unsigned int program;


    /* GLFW window */
    GLFWwindow* window;
    
    /* Methods */
    DisplayGL(unsigned int w, unsigned int h, unsigned int text_size, float rgb_ptr[3]);
    bool init();
    void exit();
    void background();
    void post_render();
    void check_input();
    int check_termination();
    void render_text(string text, float x, float y, float scale);

};

#endif
