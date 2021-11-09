/*  
==NOTE==
 // cominterface : 컴퓨터가 자동적으로 만드는 데이터
   // '포인터'로 생성 (필수)
   - release : 삭제 신청서
   - description : 생성 요청서

   // CPU에 대해서 연산하는 코드 cpp
   // GPU를 동작하는 코드 hlsl
   
 // Rendering PipeLine : 랜더링을 하기 위한 단계
   - IA -> VS -> RS -> PS -> OM (DeviceContext에서 적용)
   - IA -> VS -> HS -> TS -> DS -> GS -> SO -> RS -> PS -> OM
   
 // IA (Input Assembler Stage)
   - 정점에 대한 정보를 입력하는 단계 (Vertex Buffer, Index Buffer)
   - (Input)Layout을 설정
   - 레이아웃은 몇 바이트를 기준(주기)으로 읽을지 설정
   - PrimitiveTopology 설정

 // VS (Vertex Shader Stage)
   - Shader : GPU 연산을 할 수 있는 함수의 집합체
   - IA에게 받은 데이터들을 통하여 쉐이더 연산을 처리

 // RS (Rasterizer Stage) <- GPU (but not only GPU)
   - 정점 정보를 화면에 출력하기 위해 버텍스를 변환하여 폴리곤(레스터화 이미지) 생성
   - 정점을 바탕으로 폴리곤을 픽셀로 변환하는 단계(레스터라이즈)
   - viewport 설정

   - 백컬링, 클리핑이 자동으로 설정
   - 정점 정보를 화면에 출력하기 위해 레스터화 이미지로 변환하는 단계
   - 위 단계까지는 픽셀 단위가 아님
   - 받은 정보를 이용하여 픽셀화를 진행하는 단계

 // PS (Pixel Shader) <- GPU
   - 랜더링 대상 픽셀들의 색을 계산하는 단계
 
 // OM (Output Merge) 
   - RenderTargets 설정
   - 최종적으로 출력하기 위해 랜더링하는 단계

RS -> PS -> OM
 Local Space    
 World Space    
 View Space	     
 Viewport Space 
 
 Projection : 3D를 2D 화면에 투영하기 위한 작업
 Back Space Culling : 앞 화면에 가려 보이지 않는 뒤 화면에 대한 연산을 진행하지 않고 삭제
 Cleeping : View Soace 밖의 화면에 대한 연산을 진행하지 않고 삭제
 ViewPort : 3D인 View Space를 윈도우 상(2D)에 옮기기 위해 변환하는 작업
 */

#include <d3d11.h>
#include <cassert>
#include <iostream>
using namespace std;

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#ifndef _DEBUG
#define MUST(Expression)(	   (	     (Expression)))
#else
#define MUST(Expression)(assert(SUCCEEDED(Expression)))
#endif

#pragma region GPU
/*
// GPU에서 다룰 수 있는 도구
 // Texture : 이미 완성된 이미지를 읽기, 쓰기, 깊이를 통해 설정 
 // -> 완성된 이미지를 읽기 때문에 많은 연산과정이 필요
 // -> 따라서 기능들(읽기, 쓰기, 깊이)만 구현되어 있는 인터페이스를 넘기는 작업을 포함

 // Buffer : 그림을 그릴 때 기본적으로 점, 선, 삼각형만을 사용 (숫자를 이미지로 변환)
  // 도형을 그릴 때 정점의 좌표값을 시계방향으로 지정
   
    - vertex : 정점에 대한 좌표 (gpu에서 읽을 수 없어서 버퍼를 통해 정점에 대한 좌표(어디에 그릴지)를 넘겨줌)
	- index : 정점을 어떻게 사용할지에 대한 정보
    - constant
	DRAW :한 번 사용한 정점은 다시 사용 불가
	DRAWINDEX : 정점에 순서를 매겨 정점을 재사용 가능
*/
#pragma endregion

namespace Pipeline
{
	namespace
	{	
		ID3D11Device				* Device;				// 자원을 만드는 장소
		ID3D11DeviceContext			* DeviceContext;		// 만든 자원을 랜더링 파이프라인에 결합하기 위한 자원
		IDXGISwapChain				* SwapChain;			// 나눠진 작업을 교대하며 실행 / 위 두 구조체의 디스크립션을 포함
															// DXGI : 그래픽 인터페이스
		ID3D11RenderTargetView		* RenderTargetView;		// 자원을 그리는 장소
		ID3D11InputLayout			* InputLayout;
		
		namespace Buffer
		{
			ID3D11Buffer* Vertex;	// 정점을 입력할 버퍼
			ID3D11Buffer* Index;
		}
	}
	
	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
			case WM_CREATE:
			{
				{
					DXGI_SWAP_CHAIN_DESC Descriptor = DXGI_SWAP_CHAIN_DESC();
					Descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
					// 4바이트의 FLOAT 형태의 색상을 설정 / UNORM : 1로 정규화
					Descriptor.SampleDesc.Count = 1;
					// 픽셀의 단점(계단식 이미지)을 개선 : 안티앨리어싱
					Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
					// 렌더링 타겟을 출력하는 용도로 사용
					Descriptor.BufferCount = 1;
					// 버퍼의 사용개수
					// 0 : 버퍼 사용 X
					// 1 : 버퍼 2개 사용
					Descriptor.OutputWindow = hWindow;
					// 어느 창에 띄어줄지 설정
					Descriptor.Windowed = true;
					// 창모드 사용여부

					HRESULT hr = D3D11CreateDeviceAndSwapChain
					(
						nullptr,					// 어뎁터 미사용
						D3D_DRIVER_TYPE_HARDWARE,	// 드라이브 타입으로 하드웨어를 설정 (하드웨어의 도움을 받아 랜더링하도록 설정)
						nullptr,					// 추가적인 소프트웨어 타입 기능 미사용
						0,							// 플래그 미사용
						nullptr,					// Direct X의 버전 호환성을 기본으로 설정 // 기본으로 설정시 11_0 ~ 6_0
						0,							// 설정된 버전 호환성을 모두 사용
						D3D11_SDK_VERSION,			// 컴퓨터에게 어떤 실행 버전을 사용할지 통보
						&Descriptor,				// 설명서의 주소
						&SwapChain,					// 저장할 공간의 주소 (SWAP)
						&Device,					// 저장할 공간의 주소 (DEVICE)
						nullptr,					// Direct X의 버전 호환성을 기본으로 설정
						&DeviceContext				// 저장할 공간의 주소 (DEVICECONTEXT)
					);
					assert(SUCCEEDED(hr));			// 프로그램이 잘 만들어졌는지 확인
				}

				{
					#include "Shader/Bytecode/Vertex.h"
					{
						D3D11_INPUT_ELEMENT_DESC Descriptor[] =
						{
							// 정점을 상황에 따라 바꿔서 삼각형을 만들고 싶을때 인덱스를 설정
							{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0},
							{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1}
							/// D3D11_INPUT_ELEMENT_DESC(),
							/// D3D11_INPUT_ELEMENT_DESC()
						};
						/// Descriptor[0].SemanticName			= "POSITION";
						/// Descriptor[0].SemanticIndex			= 0;
						/// Descriptor[0].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
						/// Descriptor[0].InputSlot				= 0; // IA단계에서 식별하는 정수값을 설정하는 부분
						/// Descriptor[0].AlignedByteOffset 	= 0; // POSITION을 읽을 시작 지점 설정
						/// Descriptor[0].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA; // 읽을 데이터의 종류 설정
						/// Descriptor[0].InstanceDataStepRate	= 0; // 버퍼에서 그릴 인스턴스의 수를 설정 (정점 데이터라면 무조건 0)
						/// 
						/// Descriptor[1].SemanticName			= "COLOR";
						/// Descriptor[1].SemanticIndex			= 0;
						/// Descriptor[1].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
						/// Descriptor[1].InputSlot				= 1; 
						/// Descriptor[1].AlignedByteOffset		= 16; 
						/// Descriptor[1].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA; 
						/// Descriptor[1].InstanceDataStepRate	= 0; 

						// GPU에서 CPU로 다시 정보를 넘겨주는 과정이며 CPU에서 읽을 수 있는 2진코드로 변환
						// VS단계에서 Inputlayout을 넘기기 시작
						MUST(Device->CreateInputLayout(Descriptor, 2, Bytecode, sizeof(Bytecode), &InputLayout));

						DeviceContext->IASetInputLayout(InputLayout);
					}

					{
						// Vertex shader를 만들기 위해 byte code를 사용
						ID3D11VertexShader* VertexShader = nullptr;

						MUST(Device->CreateVertexShader(Bytecode, sizeof(Bytecode), nullptr, &VertexShader));

						DeviceContext->VSSetShader(VertexShader, nullptr, 0);
					}
				}

				{
					#include "Shader/Bytecode/Pixel.h"
					{
						ID3D11PixelShader* PixelShader = nullptr;

						MUST(Device->CreatePixelShader(Bytecode, sizeof(Bytecode), nullptr, &PixelShader));

						DeviceContext->PSSetShader(PixelShader, nullptr, 0);

						PixelShader->Release();
					}
				
				}

				// Vertex Buffer
				{ // CPU에서 작업중..
					struct Vertex
					{
						float position[4];
						float Color[4];
					};
#pragma region Vertex 선언
					// Vertex const Vertices[4]
					// {
					// 	{{ -0.5f, +0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
					// 	{{ +0.5f, +0.5f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
					// 	{{ -0.5f, -0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
					// 	{{ +0.5f, -0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
					// };
					// 
					// D3D11_BUFFER_DESC Descriptor = D3D11_BUFFER_DESC();		// CPU에서 사용하는 코드를 GPU에서 사용 가능하도록 변환 (버퍼의 기능)
					// Descriptor.ByteWidth = sizeof(Vertices);					// 버퍼의 전체 크기를 설정
					// Descriptor.Usage = D3D11_USAGE_IMMUTABLE;				// GPU에서 읽을 수 있도록 사용 설정
					// Descriptor.BindFlags = 0;								// CPU에서의 접근 권한의 정도를 설정 (다이나믹에서 CPU의 접근 허용을 안 하기 때문에 0으로 설정)
					// Descriptor.CPUAccessFlags = 0;
					// Descriptor.MiscFlags = 0;
					// Descriptor.StructureByteStride = 0;						// input layout에서 설정하고 있기 떄문에 따로 설정 X
																				// 넘겨준 데이터를 어느 지점에서 나눠서 구별할지 설정
																		
#pragma endregion
				
					float const Coordinates[4][4]
					{
						{ -0.5f, +0.5f, 0.0f, 1.0f},
						{ +0.5f, +0.5f, 0.0f, 1.0f},
						{ -0.5f, -0.5f, 0.0f, 1.0f},
						{ +0.5f, -0.5f, 0.0f, 1.0f}
					};

					D3D11_BUFFER_DESC Descriptor
					{
						sizeof(Coordinates),
						D3D11_USAGE_IMMUTABLE,
						D3D11_BIND_VERTEX_BUFFER,
						0
					};


					D3D11_SUBRESOURCE_DATA const SubResource = { Coordinates };
					// 서브리소스 : 버퍼 공간에 대해 값을 초기화하기 위해 서브리소스에 값을 담고 버퍼에 찍어내는 역할을 수행
	
					ID3D11Buffer* Buffer = nullptr;

					MUST(Device->CreateBuffer(&Descriptor, &SubResource, &Buffer));

					UINT const Stride = sizeof(*Coordinates);
					UINT const Offset = 0;

					DeviceContext->IASetVertexBuffers(0, 1, &Buffer, &Stride, &Offset);
					// 버퍼를 생성만 했고 랜더링파이프라인에 결합을 해주는 작업
					// 스타트 지점, 몇개의 버퍼를 넘길지, 어떻게 자료들을 나눌것인지, ???

					Buffer->Release();
				}

				{
					D3D11_BUFFER_DESC const Descriptor
					{
						sizeof(float[4][4]),
						D3D11_USAGE_DYNAMIC,
						D3D11_BIND_VERTEX_BUFFER,
						D3D11_CPU_ACCESS_WRITE
					};

					MUST(Device->CreateBuffer(&Descriptor, nullptr, &Buffer::Vertex));

					UINT const Stride = sizeof(float[4]);
					UINT const Offset = 0;

					DeviceContext->IASetVertexBuffers(1, 1, &Buffer::Vertex, &Stride, &Offset);
				}


				// PrimitiveTopology
				{
					// 꼭지점 데이터를 삼각형 목록으로 해석하도록 설정
					DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					// list -> strip : 기존에 도형을 삼각형으로 생각하지 않고 연결된 삼각형으로 설정
				}

#pragma region Index Buffer
				// Index Buffer
				//{
				//	typedef USHORT Index[3];
				//	Index const Indices[2]
				//	{
				//		{0, 1, 2},
				//		{3, 2, 1}
				//	};
				//
				//	D3D11_BUFFER_DESC Descriptor = D3D11_BUFFER_DESC();
				//
				//	Descriptor.ByteWidth			= sizeof(Indices);
				//	Descriptor.Usage				= D3D11_USAGE_IMMUTABLE;
				//	Descriptor.BindFlags			= D3D11_BIND_INDEX_BUFFER;
				//	Descriptor.CPUAccessFlags		= 0;
				//	Descriptor.MiscFlags			= 0;
				//	Descriptor.StructureByteStride	= 0;
				//
				//	// subresource : 열쇠 역할
				//	D3D11_SUBRESOURCE_DATA SubResource = D3D11_SUBRESOURCE_DATA();
				//	SubResource.pSysMem = Indices;
				//	SubResource.SysMemPitch = 0;
				//	SubResource.SysMemSlicePitch = 0;
				//
				//	MUST(Device->CreateBuffer(&Descriptor, &SubResource, &Buffer::Index));
				//
				//	DeviceContext->IASetIndexBuffer(Buffer::Index, DXGI_FORMAT_R16_UINT, 0);
				//
				//}
#pragma endregion
				return 0;
			}
			case WM_APP: 
			{
				D3D11_MAPPED_SUBRESOURCE Subresource = D3D11_MAPPED_SUBRESOURCE();
				MUST(DeviceContext->Map(Buffer::Vertex, 0, D3D11_MAP_WRITE_DISCARD, 0, &Subresource)); // discard : 가져온 값을 저장하지 않고 삭제
				// 배열로 넘겨주고 싶을 때 배열의 몇번째 부터 넘겨줄것인지 Map을 통해 설정
				{
					static float element = 0.0000f;
					static float delta = 0.001f;

					float const Colors[4][4]
					{
						{element,	0.0f,		0.0f,		1.0f},
						{0.0f,		element,	0.0f,		1.0f},
						{0.0f,		0.0f,		element,	1.0f},
						{element,	element,	element,	1.0f},
					};

					element += delta;

					if (element < 0.0f || 1.0f <= element)
						delta *= -1;

					memcpy_s(Subresource.pData, Subresource.RowPitch, Colors, sizeof(Colors));
				}
				DeviceContext->Unmap(Buffer::Vertex, 0);
			

				{
					float const Color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

					DeviceContext->ClearRenderTargetView(RenderTargetView, Color);
					// 작업할 렌더 영역(스크린 전체)의 초기화 (색 입히기)

					DeviceContext->Draw(4, 0);
					// Draw : Vertex에 대한 정보를 받는 메서드
					// Draw와 DrawIndex의 차이

					MUST(SwapChain->Present(1, 0));
					// flag : 이미지 넘겨줄 때 추가 옵션의 사용 여부
				}

				return 0;
			}
			case WM_DESTROY:
			{
				RenderTargetView->Release();
			
				InputLayout->Release();

				//Buffer::Vertex->Release();
				//Buffer::Index->Release();
				
				SwapChain->Release();
				Device->Release();
				DeviceContext->Release();
				
				PostQuitMessage(0);
				
				return 0;
			}
			case WM_SIZE:
			{
				{
					{
						D3D11_VIEWPORT Viewport = D3D11_VIEWPORT();

						Viewport.Width = LOWORD(lParameter);
						Viewport.Height = HIWORD(lParameter);
					
						DeviceContext->RSSetViewports(1, &Viewport);
					}

					if (RenderTargetView)
					{
						RenderTargetView->Release();

						SwapChain->ResizeBuffers
						(
							1,								// 버퍼의 개수
							LOWORD(lParameter),				// 윈도우 창의 WIDTH
							HIWORD(lParameter),				// 윈도우 창의 HEIGHT
							DXGI_FORMAT_B8G8R8A8_UNORM,
							0
						);
					}
					{
						ID3D11Texture2D* texture2D = nullptr;
						MUST(SwapChain->GetBuffer(0,IID_PPV_ARGS(&texture2D))); 
						// HRESULT를 define으로 묶어서 설정하는 방법
						// 버퍼에서 설정된 이미지를 가져와서 텍스쳐에 저장

						HRESULT hr = Device->CreateRenderTargetView(texture2D, nullptr, &RenderTargetView); // GPU에서 그리기 때문에 CPU에서 GPU로 텍스처를 반환
						assert(SUCCEEDED(hr));
						texture2D->Release();

						DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr); // 작업할 영역 = 1
					}
				}

				return 0;
			}
			default:
			{
				return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
			}
		}
		return 0;
	}
}

// 속성 -> 링커 -> 시스템 -> 하위 시스템 -> '창'으로 변경
// 속성 -> 링커 -> 모든 속성 -> 추가 종속성 -> 편집 -> d3d11.lib 추가    
// 속성 -> 구성 속성 -> 고급 -> 문자 집합 -> '멀티바이트 문자 집합 사용'으로 변경


/*
// c, c++
// 알고리즘
// 쓰레드
// 다이렉트 x : 그래픽 랜더링

지역, 전역, 동적 할당된 변수
1. 할당되는 매모리 구조
2. 라이프사이클
3. 지역변수는 스택 동적은 힙 : 동적은 먼저 생성된 값이 제일 높은 주소값을 가짐 11111 -> 11110 -> 11100
런타임, 컴파일타임
지역변수는 스택, 동적은 힙으로 메모리가 할당되는 이유?
4. 객체 지향 프로그래밍의 특성
커플링 현상

랜더링 파이프 라인 (그래픽스 프로그래밍)

행렬 : 덧셈, 뺄셈, 곱셈 (교환법칙 O)


*/
