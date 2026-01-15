// 다른 Shader 파일에서 사용할 내용을 관리

// #pragma once와 같은 코드
#ifndef _GLOBAL_FX_ // _GLOBLA_FX_가 define 안되어 있다면
#define _GLOBAL_FX_ // _GLOBLA_FX_를 define하고 아래 코드 실행

/////////////////////
// Constant Buffer //
/////////////////////

cbuffer TransformBuffer : register(b0)
{
    matrix World; // 월드 변환 행렬
}


cbuffer GlobalBuffer : register(b1)
{
    matrix View; // 뷰 변환 행렬
    matrix Projection; // 투영 변환 행렬
    matrix VP; // 뷰 변환 행렬 * 투영 변환 행렬 곱한 값
    matrix VInv; // 뷰 행렬의 역행렬. 월드좌표계의 카메라 Transform 행렬
}


/////////////////
// Vertex Data //
/////////////////

struct Vertex
{
    float4 position : POSITION;
};

struct VertexTexture
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexColor
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

struct VertexTextureNormal
{ 
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VertexTextureNormalTangent
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexTextureNormalTangentBlend
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 blendIndices : BLEND_INDICES;
    float4 blendWeights : BLEND_WEIGHTS;
};


///////////////////
// Vertex Output //
///////////////////

struct VertexOutput
{
	// SV : System Value
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

// Light 연산(Specular)중 정점의 World 좌표가 필요하여 따로 저장함.
struct MeshOutput
{
	// SV : System Value
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

/////////////
// Sampler //
/////////////

// Sampler에 설정하는 옵션
//  - Filter : 확대 / 축소가 일어났을 때 중간 값을 처리하는 방식
//  - Address : UV가 1보다 컸을때, 나머지 부분을 어떻게 처리할지
SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
	// U좌표가 넘어갔을 때
    AddressU = Wrap;    // 0~1 범위 벗어나면 텍스처 반복
	// V좌표가 넘어갔을 때
    AddressV = Wrap;
};

SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;
	// U좌표가 넘어갔을 때
    AddressU = Wrap;
	// V좌표가 넘어갔을 때
    AddressV = Wrap;
};

/////////////////////
// RasterizerState //
/////////////////////

RasterizerState FillModeWireFrame
{
    FillMode = Wireframe; // 와이퍼 프레임으로 보이도록 설정
};

// 정점의 인덱스 순서가 시계방향이 앞인지 뒤인지 설정
// 반시계 방향이 앞면으로 인식하도록 함. 
// - 기존은 시계방향이 앞으로 되어있음.
// - SkyBox에서 Sphere 안쪽이 Rendering 되도록 설정하기 위함
RasterizerState FrontCounterClockwiseTrue
{
    FrontCounterClockwise = true;
};

/////////////////
// Blend State //
/////////////////

// 일정 비율에 따라 두 색상을 Blend함
// Src : a%, Dest : (100 - a) %
BlendState AlphaBlend
{
    AlphaToCoverageEnable = false;

    BlendEnable[0] = true;
    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = INV_SRC_ALPHA;
    BlendOp[0] = ADD;

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = Zero;
    BlendOpAlpha[0] = Add;

    RenderTargetWriteMask[0] = 15;
};

BlendState AlphaBlendAlphaToCoverageEnable
{
    AlphaToCoverageEnable = true;

    BlendEnable[0] = true;
    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = INV_SRC_ALPHA;
    BlendOp[0] = ADD;

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = Zero;
    BlendOpAlpha[0] = Add;

    RenderTargetWriteMask[0] = 15;
};

// 두개의 색상을 50:50으로 섞음
BlendState AdditiveBlend
{
    AlphaToCoverageEnable = true;

    BlendEnable[0] = true;
    SrcBlend[0] = One;
    DestBlend[0] = One;
    BlendOp[0] = ADD;

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = Zero;
    BlendOpAlpha[0] = Add;

    RenderTargetWriteMask[0] = 15;
};

BlendState AdditiveBlendAlphaToCoverageEnable
{
    AlphaToCoverageEnable = true;

    BlendEnable[0] = true;
    SrcBlend[0] = One;
    DestBlend[0] = One;
    BlendOp[0] = ADD;

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = Zero;
    BlendOpAlpha[0] = Add;

    RenderTargetWriteMask[0] = 15;
};


///////////
// Macro //
///////////
#define PASS_VP(name, vs, ps)						\
pass name											\
{													\
	SetVertexShader(CompileShader(vs_5_0, vs()));	\
	SetPixelShader(CompileShader(ps_5_0, ps()));	\
}

#define PASS_RS_VP(name, rs, vs, ps)				\
pass name											\
{				                                    \
    SetRasterizerState(rs);							\
    SetVertexShader(CompileShader(vs_5_0, vs()));	\
    SetPixelShader(CompileShader(ps_5_0, ps()));	\
}

#define PASS_BS_VP(name, bs, vs, ps)				\
pass name											\
{				                                    \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF);	\
    SetVertexShader(CompileShader(vs_5_0, vs()));	\
    SetPixelShader(CompileShader(ps_5_0, ps()));	\
}

//////////////
// Function //
//////////////

float3 CameraPosition()
{
    return VInv._41_42_43;
}

#endif // _GLOBAL_FX_가 이미 define 되어 있다면 아래 코드 실행
