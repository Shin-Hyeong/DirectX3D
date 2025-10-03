// Sampler�� ������ �⺻���� ������� �ʰ� �ʿ信 ���� ����

// VS ������ ����ü
struct VertexInput
{
	float4 position : POSITION;
    float2 uv : TEXCOORD;
};

// VS ������ ����ü
struct VertexOutput
{
	// SV : System Value
	float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
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
	
    output.uv = input.uv;
	 
	return output;
}

// Texture���� � ������� ������ �������� �����ϴ� ��ü
// UV��ǥ�� ����� ����(1,1) ó��
// Sampler�� �����ϴ� �ɼ�
//  - Filter : Ȯ�� / ��Ұ� �Ͼ�� �� �߰� ���� ó���ϴ� ���
//  - Address : UV�� 1���� ������, ������ �κ��� ��� ó������
SamplerState Sampler0;
uint Address;

// 0~1 ���� ����� �ؽ�ó �ݺ�
SamplerState SamplerAddressWrap
{
	// U��ǥ�� �Ѿ�� ��
    AddressU = Wrap;
	// V��ǥ�� �Ѿ�� ��
	AddressV = Wrap;
};

// 0~1 �������� ��Ī������ ������ ����
SamplerState SamplerAddressMirror
{
	// U��ǥ�� �Ѿ�� ��
    AddressU = Mirror;
	// V��ǥ�� �Ѿ�� ��
    AddressV = Mirror;
};

// 0~1 ��谪�� ������, �ٱ� �ȼ��� �ݺ�
SamplerState SamplerAddressClamp
{
	// U��ǥ�� �Ѿ�� ��
    AddressU = Clamp;
	// V��ǥ�� �Ѿ�� ��
    AddressV = Clamp;
};

// 0~1 ���� �ٱ��� Ư�� ����(BorderColor)���� ĥ��.
SamplerState SamplerAddressBorder
{
	// U��ǥ�� �Ѿ�� ��
    AddressU = Border;
	// V��ǥ�� �Ѿ�� ��
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
