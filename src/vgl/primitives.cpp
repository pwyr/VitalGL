#include <vgl/primitives.h>
#include "primitives.h"

const std::array<GLfloat, 72> vgl::Cube::mVertices = {
    // Front face
    -0.5f, -0.5f,  0.5f, // 0
     0.5f, -0.5f,  0.5f, // 1
     0.5f,  0.5f,  0.5f, // 2
    -0.5f,  0.5f,  0.5f, // 3
    // Back face
    -0.5f, -0.5f, -0.5f, // 4
     0.5f, -0.5f, -0.5f, // 5
     0.5f,  0.5f, -0.5f, // 6
    -0.5f,  0.5f, -0.5f, // 7
    // Top face
     0.5f,  0.5f,  0.5f, // 8
    -0.5f,  0.5f,  0.5f, // 9
    -0.5f,  0.5f, -0.5f, // 10
     0.5f,  0.5f, -0.5f, // 11
    // Bottom face
    -0.5f, -0.5f,  0.5f, // 12
     0.5f, -0.5f,  0.5f, // 13
     0.5f, -0.5f, -0.5f, // 14
    -0.5f, -0.5f, -0.5f, // 15
    // Right face
     0.5f, -0.5f,  0.5f, // 16
     0.5f,  0.5f,  0.5f, // 17
     0.5f,  0.5f, -0.5f, // 18
     0.5f, -0.5f, -0.5f, // 19
    // Left face
    -0.5f, -0.5f,  0.5f, // 20
    -0.5f,  0.5f,  0.5f, // 21
    -0.5f,  0.5f, -0.5f, // 22
    -0.5f, -0.5f, -0.5f // 23
};

const std::array<GLfloat, 72> vgl::Cube::mNormals = {
    // Front face
    0.0f, 0.0f, 1.0f, // 0
    0.0f, 0.0f, 1.0f, // 1
    0.0f, 0.0f, 1.0f, // 2
    0.0f, 0.0f, 1.0f, // 3
    // Back face 
    0.0f, 0.0f, -1.0f, // 4
    0.0f, 0.0f, -1.0f, // 5
    0.0f, 0.0f, -1.0f, // 6
    0.0f, 0.0f, -1.0f, // 7
    // Top face
    0.0f, 1.0f, 0.0f, // 8
    0.0f, 1.0f, 0.0f, // 9
    0.0f, 1.0f, 0.0f, // 10
    0.0f, 1.0f, 0.0f, // 11
    // Bottom face
    0.0f, -1.0f, 0.0f, // 12
    0.0f, -1.0f, 0.0f, // 13
    0.0f, -1.0f, 0.0f, // 14
    0.0f, -1.0f, 0.0f, // 15
    // Right face
    1.0f, 0.0f, 0.0f, // 16
    1.0f, 0.0f, 0.0f, // 17
    1.0f, 0.0f, 0.0f, // 18
    1.0f, 0.0f, 0.0f, // 19
    // Left face
    -1.0f, 0.0f, 0.0f, // 20
    -1.0f, 0.0f, 0.0f, // 21
    -1.0f, 0.0f, 0.0f, // 22
    -1.0f, 0.0f, 0.0f // 23
};

const std::array<GLuint, 36> vgl::Cube::mIndices = {
    // Front face
    0, 1, 2, 2, 3, 0,
    // Back face
    4, 5, 6, 6, 7, 4,
    // Top face
    8, 9, 10, 10, 11, 8,
    // Bottom face
    12, 13, 14, 14, 15, 12,
    // Right face
    16, 17, 18, 18, 19, 16,
    // Left face
    20, 21, 22, 22, 23, 20
};

vgl::Cube::Cube(vec3 position, float scale, vec3 color)
    : mMesh()
{
    Material mat;
    mat.ambientColor = color;
    mat.diffuseColor = color;
    mat.specularColor = color;
    mat.shininess = 32.0f;
    mat.lightingModel = LightingModel::BlinnPhong;

    SharedMeshData meshData = std::make_shared<MeshData>();
    meshData->vertices = mVertices.data();
    meshData->vertexCount = static_cast<GLsizei>(mVertices.size());
    meshData->normals = mNormals.data();
    meshData->indices = mIndices.data();
    meshData->indexCount = static_cast<GLsizei>(mIndices.size());
    meshData->materials.push_back(mat);
    meshData->matTriangleCount.push_back(12);

    mMesh.set(meshData);
    mMesh.translate(position);
    mMesh.scale(scale);
}

vgl::Mesh &vgl::Cube::mesh()
{
    return mMesh;
}
