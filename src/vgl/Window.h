#pragma once

#include <string>
#ifdef _WIN32 
#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>
#endif


namespace vgl {

void initialize();

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

    int width() const;
    int height() const;

    void setPosition(int x, int y);

    void setWidth(int width);
    void setHeight(int height);
    void setSize(int width, int height);

    void setViewport(int x, int y, int width, int height);

    void setTitle(std::string title);

    void setResizable(bool resizable);

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
    const wchar_t* mWindowClassName = L"VitalGLWindow";
#endif
};

} // namespace vgl
