cmake_minimum_required(VERSION 3.00)
project(V2A)

# debug info
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# glfw additions
set( GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set( GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set( GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")

add_subdirectory(lib/glfw)
add_subdirectory(lib/ffmpeg)
add_subdirectory(lib/glm)
include_directories(lib/freetype/include)
include_directories(lib/glfw)
include_directories(lib/glfw/deps)
include_directories(lib/glfw/include)

list(APPEND EXTRA_LIBS "-lGL -lGLU -lX11")

list(APPEND SOURCES
    src/AsciiConverter.h
    src/AsciiConverter.cpp
    src/FrameParser.h
    src/FrameParser.cpp
    src/RenderText.h
    src/RenderText.cpp
    src/main.cpp
)
set(GLAD "${GLFW_SOURCE_DIR}/deps/glad/gl.h"
         "${GLFW_SOURCE_DIR}/deps/glad_gl.c")

set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")

set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")

add_executable(V2A ${SOURCES} ${TINYCTHREAD} ${GETOPT} ${GLAD} )
target_link_libraries(V2A ffmpeg glfw glm freetype ${EXTRA_LIBS})

