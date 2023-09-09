#pragma once
#include "IClientEntity.hpp"
class C_BaseEntity;
class IClientEntity;
struct FireBulletsInfo_t
{
	FireBulletsInfo_t()
	{
		m_iShots = 1;
		m_vecSpread = Vector( 0, 0, 0 );
		m_flDistance = 8192;
		m_iTracerFreq = 4;
		m_flDamage = 0;
		m_iPlayerDamage = 0;
		m_pAttacker = NULL;
		m_nFlags = 0;
		m_pAdditionalIgnoreEnt = NULL;
		m_flDamageForceScale = 1.0f;

#ifdef _DEBUG
		m_iAmmoType = -1;
		m_vecSrc = Vector(0, 0, 0);
		m_vecDirShooting = Vector(0, 0, 0);
#endif
		m_bPrimaryAttack = true;
	}

	FireBulletsInfo_t( int nShots, const Vector &vecSrc, const Vector &vecDir, const Vector &vecSpread, float flDistance, int nAmmoType, bool bPrimaryAttack = true )
	{
		m_iShots = nShots;
		m_vecSrc = vecSrc;
		m_vecDirShooting = vecDir;
		m_vecSpread = vecSpread;
		m_flDistance = flDistance;
		m_iAmmoType = nAmmoType;
		m_iTracerFreq = 4;
		m_flDamage = 0;
		m_iPlayerDamage = 0;
		m_pAttacker = NULL;
		m_nFlags = 0;
		m_pAdditionalIgnoreEnt = NULL;
		m_flDamageForceScale = 1.0f;
		m_bPrimaryAttack = bPrimaryAttack;
	}

	int m_iShots;
	Vector m_vecSrc;
	Vector m_vecDirShooting;
	Vector m_vecSpread;
	float m_flDistance;
	int m_iAmmoType;
	int m_iTracerFreq;
	float m_flDamage;
	int m_iPlayerDamage;	// Damage to be used instead of m_flDamage if we hit a player
	int m_nFlags;			// See FireBulletsFlags_t
	float m_flDamageForceScale;
	C_BaseEntity* m_pAttacker;
	C_BaseEntity* m_pAdditionalIgnoreEnt;
	bool m_bPrimaryAttack;
};

enum FireBulletsFlags_t
{
	FIRE_BULLETS_FIRST_SHOT_ACCURATE = 0x1,	// Pop the first shot with perfect accuracy
	FIRE_BULLETS_DONT_HIT_UNDERWATER = 0x2,	// If the shot hits its target underwater, don't damage it
	FIRE_BULLETS_ALLOW_WATER_SURFACE_IMPACTS = 0x4,	// If the shot hits water surface, still call DoImpactEffect
	FIRE_BULLETS_TEMPORARY_DANGER_SOUND = 0x8,	// Danger sounds added from this impact can be stomped immediately if another is queued
	FIRE_BULLETS_IGNORE_PIERCING_SHOTS = 0x10,	// Used to disable the penetration system
	FIRE_BULLETS_FORCE_NO_PIERCE = 0x20,	// Used to disable the penetration system
	FIRE_BULLETS_TEMPORARY_RAGDOLL_IMPACTS = 0x40,	// Ragdolls hit by these bullets will be made temporarily heavy
	FIRE_BULLETS_TEMPORARY_DECAL_IMPACTS = 0x80,	// Decals hit by these bullets will be made temporarily heavy
	FIRE_BULLETS_TEMPORARY_IMPACT_SOUND = 0x100,	// Impact sounds from these bullets will be made temporarily heavy
	FIRE_BULLETS_TEMPORARY_EXIT_DECALS = 0x200,	// Exit decals hit by these bullets will be made temporarily heavy
	FIRE_BULLETS_TEMPORARY_DECALS = (FIRE_BULLETS_TEMPORARY_RAGDOLL_IMPACTS | FIRE_BULLETS_TEMPORARY_DECAL_IMPACTS | FIRE_BULLETS_TEMPORARY_EXIT_DECALS),
	FIRE_BULLETS_ALLOW_WATER_SURFACE_DECALS = 0x400,	// If the shot hits water surface, still call DoImpactEffect
	FIRE_BULLETS_FORCE_LOCAL_COORDS = 0x800,	// Ignores global game state for wind, forces local coordinate system
	FIRE_BULLETS_FORCE_NO_LIFETIME_MODIFICATION = 0x1000,	// Disables lifetime modification for projectiles
	FIRE_BULLETS_FORCE_NO_PIERCE_HEADSHOT = 0x2000,	// Disables
};






class C_BaseEntity : public IClientEntity
{
	// Construction

	//friend class CPrediction;

public:
	virtual							~C_BaseEntity();

	//static C_BaseEntity* CreatePredictedEntityByName(const char* classname, const char* module, int line, bool persist = false);

	// FireBullets uses shared code for prediction.
	virtual void					FireBullets(const FireBulletsInfo_t& info);
	virtual void					ModifyFireBulletsDamage(void* dmgInfo) {}
	//	virtual void					ModifyFireBulletsDamage(CTakeDamageInfo* dmgInfo) {}

	virtual bool					ShouldDrawUnderwaterBulletBubbles();
	virtual bool					ShouldDrawWaterImpacts(void) { return true; }
	virtual bool					HandleShotImpactingWater(const FireBulletsInfo_t& info, const Vector& vecEnd, void* pTraceFilter, Vector* pVecTracerDest);
	//	virtual bool					HandleShotImpactingWater(const FireBulletsInfo_t& info, const Vector& vecEnd, ITraceFilter* pTraceFilter, Vector* pVecTracerDest);

	virtual void* GetBeamTraceFilter(void);
	//	virtual ITraceFilter* GetBeamTraceFilter(void);

	virtual void					DispatchTraceAttack(const void* info, const Vector& vecDir, void* ptr, void* pAccumulator = NULL);
	virtual void					TraceAttack(const void* info, const Vector& vecDir, void* ptr, void* pAccumulator = NULL);
	virtual void					DoImpactEffect(void* tr, int nDamageType);
	virtual void					MakeTracer(const Vector& vecTracerSrc, const void* tr, int iTracerType);
	virtual int						GetTracerAttachment(void);


};