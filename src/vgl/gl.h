#pragma once

#include <cstdint>
#if __cplusplus >= 202302L
#include <stdfloat>
#endif


// ------------------------------------------------------------------------------
// OpenGL types
// ------------------------------------------------------------------------------
using GLboolean = bool;
using GLchar = char;
using GLint = std::int32_t;
using GLuint = std::uint32_t;
using GLsizei = std::uint32_t;
using GLsizeiptr = std::uintptr_t;
using GLenum = std::uint32_t;
using GLbitfield = std::uint32_t;

#if __cplusplus >= 202302L
using GLfloat = std::float32_t;
#else
using GLfloat = float;
#endif

// ------------------------------------------------------------------------------
// OpenGL constants
// ------------------------------------------------------------------------------
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406

#define GL_FALSE 0
#define GL_TRUE 1
#define GL_DONT_CARE 0x1100

#define GL_COLOR_BUFFER_BIT 0x00004000
// #define GL_DEPTH_BUFFER_BIT 0x00000100
// #define GL_STENCIL_BUFFER_BIT 0x00000400

#define GL_VERTEX_SHADER 0x8B31
// #define GL_TESS_CONTROL_SHADER 0x8E88
// #define GL_TESS_EVALUATION_SHADER 0x8E87
// #define GL_GEOMETRY_SHADER 0x8DD9
#define GL_FRAGMENT_SHADER 0x8B30
// #define GL_COMPUTE_SHADER 0x91B9

// #define GL_INVALID_VALUE 0x0501
// #define GL_INVALID_ENUM 0x0500
// #define GL_INVALID_OPERATION 0x0502

#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84

#define GL_LINK_STATUS 0x8B82

#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893

#define GL_STATIC_DRAW 0x88E4

#define GL_TRIANGLES 0x0004

#define GL_DEBUG_OUTPUT 0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242

#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_HIGH 0x9146

// ------------------------------------------------------------------------------
// OpenGL functions
// ------------------------------------------------------------------------------
extern void (*glEnable)(GLenum);

extern void (*glViewport)(GLint, GLint, GLsizei, GLsizei);
extern void (*glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat);
extern void (*glClear)(GLbitfield);

extern GLuint (*glCreateShader)(GLenum);
extern void (*glShaderSource)(GLuint, GLsizei, const GLchar**, const GLint*);
extern void (*glCompileShader)(GLuint);
extern void (*glGetShaderiv)(GLuint, GLenum, GLint*);
extern void (*glGetShaderInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
extern void (*glDeleteShader)(GLuint);

extern GLuint (*glCreateProgram)();
extern void (*glAttachShader)(GLuint, GLuint);
extern void (*glLinkProgram)(GLuint);
extern void (*glGetProgramiv)(GLuint, GLenum, GLint*);
extern void (*glGetProgramInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
extern void (*glUseProgram)(GLuint);
extern void (*glDeleteProgram)(GLuint);

extern GLint (*glGetUniformLocation)(GLuint, const GLchar*);
extern void (*glUniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat*);

extern void (*glGenVertexArrays)(GLsizei, GLuint*);
extern void (*glBindVertexArray)(GLuint);
extern void (*glDeleteVertexArrays)(GLsizei, const GLuint*);

extern void (*glGenBuffers)(GLsizei, GLuint*);
extern void (*glBindBuffer)(GLenum, GLuint);
extern void (*glBufferData)(GLenum, GLsizeiptr, const void*, GLenum);
extern void (*glDeleteBuffers)(GLsizei, const GLuint*);

extern void (*glEnableVertexAttribArray)(GLuint);
extern void (*glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);

extern void (*glDrawElements)(GLenum, GLsizei, GLenum, const void*);

extern void (*glDebugMessageCallback)(void (*)(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*), const void*);
extern void (*glDebugMessageControl)(GLenum, GLenum, GLenum, GLsizei, const GLuint*, GLboolean);

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
namespace vgl {

void loadGLFunctions(void* (*getProcAddress)(const char*));

} // namespace vgl
