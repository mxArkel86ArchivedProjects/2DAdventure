//#include <stdafx.h>
#include "imports.h"
#include "application.h"
#include "Peripherals.h"
#include "util.h"

std::wstring ExePath() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}

wstring INFO::APP_PATH = ExePath();

static Application app;
static ID2D1HwndRenderTarget* pRT = NULL;
static ID2D1Factory* pD2DFactory = NULL;
static IDWriteFactory* pDWriteFactory;
IWICImagingFactory *pWICFactory = NULL;

using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter_1;
long tick = 0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;

    // Initialize GDI+.

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("Getting Started"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        WIN_WIDTH,            // initial x size
        WIN_HEIGHT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    TCHAR buff_121[256];

    /*DWORD ret121 = GetModuleFileNameA(NULL, buff_121, 256);

    vector<wstring> args; // #2: Search for tokens
    wstring filepath = wstring(converter_1.from_bytes(string(buff_121, ret121)));
    split(args, filepath, is_any_of("\\"), token_compress_on);
    args.pop_back();
    wstring full = join(args, L"\\");
    INFO::APP_PATH = full;

    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    */
    CoInitialize(NULL);

    
    D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &pD2DFactory
    );

    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&pDWriteFactory)
        );

    HRESULT hr = CoCreateInstance(
    CLSID_WICImagingFactory,
    NULL,
    CLSCTX_INPROC_SERVER,
    IID_PPV_ARGS(&pWICFactory)
);

    // Obtain the size of the drawing area.
    RECT rc;
    GetClientRect(hWnd, &rc);

    // Create a Direct2D render target          
    
    pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hWnd,
            D2D1::SizeU(
                rc.right - rc.left,
                rc.bottom - rc.top)
        ),
        &pRT
    );

    app = Application(hWnd, pRT, pWICFactory, pDWriteFactory);
    app.onResize(rc.right - rc.left, rc.bottom - rc.top);
    app.InitResources();

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    SetTimer(hWnd,
        TIMER1,
        16,
        (TIMERPROC)NULL);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_PAINT:
    {
        app.tick(tick++);
        pRT->BeginDraw();
        app.Paint();
        pRT->EndDraw();
        break;
    }
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);

        pRT->Resize(D2D1::SizeU(width, height));
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);

        app.onResize(width, height);
        break;
    }
    case WM_KEYDOWN:
    {
        Peripherals::KeyPressIn(wParam, true);
        break;
    }
    case WM_KEYUP:
    {
        Peripherals::KeyPressIn(wParam, false);
        break;
    }
    case WM_MOUSEMOVE:
    {
        UINT x = LOWORD(lParam);
        UINT y = HIWORD(lParam);
        Peripherals::MouseMoveIn(x, y);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        UINT x = LOWORD(lParam);
        UINT y = HIWORD(lParam);
        Peripherals::mouseClickLeftIn(x, y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        UINT x = LOWORD(lParam);
        UINT y = HIWORD(lParam);
        Peripherals::mouseClickRightIn(x, y);
        break;
    }
    case WM_TIMER:
    {
        switch (wParam)
        {
        case TIMER1://paint
        {
            //LPRECT WINSIZE = (LPRECT)malloc(sizeof(LPRECT));
            //GetClientRect(hWnd, WINSIZE);
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
        }
        case TIMER2://backend
        {
            app.tick(tick++);
        }
        }
        break;
    }
    case WM_DESTROY:
    {
        SafeRelease(pRT);
        SafeRelease(pDWriteFactory);
        app.DeinitResources();
        SafeRelease(pD2DFactory);
        FreeConsole();

        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
} // WndProc