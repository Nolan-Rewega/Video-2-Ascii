#include"RenderText.h"

DisplayGL::DisplayGL(unsigned int w, unsigned int h, unsigned int text_size, float rgb_ptr[3]){
    WIDTH = w;  HEIGHT = h;
    
    TEXT_SIZE = text_size;
    RGB_PTR = rgb_ptr;
    
    INIT_ERROR = init();
}

bool DisplayGL::init(){
    /* initializing openGL */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create the OpenGL window */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Video 2 Ascii", NULL, NULL);
    if(window == NULL){
        cout << "GLFW window init error.";
        glfwTerminate(); return false;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glViewport(0, 0, WIDTH, HEIGHT);

    /* enabaling color blending and culling (might not be needed) */
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* shader src's */
    // -- I have no idea what these shaders do
    const char* vertex_shader_src = "#version 330 core\n"
    "layout (location = 0) in vec4 vertex;\n"
    "out vec2 TexCoords;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    "   TexCoords = vertex.zw;\n"
    "}\0";
    
    // -- I have no idea what these shaders do
    const char* fragment_shader_src = "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D text;\n"
    "uniform vec3 textColor;\n"
    "void main()\n"
    "{\n"
    "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
    "   color = vec4(textColor, 1.0) * sampled;\n"
    "}\n\0";
    
    /* shader and program decleration and activation */
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment);

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    
    // -- shader deletion
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    /* modify a floating typed uniform variable array*/
    // -- not sure why a 4x4 matrix is needed (prob just RGBA values) 
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f,static_cast<float>(HEIGHT));
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    
    /* Init VAO and VBO */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /* FreeType initialization */
    const char* font = "/home/shadow/code/Video-2-Ascii/src/fonts/Hack-Regular.ttf";
    FT_Library ft;
    FT_Face face;
    
    if (FT_Init_FreeType(&ft)) { cout << "Error Initializing freetype.\n"; return false; }
    if (FT_New_Face(ft, font, 0, &face)) { cout << "Error loading font.\n"; return false; }
    
    /* Set the width of the text*/
    FT_Set_Pixel_Sizes(face, 0, TEXT_SIZE);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // -- loading all renderable ASCII chars into char_map
    for(char c = 32; c < 127; c++){
        if( FT_Load_Char(face, c, FT_LOAD_RENDER) ){
            cout << " FAILED TO LOAD GLYPH: " << c << "\n";
            continue;
        }
        
        /* creating a texture from ascii char c */
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
            );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        /*  store the texture into 'Character' struct to store int char_table */
        Character c_tex = {
            texture,
            static_cast<unsigned int>(face->glyph->advance.x),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top)
        };
        char_table.insert(pair<char, Character>(c, c_tex));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    /* free up freetype variables */
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    return true;
}

void DisplayGL::check_input(){
    /* Polling for escape key press */
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}


int DisplayGL::check_termination() {
    return glfwWindowShouldClose(window);
}


void DisplayGL::background(){
    /* setting background to solid black */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


void DisplayGL::post_render(){
    glfwSwapBuffers(window);
    glfwPollEvents();
}


void DisplayGL::render_text(string text, float x, float y, float scale){
    /* Prepair to load text teture */
    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "textColor"), RGB_PTR[0], RGB_PTR[1], RGB_PTR[2]);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    for(int i = 0; i < text.size(); i++){
        /* retrieve char_texture information from char_table*/
        Character c = char_table[text[i]];
        
        float xpos = x + c.bearing.x * scale;
        float ypos = y - (c.size.y - c.bearing.y) * scale;

        // -- width and height
        float w = c.size.x * scale;
        float h = c.size.y * scale;
        
        // -- creates the vertice array where to disaplay text
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f},
            { xpos,     ypos,       0.0f, 1.0f},
            { xpos + w, ypos,       1.0f, 1.0f},

            { xpos,     ypos + h,   0.0f, 0.0f},
            { xpos + w, ypos,       1.0f, 1.0f},
            { xpos + w, ypos + h,   1.0f, 0.0f}
        };
        /* Bind the char texture */
        glBindTexture(GL_TEXTURE_2D, c.texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        /* Display texture as trianglesv */
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (c.advance >> 6) * scale;
    }
    /* clear VAO, and un-bind char texture */
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void DisplayGL::exit(){glfwTerminate();}

