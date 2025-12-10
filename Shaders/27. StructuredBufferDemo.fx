
struct InputDesc
{
    matrix input;
};

struct OutputDesc
{
    matrix result;
};


StructuredBuffer<InputDesc> Input;      // ShaderResourceView
RWStructuredBuffer<OutputDesc> Output;  // UnorderedAccessView

// 하나의 ThreadGroup내에 Thread갯수
[numthreads(500, 1 ,1)] 

// SV_DispatchThreadID : 전체 Dispatch 범위에서 스레드의 고유한 3차원 Index
void CS(uint id : SV_GroupIndex)
{
    // Input(StructuredBuffer)에 id번째에 있는 데이터를 연산
    matrix result = Input[id].input * 2;
    
    Output[id].result = result;
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
