#include <vgl/renderer.h>

#include <iostream>
#include "renderer.h"

const std::string vsNone = R"vs_none(
#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
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

const std::string vsPhong = R"vs_phong(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = normalize(vec3(uModel * vec4(aNormal, 1.0)));
    gl_Position = uProjection * uView * uModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)vs_phong";

const std::string fsPhong = R"fs_phong(
#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 uViewPos;
uniform Material uMaterial;
uniform Light uLight;

void main()
{
    vec3 ambient = uLight.ambient * uMaterial.ambient;

    vec3 n = Normal;
    vec3 l = normalize(uLight.position - FragPos);
    vec3 diffuse = uLight.diffuse * uMaterial.diffuse * max(0.0f, dot(n, l));

    vec3 v = normalize(uViewPos - FragPos);
    vec3 r = reflect(-l, n);
    vec3 specular = uLight.specular * uMaterial.specular * pow(max(0.0f, dot(r, v)), uMaterial.shininess);

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
)fs_phong";

const std::string fsBlinnPhong = R"fs_blinn_phong(
#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 uViewPos;
uniform Material uMaterial;
uniform Light uLight;

void main()
{
    vec3 ambient = uLight.ambient * uMaterial.ambient;

    vec3 n = Normal;
    vec3 l = normalize(uLight.position - FragPos);
    vec3 diffuse = uLight.diffuse * uMaterial.diffuse * max(0.0f, dot(n, l));

    vec3 v = normalize(uViewPos - FragPos);
    vec3 h = normalize(l + v);
    vec3 specular = uLight.specular * uMaterial.specular * pow(max(0.0f, dot(h, n)), uMaterial.shininess);

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
)fs_blinn_phong";



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
            vs = std::make_unique<Shader>(GL_VERTEX_SHADER, vsPhong);
            fs = std::make_unique<Shader>(GL_FRAGMENT_SHADER, fsPhong);
            break;
        case LightingModel::BlinnPhong:
            vs = std::make_unique<Shader>(GL_VERTEX_SHADER, vsPhong);
            fs = std::make_unique<Shader>(GL_FRAGMENT_SHADER, fsBlinnPhong);
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

vgl::Mesh::Mesh(Mesh&& mesh)
{
    internal::swap(*this, mesh);
}

vgl::Mesh& vgl::Mesh::operator=(Mesh&& mesh)
{
    internal::swap(*this, mesh);
    return *this;
}

vgl::vec3 vgl::internal::operator*(const mat3 &a, const vec3 &b)
{
    vec3 res;
    for (int i = 0; i < 3; ++i) {
        res[i] = a[i][0] * b[0] + a[i][1] * b[1] + a[i][2] * b[2];
    }
    return res;
}

vgl::mat3 vgl::internal::operator*(const mat3 &a, const mat3 &b)
{
    mat3 res;
    for (int i = 0; i < 3; ++i) {
        res[i][0] = a[i][0] * b[0][0] + a[i][1] * b[1][0] + a[i][2] * b[2][0];
        res[i][1] = a[i][0] * b[0][1] + a[i][1] * b[1][1] + a[i][2] * b[2][1];
        res[i][2] = a[i][0] * b[0][2] + a[i][1] * b[1][2] + a[i][2] * b[2][2];
    }
    return res;
}

vgl::mat4 vgl::internal::operator*(const mat4 &a, const mat4 &b)
{
    mat4 res;
    for (int i = 0; i < 4; ++i) {
        res[i][0] = a[i][0] * b[0][0] + a[i][1] * b[1][0] + a[i][2] * b[2][0] + a[i][3] * b[3][0];
        res[i][1] = a[i][0] * b[0][1] + a[i][1] * b[1][1] + a[i][2] * b[2][1] + a[i][3] * b[3][1];
        res[i][2] = a[i][0] * b[0][2] + a[i][1] * b[1][2] + a[i][2] * b[2][2] + a[i][3] * b[3][2];
        res[i][3] = a[i][0] * b[0][3] + a[i][1] * b[1][3] + a[i][2] * b[2][3] + a[i][3] * b[3][3];
    }
    return res;
}

vgl::vec3 vgl::internal::cross(const vec3 &a, const vec3 &b)
{
    return vec3{
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]};
}

vgl::vec3 vgl::internal::normalize(const vec3 &v)
{
    GLfloat length = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return vec3{v[0] / length, v[1] / length, v[2] / length};
}

void vgl::internal::swap(Mesh& a, Mesh& b)
{
    std::swap(a.mData, b.mData);
    std::swap(a.mDirty, b.mDirty);
    std::swap(a.mDraw, b.mDraw);
    std::swap(a.mVerticesVBO, b.mVerticesVBO);
    std::swap(a.mEBO, b.mEBO);
    std::swap(a.mVAO, b.mVAO);
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

void vgl::Mesh::translate(const vec3 &translation)
{
    using internal::operator*;

    mat4 translationMatrix = mat4{
        1.0f, 0.0f, 0.0f, translation[0],
        0.0f, 1.0f, 0.0f, translation[1],
        0.0f, 0.0f, 1.0f, translation[2],
        0.0f, 0.0f, 0.0f, 1.0f};
    mModel = translationMatrix * mModel;
}

void vgl::Mesh::rotate(GLfloat angle, const vec3 &axis)
{
    using internal::operator*;

    GLfloat w = std::cos(angle / 2);
    GLfloat sinHalfAngle = std::sin(angle / 2);
    GLfloat x = axis[0] * sinHalfAngle;
    GLfloat y = axis[1] * sinHalfAngle;
    GLfloat z = axis[2] * sinHalfAngle;

    GLfloat xx = x * x, xy = x * y, xz = x * z, xw = x * w;
    GLfloat yy = y * y, yz = y * z, yw = y * w;
    GLfloat zz = z * z, zw = z * w;

    mat4 rotationMatrix = mat4{
        1 - 2 * (yy + zz), 2 * (xy - zw), 2 * (xz + yw), 0.0f,
        2 * (xy + zw), 1 - 2 * (xx + zz), 2 * (yz - xw), 0.0f,
        2 * (xz - yw), 2 * (yz + xw), 1 - 2 * (xx + yy), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    mModel = rotationMatrix * mModel;
}

void vgl::Mesh::scale(GLfloat scale)
{
    using internal::operator*;

    mat4 scaleMatrix = mat4{
        scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, scale, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};
    
    mModel = scaleMatrix * mModel;
}

void vgl::Mesh::scale(const vec3 &scale)
{
    using internal::operator*;

    mat4 scaleMatrix = mat4{
        scale[0], 0.0f, 0.0f, 0.0f,
        0.0f, scale[1], 0.0f, 0.0f,
        0.0f, 0.0f, scale[2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};
    
    mModel = scaleMatrix * mModel;
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
    if (mScene == nullptr) {
        PRINT_WARNING("Scene is null", "Mesh will not be rendered.");
    }
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
    
    internal::_programMap.at(mData->lightingModel).use();
    GLuint program = internal::_programMap.at(mData->lightingModel).id();

    mat4 model = mModel.load();
    glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_TRUE, &model[0][0]);

    vec3 viewPos = mScene->camera().position();
    glUniform3fv(glGetUniformLocation(program, "uViewPos"), 1, &viewPos[0]);
    mat4 view = mScene->camera().viewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(program, "uView"), 1, GL_TRUE, &view[0][0]);

    mat4 projection = mScene->camera().projectionMatrix();
    glUniformMatrix4fv(glGetUniformLocation(program, "uProjection"), 1, GL_TRUE, &projection[0][0]);

    glUniform3fv(glGetUniformLocation(program, "uLight.position"), 1, &mScene->lightPosition()[0]);
    glUniform3fv(glGetUniformLocation(program, "uLight.ambient"), 1, &mScene->lightAmbientColor()[0]);
    glUniform3fv(glGetUniformLocation(program, "uLight.diffuse"), 1, &mScene->lightDiffuseColor()[0]);
    glUniform3fv(glGetUniformLocation(program, "uLight.specular"), 1, &mScene->lightSpecularColor()[0]);

    glBindVertexArray(mVAO);
    size_t primitive = 0;
    for (int i = 0; i < mData->materials.size(); ++i) {
        glUniform3fv(glGetUniformLocation(program, "uMaterial.ambient"), 1, &mData->materials[i].ambientColor[0]);
        glUniform3fv(glGetUniformLocation(program, "uMaterial.diffuse"), 1, &mData->materials[i].diffuseColor[0]);
        glUniform3fv(glGetUniformLocation(program, "uMaterial.specular"), 1, &mData->materials[i].specularColor[0]);
        glUniform1f(glGetUniformLocation(program, "uMaterial.shininess"), mData->materials[i].shininess);
        glDrawElements(GL_TRIANGLES, mData->matTriangleCount[i] * 3, GL_UNSIGNED_INT, (void*)(primitive * sizeof(GLuint)));
        primitive += mData->matTriangleCount[i] * 3;
    }

    glBindVertexArray(0);
}

void vgl::Mesh::setScene(Scene *scene)
{
    mScene = scene;
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
    // TODO: adapt to lighting model
    glGenBuffers(1, &mNormalsVBO);
    if (mNormalsVBO == 0) {
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
        glBufferData(GL_ARRAY_BUFFER, mData->vertexCount * sizeof(GLfloat), mData->vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
    } else {
        mDraw = false;
        return;
    }

    if (mData->normals && mData->normalCount == mData->vertexCount) {
        glBindBuffer(GL_ARRAY_BUFFER, mNormalsVBO);
        glBufferData(GL_ARRAY_BUFFER, mData->normalCount * sizeof(GLfloat), mData->normals, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(1);
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
    
    glBindVertexArray(0);

    internal::_programMap.try_emplace(mData->lightingModel, mData->lightingModel);
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

vgl::Camera::Camera()
{
    updateViewMatrix();
    updateProjectionMatrix();
}

vgl::vec3 vgl::Camera::position() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mPosition;
}

vgl::mat4 vgl::Camera::viewMatrix() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mViewMatrix;
}

vgl::mat4 vgl::Camera::projectionMatrix() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mProjectionMatrix;
}

void vgl::Camera::setPosition(const vec3 &position)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mPosition = position;
    updateViewMatrix();
}

void vgl::Camera::translate(const vec3 &translation)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mPosition[0] += translation[0];
    mPosition[1] += translation[1];
    mPosition[2] += translation[2];
    updateViewMatrix();	
}

void vgl::Camera::setDirection(const vec3 &dir, const vec3 &up)
{
    using internal::normalize;
    using internal::cross;

    vec3 d = normalize(dir);
    vec3 r = normalize(cross(dir, up));
    vec3 u = cross(r, d);

    std::lock_guard<std::mutex> lock(mMutex);
    mRotationMatrix = mat3{
        r[0], r[1], r[2],
        u[0], u[1], u[2],
        d[0], d[1], d[2]};

    updateViewMatrix();
}

void vgl::Camera::rotate(GLfloat angle, const vec3 &axis)
{
    using internal::operator*;

    GLfloat w = std::cos(angle / 2);
    GLfloat sinHalfAngle = std::sin(angle / 2);
    GLfloat x = axis[0] * sinHalfAngle;
    GLfloat y = axis[1] * sinHalfAngle;
    GLfloat z = axis[2] * sinHalfAngle;

    GLfloat xx = x * x, xy = x * y, xz = x * z, xw = x * w;
    GLfloat yy = y * y, yz = y * z, yw = y * w;
    GLfloat zz = z * z, zw = z * w;

    mat3 rotationMatrix = mat3{
        1 - 2 * (yy + zz), 2 * (xy - zw), 2 * (xz + yw),
        2 * (xy + zw), 1 - 2 * (xx + zz), 2 * (yz - xw),
        2 * (xz - yw), 2 * (yz + xw), 1 - 2 * (xx + yy)};

    std::lock_guard<std::mutex> lock(mMutex);
    mRotationMatrix = rotationMatrix * mRotationMatrix;
    updateViewMatrix();
}

void vgl::Camera::rotate(mat3 rotationMatrix)
{
    using internal::operator*;

    std::lock_guard<std::mutex> lock(mMutex);
    mRotationMatrix = rotationMatrix * mRotationMatrix;
    updateViewMatrix();
}

void vgl::Camera::setNearPlane(GLfloat near)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mNear = near;
    updateProjectionMatrix();
}

void vgl::Camera::setFarPlane(GLfloat far)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mFar = far;
    updateProjectionMatrix();
}

void vgl::Camera::setFov(GLfloat fov)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mFov = fov;
    updateProjectionMatrix();
}

void vgl::Camera::setAspectRatio(GLfloat aspectRatio)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mAspectRatio = aspectRatio;
    updateProjectionMatrix();
}

void vgl::Camera::updateViewMatrix()
{
    using internal::operator*;

    mat4 translationMatrix = mat4{
        1.0f, 0.0f, 0.0f, -mPosition[0],
        0.0f, 1.0f, 0.0f, -mPosition[1],
        0.0f, 0.0f, 1.0f, -mPosition[2],
        0.0f, 0.0f, 0.0f, 1.0f};
    mat4 rotationMatrix = mat4{
        mRotationMatrix[0][0], mRotationMatrix[0][1], mRotationMatrix[0][2], 0.0f,
        mRotationMatrix[1][0], mRotationMatrix[1][1], mRotationMatrix[1][2], 0.0f,
        mRotationMatrix[2][0], mRotationMatrix[2][1], mRotationMatrix[2][2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};
    
    mViewMatrix = rotationMatrix * translationMatrix;
}

void vgl::Camera::updateProjectionMatrix()
{
    GLfloat tanHalfFov = std::tan(mFov / 2);
    GLfloat t = mNear * tanHalfFov;
    GLfloat r = t * mAspectRatio;

    mat4 projectionMatrix = mat4{
        mNear / r, 0.0f, 0.0f, 0.0f,
        0.0f, mNear / t, 0.0f, 0.0f,
        0.0f, 0.0f, -(mFar + mNear) / (mFar - mNear), -2 * mFar * mNear / (mFar - mNear),
        0.0f, 0.0f, -1.0f, 0.0f};

    mProjectionMatrix = projectionMatrix;
}

vgl::Mesh& vgl::Scene::addMesh(Mesh &&mesh)
{
    mMeshes.emplace_back(std::move(mesh));
    mMeshes.back().setScene(this);
    return mMeshes.back();
}

vgl::Mesh &vgl::Scene::addMesh(SharedMeshData data)
{
    mMeshes.emplace_back(data);
    mMeshes.back().setScene(this);
    return mMeshes.back();
}

vgl::Camera &vgl::Scene::camera()
{
    return mCamera;
}

vgl::vec3 vgl::Scene::lightPosition() const
{
    return mLightPosition;    
}

vgl::vec3 vgl::Scene::lightAmbientColor() const
{
    return mLightAmbientColor;
}

vgl::vec3 vgl::Scene::lightDiffuseColor() const
{
    return mLightDiffuseColor;
}

vgl::vec3 vgl::Scene::lightSpecularColor() const
{
    return mLightSpecularColor;
}

void vgl::Scene::update()
{
    for (Mesh& mesh : mMeshes) {
        mesh.update();
    }
}

void vgl::Scene::draw() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (const Mesh& mesh : mMeshes) {
        mesh.draw();
    }
}
