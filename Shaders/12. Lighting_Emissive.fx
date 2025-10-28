#include "00. Global.fx"

float4 MaterialEmissive; // �ܰ��� ����

// Vertex Shader
MeshOutput VS(VertexTextureNormal input)
{
	MeshOutput output;
    output.position = mul(input.position, World);
    output.worldPosition = output.position; // World ��ǥ ����
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

// Emissive (������Ʈ)
// - �ܰ����� ���� �� �ַ� �����
// - Normal���� 1�̸� ��ȭ ���� 0�� �������� ��ȭ�� �Ͼ


float4 PS(MeshOutput input) : SV_TARGET
{
	// ī�޶� ��ġ
    float3 cameraPosition = -View._41_42_43; // View����� 41(x), 42(y), 43(z) = ī�޶� ��ġ

	// ǥ�鿡�� ī�޶� ���ϴ� ���� ����
    float3 E = normalize(cameraPosition - input.worldPosition);
	
	// Normal�� ī�޶� ���� ������ ����. Clamp(0 ~ 1)
    float value = saturate(dot(E, input.normal));
	// 0 �� 1, 1 �� 0���� ����
	// Normal�� ������ �̷�� 1�� ���� ������ ��.
    float emissive = 1.0f - value; 
	
	// smoothstep() : ���� �׷����� �ε巴�� ����� ��������
	// �ܰ������� �־������� �ڿ�������
    emissive = smoothstep(0.0f, 1.0f, emissive);
	
	// �ܰ��� ����. ���� ������ �ܰ����� �������
    emissive = pow(emissive, 2);
	
    float4 color = MaterialEmissive * emissive;
	
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
