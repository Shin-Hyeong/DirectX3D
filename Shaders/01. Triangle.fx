
// VS 들어오는 구조체
struct VertexInput
{
	float4 position : POSITION;
};

// VS 나가는 구조체
struct VertexOutput
{
	// SV : System Value
	float4 position : SV_POSITION;
};

// Vertex Shader
VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.position = input.position;

	return output;
}

float4 PS(VertexOutput input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}

float4 PS2(VertexOutput input) : SV_TARGET
{
	return float4(0, 1, 0, 1);
}

float4 PS3(VertexOutput input) : SV_TARGET
{
	return float4(0, 0, 1, 1);
}

// 어떤 Shader를 사용할지 선택할 수 있음
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
		// vertexShader를 5.0버전을 사용하고 VS()함수 사용
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// pixelShader를 5.0버전을 사용하고 PS2()함수 사용
		SetPixelShader(CompileShader(ps_5_0, PS2()));
	}
};

technique11 T1
{
	pass P0
	{
		// vertexShader를 5.0버전을 사용하고 VS()함수 사용
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// pixelShader를 5.0버전을 사용하고 PS3()함수 사용
		SetPixelShader(CompileShader(ps_5_0, PS3()));
	}
};