#pragma once
#include "../sdk/GlobalVars.hpp"
#include "../sdk/CheatEntityClass.hpp"


namespace G
{
	inline PlayerEntity* LocalPlayer = nullptr;
	inline CGlobalVars* GVars = nullptr;

	inline void Init()
	{
		LocalPlayer = (PlayerEntity*)M::EntityList->GetClientEntity(1);
		GVars = (CGlobalVars*)M::PlayerInfoManager->GetGlobalVars();
	}
}