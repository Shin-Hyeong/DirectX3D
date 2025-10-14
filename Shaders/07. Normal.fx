// Normal�� ������ ���̴�

// VS ������ ����ü
struct VertexInput
{
	float4 position : POSITION;
    float2 uv : TEXCOORD;
	// ���� ���� �������� �߰���.
    float3 normal : NORMAL;
};

// VS ������ ����ü
struct VertexOutput
{
	// SV : System Value
	float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
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

cbuffer LightDirection : register(b0)
{
	// ���� ������ ����
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
	// ������ �ִ� Normal ��ǥ�� �˰� ������ ������ �̵��ϰ� �Ǹ� Normal���� ����Ǳ� ������
	// World ��ȯ ����� ���ؾ���. Ư�� ���⿡ ���� ��ȭ�� �̷����
	// World�� float3x3���� ��ȯ�ϸ� ������ǥ�谡 ���ܵ� ����� ���� �� ����
	// - ���� �̵��� ����ϴ� ��ǥ�� ���ŵ�.
    output.normal = mul(input.normal, (float3x3) World);
	 
	return output;
}

// Texture���� � ������� ������ �������� �����ϴ� ��ü
// UV��ǥ�� ����� ����(1,1) ó��
// Sampler�� �����ϴ� �ɼ�
//  - Filter : Ȯ�� / ��Ұ� �Ͼ�� �� �߰� ���� ó���ϴ� ���
//  - Address : UV�� 1���� ������, ������ �κ��� ��� ó������
uint Address;

// 0~1 ���� ����� �ؽ�ó �ݺ�
SamplerState Sampler0
{
	// U��ǥ�� �Ѿ�� ��
    AddressU = Wrap;
	// V��ǥ�� �Ѿ�� ��
	AddressV = Wrap;
};


Texture2D Texture0 : register(t0);

float4 PS(VertexOutput input) : SV_TARGET
{
	// input.normal�� �������ͷ� ��ȯ (Ȥ�� �������ͷ� �ȿö��� ���)
    float3 normal = normalize(input.normal);
	// ���� ������ ������
    float3 light = -LightDir;
	
	// �� ������ ���� = 1(0��) ~ 0(90��)
	// ��� * ������ ��. = ���� ��� ���, ������ ���� ��ο���
    // return float4(1, 1, 1, 1) * dot(light, normal);
	
	// Texture * ������ �� = ���� ��� Texture, ������ ���� ��ο���
    return Texture0.Sample(Sampler0, input.uv) * dot(light, normal);
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
