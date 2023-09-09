#include "Movement.hpp"
#include <algorithm>
#define deg2rad(x)  ( (float)(x) * (float)(3.14159265358979323846f / 180.f) )
#define rad2deg(x)  ( (float)(x) * (float)(180.f / 3.14159265358979323846f) )

void HA::Movement::Bhop(PlayerEntity* LP, CUserCmd* cmd)
{
	if (cmd->buttons & IN_JUMP && !(LP->Flags() & FL_ONGROUND))
	{
		cmd->buttons &= ~IN_JUMP;
	}
}


float get_delta(float hspeed, float maxspeed, float airaccelerate)
{
	auto term = (30.0 - (airaccelerate * maxspeed / 66.0)) / hspeed;

	if (term < 1.0f && term > -1.0f) {
		return acos(term);
	}

	return 0.f;
}



void HA::Movement::rotate_movement(PlayerEntity* pLocal, CUserCmd* pCommand)
{
	if ((pLocal->Flags() & FL_ONGROUND))
		return;

	float speed = pLocal->GetVelocity().Length2D();
	Vector velocity = pLocal->GetVelocity();
	float yawVelocity = rad2deg(atan2(velocity.y, velocity.x));
	float velocityDelta = pCommand->viewangles.NormalizeYaw(pCommand->viewangles.y - yawVelocity);
	float sideSpeed = 10000.0f;

	if (fabsf(pCommand->mousedx > 2)) {

		pCommand->sidemove = (pCommand->mousedx < 0.f) ? -sideSpeed : sideSpeed;
		return;
	}

	if (pCommand->buttons & IN_BACK)
		pCommand->viewangles.y -= 180.f;
	else if (pCommand->buttons & IN_MOVELEFT)
		pCommand->viewangles.y += 90.f;
	else if (pCommand->buttons & IN_MOVERIGHT)
		pCommand->viewangles.y -= 90.f;

	if (!speed > 0.5f || speed == NAN || speed == INFINITE) {

		pCommand->forwardmove = 450.f;
		return;
	}

	pCommand->forwardmove = std::clamp(5850.f / speed, -450.f, 450.f);

	if ((pCommand->forwardmove < -450.f || pCommand->forwardmove > 450.f))
		pCommand->forwardmove = 0.f;

	pCommand->sidemove = (velocityDelta > 0.0f) ? -sideSpeed : sideSpeed;
	pCommand->viewangles.y = pCommand->viewangles.NormalizeYaw(pCommand->viewangles.y - velocityDelta);
}
