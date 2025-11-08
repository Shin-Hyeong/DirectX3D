#pragma once

// ImGUI 관련 기능을 관리
class ImGuiManager
{
	DECLARE_SINGLE(ImGuiManager)
public:
	// Gui 기본 설정
	void Init();
	// Gui Render Begin
	void Update();
	// Gui Render End
	void Render();
};

