#pragma once
#include "C_BaseEntity.hpp"
#include "CUserCmd.hpp"



typedef void* EntityHandle_t;


class CMoveData
{
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;

	EntityHandle_t	m_nPlayerHandle;	// edict index on server, client entity handle on client

	int				m_nImpulseCommand;	// Impulse command issued.
	Vector			m_vecViewAngles;	// Command view angles (local space)
	Vector			m_vecAbsViewAngles;	// Command view angles (world space)
	int				m_nButtons;			// Attack buttons.
	int				m_nOldButtons;		// From host_client->oldbuttons;
	float			m_flForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vector			m_vecVelocity;		// edict::velocity		// Current movement direction.
	Vector			m_vecAngles;		// edict::angles
	Vector			m_vecOldAngles;

	// Output only
	float			m_outStepHeight;	// how much you climbed this move
	Vector			m_outWishVel;		// This is where you tried 
	Vector			m_outJumpVel;		// This is your jump velocity

	// Movement constraints	(radius 0 means no constraint)
	Vector			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;


private:
	Vector			m_vecAbsOrigin;		// edict::origin
};




#define INVALID_ENTITY_HANDLE INVALID_EHANDLE_INDEX

//-----------------------------------------------------------------------------
// Functions the engine provides to IGameMovement to assist in its movement.
//-----------------------------------------------------------------------------

class IMoveHelper
{
public:
	// Call this to set the singleton
	static IMoveHelper * GetSingleton() { return sm_pSingleton; }

	// Methods associated with a particular entity
	virtual	char const* GetName(EntityHandle_t handle) const = 0;

	// Adds the trace result to touch list, if contact is not already in list.
	virtual void	ResetTouchList(void) = 0;
	virtual bool	AddToTouched(const void* tr, const Vector& impactvelocity) = 0;
	virtual void	ProcessImpacts(void) = 0;

	// Numbered line printf
	virtual void	Con_NPrintf(int idx, char const* fmt, ...) = 0;

	// These have separate server vs client impementations
	virtual void	StartSound(const Vector& origin, int channel, char const* sample, float volume, int soundlevel, int fFlags, int pitch) = 0;
	virtual void	StartSound(const Vector& origin, const char* soundname) = 0;
	virtual void	PlaybackEventFull(int flags, int clientindex, unsigned short eventindex, float delay, Vector& origin, Vector& angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) = 0;

	// Apply falling damage to m_pHostPlayer based on m_pHostPlayer->m_flFallVelocity.
	virtual bool	PlayerFallingDamage(void) = 0;

	// Apply falling damage to m_pHostPlayer based on m_pHostPlayer->m_flFallVelocity.
	virtual void	PlayerSetAnimation(int playerAnim) = 0;

	virtual void* GetSurfaceProps(void) = 0;

	virtual bool IsWorldEntity(const void* handle) = 0;

protected:
	// Inherited classes can call this to set the singleton
	static void SetSingleton(IMoveHelper* pMoveHelper) { sm_pSingleton = pMoveHelper; }

	// Clients shouldn't call delete directly
	virtual			~IMoveHelper() {}

	// The global instance
	static IMoveHelper* sm_pSingleton;
};




class CPrediction
{
	// Construction
public:

	virtual			~CPrediction(void) = 0;

	virtual void	Init(void) = 0;
	virtual void	Shutdown(void) = 0;

	// Implement IPrediction
public:

	virtual void	Update
	(
		int startframe,		// World update ( un-modded ) most recently received
		bool validframe,		// Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	) = 0;

	virtual void	OnReceivedUncompressedPacket(void) = 0;

	virtual void	PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void	PostEntityPacketReceived(void) = 0;
	virtual void	PostNetworkDataReceived(int commands_acknowledged) = 0;

	virtual bool	InPrediction(void) const = 0;
	virtual bool	IsFirstTimePredicted(void) const = 0;

#if !defined( NO_ENTITY_PREDICTION )
	virtual int		GetIncomingPacketNumber(void) const = 0;
#endif


	// The engine needs to be able to access a few predicted values
	virtual void	GetViewOrigin(Vector& org) = 0;
	virtual void	SetViewOrigin(Vector& org) = 0;
	virtual void	GetViewAngles(Vector& ang) = 0;
	virtual void	SetViewAngles(Vector ang) = 0;

	virtual void	GetLocalViewAngles(Vector& ang) = 0;
	virtual void	SetLocalViewAngles(Vector& ang) = 0;

	virtual void	RunCommand(C_BaseEntity* player, CUserCmd* ucmd, IMoveHelper* moveHelper) = 0;

	// Internal
protected:
	virtual void	SetupMove(C_BaseEntity* player, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move) = 0;
	virtual void	FinishMove(C_BaseEntity* player, CUserCmd* ucmd, CMoveData* move) = 0;
	virtual void	SetIdealPitch(C_BaseEntity* player, const Vector& origin, const Vector& angles, const Vector& viewheight) = 0;

private:
	virtual void	_Update
	(
		bool received_new_world_update,
		bool validframe,		// Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	) = 0;



protected:
	// Last object the player was standing on
	void* m_hLastGround;
public:
	bool			m_bInPrediction;
	bool			m_bFirstTimePredicted;
	bool			m_bOldCLPredictValue;
	bool			m_bEnginePaused;

	// Last network origin for local player
	int				m_nPreviousStartFrame;

	int				m_nCommandsPredicted;
	int				m_nServerCommandsAcknowledged;
	int				m_bPreviousAckHadErrors;
	int				m_nIncomingPacketNumber;

	float			m_flIdealPitch;

};