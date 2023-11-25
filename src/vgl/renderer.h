#pragma once

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <vgl/gl.h>


namespace vgl {



class Shader {
public:
    Shader(GLenum shaderType, const std::string& code);
    ~Shader();

    GLuint id() const;

private:
    GLuint mID;
};

enum class LightingModel {
    None,
    Phong,
    BlinnPhong,
    CookTorrance,
};

class Program {
public:
    Program(LightingModel model);
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    ~Program();

    void use() const;

    GLuint id() const;

private:
    GLuint mID;
};

struct MeshData {
    const GLfloat* vertices = nullptr;
    GLsizei vertexCount = 0;

    // const float* normals = nullptr;
    // size_t normalCount = 0;

    // const float* texCoords = nullptr;
    // size_t texCoordCount = 0;

    const GLuint* indices = nullptr;
    GLsizei indexCount = 0;

    LightingModel lightingModel = LightingModel::None;
};

using SharedMeshData = std::shared_ptr<MeshData>;

class Mesh {
public:
    Mesh(SharedMeshData data);
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // arbitrary thread
    void set(SharedMeshData data);

    // rendering thread only
    void update();
    void draw() const;

private:
    void createGLObjects();
    void destroyGLObjects();

private:
    SharedMeshData mData = nullptr;
    bool mDirty = false;
    bool mDraw = false;

    GLuint mVerticesVBO = 0, mEBO = 0, mVAO = 0;

    mutable std::mutex mMutex;
};

class Scene {
public:
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    void addMesh(const Mesh& mesh);

    void draw() const;
};

} // namespace vgl