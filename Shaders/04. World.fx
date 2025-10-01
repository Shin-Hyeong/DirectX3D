// VS ������ ����ü
struct VertexInput
{
	float4 position : POSITION;
    float4 color : COLOR;
};

// VS ������ ����ü
struct VertexOutput
{
	// SV : System Value
	float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer TransformBuffer : register(b0)
{
    matrix World; // ���� ��ȯ ���
}

cbuffer CameraBuffer : register(b1)
{
    matrix View; // �� ��ȯ ���
    matrix Projection; // ���� ��ȯ ���
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

// ������ ���������� ���̵��� ����
RasterizerState FillModeWireFrame
{
    FillMode = Wireframe;
};



// � Shader�� ������� ������ �� ����
// technique : ������ ����� �ǹ��ϸ�, �ϳ� �̻��� pass�� �����ϴ� ���� ���
// pass : technique �ȿ� ���� �� ������ �� �ִ� ���� ���� ����, ������ ���̴� ����, ���� ����, �ؽ�ó �� �� ���� ��ο� ȣ�� �� ����� ��� ���������� ���¸� ����
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
		// ���̾� ������ ����
        SetRasterizerState(FillModeWireFrame);
		// vertexShader�� 5.0������ ����ϰ� VS()�Լ� ���
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// pixelShader�� 5.0������ ����ϰ� PS2()�Լ� ���
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};
