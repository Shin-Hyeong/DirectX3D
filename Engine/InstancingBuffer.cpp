#include "pch.h"
#include "InstancingBuffer.h"

InstancingBuffer::InstancingBuffer()
{
	CreateBuffer(MAX_MESH_INSTANCE);
}

InstancingBuffer::~InstancingBuffer()
{
}

void InstancingBuffer::ClearData()
{
	_data.clear();
}

void InstancingBuffer::AddData(InstancingData& data)
{
	_data.push_back(data);
}

void InstancingBuffer::PushData()
{
	const uint32 dataCount = GetCount();
	if (dataCount > _maxCount)
		CreateBuffer(dataCount);

	D3D11_MAPPED_SUBRESOURCE subResource;

	// 데이터를 넣기 위해 Mapping
	DC->Map(_instanceBuffer->GetComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		// 데이터 복사(전달)
		::memcpy(subResource.pData, _data.data(), sizeof(InstancingData) * GetCount());
	}
	// 데이터를 넣고 닫기 UnMapping
	DC->Unmap(_instanceBuffer->GetComPtr().Get(), 0);

	// GPU에 전달
	_instanceBuffer->PushData();
}

void InstancingBuffer::CreateBuffer(uint32 maxCount)
{
	_maxCount = maxCount;
	_instanceBuffer = make_shared<VertexBuffer>();

	vector<InstancingData> temp(maxCount);
	// 매 frame마다 갱신하기 위해서 cpuWrite를 허용한다.
	_instanceBuffer->Create(temp, /*slot*/1, /*cpuWrite*/true);
}
