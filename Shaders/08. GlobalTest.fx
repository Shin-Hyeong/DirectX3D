#include "00. Global.fx"

// Vertex Shader
VertexOutput VS(VertexTextureNormal input)
{
	VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, VP);

    output.uv = input.uv;
	// ������ �ִ� Normal ��ǥ�� �˰� ������ ������ �̵��ϰ� �Ǹ� Normal���� ����Ǳ� ������
	// World ��ȯ ����� ���ؾ���. Ư�� ���⿡ ���� ��ȭ�� �̷����
	// World�� float3x3���� ��ȯ�ϸ� ������ǥ�谡 ���ܵ� ����� ���� �� ����
	// - ���� �̵��� ����ϴ� ��ǥ�� ���ŵ�.
    output.normal = mul(input.normal, (float3x3) World);
	 
	return output;
}

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
    return Texture0.Sample(LinearSampler, input.uv) * dot(light, normal);
}


// � Shader�� ������� ������ �� ����
// - technique : ������ ����� �ǹ��ϸ�, �ϳ� �̻��� pass�� �����ϴ� ���� ���
// - pass : technique �ȿ� ���� �� ������ �� �ִ� ���� ���� ����, ������ ���̴� ����, ���� ����, �ؽ�ó �� �� ���� ��ο� ȣ�� �� ����� ��� ���������� ���¸� ����
technique11 T0
{
	// VS�� PS�� ����
	PASS_VP(P0, VS, PS)

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
