#include "00. Global.fx"

float4 LightDiffuse;		// 빛의 색상
float4 MaterialDiffuse;		// Material이 빛을 받아드리는 정도
float3 LightDir;

// Vertex Shader
VertexOutput VS(VertexTextureNormal input)
{
	VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, VP);

    output.uv = input.uv;
	// 가만히 있는 Normal 좌표는 알고 있지만 정점이 이동하게 되면 Normal값이 변경되기 때문에
	// World 변환 행렬을 곱해야함. 특히 방향에 의해 변화가 이루어짐
	// World을 float3x3으로 변환하면 동차좌표계가 제외된 행렬을 얻을 수 있음
	// - 정점 이동에 사용하는 좌표가 제거됨.
    output.normal = mul(input.normal, (float3x3) World);
	 
	return output;
}

Texture2D DiffuseMap : register(t0);

// Diffuse (분산광)
// 물체의 표면에서 분산되어 눈으로 바로 들어오는 빛
// 각도에 따라 밝기가 다르다 (Lambert 공식)

float4 PS(VertexOutput input) : SV_TARGET
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
	
	// Lambert
    float value = dot(-LightDir, normalize(input.normal)); // 빛과 Normal의 사이 각도
	// LightDiffuse * MaterialDiffuse = 빛의 색상에 표면이 빛을 받는 정도
	// value * Diffuse = 빛의 입사각에 따라 빛을 받는 비율이 달라짐.
    color = color * value * LightDiffuse * MaterialDiffuse;
	
    return color;
}


// 어떤 Shader를 사용할지 선택할 수 있음
// - technique : 렌더링 기법을 의미하며, 하나 이상의 pass를 포함하는 상위 블록
// - pass : technique 안에 여러 개 존재할 수 있는 하위 실행 단위, 실제로 셰이더 설정, 렌더 상태, 텍스처 등 한 번의 드로우 호출 시 적용될 모든 파이프라인 상태를 정의
technique11 T0
{
	// VS와 PS를 묶음
	PASS_VP(P0, VS, PS)

	pass P1
	{
		// 와이어 프레임 설정
        SetRasterizerState(FillModeWireFrame);
		// vertexShader를 5.0버전을 사용하고 VS()함수 사용
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// pixelShader를 5.0버전을 사용하고 PS2()함수 사용
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};
