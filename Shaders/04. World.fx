// VS 들어오는 구조체
struct VertexInput
{
	float4 position : POSITION;
    float4 color : COLOR;
};

// VS 나가는 구조체
struct VertexOutput
{
	// SV : System Value
	float4 position : SV_POSITION;
    float4 color : COLOR;
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

	
    output.color = input.color;

	return output;
}

float4 PS(VertexOutput input) : SV_TARGET
{
	return input.color;
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
