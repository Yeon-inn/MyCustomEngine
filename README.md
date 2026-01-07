# 🚀 DirectX 12 3D Model Viewer with ImGuizmo

본 프로젝트는 **DirectX 12(D3D12)** 로우 레벨 API를 활용하여 실시간 렌더링 파이프라인을 구축하고, **Assimp**를 이용한 외부 모델 로딩 및 **ImGuizmo**를 통한 시각적 트랜스폼 에디팅 환경을 구현하는 것을 목표로 합니다.

<img width="290" height="270" alt="Image" src="https://github.com/user-attachments/assets/36de7104-32f0-4526-b82d-0ca2ebd902b0" /> <img width="290" height="270" alt="Image" src="https://github.com/user-attachments/assets/c0b791e5-e28f-4ffd-aaa9-a1f29947522e" />  <img width="290" height="270" alt="Image" src="https://github.com/user-attachments/assets/c671f9c2-bc50-421d-b55c-a1bcd0a264d7" /> 

## 1. 개요 (Abstract)
현대의 실시간 렌더링 환경에서 GPU 연산 능력을 극대화하기 위해 D3D12 파이프라인 메커니즘을 분석했습니다. 스왑 체인 기술을 통한 화면 안정성 확보, Descriptor Heap을 이용한 리소스 통신, 그리고 3D 공간의 정점을 2D 화면으로 투영하는 기하 변환 원리를 실무 코드로 구현했습니다.

<img width="860" height="1250" alt="Image" src="https://github.com/user-attachments/assets/993c0a2c-8687-43ce-b174-c68715b2b316" />
---

## 2. 핵심 기술 분석 (Core Analysis)

### 2.1 스왑 체인 및 리소스 관리
* **스왑 체인 (Swap Chain):** 전면/후면 버퍼를 교체하는 더블 버퍼링 방식을 통해 렌더링 중간 과정이 노출되는 티어링(Tearing) 현상을 방지합니다.
* **Descriptor Heap & RTV:** CPU와 GPU 간의 효율적인 연동을 위해 서술자 힙을 사용하여 RTV, CBV 등 GPU 리소스에 대한 접근 정보를 관리합니다.

### 2.2 상수 버퍼 (CBV) 및 데이터 정렬
* 월드, 뷰, 투영 행렬 등 매 프레임 변화하는 데이터를 GPU 셰이더로 전달합니다.
* **16바이트 정렬(Alignment):** DX12의 엄격한 상수 버퍼 정렬 규칙을 준수하기 위해 C++ 구조체와 HLSL 간의 패딩(Padding) 처리를 수행했습니다.

### 2.3 기하 변환 (Coordinate Transformation)
* 3차원 좌표에 $w$ 성분이 추가된 **4차원 동차 좌표계**를 도입하여 위치 이동(Translation)을 행렬 곱으로 표현했습니다.
* **최종 출력 공식:** $V_{Screen} = M_{Proj} \cdot M_{View} \cdot M_{World} \cdot V_{Vertex}$



---

## 3. 구현 내용 (Implementation Details)

### 3.1 3D 모델 로딩 및 렌더링
* **Assimp 파싱:** 외부 FBX 파일을 로드하여 정점(Position, Normal, UV) 및 인덱스 데이터를 추출합니다.
* **고급 조명 효과:** 픽셀 셰이더에서 법선 벡터와 광원 방향을 이용한 **Lambertian Shading**을 구현하여 입체감을 부여했습니다.

### 3.2 ImGuizmo 통합 조작
* **실시간 상호작용:** ImGuizmo API를 사용하여 뷰포트 내에서 모델의 이동, 회전, 크기 조절이 가능한 기즈모를 구현했습니다.
* 기즈모를 통해 수정된 **World 행렬**은 즉각적으로 상수 버퍼에 반영되어 실시간 변환을 보장합니다.

---

## 4. 실행 결과 (Result)
* FBX 모델 로딩 및 화면 출력 성공.
* 기즈모 조작에 따른 실시간 월드 행렬 변화 및 렌더링 반영 확인.
<img width="756" height="439" alt="Image" src="https://github.com/user-attachments/assets/bf346ae5-1b1c-49ae-b3ac-f7eb8ce54132" />
