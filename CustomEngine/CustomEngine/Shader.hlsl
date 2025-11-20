// =========================================================================
// Input Assembler (Input Structure)
// =========================================================================
// Shader.hlsl 상단에 추가 (b0 레지스터와 연결)
cbuffer cbObject : register(b0)
{
    float4x4 gWVP; // World-View-Projection 행렬
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL; // Input Layout의 "NORMAL" 시맨틱과 일치해야 함
    float2 texcoord : TEXCOORD; // Input Layout의 "TEXCOORD" 시맨틱과 일치해야 함
};

// =========================================================================
// Vertex Shader (정점 셰이더)
// =========================================================================
// ====================================================
// 2. VSOutput 구조체 (픽셀 셰이더로 전달할 데이터 정의)
// ====================================================
struct VSOutput
{
    // VS에서 PS로 전달하는 필수 요소
    float4 Position : SV_POSITION; // 최종 클립 공간 위치
    // 모델 로딩을 위해 추가해야 하는 요소 (PS에서 사용할 수 있도록)
    float3 Normal : TEXCOORD0;
    float2 TexCoord : TEXCOORD1; // 텍스처 좌표 (PS로 전달)
};

// ====================================================
// 3. VSMain 함수 수정
// ====================================================
VSOutput VSMain(VSInput input)
{
    VSOutput output;
    
    // WVP 행렬을 사용하지 않고, 로컬 좌표를 직접 축소하여 클립 공간 근처로 이동
    // 0.01f는 1/100 크기로 축소합니다. 모델이 화면에 맞게 보이도록 이 값을 조정하세요.
    float scaleFactor = 0.1f;
    float3 scaledPos = input.position * scaleFactor;
    
    // 축소된 위치를 클립 공간 위치로 사용합니다.
    output.Position = float4(scaledPos, 1.0f);
    
    // 나머지 데이터 전달
    output.Normal = input.normal;
    output.TexCoord = input.texcoord;
    
    return output;
}

// =========================================================================
// Pixel Shader (픽셀 셰이더)
// =========================================================================

float4 PSMain(VSOutput input) : SV_TARGET
{
    // **임시로 법선 정보를 색상으로 시각화**
    // (이후 텍스처 로드 및 조명 코드를 추가해야 합니다.)
    float3 finalColor = input.Normal * 0.5f + 0.5f; // 법선 벡터를 0~1 범위로 매핑

    return float4(finalColor, 1.0f);
    
    // **또는 단순한 단색 출력**
    // return float4(1.0f, 1.0f, 1.0f, 1.0f); // 흰색
}