// =========================================================================
// Input Assembler (Input Structure)
// =========================================================================
// Shader.hlsl 상단에 추가 (b0 레지스터와 연결)
cbuffer cbObject : register(b0)
{
    float4x4 gWVP; // 64 bytes
    float4 gMaterialDiffuse; // 16 bytes (Offset 64)
    float4x4 gWorld; // 64 bytes (Offset 80)
};

// Light Constant Buffer (b1 레지스터와 연결)
// =========================================================================
cbuffer cbLight : register(b1)
{
    float3 gLightDirection; // 광원의 방향
    float Padding; // 4 bytes 패딩 추가 (12 + 4 = 16바이트 정렬)
    float4 gLightColor; // 광원의 색상
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

// VSMain 함수 수정
VSOutput VSMain(VSInput input)
{
    VSOutput output;
    
    // ... (위치 변환 코드는 그대로 유지)
    float4 localPos = float4(input.position, 1.0f);
    output.Position = mul(localPos, gWVP);
    
    // =========================================================================
    // [수정된 부분] 법선 벡터를 월드 공간으로 변환
    // =========================================================================
    // 법선 벡터는 위치 벡터와 달리 변환 행렬의 3x3 부분(회전/스케일)만 사용합니다.
    // 하지만 일반적인 행렬 곱셈을 사용하되, W 성분은 0.0f로 설정합니다.
    float3 worldNormal = mul(float4(input.normal, 0.0f), gWorld).xyz;

// 법선 벡터가 올바른 길이(1)를 가지도록 반드시 정규화해야 조명 계산이 정확해집니다.
    output.Normal = normalize(worldNormal);
    
    output.TexCoord = input.texcoord;
    
    return output;
}

// =========================================================================
// Pixel Shader (픽셀 셰이더)
// =========================================================================

float4 PSMain(VSOutput input) : SV_TARGET
{
    // 1. 법선 벡터를 정규화합니다.
    float3 normal = normalize(input.Normal);

    // 2. 확산(Diffuse) 조명 계산 (Lambertian 모델)
    // -gLightDirection 사용 (빛의 반대 방향)
    float diffuseFactor = max(0.0f, dot(-gLightDirection, normal));

    // 3. 재질 색상과 조명 기여도를 결합
    float3 ambient = float3(0.3f, 0.3f, 0.3f); // 0.1f 대신 0.3f로 증가
    
    // 최종 색상 = 재질 색상 * (주변광 + 광원 색상 * 확산 계수)
    float3 finalColor = gMaterialDiffuse.rgb * (ambient + gLightColor.rgb * diffuseFactor);

    return float4(finalColor, gMaterialDiffuse.a);
}