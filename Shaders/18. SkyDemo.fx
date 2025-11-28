#include "00. Global.fx"
#include "00. Light.fx"

struct VS_OUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

// Vertex Shader
VS_OUT VS(VertexTextureNormalTangent input)
{
    VS_OUT output;
    
    // SkyBox를 카메라 위치를 따라가도록 하기 위해 Local x View를 함
    // - Skybox위치가 카메라 기준으로 위치하도록 w 값을 0으로 설정함
    // - SkyBox의 Rotation이 카메라의 회전에만 반응하도록 xyz는 View와 곱함
    float4 viewPos = mul(float4(input.position.xyz, 0), View);
    float4 projectPos = mul(viewPos, Projection);
    
    // VS 이후 RS과정에서 Position의 x, y, z, w에 w를 나누어 화면의 비율에 맞는 값을 얻음
    // x/w, y/w, z/w, w/w -> (화면의 가로, 화면의 세로, 화면의 깊이) 좌표
    // 3D 물체를 평면으로 표현하기 위함
    //
    // 아래 과정에서 z를 w에 근사한 값을 변경함으로써
    // 항상 렌더링되는 범위 안에 있지만 가장 멀리에 있는 좌표를 얻게 됨.
    output.position = projectPos.xyzw;
    output.position.z = output.position.w * 0.99999f;
    
    output.uv = input.uv;
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
    pass P0
    {
        SetRasterizerState(FrontCounterClockwiseTrue);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};
