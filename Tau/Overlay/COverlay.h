#pragma once

#include <Windows.h>
#include <vector>

#include <d3d9.h>
#include "../imgui/imgui.h"

class COverlay
{
public:
	
	COverlay(LPDIRECT3DDEVICE9 pDevice);
	void Render();
	bool IsShowUI();
	void ToggleUI();
private:
	ImFont* m_pFontEsp;
	bool    m_bShowUI            = false;
	bool    m_bShowKeyBindDialog = false;

	LPDIRECT3DDEVICE9 m_pDevice;
};