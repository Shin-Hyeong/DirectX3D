#include "00. Global.fx"

float4 LightDiffuse;		// ���� ����
float4 MaterialDiffuse;		// Material�� ���� �޾Ƶ帮�� ����
float3 LightDir;

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

Texture2D DiffuseMap : register(t0);

// Diffuse (�л걤)
// ��ü�� ǥ�鿡�� �л�Ǿ� ������ �ٷ� ������ ��
// ������ ���� ��Ⱑ �ٸ��� (Lambert ����)

float4 PS(VertexOutput input) : SV_TARGET
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
	
	// Lambert
    float value = dot(-LightDir, normalize(input.normal)); // ���� Normal�� ���� ����
	// LightDiffuse * MaterialDiffuse = ���� ���� ǥ���� ���� �޴� ����
	// value * Diffuse = ���� �Ի簢�� ���� ���� �޴� ������ �޶���.
    color = color * value * LightDiffuse * MaterialDiffuse;
	
    return color;
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
