#include <windows.h> // LRESULT, HWND 등 Win32 기본 타입 정의를 위해 필수
#include <commdlg.h> // 파일 대화 상자 사용을 위한 헤더
#include <string>
#include <iostream>

// DX12 관련 헤더
#include <dxgi1_6.h>        // DXGI 팩토리 및 어댑터 열거
#include <d3d12.h>          // D3D12 핵심
#include <D3Dcompiler.h>    // 셰이더 컴파일 (나중에 사용)
#include "d3dx12.h" 

// COM 스마트 포인터 사용을 위한 템플릿 (MS에서 제공)
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "framework.h"
#include "CustomEngine.h"

// ImGui Core
#include "imgui.h" 
#include "imgui_internal.h" // 일부 내부 정의를 위해 필요할 수 있습니다.

// ImGui Backends
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

#include <assimp/Importer.hpp> // Assimp Importer 클래스
#include <assimp/scene.h>      // aiScene 구조체
#include <assimp/postprocess.h> // 파싱 후 처리 플래그

#include <DirectXMath.h> // XMFLOAT4X4, XMMATRIX 등의 수학 자료형 정의
#include <DirectXPackedVector.h> // (선택 사항)
using namespace DirectX;


IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
ComPtr<ID3D12DescriptorHeap> g_imguiSrvHeap; // ImGui 폰트 텍스처용 Heap   

std::string WstringToString(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CUSTOMENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    if (!InitializeDX12(hInstance))
    {
        // 초기화 실패 시
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CUSTOMENGINE));


    // 기본 메시지 루프입니다:
    MSG msg = {};

    // 기본 메시지 루프: GetMessage 대신 PeekMessage를 사용합니다.
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // 렌더링 코드 위치: 창을 계속 유지하는 지속적인 작업
            Render();
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CUSTOMENGINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CUSTOMENGINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.


   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

// DX12 초기화 함수
bool InitializeDX12(HINSTANCE hInstance)
{
    // ------------------------------------
    // 1. 디버그 레이어 활성화 (디버깅 시 필수)
    // ------------------------------------
#if defined(DEBUG) || defined(_DEBUG) 
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }
#endif

    // ------------------------------------
    // 2. DXGI Factory 생성
    // ------------------------------------
    // DXGI Factory는 GPU와 드라이버 정보를 열거하는 객체입니다.
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&g_dxgiFactory));
    if (FAILED(hr)) return false;

    // ------------------------------------
    // 3. Device 및 Command Queue 생성
    // ------------------------------------
    ComPtr<IDXGIAdapter1> hardwareAdapter;
    
    // 기본 어댑터(GPU)를 찾아 Device 생성
    hr = D3D12CreateDevice(
        nullptr,                    // nullptr: 기본 어댑터 사용
        D3D_FEATURE_LEVEL_11_0,     // 최소 피처 레벨
        IID_PPV_ARGS(&g_d3dDevice));

    if (FAILED(hr))
    {
        // 만약 기본 어댑터 생성에 실패하면 WARP (소프트웨어) 어댑터 사용 시도
        ComPtr<IDXGIAdapter> warpAdapter;
        g_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));

        hr = D3D12CreateDevice(
            warpAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&g_d3dDevice));

        if (FAILED(hr)) return false;
    }
    
    // Command Queue 설정
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // 직접(Direct) 명령 목록
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    
    // Command Queue 생성
    hr = g_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_commandQueue));
    if (FAILED(hr)) return false;

    // ------------------------------------
    // 4. Swap Chain 및 RTV Descriptor Heap 생성
    // ------------------------------------

    // 4-1. 창 크기 계산 및 Swap Chain 설정
    RECT clientRect;
    GetClientRect(g_hWnd, &clientRect); // 창의 현재 클라이언트 영역 크기를 가져옵니다.
    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    // 1. Swap Chain 구조체 이름 변경 (DESC -> DESC1)
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

    // 2. BufferDesc 제거 및 필드를 최상위 멤버로 이동
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;

    // 4. Format 설정 (BufferDesc.Format -> Format)
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // 5. SampleDesc는 그대로 최상위 멤버입니다.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // 6. 나머지 멤버는 그대로 사용합니다.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2; // 더블 버퍼링 (Back Buffer 1개 + Front Buffer 1개)
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // Flip 모델 사용
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    // 7. DXGI_SWAP_CHAIN_DESC1에 추가된 필수 필드 (AlphaMode)
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

    // 4-2. Swap Chain 생성
    // IDXGIFactory4::CreateSwapChain 대신 IDXGIFactory4::CreateSwapChainForHwnd 사용
    ComPtr<IDXGISwapChain1> swapChain;
    hr = g_dxgiFactory->CreateSwapChainForHwnd(
        g_commandQueue.Get(),  // Swap Chain은 Command Queue를 통해 프레젠트됨
        g_hWnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain);

    if (FAILED(hr)) return false;

    // 생성된 IDXGISwapChain을 IDXGISwapChain3로 캐스팅합니다.
    hr = swapChain.As(&g_swapChain);
    if (FAILED(hr)) return false;


    // 4-3. RTV Descriptor Heap 생성
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 버퍼 수만큼 RTV가 필요
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    hr = g_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_rtvHeap));
    if (FAILED(hr)) return false;


    // 4-4. RTV 생성 및 핸들 저장 (다음 단계에서 자세히 다룰 예정)
    // RTV (Render Target View)는 렌더링할 메모리(Swap Chain의 버퍼)를 가리키는 뷰입니다.

    // ... 이어서 RTV 생성 및 Command Allocator/List 생성 코드가 필요합니다.

    if (!CreateRootSignature())
    {
        return false;
    }

    if (!CreatePipelineStateObject())
    {
        return false;
    }

    if (!CreateVertexBuffers()) return false;

    // Command Allocator 생성: 명령을 기록할 메모리 할당자
    hr = g_d3dDevice->CreateCommandAllocator( // 🚨 g_commandAllocator 생성
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&g_commandAllocator));
    if (FAILED(hr)) return false;

    // Command List 생성: GPU에 보낼 명령어를 기록하는 목록
    hr = g_d3dDevice->CreateCommandList(
        0, // 노드 마스크
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        g_commandAllocator.Get(), // Command Allocator와 연결
        nullptr, // 초기 PSO 
        IID_PPV_ARGS(&g_commandList));
    if (FAILED(hr)) return false;

    // 생성 후 Command List를 닫아둡니다. (명령 기록은 렌더링 루프에서 시작)
    g_commandList->Close();

    // Fence 생성: CPU-GPU 동기화를 위한 객체
    hr = g_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)); // 🚨 g_fence 생성
    if (FAILED(hr)) return false;

    // 1. Constant Buffer 크기 정의 (256바이트 정렬)
    // D3D12는 상수 버퍼를 256바이트 단위로 정렬해야 합니다.
    const UINT cbSize = sizeof(ObjectConstants);
    const UINT alignedCbSize = (cbSize + 255) & ~255;

    // 2. Upload Heap에 Constant Buffer 리소스 생성
    const CD3DX12_HEAP_PROPERTIES cbHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    const CD3DX12_RESOURCE_DESC cbResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(alignedCbSize);

    hr = g_d3dDevice->CreateCommittedResource(
        &cbHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &cbResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&g_constantBuffer));
    if (FAILED(hr)) return false;

    // 3. Constant Buffer 초기화 및 데이터 복사 (Identity 행렬)
    ObjectConstants objectConstants;

    // Identity 행렬 생성 및 저장
    DirectX::XMStoreFloat4x4(&objectConstants.WorldViewProj, DirectX::XMMatrixIdentity());

    // 4. GPU 메모리에 매핑 및 데이터 복사
    // g_constantBufferPtr은 CPU가 GPU 메모리를 직접 가리키는 포인터가 됩니다.
    hr = g_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&g_constantBufferPtr));
    if (FAILED(hr)) return false;

    // 초기 Identity 행렬 데이터 복사
    memcpy(g_constantBufferPtr, &objectConstants, cbSize);

    UINT rtvDescriptorSize = g_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    // Swap Chain의 각 버퍼에 대한 RTV를 생성합니다.
    for (UINT i = 0; i < g_frameCount; i++)
    {
        //1. Swap Chain에서 i번째 Back Buffer 리소스 가져오기 및 전역 변수에 저장
        HRESULT hr = g_swapChain->GetBuffer(i, IID_PPV_ARGS(&g_renderTargets[i]));
        if (FAILED(hr)) return false;

        // 2. 해당 리소스에 대한 RTV를 생성하여 Heap에 기록합니다.
        g_d3dDevice->CreateRenderTargetView(g_renderTargets[i].Get(), nullptr, rtvHandle);

        // 3. 다음 RTV를 만들 위치로 핸들을 이동합니다.
        rtvHandle.Offset(1, rtvDescriptorSize);
    }

    // (1) ImGui용 SRV Descriptor Heap 생성 (폰트 텍스처용)
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.NumDescriptors = 1;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    hr = g_d3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&g_imguiSrvHeap));
    if (FAILED(hr)) return false;

    // (2) ImGui Core 및 Win32 백엔드 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGui::StyleColorsDark(); // (선택 사항)

    ImGui_ImplWin32_Init(g_hWnd); // Win32 창 핸들 연결

    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // (3) ImGui DX12 백엔드 연결
    ImGui_ImplDX12_Init(g_d3dDevice.Get(),
        g_frameCount,
        DXGI_FORMAT_R8G8B8A8_UNORM, // Swap Chain 포맷
        g_imguiSrvHeap.Get(),
        g_imguiSrvHeap->GetCPUDescriptorHandleForHeapStart(),
        g_imguiSrvHeap->GetGPUDescriptorHandleForHeapStart());

    return true; // 성공적으로 초기화 완료
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    // 1. ImGui 입력 처리기 호출 (가장 먼저 실행하여 메시지 선점)
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

bool CreateRootSignature()
{
    CD3DX12_ROOT_PARAMETER slotRootParameter[1];
    slotRootParameter[0].InitAsConstantBufferView(0); // b0 레지스터에 바인딩

    // 현재는 아무런 자원(Constant Buffer, Texture 등)을 사용하지 않으므로, 
    // 빈(Empty) 루트 시그니처를 생성합니다.

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = 1;              // 자원 파라미터 0개
    rootSignatureDesc.pParameters = slotRootParameter;
    rootSignatureDesc.NumStaticSamplers = 0;          // 정적 샘플러 0개
    rootSignatureDesc.pStaticSamplers = nullptr;

    rootSignatureDesc.pParameters = slotRootParameter;

    // 이 플래그는 버텍스 셰이더만 사용하는 가장 간단한 경우입니다.
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;

    HRESULT hr = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &signature,
        &error);

    if (FAILED(hr))
    {
        // 시리얼라이즈 실패 시 오류 메시지를 출력합니다. (디버깅용)
        if (error)
        {
            OutputDebugStringA((char*)error->GetBufferPointer());
        }
        return false;
    }

    // 시리얼라이즈된 데이터를 기반으로 루트 시그니처 객체를 Device에 생성합니다.
    hr = g_d3dDevice->CreateRootSignature(
        0,
        signature->GetBufferPointer(),
        signature->GetBufferSize(),
        IID_PPV_ARGS(&g_rootSignature));

    return SUCCEEDED(hr);
}

// 셰이더 컴파일을 위한 헬퍼 함수
ComPtr<ID3DBlob> CompileShader(
    const std::wstring& filename,
    const D3D_SHADER_MACRO* defines,
    const std::string& entrypoint,
    const std::string& target)
{
    UINT compileFlags = 0;
#if defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ComPtr<ID3DBlob> byteCode = nullptr;
    ComPtr<ID3DBlob> errors;

    HRESULT hr = D3DCompileFromFile(
        filename.c_str(),
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entrypoint.c_str(),
        target.c_str(),
        compileFlags,
        0,
        &byteCode,
        &errors);

    if (FAILED(hr))
    {
        if (errors)
        {
            // 오류가 발생하면 Visual Studio 출력창에 메시지 출력
            OutputDebugStringA((char*)errors->GetBufferPointer());
        }
        return nullptr;
    }
    return byteCode;
}


bool CreatePipelineStateObject()
{
    // ------------------------------------
    // 1. 셰이더 컴파일
    // ------------------------------------
    std::wstring shaderPath = L"Shader.hlsl";

    ComPtr<ID3DBlob> vsByteCode = CompileShader(shaderPath, nullptr, "VSMain", "vs_5_0");
    ComPtr<ID3DBlob> psByteCode = CompileShader(shaderPath, nullptr, "PSMain", "ps_5_0");

    if (!vsByteCode || !psByteCode) return false;

    // ------------------------------------
    // 2. Input Layout 정의 (Input Assembler 단계 설정)
    // ------------------------------------
    // 정점 데이터 구조를 정의합니다. (VSInput struct와 일치해야 합니다.)
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0} // 🚨 R32G32_FLOAT 확인
    };


    // ------------------------------------
    // 3. PSO (Pipeline State Object) 구조체 작성
    // ------------------------------------
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.pRootSignature = g_rootSignature.Get(); // 앞서 만든 Root Signature 사용

    // 셰이더 설정
    psoDesc.VS = { reinterpret_cast<UINT8*>(vsByteCode->GetBufferPointer()), vsByteCode->GetBufferSize() };
    psoDesc.PS = { reinterpret_cast<UINT8*>(psByteCode->GetBufferPointer()), psByteCode->GetBufferSize() };

    // Input Layout 설정
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };

    // Rasterizer State 설정 (기본값)
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    // Blend State 설정 (기본값)
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

    // Depth/Stencil State 설정 (깊이 버퍼를 사용하지 않음)
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE; // 깊이 버퍼 사용 안 함
    psoDesc.DepthStencilState.StencilEnable = FALSE;

    // 기타 설정
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    // ------------------------------------
    // 4. PSO 객체 생성
    // ------------------------------------
    HRESULT hr = g_d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_pipelineState));

    return SUCCEEDED(hr);
}

bool CreateVertexBuffers()
{
    // 삼각형 좌표 데이터 (클립 공간 좌표)
    Vertex vertices[] =
    {
        { 0.0f, 0.5f, 0.0f },    // 상단
        { 0.5f, -0.5f, 0.0f },   // 우하단
        { -0.5f, -0.5f, 0.0f }   // 좌하단
    };

    const UINT vertexBufferSize = sizeof(vertices);

    const CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    const CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    // 1. Upload Heap에 버퍼 생성
    HRESULT hr = g_d3dDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&g_vertexBuffer));
    if (FAILED(hr)) return false;

    // 2. GPU 메모리에 데이터 복사
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    g_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    memcpy(pVertexDataBegin, vertices, vertexBufferSize);
    g_vertexBuffer->Unmap(0, nullptr);

    // 3. Vertex Buffer View 설정 (Input Assembler에게 데이터 구조를 알림)
    g_vertexBufferView.BufferLocation = g_vertexBuffer->GetGPUVirtualAddress();
    g_vertexBufferView.StrideInBytes = sizeof(Vertex);
    g_vertexBufferView.SizeInBytes = vertexBufferSize;

    return true;
}

void Render()
{
    // ----------------------------------------------------
    // 1. ImGui 프레임 시작 및 UI 정의 (CPU 작업 - Command Reset 이전에 수행)
    // ----------------------------------------------------
    // **ImGui가 입력 및 UI 데이터를 준비하는 단계**
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // UI 정의 (화면에 표시될 내용)
    ImGui::Begin("DX12 Editor");
    ImGui::Text("Render Loop is Active.");
    // 여기에 File Load 버튼과 ImGuizmo 코드가 들어갈 예정입니다.
    if (ImGui::Button("Load FBX Model"))
    {
        // ⚠️ TODO: 파일 대화 상자 함수 호출
        std::wstring filePath = OpenFileLoadDialog();
        WCHAR* filePathRaw = OpenFileDialog();

        if (!filePath.empty())
        {
            // TODO: (다음 스텝) Assimp 로딩 함수를 호출하고 filePath를 인자로 전달
            OutputDebugStringW(L"Selected file: ");
            OutputDebugStringW(filePath.c_str());
            OutputDebugStringW(L"\n");

            // 2. LoadModel 함수 호출 (LoadModel은 WCHAR*를 인자로 받아야 합니다.)
            if (LoadModel(filePathRaw))
            {
                // 성공 시 GPU 동기화
                FlushCommandQueue();
            }

            // 🚨 3. 중요: _wcsdup가 할당한 힙 메모리를 해제합니다.
            free(filePathRaw);

            // LoadModel(filePath); // <- 최종적으로 여기에 FBX 로더가 연결됩니다.
        }
    }
    ImGui::End();

    // ImGui 렌더링 준비 (Draw Data를 최종 생성)
    ImGui::Render();


    // ----------------------------------------------------
    // 2. Command List Reset 및 파이프라인 설정 (DX12 Setup)
    // ----------------------------------------------------
    g_commandAllocator->Reset();
    g_commandList->Reset(g_commandAllocator.Get(), g_pipelineState.Get());

    // Viewport, Scissor Rect, Root Signature 설정
    RECT clientRect;
    GetClientRect(g_hWnd, &clientRect);
    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    D3D12_VIEWPORT viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
    D3D12_RECT scissorRect = { 0, 0, (LONG)width, (LONG)height };

    g_commandList->RSSetViewports(1, &viewport);
    g_commandList->RSSetScissorRects(1, &scissorRect);
    g_commandList->SetGraphicsRootSignature(g_rootSignature.Get());
    g_commandList->SetGraphicsRootConstantBufferView(0, g_constantBuffer->GetGPUVirtualAddress());

    // 1. World 행렬 (예: Identity)
    XMMATRIX world = XMMatrixIdentity();

    // 2. View 행렬 (카메라 위치 설정)
    XMVECTOR eyePos = XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f); // 카메라를 Z=-5에 배치
    XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX view = XMMatrixLookAtLH(eyePos, lookAt, up);

    // 3. Projection 행렬 (원근 투영)
    GetClientRect(g_hWnd, &clientRect);
    float aspectRatio = (float)width / height;
    // 60도 FOV, 종횡비, 0.1f ~ 1000.0f 깊이 범위
    XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, 0.1f, 1000.0f);

    // 4. WVP 행렬 계산 및 Constant Buffer에 복사
    XMMATRIX wvp = world * view * proj;

    // Transpose (셰이더에서 mul(vec, mat) 대신 mul(mat, vec)를 사용할 경우)
    XMMATRIX finalWVP = XMMatrixTranspose(wvp);

    // Constant Buffer 포인터에 데이터 복사
    ObjectConstants objCB;
    XMStoreFloat4x4(&objCB.WorldViewProj, finalWVP);

    // g_constantBufferPtr은 InitializeDX12에서 Map된 CPU 포인터입니다.
    memcpy(g_constantBufferPtr, &objCB, sizeof(ObjectConstants));

    // ----------------------------------------------------
    // 3. Back Buffer 설정 및 3D 씬 그리기 (Triangle)
    // ----------------------------------------------------
    UINT currentBackBuffer = g_swapChain->GetCurrentBackBufferIndex();

    // (3A) 리소스 배리어: Present -> Render Target 상태 전환
    const D3D12_RESOURCE_BARRIER barrier_to_rt = CD3DX12_RESOURCE_BARRIER::Transition(
        g_renderTargets[currentBackBuffer].Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    g_commandList->ResourceBarrier(1, &barrier_to_rt);

    // RTV 핸들 설정 및 클리어
    UINT rtvDescriptorSize = g_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_rtvHeap->GetCPUDescriptorHandleForHeapStart(), currentBackBuffer, rtvDescriptorSize);

    g_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f }; // 파란색
    g_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // (3B) 3D Draw Call (삼각형 그리기)
    g_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_commandList->IASetVertexBuffers(0, 1, &g_vertexBufferView);
    g_commandList->SetPipelineState(g_pipelineState.Get()); // PSO 설정

    // 🚨 1. 인덱스 버퍼 바인딩 추가 (FBX 모델은 인덱스를 사용)
    g_commandList->IASetIndexBuffer(&g_indexBufferView);

    // 🚨 2. Draw Call 변경: DrawInstanced 대신 DrawIndexedInstanced 사용
    // g_indexCount: LoadModel 함수에서 계산된 전체 인덱스 개수
    g_commandList->DrawIndexedInstanced(g_indexCount, 1, 0, 0, 0);

    // ----------------------------------------------------
    // 4. ImGui Draw (UI를 3D 씬 위에 덮어쓰기)
    // ----------------------------------------------------
    // **3D 씬을 다 그린 직후에 UI를 그려야 합니다.**

    // (4A) ImGui 폰트 텍스처를 사용하도록 Descriptor Heap 설정
    ID3D12DescriptorHeap* ppHeaps[] = { g_imguiSrvHeap.Get() };
    g_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    // (4B) ImGui 드로잉 명령을 Command List에 기록 (최종 드로우 명령)
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_commandList.Get());


    // ----------------------------------------------------
    // 5. 실행 및 Present (마무리)
    // ----------------------------------------------------

    // (5A) 리소스 배리어: Render Target -> Present 상태 전환
    const D3D12_RESOURCE_BARRIER barrier_to_present = CD3DX12_RESOURCE_BARRIER::Transition(
        g_renderTargets[currentBackBuffer].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);
    g_commandList->ResourceBarrier(1, &barrier_to_present);

    // (5B) Command Queue에 제출 및 Present
    g_commandList->Close();
    ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };
    g_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    g_swapChain->Present(1, 0);

    // ⚠️ TODO: FlushCommandQueue() 호출을 통해 GPU 작업 완료를 기다려야 합니다.
    FlushCommandQueue();
}

void FlushCommandQueue()
{
    // 1. Fence 값을 증가시켜 새로운 표식을 만듭니다.
    g_fenceValue++;

    // 2. Command Queue에 새로운 Fence 신호를 기록합니다.
    g_commandQueue->Signal(g_fence.Get(), g_fenceValue);

    // 3. GPU의 현재 완료 값이 CPU가 설정한 값보다 작다면 (작업 미완료)
    if (g_fence->GetCompletedValue() < g_fenceValue)
    {
        // GPU 완료 이벤트를 기다립니다.
        HANDLE eventHandle = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
        g_fence->SetEventOnCompletion(g_fenceValue, eventHandle);

        // CPU가 대기합니다. (무한대 시간 대기)
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

std::wstring OpenFileLoadDialog()
{
    // Win32 FILEOPEN 대화 상자를 위한 구조체
    OPENFILENAMEW ofn = { 0 };
    WCHAR szFile[260] = { 0 }; // 파일 경로를 저장할 버퍼

    // OPENFILENAME 구조체 설정
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_hWnd; // 메인 창 핸들 사용
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);

    // 파일 필터 설정 (FBX 모델만 표시)
    ofn.lpstrFilter = L"FBX Models (*.fbx)\0*.fbx\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // 파일 열기 대화 상자 호출
    if (GetOpenFileNameW(&ofn) == TRUE)
    {
        // 파일 선택 성공 시 경로 반환
        return std::wstring(ofn.lpstrFile);
    }

    // 실패 또는 취소 시 빈 문자열 반환
    return L"";
}

void LoadModelFromPath(const WCHAR* path) {
    std::wstring wpath(path);
    std::string path_ansi = WstringToString(wpath);

    Assimp::Importer importer;

    // Assimp의 핵심 함수: 파일 경로를 전달하고, 후처리 플래그를 지정
    const aiScene* scene = importer.ReadFile(
        path_ansi.c_str(), // 변환된 std::string의 char* 포인터를 전달
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace
    );

    // 로드 실패 처리
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        // ImGui 창이나 콘솔에 오류 메시지 출력
        std::cerr << "Assimp 로드 오류: " << importer.GetErrorString() << std::endl;
        return;
    }

    // --- 로드 성공: 여기서부터 aiScene 데이터 활용 ---

    std::cout << "모델 로드 성공: " << path << std::endl;
    std::cout << "총 메쉬 수: " << scene->mNumMeshes << std::endl;

    // TODO:
    // 1. scene->mNumMeshes 루프를 돌면서 메쉬 데이터(정점, 인덱스)를 추출
    // 2. scene->mNumMaterials 루프를 돌면서 재질 및 텍스처를 로드
    // 3. 추출된 데이터를 DirectX 12/Vulkan 등의 렌더링 API를 사용해 GPU 버퍼에 업로드
}

// 반환 타입: char* 대신 WCHAR* 사용
WCHAR* OpenFileDialog() {
    OPENFILENAME ofn;        // 구조체 초기화
    WCHAR szFile[260] = { 0 }; // 파일 경로를 저장할 버퍼

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_hWnd; // 전역 핸들 g_hWnd 사용 (NULL 대신)

    // 필터: FBX, OBJ, GLTF 파일만 보이게 설정
    ofn.lpstrFilter = L"3D Models (*.fbx;*.obj;*.gltf)\0*.fbx;*.obj;*.gltf\0All Files (*.*)\0*.*\0";

    ofn.lpstrFile = szFile;
    ofn.nMaxFile = _countof(szFile); // sizeof(szFile) 대신 _countof(szFile) 사용 권장
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // 파일 열기 대화 상자 호출
    if (GetOpenFileName(&ofn) == TRUE) {
        // 경로가 성공적으로 선택되면 버퍼의 내용을 복사하여 반환
        return _wcsdup(ofn.lpstrFile);
    }

    return nullptr; // 취소되거나 실패한 경우
}

void RenderGUI() {
    ImGui::Begin("모델 로드");

    // 버튼을 누르면 파일 탐색기를 띄웁니다.
    if (ImGui::Button("FBX/OBJ 파일 선택")) {
        WCHAR* filePath = OpenFileDialog();

        // 경로가 성공적으로 반환되었을 경우
        if (filePath != nullptr) {
            LoadModelFromPath(filePath);
            free(filePath); // Win32 API 메모리 해제
        }
    }

    ImGui::End();
}

// Assimp 헤더와 문자열 헬퍼가 이 파일에 정의되어 있어야 합니다.

bool LoadModel(const WCHAR* filePath)
{
    g_vertexBuffer.Reset();
    g_indexBuffer.Reset();

    // 1. WCHAR* (Wide Character) 경로를 std::string (ANSI/Multibyte)으로 변환
    std::wstring wpath(filePath);
    std::string path_ansi = WstringToString(wpath);

    Assimp::Importer importer;
    // FBX 파일 로딩 및 후처리 (삼각형화, 왼손 좌표계, 법선, 탄젠트 계산)
    const aiScene* scene = importer.ReadFile(
        path_ansi.c_str(),
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        // OutputDebugStringA("ERROR: Assimp failed to load model.");
        return false;
    }

    // ----------------------------------------------------
    // 2. 데이터 추출 (첫 번째 메시만 사용한다고 가정)
    // ----------------------------------------------------
    aiMesh* mesh = scene->mMeshes[0];

    std::vector<Vertex> vertices;
    std::vector<UINT> indices;

    // 정점 데이터 추출
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex v = {};
        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;

        // 법선 데이터 (FBX에는 필수적)
        if (mesh->mNormals) {
            v.nx = mesh->mNormals[i].x;
            v.ny = mesh->mNormals[i].y;
            v.nz = mesh->mNormals[i].z;
        }

        // 텍스처 좌표 (UV)
        if (mesh->mTextureCoords[0]) {
            v.u = mesh->mTextureCoords[0][i].x;
            v.v = mesh->mTextureCoords[0][i].y;
        }
        vertices.push_back(v);
    }

    // 인덱스 데이터 추출
    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // ----------------------------------------------------
    // 3. GPU 업로드: Vertex Buffer 및 Index Buffer 생성
    // ----------------------------------------------------

    const CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RANGE readRange(0, 0);

    // ====================================================
    // A. Vertex Buffer 리소스 생성 및 업로드
    // ====================================================
    const UINT vertexBufferSize = vertices.size() * sizeof(Vertex);
    const CD3DX12_RESOURCE_DESC vertexResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    // 리소스 생성
    if (FAILED(g_d3dDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&g_vertexBuffer)))) return false;

    // 데이터 복사
    UINT8* pVertexDataBegin;
    g_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    memcpy(pVertexDataBegin, vertices.data(), vertexBufferSize);
    g_vertexBuffer->Unmap(0, nullptr);

    // Vertex Buffer View 설정
    g_vertexBufferView.BufferLocation = g_vertexBuffer->GetGPUVirtualAddress();
    g_vertexBufferView.StrideInBytes = sizeof(Vertex);
    g_vertexBufferView.SizeInBytes = vertexBufferSize;

    // ====================================================
    // B. Index Buffer 리소스 생성 및 업로드
    // ====================================================
    const UINT indexBufferSize = indices.size() * sizeof(UINT);
    const CD3DX12_RESOURCE_DESC indexResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
    g_indexCount = indices.size(); // 인덱스 개수 저장

    // 리소스 생성
    HRESULT hr = g_d3dDevice->CreateCommittedResource(
        &heapProps, D3D12_HEAP_FLAG_NONE, &indexResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&g_indexBuffer));
    if (FAILED(hr)) return false;

    // 데이터 복사
    UINT8* pIndexDataBegin;
    g_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin));
    memcpy(pIndexDataBegin, indices.data(), indexBufferSize);
    g_indexBuffer->Unmap(0, nullptr);

    // Index Buffer View 설정
    g_indexBufferView.BufferLocation = g_indexBuffer->GetGPUVirtualAddress();
    g_indexBufferView.SizeInBytes = indexBufferSize;
    g_indexBufferView.Format = DXGI_FORMAT_R32_UINT;

    return true;
}