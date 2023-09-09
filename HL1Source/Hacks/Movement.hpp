#pragma once
#include "../sdk/CUserCMD.hpp"
#include "../sdk/CheatEntityClass.hpp"

namespace HA
{
	namespace Movement
	{
		void Bhop(PlayerEntity* LP,CUserCmd* cmd);
		void rotate_movement(PlayerEntity* pLocal, CUserCmd* pCommand);
	}
}