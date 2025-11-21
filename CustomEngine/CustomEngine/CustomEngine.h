#pragma once
#include <windows.h> // LRESULT, HWND 등 Win32 기본 타입 정의를 위해 필수
#include "resource.h"
#include <DirectXMath.h> // XMFLOAT4X4, XMMATRIX 등의 수학 자료형 정의
#include <DirectXPackedVector.h> // (선택 사항)

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
    float nx, ny, nz;       // Normal (법선)
    float u, v;             // UV (텍스처 좌표)
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

void FlushCommandQueue();
std::wstring OpenFileLoadDialog();

ComPtr<ID3D12Resource> g_indexBuffer;     // 인덱스 버퍼 리소스
D3D12_INDEX_BUFFER_VIEW g_indexBufferView; // 인덱스 버퍼 뷰
UINT g_indexCount;                        // 드로잉에 필요한 인덱스 총 개수

// 파일 대화 상자 함수 (WCHAR*를 반환하도록 수정했음)
WCHAR* OpenFileDialog();

// 모델 로딩 함수 (WCHAR* 경로를 받도록 수정했음)
bool LoadModel(const WCHAR* filePath);

// 전역 변수 정의 (ComPtr<ID3D12Resource>는 C++ 파일 상단 또는 헤더에 정의)
ComPtr<ID3D12Resource> g_constantBuffer;
UINT8* g_constantBufferPtr;


struct ObjectConstants {
    DirectX::XMFLOAT4X4 WorldViewProj; // 64 bytes (Offset 0)
    DirectX::XMFLOAT4 MaterialDiffuse; // 16 bytes (Offset 64)
    DirectX::XMFLOAT4X4 World;         // 64 bytes (Offset 80 - NOT aligned to 16-byte boundary) 
};

struct LightConstants {
    DirectX::XMFLOAT3 LightDirection; // 광원의 방향 (월드 공간)
    float Pad;
    DirectX::XMFLOAT4 LightColor;     // 광원의 색상 (RGBA)
};

ComPtr<ID3D12Resource> g_lightConstantBuffer;
UINT8* g_lightConstantBufferPtr;
// ...

// [추가] Light 데이터 초기값
LightConstants g_mainLight = {
    { -0.5f, -1.0f, -0.5f }, // LightDirection (대각선 위에서 비춤)
    0.0f,                    // Pad
    { 1.0f, 1.0f, 1.0f, 1.0f } // LightColor (흰색)
};