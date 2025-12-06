#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"


float4 PS(MeshOutput input) : SV_TARGET
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
    PASS_VP(P0, VS_Mesh, PS)            // MeshRenderer
    PASS_VP(P1, VS_Model, PS)           // ModelRenderer
    PASS_VP(P2, VS_Animation, PS)       // ModelAnimator
};
