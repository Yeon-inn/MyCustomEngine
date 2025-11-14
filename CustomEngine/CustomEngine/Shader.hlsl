// =========================================================================
// Input Assembler (Input Structure)
// =========================================================================
struct VSInput
{
    // POSITION 레지스터에 정점 위치 정보가 바인딩됩니다.
    float3 Position : POSITION;
};

// =========================================================================
// Vertex Shader (정점 셰이더)
// =========================================================================
struct VSOutput
{
    // SV_POSITION 레지스터에 최종 클립 공간 좌표가 바인딩됩니다.
    float4 Position : SV_POSITION;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    
    // 정점 위치를 클립 공간 좌표 (Clip Space, -1.0 ~ 1.0)로 그대로 사용합니다.
    output.Position = float4(input.Position, 1.0f);
    
    return output;
}

// =========================================================================
// Pixel Shader (픽셀 셰이더)
// =========================================================================

// float4는 RGBA 색상 값입니다.
float4 PSMain(VSOutput input) : SV_TARGET
{
    // 모든 픽셀을 빨간색 (R=1.0, G=0.0, B=0.0, A=1.0)으로 채색합니다.
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}