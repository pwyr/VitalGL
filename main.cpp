#include <iostream>

#include <vgl/Window.h>

//TODO: remove file (for testing purposes only)

int main() {
    vgl::initialize();
    vgl::setOption(vgl::Option::DefaultShowWindow, true);
    vgl::setOption(vgl::Option::DefaultResizableWindow, true);

    vgl::Window w(100, 100, 800, 600, "VitalGL");
    w.setTitle("VitalGL 2.0");
    w.setViewport(200, 200, 400, 300);
    w.setResizable(true);

    while (!w.shouldClose()) {
        w.pollEvents();
    }
    
    return 0;
}