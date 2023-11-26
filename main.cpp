#include <iostream>

#include <vgl/window.h>
#include <vgl/gl.h>
#include <vgl/renderer.h>
#include <chrono>

// TODO: remove file (for testing purposes only)

std::string vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

int main()
{
    vgl::initialize();
    vgl::setOption(vgl::Option::DefaultShowWindow, true);
    vgl::setOption(vgl::Option::DefaultResizableWindow, true);

    vgl::Window w(100, 100, 800, 600, "VitalGL");
    w.setTitle("VitalGL");
    w.setViewport(200, 200, 400, 300);
    w.setResizable(true);

    // w.releaseGLContext();
    // w.makeGLContextCurrent();

    // build and compile our shader program
    // ------------------------------------
    // vertex shader

    vgl::Shader vertexShader(GL_VERTEX_SHADER, vertexShaderSource);

    // unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShader);
    // // check for shader compile errors
    // int success;
    // char infoLog[512];
    // // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // // if (!success)
    // // {
    // //     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    // //     std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    // // }
    // // fragment shader
    // unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);
    // // check for shader compile errors
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    // }
    // // link shaders
    // unsigned int shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader.id());
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);
    // // check for linking errors
    // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    // if (!success) {
    //     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    // }
    // glDeleteShader(fragmentShader);

    vgl::Program program(vgl::LightingModel::None);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // add a new set of vertices to form a second triangle (a total of 6 vertices); the vertex attribute configuration remains the same (still one 3-float position vector per vertex)
    // float vertices[] = {
    //     // first triangle
    //     -0.9f, -0.5f, 0.0f,  // left
    //     -0.0f, -0.5f, 0.0f,  // right
    //     -0.45f, 0.5f, 0.0f,  // top
    //     // second triangle
    //      0.0f, -0.5f, 0.0f,  // left
    //      0.9f, -0.5f, 0.0f,  // right
    //      0.45f, 0.5f, 0.0f   // top
    // };
    // unsigned int indices[] = {
    //     0, 1, 2,
    //     3, 4, 5
    // };
    float vertices[] = {
        // Front face
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        -0.5, 0.5, -0.5,
        -0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        // Back face
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
        -0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        // Left face
        -0.5, -0.5, -0.5,
        -0.5, 0.5, -0.5,
        -0.5, 0.5, 0.5,
        -0.5, -0.5, -0.5,
        -0.5, 0.5, 0.5,
        -0.5, -0.5, 0.5,
        // Right face
        0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        0.5, 0.5, 0.5,
        0.5, -0.5, -0.5,
        0.5, 0.5, 0.5,
        0.5, -0.5, 0.5,
        // Bottom face
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, -0.5, 0.5,
        -0.5, -0.5, -0.5,
        0.5, -0.5, 0.5,
        -0.5, -0.5, 0.5,
        // Top face
        -0.5, 0.5, -0.5,
        0.5, 0.5, -0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, -0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5};

    // Indices for the cube
    float normals[] = {
        // Front face
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        // Back face
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        // Left face
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        // Right face
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        // Bottom face
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        // Top face
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0};

    GLuint indices[] = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35};

    vgl::SharedMeshData meshData = std::make_shared<vgl::MeshData>();
    meshData->vertices = vertices;
    meshData->vertexCount = 36*3;
    meshData->indices = indices;
    meshData->indexCount = 36;
    meshData->lightingModel = vgl::LightingModel::None;

    vgl::Scene scene;
    auto &mesh = scene.addMesh(meshData);
    // mesh.translate({0.0f, -10.5f, 0.0f});
    // mesh.scale(1.0f);
    // mesh.rotate(45.0f, {0.0f, 0.0f, 1.0f});
    scene.update();

    // vgl::Mesh mesh(meshData);
    // mesh.update();
    scene.camera().setPosition({0.0f, 0.3f, 2.0f});

    // measure frame time
    // ------------------
    float deltaTime = 0.0f;
    std::chrono::high_resolution_clock::time_point lastFrame;
    // render loop

    while (!w.shouldClose())
    {
        lastFrame = std::chrono::high_resolution_clock::now();
        w.pollEvents();
        glViewport(0, 0, w.width(), w.height());
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene.camera().setAspectRatio(static_cast<float>(w.aspectRatio()));
        // scene.camera().translate({0.0f, 0.0f, 0.001f});
        scene.camera().rotate(0.01f, {0.0f, 1.0f, 0.0f});

        // draw our first triangle
        scene.draw();

        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - lastFrame).count()) / 1'000'000;
        w.swapBuffers();
        // std::cout << deltaTime << std::endl;
    }

    return 0;
}