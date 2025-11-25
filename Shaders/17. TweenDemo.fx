#include "00. Global.fx"
#include "00. Light.fx"

// model의 Bone 허용 개수
#define MAX_MODEL_TRANSFROMS 250
// model의 keyframe허용 개수
#define MAX_MODEL_KEYFRAMES 500


// Bone
cbuffer BoneBuffer
{
	// 각각의 Bone이 로컬 좌표계로 변환할 수 있는 행렬들
    matrix BoneTransforms[MAX_MODEL_TRANSFROMS];
};
// 현재 렌더링하고 있는 Bone Index
uint BoneIndex;


// Animation
struct KeyframeDesc
{
    int animIndex; // 현재 재생중인 Animation 인덱스
    uint currFrame; // 현재 재생중인 Frame
    uint nextFrame; // 다음 재생할 Frame
    float ratio; // frame blending 비율

    float sumTime; // 현재 Animation 재생된 시간
    float speed; // 재생 속도
    float2 padding; // 16바이트에 맞추기 위한 Padding
};

// Tween(Transition, 두개의 Animation의 사이를 보간)
struct TweenFrameDesc
{
    float tweenDuration;	// Tween 하는 시간
    float tweenRatio;		// Tween 비율
    float tweenSumTime;		// Tween 경과 시간
    float padding;			// 16byte padding
	
    KeyframeDesc curr;		// 현재 Animation의 frame
	// 다음 Animation을 재생하지 않으면 값을 넣지 않을 수 있음
    KeyframeDesc next;		// 다음 Animation의 frame
};

cbuffer TweenBuffer
{
    TweenFrameDesc TweenFrames;
};
// Animation의 keyframe SRT데이터를 Texture로 변환한 데이터를 저장하는 변수
Texture2DArray TransformMap;


// Frame와 Frame 사이를 보간함
// 정점을 부모 좌표계 -> Local 좌표계 -> Animation Frame 적용 -> 부모좌표계로 변환
// Animation에서 정점에 (Bone에 의해)영향을 주는 SRT행렬을 구함
Matrix GetAnimationMatrix(VertexTextureNormalTangentBlend input)
{
	// 정점에 영향을 주는 bone index
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
	// 정점에 영향을 주는 bone의 가중치
    float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };
	
	// [Animation 내에서 frame간의 보간][Animation간의 frame 보간]
    int animIndex[2];			// 현재 Animation 번호
    int currFrame[2];			// 현재 재생중인 Frame
    int nextFrame[2];			// 다음으로 재생할 Frame
    float ratio[2];				// 현재 Frame와 다음 Frame과의 비율(보간)
	
	// 현재 Animation의 KeyframeDesc
    animIndex[0] = TweenFrames.curr.animIndex;
    currFrame[0] = TweenFrames.curr.currFrame;
    nextFrame[0] = TweenFrames.curr.nextFrame;
    ratio[0] = TweenFrames.curr.ratio;

	// 다음 Animation이 KeyframeDesc
    animIndex[1] = TweenFrames.next.animIndex;
    currFrame[1] = TweenFrames.next.currFrame;
    nextFrame[1] = TweenFrames.next.nextFrame;
    ratio[1] = TweenFrames.next.ratio;
	
	
	// SRT행렬을 Texture2D로 변환해서 저장할때 4byte씩해서 4개로 나눈것을 받기 위함
    float4 c0, c1, c2, c3;	// bone의 SRT행렬을 저장할 변수
    float4 n0, n1, n2, n3;  // bone의 다음 frame SRT행렬을 저장할 변수
	
	matrix curr = 0;		// 4개로 나누어진 값을 행렬로 저장할 변수
    matrix next = 0;		// 다음 frame의 4개로 나누어진 데이터를 행렬로 저장할 변수
	matrix transform = 0;	// 4개의 bone의 가중치 만큼 SRT행렬을 저장할 변수
    
	
	for (int i = 0; i < 4; i++)
	{
		// Load() : 4D좌표로 Texture에서 원하는 데이터를 읽음.
		// Texture로 변환된 SRT행렬 데이터 읽기
		// Load( X축 좌표(가로), Y축 좌표(세로), Z축 좌표(텍스처 배열), w)
		c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));	
		c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));	
		c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));	
		c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
		// 4byte씩 나누어진 값을 행렬로 재조립. i번째 bone의 SRT행렬				
        curr = matrix(c0, c1, c2, c3);										   
																			   														   
        n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
        n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
        n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
        n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
		// 4byte씩 나누어진 값을 행렬로 재조립. i번째 bone의 SRT행렬
        next = matrix(n0, n1, n2, n3);
		
		// 현재 frame과 다음 frame SRT행렬을 ratio 비율만큼 보간함
        matrix result = lerp(curr, next, ratio[0]);
		
		
		// 다음 Animation이 존재하면 
		if(animIndex[1] >= 0)
        {
            // 다음 Animation의 keyframe데이터를 가져옴
            c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
            c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
            c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
            c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
            curr = matrix(c0, c1, c2, c3);

            n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
            n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
            n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
            n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
            next = matrix(n0, n1, n2, n3);

            matrix nextResult = lerp(curr, next, ratio[1]);
            result = lerp(result, nextResult, TweenFrames.tweenRatio);
        }
		
		
		// 조립된 SRT행렬에 가중치만큼 곱한뒤 저장
        transform += mul(weights[i], result);
    }
	
	return transform;
}


// Vertex Shader
// bone으로 이루어진 Mesh의 정점은 부모좌표계에 있는 상태로 전달됨.
MeshOutput VS(VertexTextureNormalTangentBlend input)
{
	MeshOutput output;
	
	// 정점이 해당 frame에 적용되어야할 SRT행렬
    Matrix m = GetAnimationMatrix(input);
	// 정점에 Animation SRT값 적용
    output.position = mul(input.position, m);
	
	
	// Bone의 position을 넘길때 Local로 변환해서 전달하는 것이 아닌
	// GPU에서 변환 행렬과 Index를 분리해서 전달후 조합해서 계산 하는 이유
	// - Animation에서 특정 Bone만 이동을 하면 해당 Bone에 대해서 알아야하기 때문
	
	// 부모 좌표계 -> 로컬 좌표계
	// Mesh가 어느 Bone에 연결되어 있는지 BoneIndex를 통해 접근함
    // output.position = mul(input.position, BoneTransforms[BoneIndex]);

	// 로컬 좌표계 -> 월드 좌표계
    output.position = mul(output.position, World);
    output.worldPosition = output.position.xyz; // World 좌표 저장
    output.position = mul(output.position, VP);

    output.uv = input.uv;
	// 가만히 있는 Normal 좌표는 알고 있지만 정점이 이동하게 되면 Normal값이 변경되기 때문에
	// World 변환 행렬을 곱해야함. 특히 방향에 의해 변화가 이루어짐
	// World을 float3x3으로 변환하면 동차좌표계가 제외된 행렬을 얻을 수 있음
	// - 정점 이동에 사용하는 좌표가 제거됨.
	// - Local → World
    output.normal = mul(input.normal, (float3x3) World);
    output.tangent = mul(input.tangent, (float3x3) World);
	 
	return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
	// ComputeNormalMapping() : Normal Texture를 계산하는 함수
	// input.normal에 값이 변경되어 저장됨.
    // ComputeNormalMapping(input.normal, input.tangent, input.uv);
	
	// ComputeLight() : Light.fx에서 최종적으로 표면의 색상을 정하는 함수
    // float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
	
    return color;
}

float4 PS_RED(MeshOutput input) :SV_Target
{
    return float4(1, 0, 0, 1);
}

// 어떤 Shader를 사용할지 선택할 수 있음
// - technique : 렌더링 기법을 의미하며, 하나 이상의 pass를 포함하는 상위 블록
// - pass : technique 안에 여러 개 존재할 수 있는 하위 실행 단위, 실제로 셰이더 설정, 렌더 상태, 텍스처 등 한 번의 드로우 호출 시 적용될 모든 파이프라인 상태를 정의
technique11 T0
{
	// VS와 PS를 묶음
	PASS_VP(P0, VS, PS)
	
	// 와이어프레임 모드
	PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED)
};
