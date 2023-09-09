#pragma once
#include <Windows.h>
#include <polyhook2/Detour/x86Detour.hpp>
#include <memory>
#include "../sdk/CUserCMD.hpp"
#include "../MenuRender/imguiRender.hpp"

class Hook
{
public:
	Hook(uint64_t FNAddress, uint64_t fnCallback)
	{
		this->FNAddress = FNAddress;
		this->fnCallback = fnCallback;
	}

	void Init()
	{
		detour = new PLH::x86Detour(FNAddress, fnCallback, &Orignal);
		detour->hook();
	}

	~Hook()
	{
		detour->unHook();
		delete detour;
	}
	PLH::x86Detour* detour = nullptr;
	uint64_t Orignal;
	uint64_t FNAddress;
	uint64_t fnCallback;
};




namespace H
{
	inline std::unique_ptr<Hook> CreateMove;


	inline std::unique_ptr<Hook> Cl_Move;


	inline std::unique_ptr<Hook> DXEndscne;


	void init();


	void shutdown();

	bool __stdcall CreateMoveHook(float sample_input_frametime, CUserCmd* cmd);


	void __cdecl Cl_MoveHook(float accumulatedExtraSamples, bool FinalTick);

	long __stdcall EndSceneDetour(LPDIRECT3DDEVICE9 pDevice);

}

