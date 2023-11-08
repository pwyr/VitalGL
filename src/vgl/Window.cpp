#include "Window.h"

namespace vgl::internal {

bool _defaultShow = true;
bool _defaultResizable = true;

#ifdef _WIN32
    HINSTANCE instanceHandle;
#elif __unix__
#endif

} // namespace vgl


void vgl::initialize()
{
    #ifdef _WIN32
    internal::instanceHandle = GetModuleHandle(nullptr);
    #elif __unix__
    #endif
}

void vgl::setOption(Option option, bool value)
{
    switch (option) {
    case Option::DefaultShowWindow:
        internal::_defaultShow = value;
        break;
    case Option::DefaultResizableWindow:
        internal::_defaultResizable = value;
        break;
    }
}

#ifdef _WIN32
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        DestroyWindow(hWnd);
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
#endif


vgl::Window::Window(int x, int y, int width, int height, std::string title)
 : mX(x), mY(y), mWidth(width), mHeight(height), mResizable(internal::_defaultResizable)
{
    #ifdef _WIN32
    WNDCLASS wc = { };
    wc.lpszClassName = mWindowClassName;
    wc.hInstance = internal::instanceHandle;
    wc.hIcon = (HICON)LoadImage(internal::instanceHandle, L"icon/vgl.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    // wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpfnWndProc = WndProc;
    RegisterClass(&wc);

    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | (internal::_defaultResizable ? WS_SIZEBOX : 0);
    RECT rect = { x, y, x + width, y + height };
    AdjustWindowRect(&rect, style, FALSE);

    mTitle.resize(title.size());
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &mTitle[0], static_cast<int>(mTitle.size()));

    mWindowHandle = CreateWindowEx(
        0,
        mWindowClassName,
        mTitle.c_str(),
        style,
        rect.left, rect.top,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr,
        nullptr,
        internal::instanceHandle,
        nullptr
    );
    ShowWindow(mWindowHandle, internal::_defaultShow ? SW_SHOW : SW_HIDE);
    #elif __unix__
    // TODO: implement
    #endif
}

vgl::Window::~Window()
{
    #ifdef _WIN32
    UnregisterClass(mWindowClassName, internal::instanceHandle);
    DestroyWindow(mWindowHandle);
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::hide() const
{
    #ifdef _WIN32
    ShowWindow(mWindowHandle, SW_HIDE);
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::show() const
{
    #ifdef _WIN32
    ShowWindow(mWindowHandle, SW_SHOW);
    #elif __unix__
    // TODO: implement
    #endif
}

bool vgl::Window::shouldClose() const
{
    return mShouldClose;
}

void vgl::Window::pollEvents()
{
    #ifdef _WIN32
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            mShouldClose = true;
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    #elif __unix__
    // TODO: implement
    #endif
}

int vgl::Window::width() const
{
    return mWidth;
}

int vgl::Window::height() const
{
    return mHeight;
}

void vgl::Window::setPosition(int x, int y)
{
    setViewport(x, y, mWidth, mHeight);
}

void vgl::Window::setWidth(int width)
{
    setViewport(mX, mY, width, mHeight);
}

void vgl::Window::setHeight(int height)
{
    setViewport(mX, mY, mWidth, height);
}

void vgl::Window::setSize(int width, int height)
{
    setViewport(mX, mY, width, height);
}

void vgl::Window::setViewport(int x, int y, int width, int height)
{
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
    #ifdef _WIN32
    RECT rect = { x, y, x + width, y + height };
    AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
    SetWindowPos(mWindowHandle, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::setTitle(std::string title)
{
    #ifdef _WIN32
    mTitle.resize(title.size());
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &mTitle[0], static_cast<int>(mTitle.size()));
    SetWindowText(mWindowHandle, mTitle.c_str());
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::setResizable(bool resizable)
{
    mResizable = resizable;
    SetWindowLong(mWindowHandle, GWL_STYLE, GetWindowLong(mWindowHandle, GWL_STYLE) & ~WS_SIZEBOX | (mResizable ? WS_SIZEBOX : 0) );
}
