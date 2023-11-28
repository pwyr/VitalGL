#pragma once

#include <vgl/renderer.h>


namespace vgl{

class Cube {
public:
    Cube(vec3 position, float scale = 1.f, vec3 color = {0.f, 1.f, 0.f});

    Mesh& mesh();

private:
    Mesh mMesh;

    static const std::array<GLfloat, 72> mVertices;
    static const std::array<GLfloat, 72> mNormals;
    static const std::array<GLuint, 36> mIndices;
};

} // namespace vgl