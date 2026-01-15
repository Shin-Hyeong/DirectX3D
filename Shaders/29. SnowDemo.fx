#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"

// Snow Billboard의 ConstantBuffer
cbuffer SnowBuffer
{
    float4 Color;
    
    float3 Velocity;
    float DrawDistance;
    
    float3 Origin;
    float Turbulence;
    
    float3 Extent;
    float Time;
};

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float2 scale : SCALE;
    float2 random : RANDOM;
};

struct V_OUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float alpha : ALPHA;
};

V_OUT VS(VertexInput input)
{
    V_OUT output;
    
    // 눈의 이동했는지
    float3 displace = Velocity * Time * 100;

    // 눈
    // input.position.y += displace;
    // 눈이 일정 범위이상 내려오면 다시 위로 올라가도록 루프시킴
    input.position.y = Origin.y + Extent.y - (input.position.y - displace) % Extent.y;
    input.position.x += cos(Time - input.random.x) * Turbulence; // x축으로 흔들리도록 함
    input.position.z += cos(Time - input.random.y) * Turbulence; // y축으로 흔들리도록 함
    // input.position.xyz = Origin + (Extent + (input.position.xyz + displace) % Extent) % Extent - (Extent * 0.5f);
    
    float4 position = mul(input.position, World);
    
    float3 up = float3(0, 1, 0);
    float3 forward = position.xyz - CameraPosition();
    float3 right = normalize(cross(up, forward));
    
    // 정점을 재배치함
    position.xyz += (input.uv.x - 0.5f) * right * input.scale.x;        // X(-0.5 ~ 0.5)
    position.xyz += (1.0f - input.uv.y - 0.5f) * up * input.scale.y;    // Y(-0.5 ~ 0.5 )
    position.w = 1.0f;
    
    output.position = mul(mul(position, View), Projection);
    output.uv = input.uv;
    
    output.alpha = 1.0f;

    // Alpha Blending
    float4 view = mul(position, View);
    // saturate() : 0 ~ 1사이값으로 Clamp
    output.alpha = saturate(1 - view.z / DrawDistance) * 0.8f;
    
    return output;
}

float4 PS(V_OUT input) : SV_Target
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.uv);
    
    // alpha값에 따라 색상을 보간시킴
    diffuse.rgb = Color.rgb * input.alpha * 2.0f;
    diffuse.a = diffuse.a * input.alpha * 1.5f;
    
    return diffuse;
}

technique11 T0
{
    PASS_BS_VP(P0, AlphaBlend, VS, PS)
};