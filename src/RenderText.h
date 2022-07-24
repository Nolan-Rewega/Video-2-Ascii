#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include<iostream>
#include<fstream>
#include<string>

#include<glad/gl.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"FontSystem.h"

using namespace std;


class DisplayGL{
private:
    /* Buffers */
    unsigned int VBO, VAO, program;
    float TARGET_W, TARGET_H, DISPLAY_W, DISPLAY_H;

    FontSystem* fSys;
    GLuint atlasID;
    
    GLFWwindow* window;

    string readShaderCode(const char* fileName);

public:
    /* Methods */
    DisplayGL( float WIDTH,
               float HEIGHT,
               float TARGETWIDTH,
               float TARGETHEIGHT,
               unsigned int TEXTSIZE
    );

    void exit();
    void background();
    void post_render();
    void check_input();
    int check_termination();
    void render_text( string text, float scale );

};

#endif
