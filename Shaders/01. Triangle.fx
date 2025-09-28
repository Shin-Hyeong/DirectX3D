
// VS ������ ����ü
struct VertexInput
{
	float4 position : POSITION;
};

// VS ������ ����ü
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

// � Shader�� ������� ������ �� ����
technique11 T0
{
	// VS�� PS�� ����
	pass P0
	{
		// vertexShader�� 5.0������ ����ϰ� VS()�Լ� ���
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// pixelShader�� 5.0������ ����ϰ� PS()�Լ� ���
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
		// vertexShader�� 5.0������ ����ϰ� VS()�Լ� ���
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// pixelShader�� 5.0������ ����ϰ� PS2()�Լ� ���
		SetPixelShader(CompileShader(ps_5_0, PS2()));
	}
};

technique11 T1
{
	pass P0
	{
		// vertexShader�� 5.0������ ����ϰ� VS()�Լ� ���
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// pixelShader�� 5.0������ ����ϰ� PS3()�Լ� ���
		SetPixelShader(CompileShader(ps_5_0, PS3()));
	}
};