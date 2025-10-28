#include "00. Global.fx"
#include "00. Light.fx"

// Vertex Shader
MeshOutput VS(VertexTextureNormalTangent input)
{
	MeshOutput output;
    output.position = mul(input.position, World);
    output.worldPosition = output.position.xyz; // World ��ǥ ����
    output.position = mul(output.position, VP);

    output.uv = input.uv;
	// ������ �ִ� Normal ��ǥ�� �˰� ������ ������ �̵��ϰ� �Ǹ� Normal���� ����Ǳ� ������
	// World ��ȯ ����� ���ؾ���. Ư�� ���⿡ ���� ��ȭ�� �̷����
	// World�� float3x3���� ��ȯ�ϸ� ������ǥ�谡 ���ܵ� ����� ���� �� ����
	// - ���� �̵��� ����ϴ� ��ǥ�� ���ŵ�.
	// - Local �� World
    output.normal = mul(input.normal, (float3x3) World);
    output.tangent = mul(input.tangent, (float3x3) World);
	 
	return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
	// ComputeNormalMapping() : Normal Texture�� ����ϴ� �Լ�
	// input.normal�� ���� ����Ǿ� �����.
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
	
	// ComputeLight() : Light.fx���� ���������� ǥ���� ������ ���ϴ� �Լ�
    float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
	
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
