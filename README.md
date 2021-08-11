# Info
This program converts MP4 image data to ASCII characters in real time and display them via OpenGL.

# Instalation
This program uses external five external libraries: ffmpeg, GLFW (openGL), GLM, FreeType, pkg-config.
Recommended library versions are: GLFW 3.0+, FreeType 2.1, GLM 0.9+, 

Linux is needed.

Before installation, create a new directories called 'out' inside 'Video-2-Ascii'.

External library installation instructions.

  GLFW:

    1. Go to https://www.glfw.org/download and download the latest version of the 'Source package'.
    2. create a directory called 'glfw' inside the 'lib' directory
    3. extract the contents of glfw-X.X.X Source package to 'glfw'.
    path ex: ../Video-2-Ascii/lib/glfw/<cmake, docs, deps, ...etc>
    
  GLM:
  
    1. Go to https://github.com/g-truc/glm/releases/latest and download the latest version of glm-X.X.X.zip
    2. create a directory called 'glm' inside the 'lib' directory
    3. extracted the contents of glm-X.X to 'glm'
    path ex: ../Video-2-Ascii/lib/glm/<cmake, docs, deps, ...etc>

  Freetype (Linux only):

    1. go to https://www.freetype.org/download.html and download the latest version
    2. create a directory called 'freetype' inside the 'lib' directory 
    3. extracted the contents of freetype-2.X.X to 'freetype'
    path ex: ../Video-2-Ascii/lib/freetype/<builds, devel, docs, ...etc>
    
    4. cd into ../Video-2-Ascii/lib/freetype directory
      run commands in order:
          cmake -E make_directory build
          cmake -E chdir build cmake ..
          cmake --build build
          sudo cmake --build build --target install
  
  ffmpeg and pgk-config:
  
    1. install via the command line.
    
Cmake Compilation instructions:
  1. run Cmake in the Video-2-Ascii directory setting 'src' as the source directory and 'out' as the build directory.
  2. run the Makefile created in 'out', this should create a executable file called V2A.
  3. run V2A.exe

# Video demo
TODO
