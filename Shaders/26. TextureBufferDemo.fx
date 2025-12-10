
Texture2DArray<float4> Input;       // ShaderResourceView
RWTexture2DArray<float4> Output;    // UnorderedAccessView

// 하나의 ThreadGroup내에 Thread갯수
[numthreads(32, 32 ,1)] 


// SV_DispatchThreadID : 전체 Dispatch 범위에서 스레드의 고유한 3차원 Index
void CS(uint3 id : SV_DispatchThreadID)
{
    // Input에서 연산해야하는 id 위치의 데이터 가져오기
    float4 color = Input.Load(int4(id, 0));
    
    Output[id] = 1.0f - color;  // 색상 반전
    // Output[id] = (color.r, color.g, color.b) / 3.0f;    // 회색풍으로 변경
}


// 어떤 Shader를 사용할지 선택할 수 있음
// - technique : 렌더링 기법을 의미하며, 하나 이상의 pass를 포함하는 상위 블록
// - pass : technique 안에 여러 개 존재할 수 있는 하위 실행 단위, 실제로 셰이더 설정, 렌더 상태, 텍스처 등 한 번의 드로우 호출 시 적용될 모든 파이프라인 상태를 정의
technique11 T0
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CS()));
    }
};
