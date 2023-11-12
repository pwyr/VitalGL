#include <vgl/gl.h>
#include "gl.h"


void (*glViewport)(GLint, GLint, GLsizei, GLsizei) = nullptr;
void (*glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat) = nullptr;
void (*glClear)(GLbitfield) = nullptr;

GLuint (*glCreateShader)(GLenum) = nullptr;
void (*glShaderSource)(GLuint, GLsizei, const GLchar**, const GLint*) = nullptr;
void (*glCompileShader)(GLuint) = nullptr;
void (*glGetShaderiv)(GLuint, GLenum, GLint*) = nullptr;
void (*glGetShaderInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*) = nullptr;
void (*glDeleteShader)(GLuint) = nullptr;

GLuint (*glCreateProgram)() = nullptr;
void (*glAttachShader)(GLuint, GLuint) = nullptr;
void (*glLinkProgram)(GLuint) = nullptr;
void (*glGetProgramiv)(GLuint, GLenum, GLint*) = nullptr;
void (*glGetProgramInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*) = nullptr;
void (*glUseProgram)(GLuint) = nullptr;
void (*glDeleteProgram)(GLuint) = nullptr;

void (*glGenVertexArrays)(GLsizei, GLuint*) = nullptr;
void (*glBindVertexArray)(GLuint) = nullptr;
void (*glDeleteVertexArrays)(GLsizei, const GLuint*) = nullptr;

void (*glGenBuffers)(GLsizei, GLuint*) = nullptr;
void (*glBindBuffer)(GLenum, GLuint) = nullptr;
void (*glBufferData)(GLenum, GLsizeiptr, const void*, GLenum) = nullptr;
void (*glDeleteBuffers)(GLsizei, const GLuint*) = nullptr;

void (*glEnableVertexAttribArray)(GLuint) = nullptr;
void (*glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*) = nullptr;

void (*glDrawArrays)(GLenum, GLint, GLsizei) = nullptr;


void vgl::loadGLFunctions(void* (*getProcAddress)(const char*))
{
    glViewport = reinterpret_cast<decltype(glViewport)>(getProcAddress("glViewport"));
    glClearColor = reinterpret_cast<decltype(glClearColor)>(getProcAddress("glClearColor"));
    glClear = reinterpret_cast<decltype(glClear)>(getProcAddress("glClear"));

    glCreateShader = reinterpret_cast<decltype(glCreateShader)>(getProcAddress("glCreateShader"));
    glShaderSource = reinterpret_cast<decltype(glShaderSource)>(getProcAddress("glShaderSource"));
    glCompileShader = reinterpret_cast<decltype(glCompileShader)>(getProcAddress("glCompileShader"));
    glGetShaderiv = reinterpret_cast<decltype(glGetShaderiv)>(getProcAddress("glGetShaderiv"));
    glGetShaderInfoLog = reinterpret_cast<decltype(glGetShaderInfoLog)>(getProcAddress("glGetShaderInfoLog"));
    glDeleteShader = reinterpret_cast<decltype(glDeleteShader)>(getProcAddress("glDeleteShader"));

    glCreateProgram = reinterpret_cast<decltype(glCreateProgram)>(getProcAddress("glCreateProgram"));
    glAttachShader = reinterpret_cast<decltype(glAttachShader)>(getProcAddress("glAttachShader"));
    glLinkProgram = reinterpret_cast<decltype(glLinkProgram)>(getProcAddress("glLinkProgram"));
    glGetProgramiv = reinterpret_cast<decltype(glGetProgramiv)>(getProcAddress("glGetProgramiv"));
    glGetProgramInfoLog = reinterpret_cast<decltype(glGetProgramInfoLog)>(getProcAddress("glGetProgramInfoLog"));
    glUseProgram = reinterpret_cast<decltype(glUseProgram)>(getProcAddress("glUseProgram"));
    glDeleteProgram = reinterpret_cast<decltype(glDeleteProgram)>(getProcAddress("glDeleteProgram"));

    glGenVertexArrays = reinterpret_cast<decltype(glGenVertexArrays)>(getProcAddress("glGenVertexArrays"));
    glBindVertexArray = reinterpret_cast<decltype(glBindVertexArray)>(getProcAddress("glBindVertexArray"));
    glDeleteVertexArrays = reinterpret_cast<decltype(glDeleteVertexArrays)>(getProcAddress("glDeleteVertexArrays"));

    glGenBuffers = reinterpret_cast<decltype(glGenBuffers)>(getProcAddress("glGenBuffers"));
    glBindBuffer = reinterpret_cast<decltype(glBindBuffer)>(getProcAddress("glBindBuffer"));
    glBufferData = reinterpret_cast<decltype(glBufferData)>(getProcAddress("glBufferData"));
    glDeleteBuffers = reinterpret_cast<decltype(glDeleteBuffers)>(getProcAddress("glDeleteBuffers"));

    glEnableVertexAttribArray = reinterpret_cast<decltype(glEnableVertexAttribArray)>(getProcAddress("glEnableVertexAttribArray"));
    glVertexAttribPointer = reinterpret_cast<decltype(glVertexAttribPointer)>(getProcAddress("glVertexAttribPointer"));

    glDrawArrays = reinterpret_cast<decltype(glDrawArrays)>(getProcAddress("glDrawArrays"));
}