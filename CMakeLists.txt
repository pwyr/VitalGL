cmake_minimum_required(VERSION 3.27.4)

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
#TODO: remove
add_compile_definitions(VGL_OPENGL_DEBUG_MODE=1)

set(VITALGL_SOURCES
    main.cpp
    src/vgl/window.h
    src/vgl/window.cpp
    src/vgl/renderer.h
    src/vgl/renderer.cpp
    src/vgl/gl.h
    src/vgl/gl.cpp
)

add_executable(vital-gl ${VITALGL_SOURCES})

find_package(OpenGL REQUIRED)
target_include_directories(vital-gl PRIVATE ${OPENGL_INCLUDE_DIRS})
target_include_directories(vital-gl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/)

target_link_libraries(vital-gl ${OPENGL_LIBRARIES})

get_target_property(VITALGL_BINARY_DIR vital-gl BINARY_DIR)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/icon/vgl.ico DESTINATION ${BIN_DIR}/icon/)