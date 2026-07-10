#pragma once
#include <windows.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Renderer
{
private:
    static ID3D11Device* pd3dDevice;
    static ID3D11DeviceContext* pd3dDeviceContext;
    static IDXGISwapChain* pSwapChain;
    static ID3D11RenderTargetView* pMainRenderTargetView;

    static bool CreateDeviceD3D(HWND hWnd);
    static void CleanupDeviceD3D();
    static void CreateRenderTarget();
    static void CleanupRenderTarget();
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    static HMODULE hCurrentModule;

    static void Run();
};
