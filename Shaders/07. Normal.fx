// Normal에 적용할 쉐이더

// VS 들어오는 구조체
struct VertexInput
{
	float4 position : POSITION;
    float2 uv : TEXCOORD;
	// 정점 정보 설정에서 추가됨.
    float3 normal : NORMAL;
};

// VS 나가는 구조체
struct VertexOutput
{
	// SV : System Value
	float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer TransformBuffer : register(b0)
{
    matrix World; // 월드 변환 행렬
}

cbuffer CameraBuffer : register(b1)
{
    matrix View; // 뷰 변환 행렬
    matrix Projection; // 투영 변환 행렬
}

cbuffer LightDirection : register(b0)
{
	// 빛이 들어오는 방향
    float3 LightDir;
}

// Vertex Shader
VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
	
    output.uv = input.uv;
	// 가만히 있는 Normal 좌표는 알고 있지만 정점이 이동하게 되면 Normal값이 변경되기 때문에
	// World 변환 행렬을 곱해야함. 특히 방향에 의해 변화가 이루어짐
	// World을 float3x3으로 변환하면 동차좌표계가 제외된 행렬을 얻을 수 있음
	// - 정점 이동에 사용하는 좌표가 제거됨.
    output.normal = mul(input.normal, (float3x3) World);
	 
	return output;
}

// Texture에서 어떤 방식으로 색상을 추출할지 결정하는 객체
// UV좌표를 벗어나는 영역(1,1) 처리
// Sampler에 설정하는 옵션
//  - Filter : 확대 / 축소가 일어났을 때 중간 값을 처리하는 방식
//  - Address : UV가 1보다 컸을때, 나머지 부분을 어떻게 처리할지
uint Address;

// 0~1 범위 벗어나면 텍스처 반복
SamplerState Sampler0
{
	// U좌표가 넘어갔을 때
    AddressU = Wrap;
	// V좌표가 넘어갔을 때
	AddressV = Wrap;
};


Texture2D Texture0 : register(t0);

float4 PS(VertexOutput input) : SV_TARGET
{
	// input.normal을 단위벡터로 변환 (혹시 단위벡터로 안올때를 대비)
    float3 normal = normalize(input.normal);
	// 빛이 들어오는 역방향
    float3 light = -LightDir;
	
	// 두 벡터의 내적 = 1(0˚) ~ 0(90˚)
	// 흰색 * 내적의 값. = 빛이 쎄면 흰색, 약해질 수록 어두워짐
    // return float4(1, 1, 1, 1) * dot(light, normal);
	
	// Texture * 내적의 값 = 빛이 쎄면 Texture, 약해질 수록 어두워짐
    return Texture0.Sample(Sampler0, input.uv) * dot(light, normal);
}


// 와이퍼 프레임으로 보이도록 설정
RasterizerState FillModeWireFrame
{
    FillMode = Wireframe;
};

// 어떤 Shader를 사용할지 선택할 수 있음
// technique : 렌더링 기법을 의미하며, 하나 이상의 pass를 포함하는 상위 블록
// pass : technique 안에 여러 개 존재할 수 있는 하위 실행 단위, 실제로 셰이더 설정, 렌더 상태, 텍스처 등 한 번의 드로우 호출 시 적용될 모든 파이프라인 상태를 정의
technique11 T0
{
	// VS와 PS를 묶음
	pass P0
	{
		// vertexShader를 5.0버전을 사용하고 VS()함수 사용
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// pixelShader를 5.0버전을 사용하고 PS()함수 사용
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

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
