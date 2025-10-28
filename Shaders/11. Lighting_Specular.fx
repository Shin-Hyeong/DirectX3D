#include "00. Global.fx"

float4 LightSpecular;
float4 MaterialSpecular;
float3 LightDir;

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

// Specular(�ݻ籤)
// - �ѹ������� ������ �ݻ�� ��(Phong)
// - ���� �ݻ�� �������� ī�޶� ������ �ش� ������ ��� ����

float4 PS(MeshOutput input) : SV_TARGET
{
	// ���� �ݻ� ���� ���� (R�� R2�� ���� ��. �ٸ� ��)
    float3 R = normalize(reflect(LightDir, input.normal));
    float3 R2 = LightDir - (2 * input.normal * dot(LightDir, input.normal));
	
	// ī�޶� ��ġ
    float3 cameraPosition = -View._41_42_43; // View����� 41(x), 42(y), 43(z)�� ����� = ī�޶� ��ġ

	// �������� ī�޶� ���ϴ� ���⺤��
    float3 E = normalize(cameraPosition - input.worldPosition);
	
	// ī�޶� ���⺤�Ϳ� �ݻ簢�� ����
    float value = saturate(dot(R, E)); // saturate �� Clamp(0 ~ 1)
	
	// Specular�� ��. ���� �ظ��ϰ� ���̱� ���� ������ �Ѵ�. 
	// ���� �������� ������ ������ ������. (0 ~ 1)�� ���� ������ ���� ���� ���� ��.
    float specular = pow(value, 10); // value�� 10����

    // ���������� ǥ���Ǵ� color
    float4 color = LightSpecular * MaterialSpecular * specular;
	
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
