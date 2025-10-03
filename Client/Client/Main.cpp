#include "pch.h"
#include "Main.h"
// Engine���Ͽ� �ִ� Game
#include "Engine/Game.h"

// Client
#include "01. TriangleDemo.h"
#include "02. QuadDemo.h"
#include "03. ConstBufferDemo.h"
#include "04. CameraDemo.h"
#include "05. TextureDemo.h"
#include "06. SamplerDemo.h"

// ���α׷� ���� �Լ�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GameDesc desc;
	desc.appName = L"DirectX3D";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 800;
	desc.height = 600;
	desc.clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
	// �����ų ��(���� ����)
	desc.app = make_shared<SamplerDemo>();

	GAME->Run(desc);

	return 0;
}