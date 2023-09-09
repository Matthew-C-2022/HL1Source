#pragma once
#include "../sdk/CheatEntityClass.hpp"
#include "Globals.hpp"



namespace HA
{
	namespace Visual
	{
		void UpdatePlayerTrail(PlayerEntity* LP);
		void RenderPlayerTrail(PlayerEntity* LP);
		bool WorldToScreen(Vector& WorldLocation, Vector& ScreenLocation);
		void RenderBones(PlayerEntity* LP);
	}
}