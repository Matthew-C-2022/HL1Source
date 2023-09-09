#pragma once
#include "../sdk/CheatEntityClass.hpp"
#include "../sdk/IClientEntity.hpp"

namespace HA
{
	template<typename T = IClientEntity*>
	inline PlayerEntity* ReturnPlayerEntity(T* Ent)
	{
			return (PlayerEntity*)Ent;
	}
}