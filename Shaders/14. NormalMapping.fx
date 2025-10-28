#include "00. Global.fx"
#include "00. Light.fx"

// Vertex Shader
MeshOutput VS(VertexTextureNormalTangent input)
{
	MeshOutput output;
    output.position = mul(input.position, World);
    output.worldPosition = output.position.xyz; // World 좌표 저장
    output.position = mul(output.position, VP);

    output.uv = input.uv;
	// 가만히 있는 Normal 좌표는 알고 있지만 정점이 이동하게 되면 Normal값이 변경되기 때문에
	// World 변환 행렬을 곱해야함. 특히 방향에 의해 변화가 이루어짐
	// World을 float3x3으로 변환하면 동차좌표계가 제외된 행렬을 얻을 수 있음
	// - 정점 이동에 사용하는 좌표가 제거됨.
	// - Local → World
    output.normal = mul(input.normal, (float3x3) World);
    output.tangent = mul(input.tangent, (float3x3) World);
	 
	return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
	// ComputeNormalMapping() : Normal Texture를 계산하는 함수
	// input.normal에 값이 변경되어 저장됨.
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
	
	// ComputeLight() : Light.fx에서 최종적으로 표면의 색상을 정하는 함수
    float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
	
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
