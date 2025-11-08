// Light의 공통 쉐이더

// #pragma once같은 기능
#ifndef _LIGHT_FX_
#define _LIGHT_FX

#include "00. Global.fx"

////////////
// Struct //
////////////

// 광원에 대한 정보
struct LightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float3 direction;
    float padding; // dirction이 float3이기 때문에 남는 4byte를 채움. 더미값
};

// 물체의 Material의 정보
struct MaterialDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
};


/////////////////////
// Constant Buffer //
/////////////////////

// 광원의 Desc에 대한 ConstantBuffer
cbuffer LightBuffer : register(b3)
{
    LightDesc GlobalLight;
};

// 물체마다의 Material에 대한 ConstantBuffer
cbuffer MaterialBuffer : register(b2)   
{
    MaterialDesc Material;
};


//////////////////////////
// Shader Resource View //
//////////////////////////

Texture2D DiffuseMap : register(t0);
Texture2D SpecularMap : register(t1);
Texture2D NormalMap : register(t2);


//////////////
// Funciton //
//////////////

// Ambient, Diffuse, Specular, Emissive을 통합하여 Light 계산
float4 ComputeLight(float3 normal, float2 uv, float3 worldPosition)
{ 
    float4 ambientColor = 0;    // Ambient의 결과물
    float4 diffuseColor = 0;    // Diffuse의 결과물
    float4 specularColor = 0;   // Specular의 결과물
    float4 emissiveColor = 0;   // Emissive의 결과물
    
    // Ambient : 표면의 전체적으로 빛추게 되는 밝기
    {
        float4 color = GlobalLight.ambient * Material.ambient;
	
        ambientColor = DiffuseMap.Sample(LinearSampler, uv) * color;
    }
    
    // Diffuse : 빛의 입사 방향벡터의 역방향과 표면의 Normal이 가까울 수록 밝음
    {
        float4 color = DiffuseMap.Sample(LinearSampler, uv);
	    // Lambert 공식
        float value = dot(-GlobalLight.direction, normalize(normal));
        
        diffuseColor = color * value * GlobalLight.diffuse * Material.diffuse;
    }
    
    // Specular : 빛의 반사 방향벡터와 카메라의 방향이 같으면 표면이 빛나도록 보임
    {
        // 빛의 반사 방향 벡터
        float3 R = normalize(reflect(GlobalLight.direction, normal));
        // 표면에서 카메라의 방향 벡터
        float3 E = normalize(CameraPosition() - worldPosition);
	    // 카메라 방향벡터와 반사각의 내적
        float value = saturate(dot(R, E)); // saturate → Clamp(0 ~ 1)
        float specular = pow(value, 10); // value의 10제곱

        specularColor = GlobalLight.specular * Material.specular * specular;
    }
    
    // Emissive
    {
	    // 표면에서 카메라를 향하는 방향 벡터
        float3 E = normalize(CameraPosition() - worldPosition);
	    // Normal과 카메라 방향 벡터의 내적. Clamp(0 ~ 1)
        float value = saturate(dot(E, normal));
	    // 0 → 1, 1 → 0으로 변경. Normal과 수직을 이루면 1의 값을 가지게 됨.
        float emissive = 1.0f - value;
	    // smoothstep() : 선형 그래프를 부드럽게 곡선으로 변경해줌
	    // 외각선에서 멀어질수록 자연스럽게 어두워짐
        emissive = smoothstep(0.0f, 1.0f, emissive);
	    // 외각선 강조. 색상 범위가 외각선에 가까워짐
        emissive = pow(emissive, 2); // emissive의 2제곱
	
        emissiveColor = GlobalLight.emissive * Material.emissive * emissive;
    }
    
    return ambientColor + diffuseColor + specularColor + emissiveColor;
}

// Normal Map을 적용하는 함수
// inout : 입력과 출력이 동시에 되는 함수, C++에서 &와 비슷함
void ComputeNormalMapping(inout float3 normal, float3 tangent, float2 uv)
{
    // Normal Mpa [0 ~ 255]  -> Sampling -> [0 ~ 1]로 변환
    float4 map = NormalMap.Sample(LinearSampler, uv);
    
    if (any(map.rgb) == false) // map의 값이 없으면 반환
        return;

    float3 N = normalize(normal);       // z
    float3 T = normalize(tangent);      // x
    float3 B = normalize(cross(N, T));  // y
    float3x3 TBN = float3x3(T, B, N);   // Tangent Space -> World 변환 행렬

    // [0 ~ 1] 범위에서 [-1 ~ 1] 범위로 변환
    // Tangent Space 좌표계에 있는 Normal Map 값
    float3 tangentSpaceNormal = (map.rgb * 2.0f - 1.0f); 
    // World 좌표계로 변환
    float3 worldNormal = mul(tangentSpaceNormal, TBN);
    
    normal = worldNormal;
}

#endif