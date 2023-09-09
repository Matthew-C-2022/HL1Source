#pragma once
#include "GlobalVars.hpp"

class CPlayerInfoManager
{
public:
	virtual void* GetPlayerInfo(void* pEdict) = 0;
	virtual CGlobalVars* GetGlobalVars() = 0;
};