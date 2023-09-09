#include "AimHack.hpp"
#include <unordered_map>
#include <deque>
#include "../Interfaces/Interfaces.hpp"
struct EntityInfo {
    int entIndex;
    Vector Position;
    //ViewMatrix BoneMatrix[128];
    int tick;
};


inline std::unordered_map<int, std::deque<EntityInfo>> entityHistories; 
const int maxHistorySize = 128; 


void addEntityState(const EntityInfo& entityInfo) {
    int entIndex = entityInfo.entIndex;
    entityHistories[entIndex].push_back(entityInfo);

    while (entityHistories[entIndex].size() > maxHistorySize) {
        entityHistories[entIndex].pop_front();
    }
}




void HA::Aim::BackTrack(CUserCmd* CMD)
{
    for (size_t i = 1; i < 64; i++)
    {
        auto curent = M::EntityList->GetClientEntity(i);
        ViewMatrix BoneMatrix[128];
        curent->SetupBones(BoneMatrix, 128, 0x00000100, M::Engine->GetLastTimeStamp());
        EntityInfo Temp;
        Temp.Position = Vector(BoneMatrix[9][0][3], BoneMatrix[9][1][3], BoneMatrix[9][2][3]);
        Temp.entIndex = i;
        Temp.tick = CMD->tick_count;
        addEntityState(Temp);
    }

}
