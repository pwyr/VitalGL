#pragma once

#include <string>
#ifdef _WIN32 
#ifndef UNICODE
#define UNICODE
#endif
// TODO: avoid including Windows.h here
#include <windows.h>
#endif


namespace vgl {

void initialize();
void* getProcAddress(const char* name);

enum class Option {
    DefaultShowWindow,
    DefaultResizableWindow,
};

void setOption(Option option, bool value);


class Window
{
public:
    Window(int x, int y, int width, int height, std::string title = "");
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window();

    void hide() const;
    void show() const;

    bool shouldClose() const;

    void pollEvents();

    int x() const;
    int y() const;
    int width() const;
    int height() const;

    void setPosition(int x, int y);

    void setWidth(int width);
    void setHeight(int height);
    void setSize(int width, int height);

    void setViewport(int x, int y, int width, int height);

    void setTitle(std::string title);

    void setResizable(bool resizable);

    void makeGLContextCurrent() const;
    void releaseGLContext() const;

    void swapBuffers() const;

private:
    void setupRenderingContext();

private:
    int mX;
    int mY;
    int mWidth;
    int mHeight;

    bool mResizable = true;
    bool mShouldClose = false;

#ifdef _WIN32
    std::wstring mTitle;
    HWND mWindowHandle;
    HGLRC mRenderingContext;
    const wchar_t* mWindowClassName = L"VitalGLWindow";
#endif
};

} // namespace vgl
