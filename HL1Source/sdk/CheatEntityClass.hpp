#pragma once
#include "../Netvar/Netvar.hpp"
#include "../sdk/C_BaseEntity.hpp"

#define	LIFE_ALIVE				0 // alive
#define	LIFE_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	LIFE_DEAD				2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4


class PlayerEntity
{
public:

	NETVAR(Flags, "CBasePlayer->m_fFlags", int);
	NETVAR(GetVelocity, "CBasePlayer->m_vecVelocity[0]", Vector);
	NETVAR(GetOrigin, "CBaseEntity->m_vecOrigin", Vector);
	NETVAR(GetHealth, "CBasePlayer->m_iHealth", int);
	NETVAR(GetLifestate,"CBasePlayer->m_lifeState",int);

	inline C_BaseEntity* ReturnBaseEntity()
	{
		return (C_BaseEntity*)this;
	}


	inline Vector GetBonePos(int bone)
	{
		auto ICEnt = (IClientEntity*)this;
		ViewMatrix BoneMatrix[128];
		ICEnt->SetupBones(BoneMatrix, 128, 0x100, M::Engine->GetLastTimeStamp());
		ViewMatrix HitboxMatrix = BoneMatrix[bone];
		return Vector(HitboxMatrix[0][3], HitboxMatrix[1][3], HitboxMatrix[2][3]);
	}


};
