#include "Renderer.hpp"
#include "Ui.hpp"
#include "Config.hpp"
#include "Utils.hpp"
#include <windowsx.h>

ID3D11Device* Renderer::pd3dDevice = nullptr;
ID3D11DeviceContext* Renderer::pd3dDeviceContext = nullptr;
IDXGISwapChain* Renderer::pSwapChain = nullptr;
ID3D11RenderTargetView* Renderer::pMainRenderTargetView = nullptr;
HMODULE Renderer::hCurrentModule = nullptr;

bool Renderer::CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevelArray, 2, D3D11_SDK_VERSION, &sd,
            &pSwapChain, &pd3dDevice, &featureLevel, &pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void Renderer::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer)
    {
        pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pMainRenderTargetView);
        pBackBuffer->Release();
    }
}

void Renderer::CleanupRenderTarget()
{
    if (pMainRenderTargetView)
    {
        pMainRenderTargetView->Release();
        pMainRenderTargetView = nullptr;
    }
}

void Renderer::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (pSwapChain)
    {
        pSwapChain->Release();
        pSwapChain = nullptr;
    }
    if (pd3dDeviceContext)
    {
        pd3dDeviceContext->Release();
        pd3dDeviceContext = nullptr;
    }
    if (pd3dDevice)
    {
        pd3dDevice->Release();
        pd3dDevice = nullptr;
    }
}

LRESULT WINAPI Renderer::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;

        // shitty solution, will work on a better implementation.
    case WM_NCHITTEST: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

        RECT rcWindow;
        ::GetWindowRect(hWnd, &rcWindow);

        const int borderX = 8;
        const int borderY = 8;

        bool onLeft = pt.x >= rcWindow.left && pt.x < rcWindow.left + borderX;
        bool onRight = pt.x < rcWindow.right && pt.x >= rcWindow.right - borderX;
        bool onTop = pt.y >= rcWindow.top && pt.y < rcWindow.top + borderY;
        bool onBottom = pt.y < rcWindow.bottom && pt.y >= rcWindow.bottom - borderY;

        if (onTop && onLeft)
            return HTTOPLEFT;
        if (onTop && onRight)
            return HTTOPRIGHT;
        if (onBottom && onLeft)
            return HTBOTTOMLEFT;
        if (onBottom && onRight)
            return HTBOTTOMRIGHT;
        if (onTop)
            return HTTOP;
        if (onBottom)
            return HTBOTTOM;
        if (onLeft)
            return HTLEFT;
        if (onRight)
            return HTRIGHT;

        break;
    }

    case WM_NCCALCSIZE:
        if (wParam == TRUE)
            return 0;
        break;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void Renderer::Run()
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    const int windowWidth = 770;
    const int windowHeight = 680;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int xPos = (screenWidth - windowWidth) / 2;
    int yPos = (screenHeight - windowHeight) / 2;

    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
        "GardenGateClass", nullptr };
    ::RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "GardenGate Launcher", WS_OVERLAPPEDWINDOW, xPos, yPos, windowWidth, windowHeight, NULL,
        NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
    io.IniFilename = nullptr;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);

    float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(hwnd);

    io.Fonts->Clear();
    float fontSize = 16.0f * dpiScale;
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", fontSize);
    if (!font)
        io.Fonts->AddFontDefault();
    ImGui_ImplDX11_CreateDeviceObjects();

    ImGui::StyleColorsDark();
    if (dpiScale > 1.0f)
        ImGui::GetStyle().ScaleAllSizes(dpiScale);

    bool bDone = false;
    while (!bDone)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                bDone = true;
        }
        if (bDone)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        UI::DrawUI(hwnd, dpiScale);

        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.10f, 0.10f, 0.10f, 1.0f };
        pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
        pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, clear_color_with_alpha);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        pSwapChain->Present(1, 0);
    }

    Utils::Process::RestoreEAArgs();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

}
