#include "00. Global.fx"

float4 MaterialEmissive; // 외각선 색상

// Vertex Shader
MeshOutput VS(VertexTextureNormal input)
{
	MeshOutput output;
    output.position = mul(input.position, World);
    output.worldPosition = output.position; // World 좌표 저장
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

// Emissive (림라이트)
// - 외곽선을 구할 때 주로 사용함
// - Normal값이 1이면 변화 없고 0에 가까울수록 변화가 일어남


float4 PS(MeshOutput input) : SV_TARGET
{
	// 카메라 위치
    float3 cameraPosition = -View._41_42_43; // View행렬의 41(x), 42(y), 43(z) = 카메라 위치

	// 표면에서 카메라를 향하는 방향 벡터
    float3 E = normalize(cameraPosition - input.worldPosition);
	
	// Normal과 카메라 방향 벡터의 내적. Clamp(0 ~ 1)
    float value = saturate(dot(E, input.normal));
	// 0 → 1, 1 → 0으로 변경
	// Normal과 수직을 이루면 1의 값을 가지게 됨.
    float emissive = 1.0f - value; 
	
	// smoothstep() : 선형 그래프를 부드럽게 곡선으로 변경해줌
	// 외각선에서 멀어질수록 자연스럽게
    emissive = smoothstep(0.0f, 1.0f, emissive);
	
	// 외각선 강조. 색상 범위가 외각선에 가까워짐
    emissive = pow(emissive, 2);
	
    float4 color = MaterialEmissive * emissive;
	
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
