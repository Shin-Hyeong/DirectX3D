


RWByteAddressBuffer Output; // UnorderedAccessView


struct ComputeInput
{
    // https://learn.microsoft.com/ko-kr/windows/win32/direct3dhlsl/sv-groupindex
    // Dispatch()에 의해 설정된 ThreadGroup에서 몇번 Group를 사용하는지
    uint3 groupID : SV_GroupID;                 
    // numthreads()에 의해 설정된 하나의 Group에 할당된 Thread들 중 몇번 Thread를 사용하는지
    // ThreadGroup마다 동일한 좌표가 존재 함
    uint3 groupThreadID : SV_GroupThreadID;
    // 모든 ThreadGroup의 기준으로 Thread마다의 고유한 3차원 좌표. 모든 ThreadGroup을 통틀어 중복된 좌표 없음
    // (GroupID) * GroupSize + groupThreadID
    uint3 dispatchThreadID : SV_DispatchThreadID;
    // 하나의 Group내에서 사용중 인 Thread의 1차원 좌표
    uint groupIndex : SV_GroupIndex;
};

// GPU에서 사용할 thread갯수 = 10 * 8 * 3 = 240 = 반복문을 240 반복한 것과 비슷
// x = 10, y = 8, z = 3
[numthreads(10, 8 ,3)] 

// GPU에서 연산를 하는 함수
// ComputeShader : 그래픽스 파이프라인과 독립적으로 범용 병렬 계산을 수행하는 셰이더. 
// 렌더링(버텍스/픽셀 셰이더) 대신 물리 시뮬레이션, 파티클, 이미지 처리 등 대량 데이터 연산에 특화
void CS(ComputeInput input)
{
    uint index = input.groupIndex;
    // uint = 4byte. 
    // ComputeInput이 uint3, uint3, uint3, uint = uint가 10개 = 4byte * 10
    // 주소의 offset. 하나의 데이터가 40byte이기때문에 데이터 Index에 따라 주소를 넘김
    uint outAddress = index * 10 * 4;
    
    // 이전 구조체 멤버들의 byte 크기만큼 outAddress + N을 함
    Output.Store3(outAddress + 0, input.groupID);
    Output.Store3(outAddress + 12, input.groupThreadID);
    Output.Store3(outAddress + 24, input.dispatchThreadID);
    Output.Store(outAddress + 36, input.groupIndex);
    
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
