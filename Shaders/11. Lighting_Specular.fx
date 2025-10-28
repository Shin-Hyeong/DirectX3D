#include "00. Global.fx"

float4 LightSpecular;
float4 MaterialSpecular;
float3 LightDir;

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

// Specular(반사광)
// - 한방향으로 완전히 반사된 빛(Phong)
// - 빛이 반사된 방향으로 카메라가 있으면 해당 정점이 밝게 보임

float4 PS(MeshOutput input) : SV_TARGET
{
	// 빛의 반사 방향 벡터 (R와 R2은 같은 값. 다른 식)
    float3 R = normalize(reflect(LightDir, input.normal));
    float3 R2 = LightDir - (2 * input.normal * dot(LightDir, input.normal));
	
	// 카메라 위치
    float3 cameraPosition = -View._41_42_43; // View행렬의 41(x), 42(y), 43(z)의 역행렬 = 카메라 위치

	// 정점에서 카메라를 향하는 방향벡터
    float3 E = normalize(cameraPosition - input.worldPosition);
	
	// 카메라 방향벡터와 반사각의 내적
    float value = saturate(dot(R, E)); // saturate → Clamp(0 ~ 1)
	
	// Specular의 값. 값을 극명하게 보이기 위해 제곱을 한다. 
	// 값이 높을수록 빛나는 영역이 좁아짐. (0 ~ 1)의 값을 제곱할 수록 작은 값이 됨.
    float specular = pow(value, 10); // value의 10제곱

    // 실질적으로 표현되는 color
    float4 color = LightSpecular * MaterialSpecular * specular;
	
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
