# Info
This program converts MP4 image data to ASCII characters in real time and display them via OpenGL.

# Project Dependencies
External libraries: 
  1. ffmpeg  
  2. GLFW 3.0+  
  3. GLM 0.9+  
  4. FreeType 2.1X  
  5. pkg-config. 
  
Additional programs:
  1. CMake


*This project only supports Linux distros.  *
 

# Installation
Inside the cloned repo 'Video-2-Ascii' create a new directory called 'out'. 

External library installation instructions.

  GLFW:

    1. Go to https://www.glfw.org/download and download the latest version of the 'Source package'.  
    2. Create a directory called 'glfw' inside the 'lib' directory. 
    3. Extract the contents of glfw-X.X.X Source package to 'glfw'.  
    
  GLM:
  
    1. Go to https://github.com/g-truc/glm/releases/latest and download the latest version of glm-X.X.X.zip.
    2. Create a directory called 'glm' inside the 'lib' directory.
    3. Extract the contents of glm-X.X to 'glm'.

  Freetype:

    1. Go to https://www.freetype.org/download.html and download the latest version.
    2. Create a directory called 'freetype' inside the 'lib' directory.
    3. Extract the contents of freetype-2.X.X to 'freetype'.
    4. cd into ../Video-2-Ascii/lib/freetype directory
      run commands in order:
          cmake -E make_directory build
          cmake -E chdir build cmake ..
          cmake --build build
          sudo cmake --build build --target install
  
  ffmpeg and pgk-config:
  
    1. Install via the command line.
    
Cmake Compilation instructions:
  1. Run Cmake in the Video-2-Ascii directory setting 'src' as the source directory and 'out' as the build directory.
  2. Run the Makefile created in 'out', this should create a executable file called V2A.
  3. Run V2A.exe

# Video demo
[Demo Video](https://www.youtube.com/watch?v=weGYrn5RfOY)
