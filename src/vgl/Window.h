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
    DefaultVSync,
};

void setOption(Option option, bool value);


#ifdef _WIN32
namespace internal {
LRESULT CALLBACK windowMsgCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}
#endif


class Window
{
public:
    Window(int x, int y, int width, int height, std::string title = "");
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window();

    void hide() const;
    void show() const;

    void setShouldClose(bool shouldClose);
    bool shouldClose() const;

    void enableVSync();
    void disableVSync();

    void pollEvents();

    int x() const;
    int y() const;
    int width() const;
    int height() const;

    double aspectRatio() const;

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
#ifdef _WIN32
    friend LRESULT vgl::internal::windowMsgCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

private:
    void setupRenderingContext();
    void setupOpenGLDebugCallback();

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
    HDC mDeviceContext;
    HGLRC mRenderingContext;
    const wchar_t* mWindowClassName = L"VitalGLWindow";
#endif
};

} // namespace vgl
