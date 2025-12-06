#include "00. Global.fx"
#include "00. Light.fx"

// model의 Bone 허용 개수
#define MAX_MODEL_TRANSFROMS 250

cbuffer BoneBuffer
{
	// 각각의 Bone이 로컬 좌표계로 변환할 수 있는 행렬들
    matrix BoneTransforms[MAX_MODEL_TRANSFROMS];
};


// 현재 렌더링하고 있는 Bone Index
uint BoneIndex;


struct VS_IN
{
    float4 position : POSITION;
    float4 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 blendIndices : BLEND_INDICES;
    float4 lendWeights : BLEND_WEIGHTS;
    
    
    // INSTANCING
    // INST : Input Slot이 1로 설정되어 있음.
    // slot1으로 전달하면 world에 값을 전달받을 수 있음.
    Matrix world : INST;
    uint instanceID : SV_INSTANCEID;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

// Vertex Shader
VS_OUT VS(VS_IN input)
{
    VS_OUT output;
    
    // 부모 좌표계 -> 로컬 좌표계
	// 정점이 어느 Bone에 연결되어 있는지 BoneIndex를 통해 접근함
    output.position = mul(input.position, BoneTransforms[BoneIndex]);
    
    // input.world : 각 물체가 가지고 있는 World 좌표변환 행렬
    output.position = mul(output.position, input.world);
    output.worldPosition = output.position;
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.normal = input.normal;
    
	return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	// ComputeLight() : Light.fx에서 최종적으로 표면의 색상을 정하는 함수
    // float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
	
    return color;
}

// 어떤 Shader를 사용할지 선택할 수 있음
// - technique : 렌더링 기법을 의미하며, 하나 이상의 pass를 포함하는 상위 블록
// - pass : technique 안에 여러 개 존재할 수 있는 하위 실행 단위, 실제로 셰이더 설정, 렌더 상태, 텍스처 등 한 번의 드로우 호출 시 적용될 모든 파이프라인 상태를 정의
technique11 T0
{
    PASS_VP(P0, VS, PS)
};
