#include <vgl/renderer.h>

#include <iostream>
#include "renderer.h"

const std::string vsNone = R"vs_none(
#version 460 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)vs_none";

const std::string fsNone = R"fs_none(
#version 460 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
)fs_none";

namespace vgl::internal {
    std::map<LightingModel, Program> _programMap;
} // namespace vgl::internal

#define PRINT_WARNING(msg, desc) std::cout  << "[WARNING] " << __FILE__ << " (" << __LINE__ << "):  " \
                                            << __func__ << " (" << msg << ")\n         " << desc << std::endl;

vgl::Shader::Shader(GLenum shaderType, const std::string& code)
{
    mID = glCreateShader(shaderType);
    const char* cstr = code.c_str();
    glShaderSource(mID, 1, &cstr, nullptr);
    glCompileShader(mID);
    int success;
    glGetShaderiv(mID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint length;
        glGetShaderiv(mID, GL_INFO_LOG_LENGTH, &length);
        char* infoLog = new char[length];
        glGetShaderInfoLog(mID, length, nullptr, infoLog);
        std::cout << "[ERROR] " << __FILE__ << " (" << __LINE__ << "):  " << __func__ << " (Compilation failed)" << std::endl;
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        std::abort();
    }
}

vgl::Shader::~Shader()
{
    glDeleteShader(mID);
}

GLuint vgl::Shader::id() const
{
    return mID;
}
vgl::Program::Program(LightingModel model)
{
    mID = glCreateProgram();

    {
        std::unique_ptr<Shader> vs = nullptr;
        std::unique_ptr<Shader> fs = nullptr;
        switch (model)
        {
        case LightingModel::Phong:
            // TODO: implement
            break;
        case LightingModel::BlinnPhong:
            // TODO: implement
            break;
        case LightingModel::CookTorrance:
            // TODO: implement
            break;
        default:
            vs = std::make_unique<Shader>(GL_VERTEX_SHADER, vsNone);
            fs = std::make_unique<Shader>(GL_FRAGMENT_SHADER, fsNone);
            break;
        }
        glAttachShader(mID, vs->id());
        glAttachShader(mID, fs->id());
        glLinkProgram(mID);
    }

    int success;
    glGetProgramiv(mID, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint length;
        glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &length);
        char* infoLog = new char[length];
        glGetProgramInfoLog(mID, length, nullptr, infoLog);
        std::cout << "[ERROR] " << __FILE__ << " (" << __LINE__ << "):  " << __func__ << " (Linking failed)" << std::endl;
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        std::abort();
    }
}

vgl::Program::~Program()
{
    glDeleteProgram(mID);
}

void vgl::Program::use() const
{
    glUseProgram(mID);
}

GLuint vgl::Program::id() const
{
    return mID;
}

vgl::Mesh::Mesh(SharedMeshData data)
{
    set(data);
}

void vgl::Mesh::set(SharedMeshData data)
{  
    mMutex.lock();
    if (data != mData) {
        mData = data;
        mDirty = true;
        mDraw = false;
    }
    mMutex.unlock();
}

void vgl::Mesh::update()
{
    mMutex.lock();
    if (mDirty) {
        destroyGLObjects();
        createGLObjects();
        mDirty = false;
    }
    mMutex.unlock();
}

void vgl::Mesh::draw() const
{
    #ifdef VGL_OPENGL_DEBUG_MODE
    if (mData == nullptr) {
        PRINT_WARNING("Mesh data is null", "Mesh will not be rendered.");
    }
    if (mVAO == 0) {
        PRINT_WARNING("VAO is not initialized", "Call update() before draw()");
    }
    if (mData->vertices == nullptr || mData->vertexCount == 0) {
        PRINT_WARNING("No vertices", "Mesh will not be rendered.");
    }
    if (mVerticesVBO == 0) {
        PRINT_WARNING("Vertices VBO is not initialized", "Corrupted data?");
    }
    if (mData->indices == nullptr || mData->indexCount == 0) {
        PRINT_WARNING("No indices", "Mesh will not be rendered.");
    }
    if (mEBO == 0) {
        PRINT_WARNING("EBO is not initialized", "Corrupted data?");
    }
    #endif

    if (!mDraw) {
        return;
    }
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mData->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void vgl::Mesh::createGLObjects()
{   
    if (mData == nullptr) {
        mDraw = false;
        return;
    }
    mDraw = true;

    glGenVertexArrays(1, &mVAO);
    if (mVAO == 0) {
        mDraw = false;
        return;
    }
    glGenBuffers(1, &mVerticesVBO);
    if (mVerticesVBO == 0) {
        mDraw = false;
        return;
    }
    glGenBuffers(1, &mEBO);
    if (mEBO == 0) {
        mDraw = false;
        return;
    }

    glBindVertexArray(mVAO);

    if (mData->vertices && mData->vertexCount > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, mVerticesVBO);
        glBufferData(GL_ARRAY_BUFFER, mData->vertexCount * sizeof(float), mData->vertices, GL_STATIC_DRAW);
    } else {
        mDraw = false;
        return;
    }

    if (mData->indices && mData->indexCount > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mData->indexCount * sizeof(GLuint), mData->indices, GL_STATIC_DRAW);
    } else {
        mDraw = false;
        return;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void vgl::Mesh::destroyGLObjects()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVerticesVBO);
    glDeleteBuffers(1, &mEBO);
    mVAO = 0;
    mVerticesVBO = 0;
    mEBO = 0;
}
