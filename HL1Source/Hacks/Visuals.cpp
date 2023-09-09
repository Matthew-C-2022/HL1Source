#include "Visuals.hpp"
#include "../ImGui/imgui.h"
#include <vector>
#include "../Interfaces/Interfaces.hpp"
#include <format>
struct TrailObject {
	Vector Pos; 
	float lifetime; 
};

std::vector<TrailObject> TrailPos;


#define maxLength 50

void HA::Visual::UpdatePlayerTrail(PlayerEntity* LP)
{
    auto Globals = G::GVars;
	auto PlayerPos = M::EntityList->GetClientEntity(1)->GetRenderOrigin();
	TrailObject NewTrailObject;
	NewTrailObject.Pos = PlayerPos;
	NewTrailObject.lifetime = 40.0f;
    TrailPos.push_back(NewTrailObject);
	for (auto it = TrailPos.begin(); it != TrailPos.end() - 1; ) {
		it->lifetime -= Globals->frametime * 5;
		if (it->lifetime <= 0.0f) {
			it = TrailPos.erase(it);  // erase returns the iterator to the next element
		}
		else {
			++it;
		}
	}

}



void HA::Visual::RenderPlayerTrail(PlayerEntity* LP)
{

    if (TrailPos.size() > 3)
    {
        for (auto it = TrailPos.begin(); it != TrailPos.end() - 1; ) {
            Vector ScreenPos;
            Vector ScreenPos2;
            auto next = it + 1;
			if (WorldToScreen(next->Pos, ScreenPos) && WorldToScreen(it->Pos, ScreenPos2))
			{
				auto drawlist = ImGui::GetBackgroundDrawList();
				drawlist->AddLine(ImVec2(ScreenPos2.x, ScreenPos2.y), ImVec2(ScreenPos.x, ScreenPos.y), ImGui::GetColorU32(ImVec4(1, 0, 0, 1)), 1.0f);
			}
			++it;
        }
    }

}



bool HA::Visual::WorldToScreen(Vector& world, Vector& screen)
{
	auto screen_transform = [&world, &screen]() -> bool {
		static auto& matrix = M::Engine->WorldToScreenMatrix();

		screen.x = matrix[0][0] * world.x + matrix[0][1] * world.y + matrix[0][2] * world.z + matrix[0][3];
		screen.y = matrix[1][0] * world.x + matrix[1][1] * world.y + matrix[1][2] * world.z + matrix[1][3];
		screen.z = 0.0f;

		float w = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];

		if (w < 0.001f) {
			screen.x *= 0;
			screen.y *= 0;
			return false;
		}

		screen.x /= w;
		screen.y /= w;

		return true;
	};

	if (screen_transform()) {
		int w, h;
		M::Engine->GetScreenSize(w, h);

		screen.x = (w / 2.0f) + (screen.x * w) / 2.0f;
		screen.y = (h / 2.0f) - (screen.y * h) / 2.0f;

		return true;
	}

	return false;
}

void HA::Visual::RenderBones(PlayerEntity* LP)
{
	for (size_t i = 0; i < 90; i++)
	{
		auto a = LP->GetBonePos(i);
		Vector screen;
		auto b = (IClientEntity*)LP;
		auto C = b->GetRenderOrigin();
		if (WorldToScreen(a, screen))
		{
			auto drawlist = ImGui::GetBackgroundDrawList();
			auto text = std::format("{}", i);
			drawlist->AddText(ImVec2(screen.x, screen.y), ImGui::GetColorU32(ImVec4(1, 0, 0, 1)), text.c_str());
		}


		if (WorldToScreen(C, screen))
		{
			auto drawlist = ImGui::GetBackgroundDrawList();
			drawlist->AddText(ImVec2(screen.x, screen.y), ImGui::GetColorU32(ImVec4(1, 0, 0, 1)), "Origin");
		}

	}

}
