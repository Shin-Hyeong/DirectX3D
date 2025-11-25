#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Mesh.fbx");
		converter->ExportMaterialData(L"Kachujin/Kachujin");
		converter->ExportModelData(L"Kachujin/Kachujin");
	}
	// Animation
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Idle.fbx");
		converter->ExportAnimationData(L"kachujin/Idle");
	}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Run.fbx");
		converter->ExportAnimationData(L"kachujin/Run");
	}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Slash.fbx");
		converter->ExportAnimationData(L"kachujin/Slash");
	}
}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
