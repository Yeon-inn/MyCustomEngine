// DX12 관련 헤더
#include <dxgi1_6.h>        // DXGI 팩토리 및 어댑터 열거
#include <d3d12.h>          // D3D12 핵심
#include <D3Dcompiler.h>    // 셰이더 컴파일 (나중에 사용)

// COM 스마트 포인터 사용을 위한 템플릿 (MS에서 제공)
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

// ====================================================================
// DX12 전역 변수 (Global Variables)
// ====================================================================
ComPtr<ID3D12Device> g_d3dDevice;               // D3D12 Device
ComPtr<IDXGIFactory4> g_dxgiFactory;            // DXGI Factory
ComPtr<ID3D12CommandQueue> g_commandQueue;      // Command Queue
ComPtr<IDXGISwapChain3> g_swapChain;            // Swap Chain

#include "framework.h"
#include "CustomEngine.h"

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

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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
