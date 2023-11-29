cmake_minimum_required(VERSION 3.27.4)

project(vital-gl VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: remove
if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin/default)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})

set(OPENGL_DEBUG_MODE OFF CACHE BOOL "Enable OpenGL debug mode")
if(${OPENGL_DEBUG_MODE})
    add_compile_definitions(VGL_OPENGL_DEBUG_MODE=1)
endif()

set(ASYNC_RENDERING OFF CACHE BOOL "Enable async rendering")
if(${ASYNC_RENDERING})
    add_compile_definitions(VGL_ASYNC_RENDERING=1)
endif()

set(PRINT_FPS OFF CACHE BOOL "Print FPS in console")
if(${PRINT_FPS})
    add_compile_definitions(VGL_PRINT_FPS=1)
endif()

#TODO: remove
add_compile_definitions(VGL_OPENGL_DEBUG_MODE=1)

set(VITALGL_SOURCES
    src/vgl/vgl.h
    src/vgl/app.h
    src/vgl/app.cpp
    src/vgl/primitives.h
    src/vgl/primitives.cpp
    src/vgl/window.h
    src/vgl/window.cpp
    src/vgl/renderer.h
    src/vgl/renderer.cpp
    src/vgl/gl.h
    src/vgl/gl.cpp
)

add_library(vital-gl STATIC ${VITALGL_SOURCES})

find_package(OpenGL REQUIRED)
target_include_directories(vital-gl PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(vital-gl PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/)

target_link_libraries(vital-gl ${OPENGL_LIBRARIES})

get_target_property(VITALGL_BINARY_DIR vital-gl BINARY_DIR)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/icon/vgl.ico DESTINATION ${BIN_DIR}/icon/)

add_executable(example main.cpp)
target_include_directories(example PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/)
target_link_libraries(example vital-gl)