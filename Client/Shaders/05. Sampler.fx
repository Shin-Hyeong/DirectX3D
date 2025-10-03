// Sampler의 설정을 기본으로 사용하지 않고 필요에 따라 수정

// VS 들어오는 구조체
struct VertexInput
{
	float4 position : POSITION;
    float2 uv : TEXCOORD;
};

// VS 나가는 구조체
struct VertexOutput
{
	// SV : System Value
	float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
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

// Vertex Shader
VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
	
    output.uv = input.uv;
	 
	return output;
}

// Texture에서 어떤 방식으로 색상을 추출할지 결정하는 객체
// UV좌표를 벗어나는 영역(1,1) 처리
// Sampler에 설정하는 옵션
//  - Filter : 확대 / 축소가 일어났을 때 중간 값을 처리하는 방식
//  - Address : UV가 1보다 컸을때, 나머지 부분을 어떻게 처리할지
SamplerState Sampler0;
uint Address;

// 0~1 범위 벗어나면 텍스처 반복
SamplerState SamplerAddressWrap
{
	// U좌표가 넘어갔을 때
    AddressU = Wrap;
	// V좌표가 넘어갔을 때
	AddressV = Wrap;
};

// 0~1 범위마다 대칭적으로 반전해 복제
SamplerState SamplerAddressMirror
{
	// U좌표가 넘어갔을 때
    AddressU = Mirror;
	// V좌표가 넘어갔을 때
    AddressV = Mirror;
};

// 0~1 경계값에 고정해, 바깥 픽셀을 반복
SamplerState SamplerAddressClamp
{
	// U좌표가 넘어갔을 때
    AddressU = Clamp;
	// V좌표가 넘어갔을 때
    AddressV = Clamp;
};

// 0~1 영역 바깥을 특수 색상(BorderColor)으로 칠함.
SamplerState SamplerAddressBorder
{
	// U좌표가 넘어갔을 때
    AddressU = Border;
	// V좌표가 넘어갔을 때
    AddressV = Border;
    BorderColor = float4(1, 0, 0, 1);
};

Texture2D Texture0 : register(t0);

float4 PS(VertexOutput input) : SV_TARGET
{
	if(Address == 0)
        return Texture0.Sample(SamplerAddressWrap, input.uv);
    if (Address == 1)
        return Texture0.Sample(SamplerAddressMirror, input.uv);
    if (Address == 2)
        return Texture0.Sample(SamplerAddressClamp, input.uv);
    if (Address == 3)
        return Texture0.Sample(SamplerAddressBorder, input.uv);

    return Texture0.Sample(Sampler0, input.uv);
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
