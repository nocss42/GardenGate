#pragma once
#include <windows.h>

#include "imgui.h"
#include "imgui_internal.h"

class UI
{
public:
    static void DrawUI(HWND hwnd, float dpiScale);
};
