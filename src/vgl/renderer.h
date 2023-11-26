#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <array>
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

    const GLfloat* normals = nullptr;
    GLsizei normalCount = 0;

    // const float* texCoords = nullptr;
    // size_t texCoordCount = 0;

    const GLuint* indices = nullptr;
    GLsizei indexCount = 0;

    LightingModel lightingModel = LightingModel::None;
};

using SharedMeshData = std::shared_ptr<MeshData>;

using vec3 = std::array<GLfloat, 3>;
using mat3 = std::array<std::array<GLfloat, 3>, 3>;
using mat4 = std::array<std::array<GLfloat, 4>, 4>;

class Mesh;
namespace internal{
    void swap(Mesh& a, Mesh& b);

    vec3 operator*(const mat3& a, const vec3& b);
    mat3 operator*(const mat3& a, const mat3& b);
    mat4 operator*(const mat4& a, const mat4& b);

    vec3 cross(const vec3& a, const vec3& b);
    vec3 normalize(const vec3& v);
} // namespace internal

class Scene;

class Mesh {
public:
    Mesh(SharedMeshData data);
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&);
    Mesh& operator=(Mesh&&);
    friend void internal::swap(Mesh& a, Mesh& b);

    // arbitrary thread
    void set(SharedMeshData data);

    void translate(const vec3& translation);
    void rotate(GLfloat angle, const vec3& axis);
    void scale(GLfloat scale);
    void scale(const vec3& scale);


    // rendering thread only
    void update();
    void draw() const;

private:
    friend class Scene;
    void setScene(Scene* scene);

private:
    void createGLObjects();
    void destroyGLObjects();

private:
    SharedMeshData mData = nullptr;
    bool mDirty = false;
    bool mDraw = false;

    GLuint mVerticesVBO = 0, mEBO = 0, mVAO = 0;

    mutable std::mutex mMutex;
    std::atomic<mat4> mModel = mat4{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    Scene* mScene = nullptr;
};

class Camera {
public:
    Camera();

    mat4 viewMatrix() const;
    mat4 projectionMatrix() const;

    void setPosition(const vec3& position);
    void translate(const vec3& translation);

    void setDirection(const vec3& dir, const vec3& up);
    void rotate(GLfloat angle, const vec3& axis);
    void rotate(mat3 rotationMatrix);

    void setNearPlane(GLfloat near);
    void setFarPlane(GLfloat far);

    void setFov(GLfloat fov);
    void setAspectRatio(GLfloat aspectRatio);

private:
    // requires a lock on mMutex
    void updateViewMatrix();
    void updateProjectionMatrix();

private:
    // view matrix components
    vec3 mPosition{0.0f, 0.0f, 2.f};
    mat3 mRotationMatrix = mat3{
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0, 0.0f,
        0.0f, 0.0f, 1.0f};

    // projection matrix components
    GLfloat mNear = 0.1f, mFar = 100.0f;
    GLfloat mFov = 70.0f;
    GLfloat mAspectRatio = 1.0f;

    mat4 mViewMatrix{};
    mat4 mProjectionMatrix{};

    mutable std::mutex mMutex;
};

class Scene {
public:
    Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Mesh& addMesh(Mesh&& mesh);
    Mesh& addMesh(SharedMeshData data);

    Camera& camera();

    // rendering thread only
    void update();
    void draw() const;
private:
    std::vector<Mesh> mMeshes{};

    Camera mCamera{};
    vec3 mLightPosition{0.5f, 2.0f, 1.0f};
};

} // namespace vgl