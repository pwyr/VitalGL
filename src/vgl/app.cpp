#include "app.h"

#include <chrono>
#include <iostream>


vgl::App::App(int x, int y, int width, int height, const std::string& title)
    : mWindow(x, y, width, height, title), mScene()
{
}

vgl::Scene &vgl::App::scene()
{
    return mScene;
}

vgl::Window &vgl::App::window()
{
    return mWindow; 
}

// struct Timer {
//     Timer() {
//         startTime = std::chrono::high_resolution_clock::now();
//     }

//     void stop() {
//         auto endTime = std::chrono::high_resolution_clock::now();
//         std::cout << std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count() << std::endl;
//     }

//     std::chrono::high_resolution_clock::time_point startTime;
// };

[[noreturn]] void vgl::App::run()
{
    using time_point = std::chrono::high_resolution_clock::time_point;

    // print fps each second
    #ifdef VGL_PRINT_FPS
    double timePassed = 0.;
    time_point frameStart;
    size_t frames = 0;
    #endif
    
    time_point lastTime, currentTime;
    size_t maxFrameSkip = 5;
    size_t skippedFrames = 0;
    double deltaTime = 0.0;

    lastTime = std::chrono::high_resolution_clock::now();
    while (!mWindow.shouldClose())
    {
        #ifdef VGL_PRINT_FPS
        frameStart = std::chrono::high_resolution_clock::now();
        #endif
        mWindow.pollEvents();

        currentTime = std::chrono::high_resolution_clock::now();
        deltaTime += std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - lastTime).count();
        lastTime = currentTime;
        while (deltaTime > mTimeStep) {
            update(mTimeStep);
            deltaTime -= mTimeStep;

            ++skippedFrames;
            if (skippedFrames >= maxFrameSkip) {
                deltaTime = 0.0;
                break;
            }
        }

        draw();
        skippedFrames = 0;

        mWindow.swapBuffers();

        #ifdef VGL_PRINT_FPS
        ++frames;
        timePassed += std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - frameStart).count();
        if (timePassed >= 1.0) {
            std::cout << "FPS: " << frames << std::endl;
            timePassed = 0.;
            frames = 0;
        }
        #endif
    }
}

double vgl::App::timeStep() const
{
    return mTimeStep;
}

void vgl::App::setTimeStep(double timeStep)
{
    mTimeStep = timeStep;
}

void vgl::App::draw()
{
    glViewport(0, 0, mWindow.width(), mWindow.height());
    mScene.camera().setAspectRatio(static_cast<float>(mWindow.aspectRatio()));
    mScene.update();
    mScene.draw();
}

vgl::LambdaApp::LambdaApp(int x, int y, int width, int height, const std::string& title)
    : App(x, y, width, height, title)
{
}

void vgl::LambdaApp::setUpdateFunc(std::function<void(double)> updateFunc)
{
    mUpdateFunc = updateFunc;
}

void vgl::LambdaApp::update(double deltaTime)
{
    mUpdateFunc(deltaTime);
}

vgl::AsyncApp::AsyncApp(int x, int y, int width, int height, const std::string &title)
    : App(x, y, width, height, title)
{
}

void vgl::AsyncApp::run()
{
    window().releaseGLContext();
    mRenderThread = std::thread(&AsyncApp::renderLoop, this);

    using time_point = std::chrono::high_resolution_clock::time_point;
    
    time_point lastTime, currentTime;
    double deltaTime = 0.0;

    lastTime = std::chrono::high_resolution_clock::now();
    while (!mWindow.shouldClose())
    {
        mWindow.pollEvents();

        currentTime = std::chrono::high_resolution_clock::now();
        deltaTime += std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - lastTime).count();
        lastTime = currentTime;
        if (deltaTime > mTimeStep) {
            update(mTimeStep);
            deltaTime -= mTimeStep;
        }
    }
    mShouldClose = true;
    mRenderThread.join();
}

void vgl::AsyncApp::renderLoop()
{
    window().makeGLContextCurrent();
    while (!mShouldClose.load()) {
        draw();
        mWindow.swapBuffers();
    }
}

vgl::AsyncLambdaApp::AsyncLambdaApp(int x, int y, int width, int height, const std::string &title)
    : AsyncApp(x, y, width, height, title)
{
}

void vgl::AsyncLambdaApp::setUpdateFunc(std::function<void(double)> updateFunc)
{
    mUpdateFunc = updateFunc;
}

void vgl::AsyncLambdaApp::update(double deltaTime)
{
    mUpdateFunc(deltaTime);
}
