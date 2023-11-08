#include "Window.h"

#include <map>
#include <iostream>


namespace vgl::internal {

bool _defaultShow = true;
bool _defaultResizable = true;


#ifdef _WIN32
    HINSTANCE _instanceHandle;
    std::map<HWND, vgl::Window*> _windowMap;
#elif __unix__
#endif

} // namespace vgl


void vgl::initialize()
{
    #ifdef _WIN32
    internal::_instanceHandle = GetModuleHandle(nullptr);
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
namespace vgl::internal {

LRESULT CALLBACK windowMsgCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        internal::_windowMap[hWnd]->shouldClose();
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void printLastError() {
    LPSTR msgBuffer;
    DWORD errorIndex = GetLastError();
    if (errorIndex == 0) {
        return;
    }

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errorIndex,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&msgBuffer,
        0,
        nullptr
    );

    // TODO: configurable output stream
    std::cout << msgBuffer << std::endl;
    LocalFree(msgBuffer);
}

#define PRINT_ERROR_HEADER std::cout << "[ERROR] " << __FILE__ << " (" << __LINE__ << "):  " << __func__ << std::endl;
#define ASSERT_SUCCESS(cmd) if (!cmd) { PRINT_ERROR_HEADER; internal::printLastError(); std::abort(); }
#define ASSERT_NOT_NULL(val) if (val == NULL) { PRINT_ERROR_HEADER; internal::printLastError(); std::abort(); }

} // namespace vgl::internal
#endif


vgl::Window::Window(int x, int y, int width, int height, std::string title)
 : mX(x), mY(y), mWidth(width), mHeight(height), mResizable(internal::_defaultResizable)
{
    #ifdef _WIN32
    WNDCLASS wc = { };
    wc.lpszClassName = mWindowClassName;
    wc.hInstance = internal::_instanceHandle;
    wc.hIcon = static_cast<HICON>(LoadImage(internal::_instanceHandle, L"icon/vgl.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));
    if (!wc.hIcon) {
        internal::printLastError();
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        ASSERT_NOT_NULL(wc.hIcon);
    }
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    ASSERT_NOT_NULL(wc.hCursor);
    wc.lpfnWndProc = internal::windowMsgCallback;
    ASSERT_NOT_NULL( RegisterClass(&wc) );

    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | (internal::_defaultResizable ? WS_SIZEBOX : 0);
    RECT rect = { x, y, x + width, y + height };
    ASSERT_SUCCESS( AdjustWindowRect(&rect, style, FALSE) );

    // fix: Der an einen Systemaufruf ³bergebene Datenbereich ist zu klein.
    mTitle.resize(title.size(), L' ');
    ASSERT_NOT_NULL( MultiByteToWideChar(CP_UTF8, 0, title.c_str(), static_cast<int>(title.size()), &mTitle[0], static_cast<int>(mTitle.size())) );

    mWindowHandle = CreateWindowEx(
        0,
        mWindowClassName,
        mTitle.c_str(),
        style,
        rect.left, rect.top,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr,
        nullptr,
        internal::_instanceHandle,
        nullptr
    );
    ASSERT_NOT_NULL( mWindowHandle );
    internal::_windowMap[mWindowHandle] = this;
    ShowWindow(mWindowHandle, internal::_defaultShow ? SW_SHOW : SW_HIDE);
    #elif __unix__
    // TODO: implement
    #endif
}

vgl::Window::~Window()
{
    mShouldClose = true;
    #ifdef _WIN32
    internal::_windowMap.erase(mWindowHandle);
    ASSERT_SUCCESS( DestroyWindow(mWindowHandle) );
    ASSERT_SUCCESS( UnregisterClass(mWindowClassName, internal::_instanceHandle) );
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
    ASSERT_SUCCESS( AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE) );
    ASSERT_SUCCESS( SetWindowPos(mWindowHandle, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER) );
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::setTitle(std::string title)
{
    #ifdef _WIN32
    mTitle.resize(title.size());
    ASSERT_NOT_NULL( MultiByteToWideChar(CP_UTF8, 0, title.c_str(), static_cast<int>(title.size()), &mTitle[0], static_cast<int>(mTitle.size())) );
    ASSERT_SUCCESS( SetWindowText(mWindowHandle, mTitle.c_str()) );
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::setResizable(bool resizable)
{
    mResizable = resizable;
    ASSERT_NOT_NULL( SetWindowLong(mWindowHandle, GWL_STYLE, GetWindowLong(mWindowHandle, GWL_STYLE) & ~WS_SIZEBOX | (mResizable ? WS_SIZEBOX : 0)) );
}
