#include "COverlay.h"

#include "../imgui/misc/freetype/imgui_freetype.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"
#include "DirectX9ColorFix/DX9ColorFix.h"
#include "../Hacks/ESP/CBaseEsp.h"
#include "../SSDK/IClientEntityList.h"
#include "../SSDK/SSDK.h"
#include <fmt/format.h>
#include "../imgui/imgui_internal.h"
#include  "../Hacks/Aimbot/AimBot.h"


COverlay::COverlay(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice      = pDevice;
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(FindWindowA(NULL, "Team Fortress 2"));
	ImGui_ImplDX9_Init(m_pDevice);

	ImGui::GetStyle().AntiAliasedLinesUseTex = false;
	auto io = ImGui::GetIO();
	
	ImFontConfig cfg;
	cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_MonoHinting;
	static ImWchar ranges[] = { 0x1, 0xFFFD, 0 };
	m_pFontEsp  = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 13.f, &cfg, ranges);
	auto& style = ImGui::GetStyle();
	auto& theme = style.Colors;

	style.FrameBorderSize         = 1;
	style.AntiAliasedLinesUseTex = false;
	style.AntiAliasedLines       = false;
	style.AntiAliasedFill        = false;
	style.ScrollbarRounding      = 0.f;
	style.WindowMinSize          = ImVec2(10, 10);

	
	theme[ImGuiCol_WindowBg]		 = ImColor(24, 31, 35, 255);
	theme[ImGuiCol_Button]			 = ImVec4(1.f, 0.372f, 0.372f, 1.f);
	theme[ImGuiCol_Tab]				 = ImVec4(1.f, 0.372f, 0.372f, 1.f);
	theme[ImGuiCol_SeparatorActive]  = ImVec4(1.f, 0.372f, 0.372f, 1.f);
	theme[ImGuiCol_Border]			 = ImVec4(1.f, 0.372f, 0.372f, 1.f);
	theme[ImGuiCol_Text]			 = ImVec4(1.f, 1.f, 1.f, 1.f);
	theme[ImGuiCol_ButtonActive]	 = ImVec4(1.f, 0.57f, 0.57f, 1.f);
	theme[ImGuiCol_ButtonHovered]    = ImVec4(1.f, 0.4f, 0.4f, 1.f);
	theme[ImGuiCol_CheckMark]		 = ImVec4(1.f, 0.372f, 0.372f, 1.f);
	theme[ImGuiCol_TextSelectedBg]   = ImVec4(1.f, 0.372f, 0.372f, 1.f);
	theme[ImGuiCol_FrameBg]			 = ImVec4(0.31f, 0.31f, 0.31f, 1.f);
	theme[ImGuiCol_FrameBgActive]    = ImVec4(1.f, 0.57f, 0.57f, 1.f);
	theme[ImGuiCol_FrameBgHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.f);
	theme[ImGuiCol_PopupBg]			 = ImColor(24, 31, 35, 255);
	theme[ImGuiCol_ScrollbarBg]		 = ImVec4(1.f, 0.372f, 0.372f, 0.f);
	theme[ImGuiCol_ScrollbarGrab]	 = ImVec4(1.f, 0.372f, 0.372f, 1.f);
	theme[ImGuiCol_SliderGrab]		 = ImVec4(1.f, 0.372f, 0.372f, 1.f);
	theme[ImGuiCol_SliderGrabActive] = ImVec4(1.f, 0.372f, 0.372f, 1.f);
	theme[ImGuiCol_TabHovered]		 = ImVec4(1.f, 0.57f, 0.57f, 1.f);
	theme[ImGuiCol_TabActive]		 = ImVec4(1.f, 0.372f, 0.372f, 1.f);

}


void COverlay::Render()
{
	static auto pEntList = SSDK::GetInterface<SSDK::IClientEntityList>("client.dll", "VClientEntityList003");

	DX9ColorFix color_fix(m_pDevice);
	color_fix.RemoveColorFilter();

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto pDrawList = ImGui::GetBackgroundDrawList();

	pDrawList->AddText(ImVec2(), ImColor(255, 255, 255), u8"τau project");

	auto pLocalPlayer = SSDK::GetLocalPlayer();

	for (int i = 1; i < 33; ++i)
	{
		auto pEnt = pEntList->GetClientEntity(i);

		if (not pEnt or (pEnt->m_iTeamNum == pLocalPlayer->m_iTeamNum) or pEnt->m_bDead or not pEnt->m_iHealth or pEnt->m_bDormant)
			continue;

		auto pos = Esp::CBaseEsp::WorldToScreen(pEnt->m_vecOrigin);
		auto pred_pos = Esp::CBaseEsp::WorldToScreen(Hacks::CAimBot::Predict(pEnt, 2000.f));

		if (pos.z <= 0.f)
			continue;

		auto txt = fmt::format("Health: {}/{}", pEnt->m_iHealth, pEnt->m_iMaxHealth);
		pDrawList->AddText(pos-ImVec2(ImGui::CalcTextSize(txt.c_str()).x / 2, 0), pEnt->GetHealthColor(), txt.c_str());
		pDrawList->AddLine(pos, pred_pos, ImColor(255, 0, 0), 2.f);
		pDrawList->AddCircleFilled(pred_pos, 5.f, ImColor(255, 255, 255));

	}


	if (m_bShowUI)
	{

	}


	color_fix.RestoreRenderState();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	color_fix.RestoreColorFilter();
}
bool COverlay::IsShowUI()
{
	return m_bShowUI;
}
void COverlay::ToggleUI()
{
	m_bShowUI = !m_bShowUI;
}