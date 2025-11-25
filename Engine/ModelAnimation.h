#pragma once

struct ModelKeyframeData
{
	float time;
	Vec3 scale;
	Quaternion rotation;
	Vec3 translation;
};

struct ModelKeyframe
{
	wstring boneName;
	vector<ModelKeyframeData> transforms;
};

struct ModelAnimation
{
	wstring name;
	float duration = 0.f;
	float frameRate = 0.f;		// 1초당 재생될 frame 갯수
	uint32 frameCount = 0;		// Animation의 총 Frame 갯수
	unordered_map<wstring, shared_ptr<ModelKeyframe>> keyframes;

	// 해당 이름으로 되어 있는 ModelKeyframe 찾기
	// @params name : keyframes에 저장된 key를 넘김 <wstring, shared_ptr<ModelKeyframe>>
	shared_ptr<ModelKeyframe> GetKeyframe(const wstring& name);
};
