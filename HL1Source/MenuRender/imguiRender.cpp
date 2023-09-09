#include "imguiRender.hpp"
#include <map>
#include "../Hacks/Visuals.hpp"
#include "../Hacks/MainHacks.hpp"
#include "MenuGlobals.hpp"
WNDPROC oWndproc;
HWND pWindow = NULL;
bool init = false;
bool finishedshutdown = false;
bool shutdownIMG = false;


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(const HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	return CallWindowProc(oWndproc, hwnd, msg, wParam, lParam);
}


void menurender();


BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	pWindow = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	pWindow = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return pWindow;
}

void rendermenu(LPDIRECT3DDEVICE9 a1)
{

	if (shutdownIMG)
	{
		if(!finishedshutdown)
			shutdownImGui();
		return;
	}

	if (!init)
	{

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplWin32_Init(pWindow);
		ImGui_ImplDX9_Init(a1);
		init = true;
	}


	menurender();


	return;

}




void menurender()
{


	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();

	ImGui::NewFrame();
	ImGui::Begin("Test Menu", NULL);
	HA::Visual::UpdatePlayerTrail(HA::ReturnPlayerEntity(M::EntityList->GetClientEntity(M::Engine->GetLocalPlayer())));
	HA::Visual::RenderPlayerTrail(HA::ReturnPlayerEntity(M::EntityList->GetClientEntity(M::Engine->GetLocalPlayer())));
	ImGui::SliderInt("Max Ticks To Charge", &MG::MaxTicksToCharge, 1, 5000);
	ImGui::SliderInt("Amt of Ticks To Release", &MG::TicksToRelease, 1, MG::MaxTicksToCharge);
	/*
	for (size_t i = 0; i < 60; i++)
	{
		auto Curent = M::EntityList->GetClientEntity(i);
		if (!Curent)
			continue;
		auto dormant = Curent->IsDormant();
		auto Health = HA::ReturnPlayerEntity(Curent)->GetHealth();
		auto lifestate = HA::ReturnPlayerEntity(Curent)->GetLifestate();
		if (!dormant && lifestate == 0 || i == 1)
			HA::Visual::RenderBones(HA::ReturnPlayerEntity(Curent));
	}
	*/
	ImGui::Text("Test");
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}



void shutdownImGui()
{

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX9_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(pWindow, GWLP_WNDPROC, (LONG_PTR)oWndproc);

	finishedshutdown = true;
}

void startshutdown()
{
	shutdownIMG = true;
}