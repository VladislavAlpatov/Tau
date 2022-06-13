#pragma once
#include "../../imgui/imgui_internal.h"
#include "CBaseEsp.h"
#include "../../SSDK/SSDK.h"

using namespace Esp;

ImVec3 CBaseEsp::WorldToScreen(const ImVec3& vecPosition)
{
    // Screen size 
    ImVec2 viewPorstSize = ImGui::GetMainViewport()->Size;

    auto viewMatrix = SSDK::GetViewMatrix();

    float _x = viewMatrix[0][0] * vecPosition.x + viewMatrix[0][1] * vecPosition.y + viewMatrix[0][2] * vecPosition.z + viewMatrix[0][3];
    float _y = viewMatrix[1][0] * vecPosition.x + viewMatrix[1][1] * vecPosition.y + viewMatrix[1][2] * vecPosition.z + viewMatrix[1][3];
    // Dont need z cuz we need only x,y and w
    //float _z = pClient->dwViewmatrix[2][0] * vecPosition.x + pClient->dwViewmatrix[2][1] * vecPosition.y + pClient->dwViewmatrix[2][2] * vecPosition.z + pClient->dwViewmatrix[2][3];

    // w is depth
    float w = viewMatrix[3][0] * vecPosition.x + viewMatrix[3][1] * vecPosition.y + viewMatrix[3][2] * vecPosition.z + viewMatrix[3][3];

    ImVec2 ndc;

    ndc.x = _x / w;
    ndc.y = _y / w;

    // Normalize screen cords
    float x =  (viewPorstSize.x / 2.f * ndc.x) + (ndc.x + viewPorstSize.x / 2.f);
    float y = -(viewPorstSize.y / 2.f * ndc.y) + (ndc.y + viewPorstSize.y / 2.f);

    return ImVec3(x, y, w);
}