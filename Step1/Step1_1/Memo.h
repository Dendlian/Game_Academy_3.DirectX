#pragma region ETC Concept
/*
// 코딩 문자 색
노란색 : 함수
보라색 : 메크로 상수, 메크로 함수 (Symbol)
파랑색 : 키워드 (자료형, true, sizeof)
 - 키워드 : C, C++에서 제공하는 기본 형식으로 자료형으로 사용하는 것이 불가
녹  색 : struct, class, typedef
하늘색 : 지역변수
흰  색 : 전역변수
연두색 : 상수
자주색 : 프로그램 흐름을 조정하는 키워드 (return, break, continue)
*/
#pragma endregion


// Entry.cpp
#pragma region Basic Concept
/*
- DirectX			: GPU 기능을 사용할 수 있는 API
- GPU				: 그래픽 처리 장치
					: 병렬연산을 통해 대량의 픽셀에 대한 연산을 동시에 처리하기 때문에 속도↑
					: 렌더링 픽셀 각 각에 대한 연산이 필요하므로 대량의 연산을 요구
- CPU				: 입출력 장치, 기억 장치, 연산 장치등 컴퓨터의 리소스를 처리해주는 중앙 처리 장피
					: 직렬연산을 통해 순차적으로 연산하여 복잡한 연산을 처리할 때 효울적

//https://over-stack.tistory.com/23
- Coding Convention	: 함수를 호출하는 방식에 대한 규약 (함수호출규약)
	(1) __cdecl   : C, C++ 함수 *라이브러리에서 표준으로 쓰이는 호출규약
				  : 스택으로 전달한 매개변수를 함수를 호출한 위치에서 정리하는 방식
				  : cdecl로 만든 함수는 매개변수 전달을 위해 사용한 스택을 call명령 다음에 정리
				  : 함수를 호출한 위치에서는 매개변수의 총 크기를 정확히 알고 있으므로 안정적인 스택의 정리가 가능
				  :: 우측->좌측 방향으로 스택메모리를 이용하여 인자를 전달하며, '호출자'가 스택프레임을 정리

	(2) __stdcall : WINAPI처럼 스택의 정리를 함수내부에서 처리하는 호출규약 
				  : 함수를 호출하는 위치에서는 함수를 호출만 하고 스택정리를 하지 않는 방식
				  : stdcall로 만든 함수는 이름 뒤에 @ 기호를 붙힌 뒤 정리할 스택의 크기를 명시
				  : 스택의 크기는 고정되어 있기 때문에 *가변인수를 받는 함수에는 stdcall 규약 사용 불가
				  :: 좌측->우측 방향으로 스택메모리를 이용하여 인자를 전달하며, '피호출자'가 스택프레임을 정리

	(3) __fastcall : 처음 n개의 매개변수를 스택대신 레지스터로 전달하는 방식
				   : 매개변수의 수가 n개 이하면 스택 정리 과정이 아예 생략되므로 실행 속도↑
	(4) __thiscall : 매개변수에 인스턴스의 주소가 기억된 후 호출되는 함수
				   : 그 외에는 stdcall과 동일하고 가변인수를 사용하는 함수에 한해 cdecl 규약으로 처리
*가변인수   : printf처럼 매개변수의 개수가 정해지지 않는 함수
*라이브러리 : C++등 표준 자체로 쓰여진 클래스들과 함수들의 집합
		   : visual studio에서는 dirextX에 대한 라이브러리도 제시하며 주로 프로그래머가 확인할 수 없도록 유지
		   : ex) iostream은 정확한 코드의 내용을 확인할 수 없으며, 그 자물쇠의 역할 또한 라이브러리의 역할 중 하나

- Entry.cpp  : CPU에 대해서 연산하는 코드 
- Pixel.hlsl : GPU를 동작하도록 하는 코드

*/
#pragma endregion

#pragma region CPU <-> GPU
/*
//https://ssinyoung.tistory.com/37
- CPU : C++ 라이브러리를 통해 프로그램을 실행
- GPU : Direct 3D 라이브러리를 통해 프로그램을 실행

- Shader	: GPU가 어떤 일(렌더링)을 하기 위해 필요한 프로그램
			: Direct 3D는 셰이더로 렌더링을 하기 위해 특정 파이프라인을 제공
- Resource  : GPU가 사용하는 데이터 / 셰이더로 넘겨줘야 하는 모든 데이터
	(1) Texture : 이미 완성돼있는 이미지, 그림 데이터로서 읽기, 쓰기, 깊이 등을 통해 설정
				: 완성된 이미지를 읽기 때문에 많은 연산과정이 필요
				: 따라서 기능들(읽기, 쓰기, 깊이)만 구현되어 있는 인터페이스를 넘기는 작업을 포함

	(2) Buffer  : 행렬, 벡터, 사용자 정의 데이터 자료형 등 그림이 아닌 데이터
				: 점, 선, 삼각형을 이용하여 숫자를 이미지로 변환
				: MUST 도형을 그릴 때 정점의 좌표값을 시계방향으로 지정
				: CPU에서 설정한 값을 GPU에서 사용할 수 있도록 변환하는 작업 수행
			
			[1] Vertex	 : 정점에 대한 좌표 (그리는 장소)
			[2] Index    : 정점을 어떻게 사용할지에 대한 정보 (그리는 순서)
			[3] constant : 행렬을 저장할 수 있는 상수 버퍼 (위치, 크기, 회전)
*/
#pragma endregion 


// Pipeline.cpp
#pragma region Basic Concept
/*
- Device			: 버퍼 및 자원의 생성을 담당 (ex : Device->CreateVertexShader)
- DeviceContex		: 렌더링 파이프라인 5단계의 처리를 담당
- SwapChain			: 번갈아가며 버퍼의 연산과 씬으로 보여주는 역할을 담당
					: IDXGISwapChain의 Descriptor에는 Device와 DeviceConrext에 대한 Descriptor를 포함
- RenderTargetView	: 어디에 그릴지 장소 설정을 담당

- ID3D11~			: 첫 글자의 I는 cominterface로서 컴퓨터가 자동으로 만드는 데이터
					: MUST 포인터로 생성
- cominterface		: 컴퓨터가 자동으로 생성하는 대상에 대해 개입하기 위해 일종의 설명서(description)가 필요
					: cominterface으로 만든 Device, Swapchain등은 설명서를 통해 필요한 장치의 생성이 끝나면 삭제(release)가 필요

- Buffer
	1. 콘솔창에서 입력한 개행문자등의 문자 정보가 남아 있는 장소
	2. Front/Back Buffer처럼 연산 및 이미지 출력을 담당하는 장소
	3. Vertex/Index Buffer처럼 데이터를 GPU에서 사용하게 할 수 있는 전달체
*/
#pragma endregion

#pragma region Rendering Pipeline
/*
 // Rendering PipeLLine

  - IA -> VS -> RS -> PS -> OM (DeviceContext에서 적용)
  - IA -> VS -> HS -> TS -> DS -> GS -> SO -> RS -> PS -> OM
 
 1.IA (Input Assembler) Stage 
  - 정점의 정보를 입력하는 단계 (Vertex Buffer, Index Buffer)  

  - Direct3D에서의 정점은 공간적 위치 이외의 정보를 포함 가능

  - VS 단계에서 사용할 (Input)Layout을 설정 : 몇 바이트를 기준(주기)으로 읽을지 설정
  - PrimitiveTopology을 설정 : 꼭지점 데이터를 해석하는 방식을 설정
  
  - 정점들은 정점 버퍼(Vertex Buffer)라고 하는 Direct3D 자료구조 안에 담겨서 렌더링 파이프라인에 연결 (즉 정점들을 연속적인 메모리에 저장하는 자료구조)
  - 정점들을 통해 삼각형을 그리는 순서(방법)는 인덱스 버퍼(Index Buffer)라고 하는 Direct3D 자료구조 안에 담겨서 렌더링 파이프라인에 연결
  

  
 2.VS (Vertex Shader) Stage 
  - IA Stage에서 받은 데이터를 통하여 쉐이더 연산을 처리하는 단계

  - Shader : GPU 연산을 할 수 있는 함수의 집합체

  - Local, World, View Space 생성
  - Projection : 3D를 2D 화면에 투영하기 위한 작업



 3.RS (Rasterizer Stage)
  - 정점 정보를 화면에 출력하기 위해 Vertex를 변환하여 폴리곤(레스터화 이미지)을 생성하는 단계
  - 정점을 바탕으로 폴리곤을 픽셀로 변환하는 단계(레스터라이즈)
   
  - viewport 설정 : (이미지를 어떻게, 어디에 그릴지 설정한 후) 보여줄 영역을 설정하는 작업 (NDS좌표 사용)
  - Viewport 변환 : 프로젝트 윈도우의 좌표를 Viewport라 불리는 화면의 직사각형으로 변환하는 과정
     (뷰포트 사각형은 윈도우와 상대적이며, 윈도우 좌표를 이용해 지정)
   
  - System Value 사용 : GPU의 병렬처리 특성으로 인해 값을 무시하고 넘길 수 있어 꼭 확인하고 넘어가야 하는 정보임을 명시

  - Cleeping : View Space 밖의 화면에 대한 연산을 진행하지 않고 삭제
  - Back Space Culling : 앞 화면에 가려 보이지 않는 뒤 화면에 대한 연산을 진행하지 않고 삭제
   
  

 4.PS (Pixel Shader) Stage
  - 랜더링 대상 픽셀들의 색을 계산하는 단계
    (색을 계산하지만 위치까지는 지정하지 않고 OM으로 전달)
 


 5.OM (Output Merge) Stage
  - 최종적으로 출력하기 위해 랜더링하는 단계
  
  - Render Target 설정(Render Target에 픽셀을 입력)
  - Render Target은 application의 해상도와 일치하는 2D Texture이며, 궁극적으로 화면에 표시

  - PixelColor와 Depth를 위한 별도의 Buffer가 존재

  ========================================================================================================================================================

Local Space
  - 하나의 물체를 이루는 정점에 대한 정보를 처리하는 장소
  - 삼각형 리스트를 정의하는데 이용하는 좌표 시스템
  - 로컬 스페이스는 모델링 과정을 쉽고 단순하게 만들어 모델 자체의 로컬 좌표 시스템을 이용하는 것이 월드에서 직접 모델을 구성하는 것보다 수월

World Space
  - 로컬 스페이스로 구성한 다수의 모델들은 월드 좌표 시스템으로 옮겨 하나의 장면을 구성
  - 로컬 스페이스의 물체들은 이동, 회전, 크기 변환 등을 포함하는 작업을 거쳐 월드 스페이스로 전송
 // 기본 요소 : Translation(위치), Scale(크기), Rotaion(회전), Revolution(공전), Parents(상대적 위치)
 // 설정 순서 : S -> R -> T

View Space  
  - 월드 스페이스 내에서 기하물체와 카메라는 월드 좌표 시스템과 연계되어 정의
  - But 카메라가 월드 내 임의의 위치나 방위를 가진다면 투영이나 그 밖의 작업이 어렵가나 비효율적
  - 이를 해결하기 위해 카메라를 월드 시스템의 원점으로 반환하고, 카메라가 양의 z축을 내려다 보도록 회전시키는 작업 수행

Projection  
  - 뷰 스페이스에서는 3D 장면의 2D 표면을 얻는 과정이 필요하므로 원근 투영을 이용하여 기하물체를 투사
  - 즉 카메라에서 멀리 떨어진 물체는 가까운 물체보다 작게 표현
 // 원근 투영 : 원근법에 기초하여 오브젝트들을 반영한 후 투영
 // 직교 투영 : 원근법에 상관없이 있는 그대로 투영

 Back Space Culling : 
  - 앞 화면에 가려 보이지 않는 뒤 화면에 대한 연산을 진행하지 않고 삭제
 
 Cleeping : 
  - View Soace 밖의 화면에 대한 연산을 진행하지 않고 삭제 ()
 
 ViewPort : 
  - 3D인 View Space를 윈도우 상(2D)에 옮기기 위해 변환하는 작업
  = 3D 공간을 윈도우 좌표에서 이용할 수 있도록 변환하는 작업
*/
#pragma endregion

#pragma region Matrix
/*
	{
		1, 0, 0, 0	//
		0, 1, 0, 0	// 크기
		0, 0, 1, 0	//
		0, 0, 0, 1  // 위치
				 -> 방향값으로 쓰겠다는 기준
	}

 1. 곱셈에 대한 교환법칙이 성립하는 경우
  - 영행렬
  - 역행렬
  - 단위행렬
  - 우연의 일치
*/
#pragma endregion



#pragma region Rendering Pipeline Review
/*
IA :
 - InputLayout : Descriptor, Bytecode등을 담아 GPU가 번역 할 수 있도록 가이드라인 제시
			   : GPU에 Vertex 구조의 레이아웃을 설명하는 오브젝트
VS :

RS :

PS :

OM :

 - Subresource : buffer등의 전달체에 position, Color등의 값을 저장하기 위해 쓰는 요소

*/
#pragma endregion

#pragma region Direct X Step1_1
/*


*/
#pragma endregion
