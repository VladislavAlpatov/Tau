#pragma once
#include "Hooks.h"
#include <MinHook.h>
#include "../../Utils/memory.h"
#include <d3d9.h>	
#include <memory>
#include "../../Overlay/COverlay.h"
#include "../../SSDK/SSDK.h"
#include "../Aimbot/AimBot.h"

static void* oPresentAddr = NULL;
static void* oCreateMove = NULL;
static std::unique_ptr<COverlay> pOverlay;

int __stdcall  hkPresent(LPDIRECT3DDEVICE9 pDevice, int a2, int a3, int a4, int a5)
{
	if (not pOverlay)
		pOverlay = std::unique_ptr<COverlay>(new COverlay(pDevice));

	pOverlay->Render();


	typedef int(__stdcall* Present)(LPDIRECT3DDEVICE9, int, int, int, int);
	return reinterpret_cast<Present>(oPresentAddr)(pDevice, a2, a3, a4, a5);
}

int __stdcall hkCreateMove(float fSampleTime, SSDK::CUserCmd* pUserCmd)
{
	//std::printf("%f %f %f\n", pUserCmd->viewangles.x, pUserCmd->viewangles.y, pUserCmd->viewangles.z);
	if (GetAsyncKeyState(VK_SHIFT))
		Hacks::CAimBot(pUserCmd).Work();
	typedef int(__stdcall* CreateMovet)(float, void*);
	return reinterpret_cast<CreateMovet>(oCreateMove)(fSampleTime, pUserCmd);
}

void Hooks::Attach()
{
	MH_Initialize();
	auto present = (LPVOID)Memory::FindPattern("d3d9.dll", "?? ?? ?? ?? ?? 83 E4 F8 51 51 56 8B 75 08 8B CE F7 D9 57 1B C9 8D 46 04 23 C8 6A ?? 51 8D 4C 24 10 E8 ?? ?? ?? ?? F7 46 ?? ?? ?? ?? ?? 74 07 BF ?? ?? ?? ?? EB 17");
	auto createMove = (LPVOID)Memory::FindPattern("client.dll", "55 8B EC E8 ? ? ? ? 8B C8 85 C9 75 06");


	MH_CreateHook(present,    hkPresent,    (LPVOID*)&oPresentAddr);
	MH_CreateHook(createMove, hkCreateMove, (LPVOID*)&oCreateMove);

	MH_EnableHook(MH_ALL_HOOKS);

}

void Hooks::Detach()
{
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}
