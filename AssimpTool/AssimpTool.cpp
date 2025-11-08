#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{
	{
		shared_ptr<Converter> converter = make_shared<Converter>();
		
		// FBX를 메모리에 로드
		converter->ReadAssetFile(L"Tank/Tank.fbx");
		
		// 메모리에 있는 데이터를 CustomData로 변환
		converter->ExportMaterialData(L"Tank/Tank");
		converter->ExportModelData(L"Tank/Tank");

		// CustomData를 메모리에 로드

	}

	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		// FBX를 메모리에 로드
		converter->ReadAssetFile(L"Tower/Tower.fbx");

		// 메모리에 있는 데이터를 CustomData로 변환
		converter->ExportMaterialData(L"Tower/Tower");
		converter->ExportModelData(L"Tower/Tower");

		// CustomData를 메모리에 로드

	}
}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
