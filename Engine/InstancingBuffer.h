#pragma once

class VertexBuffer;

struct InstancingData
{
	Matrix world;
};

#define MAX_MESH_INSTANCE 500

class InstancingBuffer
{
public:
	InstancingBuffer();
	~InstancingBuffer();

public:
	// 매 frame 마다 갱신해야함
	// 데이터를 한번 밀고 새로운 데이터를 넣기 위함
	void ClearData();

	// 매 frame마다 새로운 데이터를 넣기 위함
	void AddData(InstancingData& data);
	
	// _data에 있는 데이터를 GPU에게 전달함
	void PushData();

public:
	// 저장한 데이터의 개수
	uint32 GetCount() { return static_cast<uint32>(_data.size()); }
	// 정점 데이터를 가지는 Buffer 반환
	shared_ptr<VertexBuffer> GetBuffer() { return _instanceBuffer; }

private:
	// InstancingData * maxCount만큼 들어갈수 있는 VertexBuffer 생성
	void CreateBuffer(uint32 maxCount = MAX_MESH_INSTANCE);

private:
	// Mesh와 Material에 번호를 부여하고 서로 매칭되도록 관리
	// uint64						_instanceId = 0;
	// 정점 데이터
	shared_ptr<VertexBuffer>	_instanceBuffer;
	// Buffer의 크기
	uint32						_maxCount = 0;	
	// Instancing Mesh의 world 변환행렬
	vector<InstancingData>		_data;
};

