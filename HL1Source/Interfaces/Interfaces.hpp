#pragma once
#include "../sdk/CEntitylist.hpp"
#include "../sdk/IBaseClient.hpp"
#include "../sdk/ClientMode.hpp"
#include "../sdk/IVEngineClient.hpp"
#include "../sdk/PlayerInfoManager.hpp"
#include "../sdk/IVDebugOverlay.hpp"
namespace M
{
	inline IClientEntityList* EntityList;
	inline IBaseClientDLL* Client;
	inline IClientMode* ClientMode;
	inline IVEngineClient013* Engine;
	inline CPlayerInfoManager* PlayerInfoManager;
	inline IVDebugOverlay* DebugOverlay;
}


namespace I
{
	template<typename T>
	inline T* GetInterface(const char* mod, const char* name)
	{
		typedef T* (*CreateInterfaceFn)(const char* name, int* ret);
		CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(mod), "CreateInterface");

		return CreateInterface(name, 0);
	}


	inline void init()
	{
		M::EntityList = I::GetInterface<IClientEntityList>("client.dll", "VClientEntityList003");
		M::Client = I::GetInterface<IBaseClientDLL>("client.dll", "VClient017");
		M::ClientMode = **(IClientMode***)((*(uintptr_t**)M::Client)[10] + 0x5);
		M::Engine = I::GetInterface<IVEngineClient013>("engine.dll", "VEngineClient013");
		M::PlayerInfoManager = I::GetInterface<CPlayerInfoManager>("server.dll", "PlayerInfoManager002");
		M::DebugOverlay = I::GetInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay003");
	}

}


