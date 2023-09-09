#include <windows.h>
#include <iostream>
#include "Interfaces/Interfaces.hpp"
#include "sdk/C_BaseEntity.hpp"
#include "Hooks/Hooks.hpp"
#include "Hacks/Globals.hpp"
#include "Netvar/Netvar.hpp"

void main(HMODULE mod)
{
	FILE* Dummy;
	AllocConsole();
	freopen_s(&Dummy,"CONOUT$", "w", stdout);

	I::init();
	G::Init();
	SetupNetvars();
	H::init();

	while (!GetAsyncKeyState(VK_END))
	{

		
	}

	H::shutdown();
	startshutdown();
	FreeConsole();
	FreeLibraryAndExitThread(mod,0);
}







BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)main,NULL,NULL,NULL);
		break;
	}
	return TRUE;
}
