#include <iostream>

#include <vgl/window.h>
#include <vgl/gl.h>
#include <vgl/renderer.h>

//TODO: remove file (for testing purposes only)

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

int main() {
    vgl::initialize();
    vgl::setOption(vgl::Option::DefaultShowWindow, true);
    vgl::setOption(vgl::Option::DefaultResizableWindow, true);

    vgl::Window w(100, 100, 800, 600, "VitalGL");
    w.setTitle("VitalGL 2.0");
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
    float vertices[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
        // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 
    }; 
    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5
    };

    vgl::SharedMeshData meshData = std::make_shared<vgl::MeshData>();
    meshData->vertices = vertices;
    meshData->vertexCount = 18;
    meshData->indices = indices;
    meshData->indexCount = 6;
    meshData->lightingModel = vgl::LightingModel::None;

    vgl::Mesh mesh(meshData);
    mesh.update();

    while (!w.shouldClose()) {
        w.pollEvents();
        glViewport(0, 0, w.width(), w.height());
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        program.use();
        mesh.draw();

        w.swapBuffers();
        Sleep(10);
    }



    return 0;
}