#include"RenderText.h"

DisplayGL::DisplayGL( float WIDTH, 
                      float HEIGHT,
                      float TARGETWIDTH,
                      float TARGETHEIGHT,
                      unsigned int TEXTSIZE)
{
    fSys = new FontSystem(
        "/home/shadow/code/Video-2-Ascii/src/fonts/Hack-Regular.ttf",
        TEXTSIZE,
        "FontAtlas.bmp"
    );
    fSys->generateFontAtlas();

    //color = glm::vec3(1.0f, 1.0f, 1.0f);
    TARGET_W = TARGETWIDTH;
    TARGET_H = TARGETHEIGHT;
    DISPLAY_W = WIDTH;
    DISPLAY_H = HEIGHT;


    /* load shader src's */
    string TVSSinput = readShaderCode("../src/TVSS.glsl");
    string TFSSinput = readShaderCode("../src/TFSS.glsl");
    const char* VSS = TVSSinput.c_str();
    const char* FSS = TFSSinput.c_str();
    

    /* initializing openGL */
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create the OpenGL window */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Video 2 Ascii", NULL, NULL);
    if(window == NULL){
        cout << "ERROR IN DISPLAYGL::DISPLAYGL, could not create glfw Window.\n";
        std::exit(-10);
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glViewport(0, 0, WIDTH, HEIGHT);

    // -- Enabling Culling and texture blending.
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // -- Shader Activation
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    
    glShaderSource(vertex, 1, &VSS, NULL);
    glShaderSource(fragment, 1, &FSS, NULL);
    glCompileShader(vertex);
    glCompileShader(fragment);

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    
    // -- Shader Deletion
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    /* modify a floating typed uniform variable array*/
    // -- We use the matrix to project from screen pixel space (0, 800)
    // -- to openGl world space (-1.0, 1.0).
    glm::mat4 projection = glm::ortho( 0.0f,
                                       static_cast<float>(WIDTH),
                                       0.0f,
                                       static_cast<float>(HEIGHT)
    );
    glUseProgram(program);
    glUniformMatrix4fv( glGetUniformLocation(program, "projection"),
                        1,
                        GL_FALSE,
                        glm::value_ptr(projection)
    );

    
    /* Init VAO and VBO */
    // -- One pixel = One Ascii Char.
    int bufferSize = sizeof(float) * 6 * 4 * (TARGETWIDTH * TARGETHEIGHT);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /* creating a texture from a font atlas*/
    glGenTextures(1, &atlasID);
    glBindTexture(GL_TEXTURE_2D, atlasID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        fSys->getAtlasWidth(),
        fSys->getAtlasHeight(),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        fSys->m_atlasData
    );
    
    glBindTexture(GL_TEXTURE_2D, 0);
}


string DisplayGL::readShaderCode(const char* fileName){
    string shader, line;
    ifstream inputFile(fileName);
    if(!inputFile.good()){
        cout << "ERROR, " << fileName << "NOT FOUND. In DisplayGL::readShaderCode()\n";
        std::exit(-11);
    }
    while(getline(inputFile, line)){
        shader.append(line);
        shader.append("\n");
    }
    return shader;
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


void DisplayGL::render_text( string text, float scale){

    // -- Bind buffers and textures
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasID);

    float ASPECT_H = DISPLAY_H / TARGET_H;

    int offset = 0;
    float x = 0;
    float y = 0;
    
    // -- loops O(WIDTH * HEIGHT) times.
    for(int r = 0; r < TARGET_H; ++r){
        x = 0;
        y = r * ASPECT_H;
        for(int c = 0; c < TARGET_W; ++c){
            // -- Retrieve Char's Glyph info.
            int idx = (TARGET_H - r - 1) * ((int)TARGET_W) + c;
            FontSystem::Glyph ch = fSys->getCharacterInformation(text[idx]);
            
            float xpos = x + ch.bearingX;
            float ypos = y - (ch.height - ch.bearingY);
            float w = ch.width * scale;
            float h = ch.height * scale;
            
            // -- Create a Quad to hold the position and texture coords.
            float vertices[6][4] = {
                { xpos,     ypos + h,   ch.atlasLeft,    ch.atlasTop},
                { xpos,     ypos,       ch.atlasLeft,    ch.atlasBottom},
                { xpos + w, ypos,       ch.atlasRight,   ch.atlasBottom},

                { xpos,     ypos + h,   ch.atlasLeft,    ch.atlasTop},
                { xpos + w, ypos,       ch.atlasRight,   ch.atlasBottom},
                { xpos + w, ypos + h,   ch.atlasRight,   ch.atlasTop}
            };
            
            // -- Added texture Quad to the buffer.
            glBufferSubData(GL_ARRAY_BUFFER,
                            offset,
                            sizeof(vertices),
                            vertices
            );
            offset += sizeof(vertices);
            x += (ch.advance >> 6) * scale;
        }
    }

    // -- Draw !
    glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, text.length() * 6);
    
    // -- Unbind buffers and textures
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void DisplayGL::exit(){
    glfwTerminate();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete fSys;
}

