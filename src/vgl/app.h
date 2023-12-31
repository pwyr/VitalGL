#pragma once

#include <thread>
#include <functional>
#include <vgl/window.h>
#include <vgl/gl.h>
#include <vgl/renderer.h>

namespace vgl{

class App {
public:
    App(int x, int y, int width, int height, const std::string& title);

    Scene& scene();
    Window& window();

    [[noreturn]] virtual void run();

    double timeStep() const;
    void setTimeStep(double timeStep);

    virtual void update(double deltaTime) = 0;
    virtual void draw();

protected:
    double mTimeStep = 1.0 / 60.0;

    Window mWindow;
    Scene mScene;
};


class LambdaApp : public App {
public:
    LambdaApp(int x, int y, int width, int height, const std::string& title);

    void setUpdateFunc(std::function<void(double)> updateFunc);

    void update(double deltaTime) override;
private:
    std::function<void(double)> mUpdateFunc = [](double) {};
};

class AsyncApp : public App {
public:
    AsyncApp(int x, int y, int width, int height, const std::string& title);

    [[noreturn]] void run() override;
    [[noreturn]] void renderLoop();

    virtual void update(double deltaTime) = 0;

protected:
    std::thread mRenderThread;
    std::atomic<bool> mShouldClose = false;
};

class AsyncLambdaApp : public AsyncApp {
public:
    AsyncLambdaApp(int x, int y, int width, int height, const std::string& title);

    void setUpdateFunc(std::function<void(double)> updateFunc);

    void update(double deltaTime) override;
private:
    std::function<void(double)> mUpdateFunc = [](double) {};
};

} // namespace vgl