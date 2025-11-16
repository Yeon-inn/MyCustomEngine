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

const int g_frameCount = 2; // 더블 버퍼링 사용
extern ComPtr<ID3D12Resource> g_renderTargets[g_frameCount]; // Back Buffer 리소스 저장용

ComPtr<ID3D12DescriptorHeap> g_rtvHeap; // RTV Heap
bool InitializeDX12(HINSTANCE hInstance);

HWND g_hWnd = nullptr;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

ComPtr<ID3D12CommandAllocator> g_commandAllocator = nullptr;
ComPtr<ID3D12GraphicsCommandList> g_commandList = nullptr;

ComPtr<ID3D12RootSignature> g_rootSignature;

ComPtr<ID3D12Fence> g_fence;
UINT64 g_fenceValue = 0;

// PSO 객체 정의 추가
ComPtr<ID3D12PipelineState> g_pipelineState;

struct Vertex
{
    float x, y, z;
};

ComPtr<ID3D12Resource> g_vertexBuffer;
D3D12_VERTEX_BUFFER_VIEW g_vertexBufferView;

ComPtr<ID3D12Resource> g_renderTargets[2] = {};

// ====================================================================
// DX12 전역 변수 (Global Variables)
// ====================================================================
ComPtr<ID3D12Device> g_d3dDevice;               // D3D12 Device
ComPtr<IDXGIFactory4> g_dxgiFactory;            // DXGI Factory
ComPtr<ID3D12CommandQueue> g_commandQueue;      // Command Queue
ComPtr<IDXGISwapChain3> g_swapChain;            // Swap Chain