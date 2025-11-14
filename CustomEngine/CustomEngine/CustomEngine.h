#pragma once

#include "resource.h"

// 루트 시그니처 추가
extern ComPtr<ID3D12RootSignature> g_rootSignature;

bool CreateRootSignature(); // 추가
bool InitializeDX12(HINSTANCE hInstance);

extern ComPtr<ID3D12Fence> g_fence;
extern UINT64 g_fenceValue;

// PSO 함수 선언 추가
bool CreatePipelineStateObject();

// 함수 선언
bool CreateVertexBuffers();
void Render();