#include "Hooks.hpp"
#include "../interfaces/Interfaces.hpp"
#include "../Hacks/Movement.hpp"
#include "../Hacks/MainHacks.hpp"
#include "../Mem/Mem.hpp"
#include "../MenuRender/MenuGlobals.hpp"
#include "../Hacks/Visuals.hpp"



#ifdef _WIN64
#define WndProcV GWLP_WNDPROC
#else
#define WndProcV GWL_WNDPROC
#endif // _WIN64



#define D3DDEV9_LEN 119
void* pDeviceTable[D3DDEV9_LEN];



bool GetD3D9Device(void** pTable, size_t Size)
{
	if (!pTable)
		return false;

	Size *= sizeof(void*);

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = NULL;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();
	oWndproc = (WNDPROC)SetWindowLongPtr(d3dpp.hDeviceWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

	HRESULT dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDeviceCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDeviceCreated != S_OK)
		{
			pD3D->Release();
			return false;
		}
	}

	memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), Size);




	pDummyDevice->Release();
	pD3D->Release();
	return true;
}





void H::init()
{

	CreateMove = std::make_unique<Hook>(reinterpret_cast<uint32_t**>(M::ClientMode)[0][21], (uint32_t)&CreateMoveHook);
	CreateMove.get()->Init();

	auto Dbug = M::scanpattern("engine.dll", "55 8B EC 83 EC 34 83");

	Cl_Move = std::make_unique<Hook>((uint32_t)M::scanpattern("engine.dll", "55 8B EC 83 EC 34 83"), (uint32_t)&Cl_MoveHook);
	Cl_Move.get()->Init();

	if (GetD3D9Device((void**)pDeviceTable, D3DDEV9_LEN))
	{
		DXEndscne = std::make_unique<Hook>((uint32_t)pDeviceTable[42], (uint32_t)&EndSceneDetour);
		DXEndscne.get()->Init();
	}

}


void H::shutdown()
{
	CreateMove.get()->~Hook();
	Cl_Move.get()->~Hook();
	DXEndscne.get()->~Hook();
}

int doubletapCharge = 0,
ticksToShift = 0;

#define AmtOfTicksToShift 64

int TicksShiftedManually = 0;
bool curshift = false;
bool isShifting = false;
float shiftTime = 0.0f;
int abc = 0;
float Waittime = 0.0f;
bool __stdcall H::CreateMoveHook(float sample_input_frametime, CUserCmd* cmd)
{

	auto Globals = M::PlayerInfoManager->GetGlobalVars();
	

	auto base = (bool****)((DWORD)_AddressOfReturnAddress() - sizeof(DWORD));
	bool* sendpacket = ***base - 1;
	*sendpacket = true;

	auto a = M::EntityList->GetClientEntity(1);
	auto LP = HA::ReturnPlayerEntity(a);

	if (isShifting) {
		//*sendpacket = ticksToShift == 1; 
		//cmd->buttons &= ~(IN_ATTACK | IN_ATTACK2); 
		Waittime = Globals->curtime + 0.2;
		HA::Movement::Bhop(LP, cmd);
		HA::Movement::rotate_movement(LP, cmd);
		return 0;
	}


	if (Globals->curtime < 5)
	{
		Waittime = Globals->curtime + 0.2f;
		return 0;
	}


	int shiftAmount = MG::MaxTicksToCharge - MG::TicksToRelease > 0 ? MG::TicksToRelease : TicksShiftedManually;
	shiftTime = shiftAmount * Globals->interval_per_tick;

	if ((cmd->buttons & IN_ATTACK) && !isShifting && Waittime < Globals->curtime)
	{
		ticksToShift = MG::MaxTicksToCharge - MG::TicksToRelease > 0 ? MG::TicksToRelease : TicksShiftedManually;
	}

	if (!cmd->command_number)
		return PLH::FnCast(CreateMove.get()->Orignal, H::CreateMoveHook)(sample_input_frametime, cmd);

	PLH::FnCast(CreateMove.get()->Orignal, H::CreateMoveHook)(sample_input_frametime, cmd);

	
	HA::Movement::Bhop(LP,cmd);
	HA::Movement::rotate_movement(LP, cmd);





	//abc++;
	return false;

}


void __cdecl H::Cl_MoveHook(float accumulatedExtraSamples, bool FinalTick)
{
	auto Globals = M::PlayerInfoManager->GetGlobalVars();
	//	if (Globals->tickcount % 2 && GetAsyncKeyState(VK_XBUTTON1))

	if (Globals->tickcount % 2 && GetAsyncKeyState(VK_XBUTTON1) && doubletapCharge <= MG::MaxTicksToCharge)
	{
		doubletapCharge++;
		TicksShiftedManually++;
		Globals->interpolation_amount = 0.0f;
		printf("Doubletap Charge: %d\n", doubletapCharge);
		return;
	}
	
	printf("Ticks Shifted: %d; Shift Time %f\n", TicksShiftedManually, shiftTime);

	PLH::FnCast(Cl_Move.get()->Orignal, H::Cl_MoveHook)(accumulatedExtraSamples, FinalTick);

	isShifting = true;
	{
		for (ticksToShift = min(doubletapCharge, ticksToShift); ticksToShift > 0; ticksToShift--, doubletapCharge--, TicksShiftedManually--)
		{
			curshift = true;
			printf("Ticks Shifted: %d; Shift Time %f\n", TicksShiftedManually, shiftTime);
			PLH::FnCast(Cl_Move.get()->Orignal, H::Cl_MoveHook)(accumulatedExtraSamples, FinalTick);
		}
	}
	isShifting = false;
}

long __stdcall H::EndSceneDetour(LPDIRECT3DDEVICE9 pDevice)
{

	if (!oWndproc)
	{
		D3DDEVICE_CREATION_PARAMETERS d3dpp2 = {};
		pDevice->GetCreationParameters(&d3dpp2);
		pWindow = d3dpp2.hFocusWindow;
		oWndproc = (WNDPROC)SetWindowLongPtr(d3dpp2.hFocusWindow, WndProcV, (LONG_PTR)WndProc);
	}

	rendermenu(pDevice);



	return PLH::FnCast(DXEndscne.get()->Orignal, H::EndSceneDetour)(pDevice);
}
