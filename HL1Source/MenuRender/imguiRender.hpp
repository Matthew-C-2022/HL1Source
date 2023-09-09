#pragma once
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../ImGui/imgui_impl_win32.h"
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")


extern WNDPROC oWndproc;
extern HWND pWindow;
extern bool init;
extern bool shutdownIMG;
HWND GetProcessWindow();
BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);

LRESULT CALLBACK WndProc(const HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void startshutdown();
void shutdownImGui();

void rendermenu(LPDIRECT3DDEVICE9 a1);