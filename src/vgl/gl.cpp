#include <vgl/gl.h>
#include "gl.h"


void (*glEnable)(GLenum) = nullptr;

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

GLint (*glGetUniformLocation)(GLuint, const GLchar*) = nullptr;
void (*glUniform1f)(GLint, GLfloat) = nullptr;
void (*glUniform3fv)(GLint, GLsizei, const GLfloat*) = nullptr;
void (*glUniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat*) = nullptr;

void (*glGenVertexArrays)(GLsizei, GLuint*) = nullptr;
void (*glBindVertexArray)(GLuint) = nullptr;
void (*glDeleteVertexArrays)(GLsizei, const GLuint*) = nullptr;

void (*glGenBuffers)(GLsizei, GLuint*) = nullptr;
void (*glBindBuffer)(GLenum, GLuint) = nullptr;
void (*glBufferData)(GLenum, GLsizeiptr, const void*, GLenum) = nullptr;
void (*glDeleteBuffers)(GLsizei, const GLuint*) = nullptr;

void (*glEnableVertexAttribArray)(GLuint) = nullptr;
void (*glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*) = nullptr;

void (*glDrawElements)(GLenum, GLsizei, GLenum, const void*) = nullptr;

void (*glDebugMessageCallback)(void (*)(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*), const void*) = nullptr;
void (*glDebugMessageControl)(GLenum, GLenum, GLenum, GLsizei, const GLuint*, GLboolean) = nullptr;


void vgl::loadGLFunctions(void* (*getProcAddress)(const char*))
{
    glEnable = reinterpret_cast<decltype(glEnable)>(getProcAddress("glEnable"));

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

    glGetUniformLocation = reinterpret_cast<decltype(glGetUniformLocation)>(getProcAddress("glGetUniformLocation"));
    glUniform1f = reinterpret_cast<decltype(glUniform1f)>(getProcAddress("glUniform1f"));
    glUniform3fv = reinterpret_cast<decltype(glUniform3fv)>(getProcAddress("glUniform3fv"));
    glUniformMatrix4fv = reinterpret_cast<decltype(glUniformMatrix4fv)>(getProcAddress("glUniformMatrix4fv"));

    glGenVertexArrays = reinterpret_cast<decltype(glGenVertexArrays)>(getProcAddress("glGenVertexArrays"));
    glBindVertexArray = reinterpret_cast<decltype(glBindVertexArray)>(getProcAddress("glBindVertexArray"));
    glDeleteVertexArrays = reinterpret_cast<decltype(glDeleteVertexArrays)>(getProcAddress("glDeleteVertexArrays"));

    glGenBuffers = reinterpret_cast<decltype(glGenBuffers)>(getProcAddress("glGenBuffers"));
    glBindBuffer = reinterpret_cast<decltype(glBindBuffer)>(getProcAddress("glBindBuffer"));
    glBufferData = reinterpret_cast<decltype(glBufferData)>(getProcAddress("glBufferData"));
    glDeleteBuffers = reinterpret_cast<decltype(glDeleteBuffers)>(getProcAddress("glDeleteBuffers"));

    glEnableVertexAttribArray = reinterpret_cast<decltype(glEnableVertexAttribArray)>(getProcAddress("glEnableVertexAttribArray"));
    glVertexAttribPointer = reinterpret_cast<decltype(glVertexAttribPointer)>(getProcAddress("glVertexAttribPointer"));

    glDrawElements = reinterpret_cast<decltype(glDrawElements)>(getProcAddress("glDrawElements"));

    glDebugMessageCallback = reinterpret_cast<decltype(glDebugMessageCallback)>(getProcAddress("glDebugMessageCallback"));
    glDebugMessageControl = reinterpret_cast<decltype(glDebugMessageControl)>(getProcAddress("glDebugMessageControl"));
}