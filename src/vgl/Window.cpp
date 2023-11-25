#include "window.h"

#include <map>
#include <iostream>

#include <vgl/gl.h>


// ------------------------------------------------------------------------------
// [Windows] - Error handling
// ------------------------------------------------------------------------------
#ifdef _WIN32
namespace vgl::internal {

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
#define ENSURE_SUCCESS(cmd) if (!cmd) { PRINT_ERROR_HEADER; internal::printLastError(); std::abort(); }
#define ENSURE_NOT_NULL(val) if (val == NULL) { PRINT_ERROR_HEADER; internal::printLastError(); std::abort(); }

#define PRINT_PROC_NOT_FOUND_ERROR(proc) std::cout << "[ERROR] " << __FILE__ << "(" << __LINE__ << "):  " << __func__ << "(\"" << proc << "\")" << std::endl;
#define ENSURE_PROC_FOUND(val, proc) if (val == NULL) { PRINT_PROC_NOT_FOUND_ERROR(proc); internal::printLastError(); std::abort(); }

} // namespace vgl::internal
#endif


// ------------------------------------------------------------------------------
// [OpenGL] - Error handling
// ------------------------------------------------------------------------------

#ifdef VGL_OPENGL_DEBUG_MODE

void APIENTRY openglDebugMessageCallback(   GLenum source,
                                            GLenum type,
                                            GLuint id,
                                            GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* userParam) {
	std::cout << "---------------------opengl-callback-start------------" << std::endl;
	std::cout << "message: "<< message << std::endl;
	std::cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER";
		break;
	}
	std::cout << std::endl;

	std::cout << "id: " << id << std::endl;
	std::cout << "severity: ";
	switch (severity){
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "NOTIFICATION";
        break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "HIGH";
		break;
	}
	std::cout << std::endl;
	std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

#endif

// ------------------------------------------------------------------------------
// Window management state with static storage duration
// ------------------------------------------------------------------------------
namespace vgl::internal {

bool _defaultShow = true;
bool _defaultResizable = true;


#ifdef _WIN32
    HINSTANCE _instanceHandle;
    std::map<HWND, vgl::Window*> _windowMap;
#elif __unix__
#endif

} // namespace vgl


// ------------------------------------------------------------------------------
// General functions
// ------------------------------------------------------------------------------
void vgl::initialize()
{
    #ifdef _WIN32
    internal::_instanceHandle = GetModuleHandle(nullptr);
    #elif __unix__
    #endif
}

void* vgl::getProcAddress(const char* name)
{
    #ifdef _WIN32

    HMODULE module = LoadLibraryA("opengl32.dll");
    ENSURE_NOT_NULL( module );

    void* procAddress = reinterpret_cast<void*>(GetProcAddress(module, name));

    // load OpenGL extension function
    if (procAddress == nullptr) {
        SetLastError(0);
        procAddress = reinterpret_cast<void*>(wglGetProcAddress(name));
        ENSURE_PROC_FOUND(procAddress, name);
    }
    return procAddress;

    #elif __unix__
    // TODO: implement
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


// ------------------------------------------------------------------------------
// Window implementation
// ------------------------------------------------------------------------------
#ifdef _WIN32
namespace vgl::internal {

LRESULT CALLBACK windowMsgCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE: 
        {
            internal::_windowMap[hWnd]->setShouldClose(true);
            return 0;
        }
    case WM_MOVE:
        {
            internal::_windowMap[hWnd]->mX = LOWORD(lParam);
            internal::_windowMap[hWnd]->mY = HIWORD(lParam);
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    case WM_SIZE:
        {
            internal::_windowMap[hWnd]->mWidth = LOWORD(lParam);
            internal::_windowMap[hWnd]->mHeight = HIWORD(lParam);
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
    
}
} // namespace vgl::internal
#endif


vgl::Window::Window(int x, int y, int width, int height, std::string title)
 : mX(x), mY(y), mWidth(width), mHeight(height), mResizable(internal::_defaultResizable)
{
    #ifdef _WIN32
    WNDCLASSEX wc = { };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszClassName = mWindowClassName;
    wc.hInstance = internal::_instanceHandle;
    // wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = static_cast<HICON>(LoadImage(internal::_instanceHandle, L"icon/vgl.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));
    if (!wc.hIcon) {
        internal::printLastError();
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        ENSURE_NOT_NULL(wc.hIcon);
    }
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    ENSURE_NOT_NULL(wc.hCursor);
    wc.lpfnWndProc = internal::windowMsgCallback;
    ENSURE_NOT_NULL( RegisterClassEx(&wc) );

    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | (internal::_defaultResizable ? WS_SIZEBOX : 0);
    RECT rect = { x, y, x + width, y + height };
    ENSURE_SUCCESS( AdjustWindowRect(&rect, style, FALSE) );

    mTitle.resize(title.size(), L' ');
    ENSURE_NOT_NULL( MultiByteToWideChar(CP_UTF8, 0, title.c_str(), static_cast<int>(title.size()), &mTitle[0], static_cast<int>(mTitle.size())) );

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
    ENSURE_NOT_NULL( mWindowHandle );
    internal::_windowMap[mWindowHandle] = this;
    ShowWindow(mWindowHandle, internal::_defaultShow ? SW_SHOW : SW_HIDE);

    #elif __unix__
    // TODO: implement
    #endif

    setupRenderingContext();
    loadGLFunctions(getProcAddress);
    setupOpenGLDebugCallback();
}

void vgl::Window::setupRenderingContext()
{
    #ifdef _WIN32
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    HDC deviceContext = GetDC(mWindowHandle);
    ENSURE_NOT_NULL( deviceContext );

    int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
    ENSURE_NOT_NULL( pixelFormat );
    ENSURE_SUCCESS( SetPixelFormat(deviceContext, pixelFormat, &pfd) );

    mRenderingContext = wglCreateContext(deviceContext);
    ENSURE_NOT_NULL( mRenderingContext );

    makeGLContextCurrent();

    #define WGL_CONTEXT_MAJOR_VERSION_ARB               0x2091
    #define WGL_CONTEXT_MINOR_VERSION_ARB               0x2092
    #define WGL_CONTEXT_LAYER_PLANE_ARB                 0x2093
    #define WGL_CONTEXT_FLAGS_ARB                       0x2094
    #define WGL_CONTEXT_PROFILE_MASK_ARB                0x9126

    #define WGL_CONTEXT_DEBUG_BIT_ARB                   0x0001
    #define  WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB     0x0002

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,   // Set the MAJOR version of OpenGL to 3
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,   // Set the MINOR version of OpenGL to 2
        WGL_CONTEXT_FLAGS_ARB,
        #ifdef VGL_OPENGL_DEBUG_MODE
        WGL_CONTEXT_DEBUG_BIT_ARB |              // Set our OpenGL context to be forward compatible
        #endif
        WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
        0
    };
    using createContext_t = HGLRC(WINAPI*)(HDC, HGLRC, const int*);

    createContext_t wglCreateContextAttribsARB = reinterpret_cast<createContext_t>(getProcAddress("wglCreateContextAttribsARB"));
    HGLRC newContext = wglCreateContextAttribsARB(deviceContext, nullptr, attribs);
    ENSURE_NOT_NULL( newContext );

    ENSURE_SUCCESS( wglMakeCurrent(deviceContext, nullptr) );
    ENSURE_SUCCESS( wglDeleteContext(mRenderingContext) );

    mRenderingContext = newContext;
    ENSURE_SUCCESS( wglMakeCurrent(deviceContext, mRenderingContext) );

    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::setupOpenGLDebugCallback()
{
    #ifdef VGL_OPENGL_DEBUG_MODE

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglDebugMessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    // TODO: remove or add again
    // glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);

    #endif
}

void vgl::Window::makeGLContextCurrent() const
{
    #ifdef _WIN32
    HDC deviceContext = GetDC(mWindowHandle);
    ENSURE_NOT_NULL( deviceContext );
    ENSURE_SUCCESS( wglMakeCurrent(deviceContext, mRenderingContext) );
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::releaseGLContext() const
{
    #ifdef _WIN32
    HDC deviceContext = GetDC(mWindowHandle);
    ENSURE_NOT_NULL( deviceContext );
    ENSURE_SUCCESS( wglMakeCurrent(deviceContext, nullptr) );
    #elif __unix__
    // TODO: implement
    #endif
}

vgl::Window::~Window()
{
    mShouldClose = true;
    #ifdef _WIN32
    internal::_windowMap.erase(mWindowHandle);
    ENSURE_SUCCESS( DestroyWindow(mWindowHandle) );
    ENSURE_SUCCESS( UnregisterClass(mWindowClassName, internal::_instanceHandle) );
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

void vgl::Window::setShouldClose(bool shouldClose)
{
    mShouldClose = shouldClose;
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
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    #elif __unix__
    // TODO: implement
    #endif
}

int vgl::Window::x() const
{
    return mX;
}

int vgl::Window::y() const
{
    return mY;
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
    ENSURE_SUCCESS( AdjustWindowRect(&rect, GetWindowLong(mWindowHandle, GWL_STYLE), FALSE) );
    ENSURE_SUCCESS( MoveWindow(mWindowHandle, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE) );
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::setTitle(std::string title)
{
    #ifdef _WIN32
    mTitle.resize(title.size());
    ENSURE_NOT_NULL( MultiByteToWideChar(CP_UTF8, 0, title.c_str(), static_cast<int>(title.size()), &mTitle[0], static_cast<int>(mTitle.size())) );
    ENSURE_SUCCESS( SetWindowText(mWindowHandle, mTitle.c_str()) );
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::setResizable(bool resizable)
{
    #ifdef _WIN32
    mResizable = resizable;
    ENSURE_NOT_NULL( SetWindowLong(mWindowHandle, GWL_STYLE, GetWindowLong(mWindowHandle, GWL_STYLE) & ~WS_SIZEBOX | (mResizable ? WS_SIZEBOX : 0)) );
    #elif __unix__
    // TODO: implement
    #endif
}

void vgl::Window::swapBuffers() const
{
    #ifdef _WIN32
    HDC deviceContext = GetDC(mWindowHandle);
    ENSURE_NOT_NULL( deviceContext );
    ENSURE_SUCCESS( SwapBuffers(deviceContext) );
    #elif __unix__
    // TODO: implement
    #endif
}