#include <d3d11.h>
#include <iostream>
#pragma region cassert ***
/*
cassert 
 - 빌드를 할 때 프로그램 실행 여부를 설정하는 헤더파일
 - 컴파일 과정에 문제가 없어 컴파일이 완료되더라도 예상치 못한 오류를 검사
 - 모든 comInterface는 잘 만들었는지의 여부(HRESULT : Long type)를 반환
 - assert 메크로 함수를 통해 HRESULT를 검사
*/
#pragma endregion
#include <cassert>
#include "FreeImage.h"
using namespace std;

#pragma region Console
// #pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma endregion

#ifndef _DEBUG
#define MUST(Expression)(	   (	     (Expression)))
#else
#define MUST(Expression)(assert(SUCCEEDED(Expression)))
#endif

// namespace : static 기능 / 선언한 공간 안에서 생성된 값들은 외부에서 사용 불가
// Pipeline  : Rendering을 위한 단계
namespace Pipeline 
{
	// DXGI : 그래픽 인터페이스
	namespace
	{	
		ID3D11Device				* Device;				// 버퍼 및 자원의 생성을 담당
		ID3D11DeviceContext			* DeviceContext;		// 만든 자원을 랜더링 파이프라인에 결합
		IDXGISwapChain				* SwapChain;			// (Back Buffer) 나눠진 작업을 교대하며 실행 (연산 + Scene)
		ID3D11RenderTargetView		* RenderTargetView;		// 자원을 그리는 장소 설정
		ID3D11InputLayout			* InputLayout;
		
		namespace Buffer
		{
			ID3D11Buffer* Vertex;
		    //ID3D11Buffer* Index;
			ID3D11Buffer* Constant[3];

			template<typename Data>
			void Update(ID3D11Buffer* const buffer, Data const& data)
			// Data 를 buffer에 저장 / buffer의 내용을 data로 초기화
			{
				D3D11_MAPPED_SUBRESOURCE Subresource = D3D11_MAPPED_SUBRESOURCE();
				MUST(DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Subresource));
				// Subresource를 변경하면 buffer도 함께 자동으로 변경
				{
					memcpy_s(Subresource.pData, Subresource.RowPitch, data, sizeof(data));
				}
				DeviceContext->Unmap(buffer, 0);
			}
		}
	}
	
	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
			// 창이 생성될 때 발생하는 메세지
			case WM_CREATE:
			{
#pragma region Create SwapChain

#pragma region DXGI_SWAP_CHAIN_DESC 객체 정보 ***
/*
DXGI_SWAP_CHAIN_DESC
 - DXGI_MODE_DESC SampleDesc;
	{
	UINT Width;									// 가로
    UINT Height;								// 세로
    DXGI_RATIONAL RefreshRate;					// 화면의 갱신률
		{
			프레임을 맞추는 데이터 : 60 프레임
			0/0 : 컴퓨터가 1로 해석하며 자동 프레임으로 설정
			UINT Numerator;		// 60/1 중 60
			UINT Denominator;	// 60/1 중 1
		}
    DXGI_FORMAT Format;							// 색상정보 (Buffer는 색상만 전달하면 완료)
    DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;	// 이미지를 표현하는 순서
		{
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED        = 0,	// 기본값 : 값 지정 X
			DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE        = 1,	// 이미지를 한꺼번에 생성
			DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST  = 2,	// 상단 이미지부터 먼저 생성
			DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST  = 3		// 하단 이미지부터 먼저 생성
		}
    DXGI_MODE_SCALING Scaling;					// 창을 확대할 때 적용 방식 설정
		{
		DXGI_MODE_SCALING_UNSPECIFIED   = 0,	// 기본값 : 값 지정 X
		DXGI_MODE_SCALING_CENTERED      = 1,	// 배율 지정 X : 창을 확대하면 숨어있던 이미지 출력
		DXGI_MODE_SCALING_STRETCHED     = 2		// 배울 지정 : 창을 확대하면 기존 이미지가 확대
		}
	}

 - DXGI_SAMPLE_DESC SampleDesc;
	// 안티얼리어싱
	// 수퍼 샘플링 : 모든 각 픽셀을 나눠서 다시 계산 후 반환
	// 멀티 샘플링 : 테두리만 계산 후 반환
	{
	UINT Count;		// 멀티 샘플링을 할 때 1개의 Pixel을 몇 개로 나눌지 설정
					// (Pixel 당 멀티 샘플링 수)
	UINT Quality;	// 기본값 = 0 : 이미지의 품질로 최소한의 품질 개선만 실행
	}

 - DXGI_USAGE BufferUsage;
	// 그린 물체를 넘겨주는 방식을 설정
	// 렌더링 타겟을 출력하는 용도로 사용
	// => DXGI_USAGE_RENDER_TARGET_OUTPUT

 - UINT BufferCount;
	// 버퍼의 사용 개수
	// 0 : 버퍼 0개 사용
	// 1 : 버퍼 2개 사용

 - HWND OutputWindow;
	// 어느 창에 띄어줄지 설정

 - BOOL Windowed;
	// 창모드 사용여부

 - DXGI_SWAP_EFECT SwapEffect; 
	// Buffer에서 넘긴 데이터를 계속 가지고 있을지의 여부를 설정
	// 기본값 0 = DISCARD : 데이터를 소지하지 않고 폐기
	// ex) back buffer에서 연산을 한 데이터를 front buffer로 넘긴 후 삭제
	{
	DXGI_SWAP_EFFECT_DISCARD			= 0,	// 데이터 삭제
    DXGI_SWAP_EFFECT_SEQUENTIAL			= 1,	// 데이터 소지
    DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL	= 3, 
    DXGI_SWAP_EFFECT_FLIP_DISCARD		= 4	
	}

 - UINT Flags;
	// 추가 옵션
	// 기본값 0 : Flag 사용 X
*/
#pragma endregion				
				{
					// SwapChain의 Description 생성
					DXGI_SWAP_CHAIN_DESC Descriptor = DXGI_SWAP_CHAIN_DESC();
					
					// 4바이트의 FLOAT 형태의 색상을 설정
					// Format : 색상을 넘기는 방식
					// UNORM  : 1로 정규화
					Descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
					
					// 픽셀의 단점(계단식 이미지)을 개선 : 안티앨리어싱
					// SampleDesc : 안티 에일리어싱의 방식
					// Count : 픽셀당 멀티 샘플링으로 검사하는 횟수
					Descriptor.SampleDesc.Count = 1;
					
					// 그린 물체를 넘겨주는 방식을 설정
					// 렌더링 타겟을 출력하는 용도로 사용
					Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
					
					// 버퍼의 사용 개수
					// 0 : 버퍼 0개 사용
					// 1 : 버퍼 2개 사용
					Descriptor.BufferCount = 1;
					
					// 어느 창에 띄어줄지 설정
					Descriptor.OutputWindow = hWindow;
					
					// 창모드 사용여부
					Descriptor.Windowed = true;
				

					HRESULT hr = D3D11CreateDeviceAndSwapChain
					(
						nullptr,					// 어뎁터(Display에 대한 서브 시스템) 미사용
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
#pragma endregion

#pragma region Vertex Shader Setting
				{
					#include "Shader/Bytecode/Vertex.h"
					{
						D3D11_INPUT_ELEMENT_DESC Descriptor[] =
						{
							// 정점을 상황에 따라 바꿔서 삼각형을 만들고 싶을때 인덱스를 설정
							{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0},
							{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1}
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
						// VertexBuffer는 최대 15까지 즉 , 0 ~ 15 까지 16개가 가능하다
						MUST(Device->CreateInputLayout(Descriptor, 2, Bytecode, sizeof(Bytecode), &InputLayout));

						DeviceContext->IASetInputLayout(InputLayout);
					}

					{
						// Vertex shader를 만들기 위해 byte code를 사용
						ID3D11VertexShader* VertexShader = nullptr;

						MUST(Device->CreateVertexShader(Bytecode, sizeof(Bytecode), nullptr, &VertexShader));
						// Bytecode : vertexshader 를 만들기 위해 필요하다

						DeviceContext->VSSetShader(VertexShader, nullptr, 0);

						VertexShader->Release();
					}
				}
#pragma endregion

#pragma region Pixel Shdaer Setting
				{
					#include "Shader/Bytecode/Pixel.h"
					{
						ID3D11PixelShader* PixelShader = nullptr;

						MUST(Device->CreatePixelShader(Bytecode, sizeof(Bytecode), nullptr, &PixelShader));

						DeviceContext->PSSetShader(PixelShader, nullptr, 0);

						PixelShader->Release();
					}
				}
#pragma endregion
				
#pragma region Vertex 선언 ***			
					// struct Vertex
					// {
					// 	float position[4];
					// 	float Color[4];
					// };
					
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

#pragma region Vertex Buffer Setting
				{
					float const Coordinates[4][2]
					{
						{ -0.5f, +0.5f},
						{ +0.5f, +0.5f},
						{ -0.5f, -0.5f},
						{ +0.5f, -0.5f}
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
						sizeof(float[4][2]),
						D3D11_USAGE_DYNAMIC,
						D3D11_BIND_VERTEX_BUFFER,
						D3D11_CPU_ACCESS_WRITE
					};

					MUST(Device->CreateBuffer(&Descriptor, nullptr, &Buffer::Vertex));

					UINT const Stride = sizeof(float[2]);

					UINT const Offset = 0;

					DeviceContext->IASetVertexBuffers(1, 1, &Buffer::Vertex, &Stride, &Offset);
				}


				// PrimitiveTopology
				{
					// 꼭지점 데이터를 삼각형 목록으로 해석하도록 설정
					DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					// list -> strip : 기존에 도형을 삼각형으로 생각하지 않고 연결된 삼각형으로 설정
				}
#pragma endregion

#pragma region Index Buffer**
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

#pragma region Constant Buffer Setting
				{
					D3D11_BUFFER_DESC const Descriptor
					{
						sizeof(float[4][4]),
						D3D11_USAGE_DYNAMIC,
						D3D11_BIND_CONSTANT_BUFFER,
						D3D11_CPU_ACCESS_WRITE
					};
					
					for (UINT u = 0; u < 3; u++) 
						MUST(Device->CreateBuffer(&Descriptor, nullptr, &Buffer::Constant[u]));
					
					DeviceContext->VSSetConstantBuffers(0, 3, Buffer::Constant);
				}
#pragma endregion

#pragma region Create ShaderResourceView
				{
					char const* const File = "Walk.png";
					FreeImage_Initialise();
					{
						FIBITMAP* Bitmap = FreeImage_Load(
							FreeImage_GetFileType(File),
							File);
						{
							// 뒷면으로 로드되는 이미지를 다시 뒤집기
							FreeImage_FlipVertical(Bitmap);

							// Buffer가 아닌 Texture로 전달
							// 따라서 텍스처에 대한 디스크립션 작성
							D3D11_TEXTURE2D_DESC Descriptor = D3D11_TEXTURE2D_DESC();

							Descriptor.Width = FreeImage_GetWidth(Bitmap);
							Descriptor.Height = FreeImage_GetHeight(Bitmap);
							// MipLevels : 픽셀이 너무 많은 이미지를 띄우기 위해 텍스처를 축소하는 레벨
										// -> 데이터를 축소하므로서 렌더링 속도 증가
							Descriptor.MipLevels = 1;

							Descriptor.ArraySize = 1;
							Descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

							// 안티얼리어싱
							Descriptor.SampleDesc.Count = 1;
							Descriptor.SampleDesc.Quality = 0;

							Descriptor.Usage = D3D11_USAGE_IMMUTABLE;

							// 용도 : 셰이더를 읽음
							Descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;

							Descriptor.CPUAccessFlags = 0;
							Descriptor.MiscFlags = 0;

							/// 버퍼랑 차이점 : 위드, 헤이트, 안티얼리어싱 등

							D3D11_SUBRESOURCE_DATA SubResource = D3D11_SUBRESOURCE_DATA();

							// 서브리소스
							// 비트맵 : 읽지 못하는 영어 데이터값을 해석할 수 있는 데이터 시트(?)
							SubResource.pSysMem = FreeImage_GetBits(Bitmap);

							// 넘길 이미지를 창의 크기에 딱 맞게 설정
							SubResource.SysMemPitch = FreeImage_GetPitch(Bitmap);
							// 넘길 이미지의 띄울 깊이를 설정
							SubResource.SysMemSlicePitch = 0;

							ID3D11Texture2D* Texture2D = nullptr;
							MUST(Device->CreateTexture2D(&Descriptor, &SubResource, &Texture2D));
							{
								ID3D11ShaderResourceView* ShaderResourceView = nullptr;
								MUST(Device->CreateShaderResourceView(Texture2D, nullptr, &ShaderResourceView));
								{
									// 만든 후 렌더링 파이프라인(PS)에 결합 / PS에서 받아거 렌더타겟으로 배송 
									DeviceContext->PSSetShaderResources(0, 1, &ShaderResourceView);
									// 몇번째 슬롯에 넘길지, 몇개를 넘길지, 무엇을 넘길지
								}
								ShaderResourceView->Release(); 
							}
							Texture2D->Release();
						}
						FreeImage_Unload(Bitmap);
					}
					FreeImage_DeInitialise();
				} 
#pragma endregion
				return 0;
			}
			// (PeekMessage에서 메세지를 받고 있지 않을 때)사용자가 정의하는 메세지
			case WM_APP: 
			{
#pragma region CPU Access
				{

#pragma region 픽셀화된 이미지 그리기
/*	
				static float element = 0.0000f;
				static float delte = 0.0001f;

				float const Colors[4][4]
				{
					{element,	0.0f,		0.0f,		1.0f},
					{0.0f,		element,	0.0f,		1.0f},
					{0.0f,		0.0f,		element,	1.0f},
					{element,	element,	element,	1.0f},
				};	

				// ClearRenderTargetView : 작업할 영역(스크린 전체)을 초기화
				DeviceContext->ClearRenderTargetView(RenderTargetView, Color);

				// 연산하는 버퍼에서 출력하는 버퍼로 넘기는 작업
				MSUT(SwapChain->Present(1,0));

				element += delta;

				std::cout << element << std::endl;

				if(element < 0.0f || 1.0f <= element)
					delta *= -1;

				실행결과 : 검정색 화면에서 조금씩 흰색으로 전환되는 창 생성
*/
#pragma endregion
					// D3D11_MAPPED_SUBRESOURCE Subresource = D3D11_MAPPED_SUBRESOURCE();
					// MUST(DeviceContext->Map(Buffer::Vertex, 0, D3D11_MAP_WRITE_DISCARD, 0, &Subresource)); // discard : 가져온 값을 저장하지 않고 삭제
					// 배열로 넘겨주고 싶을 때 배열의 몇번째 부터 넘겨줄것인지 Map을 통해 설정
					{
						static struct
						{
							float const Width = 156;
							float const Height = 160;
						}
						Frame;

						// static을 통해 지역 밖에서도 값을 유지
						// (= unsigned int)
						static unsigned Count	= 0;
						static unsigned FPM		= 256;
						static unsigned Motion	= 9;
						static unsigned Row		= 0;

						float const Coordinates[4][2]
						{
							{Frame.Width * (Count / FPM + 0),	Frame.Height * 0},
							{Frame.Width * (Count / FPM + 1),	Frame.Height * 0},
							{Frame.Width * (Count / FPM + 0),	Frame.Height * 1},
							{Frame.Width * (Count / FPM + 1),	Frame.Height * 1}
						};

						Count+=1;
						if (FPM * Motion - 1 < Count)
							Count = 0;
						
						Buffer::Update(Buffer::Vertex, Coordinates);
						// memcpy_s(Subresource.pData, Subresource.RowPitch, Colors, sizeof(Colors));
					}
					// DeviceContext->Unmap(Buffer::Vertex, 0);
					{
						float const W = 156;
						float const H = 160;

						float const X = 0; 
						float const Y = 0;

						float const Transform[4][4]
						{
							W, 0, 0, X,
							0, H, 0, Y,
							0, 0, 1, 0,
							0, 0, 0, 1
						};
						Buffer::Update(Buffer::Constant[0], Transform);
					}
					{
						float const Transform[4][4]
						{
							1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1
						};
						Buffer::Update(Buffer::Constant[1], Transform);
					}
					{
						float const X = 2.0f / 500.0f;
						float const Y = 2.0f / 500.0f;

						float const Transform[4][4]
						{
							X, 0, 0, 0,
							0, Y, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1
						};
						Buffer::Update(Buffer::Constant[2], Transform);
					}
				}
#pragma endregion

#pragma region SwapChain Event
				{
					float const Color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

					DeviceContext->ClearRenderTargetView(RenderTargetView, Color);
					// 작업할 렌더 영역(스크린 전체)의 초기화 (색 입히기)

					DeviceContext->Draw(4, 0);
					// Draw : Vertex에 대한 정보를 받는 메서드
					// Draw와 DrawIndex의 차이
					// DRAW :한 번 사용한 정점은 다시 사용 불가
					// DRAWINDEX: 정점에 순서를 매겨 정점을 재사용 가능

					MUST(SwapChain->Present(0, 0));
					// flag : 이미지 넘겨줄 때 추가 옵션의 사용 여부
				}
#pragma endregion
				return 0;
			}
			// 창이 없어질 때 발생하는 메세지
			// WM_QUIT : 종료 메세지(운용 프로그램) / WM_DESTROY : 소멸 메세지(Only 창)
			case WM_DESTROY:
			{
#pragma region Release
				RenderTargetView->Release();
				InputLayout->Release();
				Buffer::Vertex->Release();
				//Buffer::Index->Release();
				SwapChain->Release();
				Device->Release();
				DeviceContext->Release();
#pragma endregion		
				// WM_QUIT을 호출하는 함수 : 프로그램 종료
				PostQuitMessage(0);
				return 0;
			}
			// 창의 사이즈가 변경될 때 발생하는 메세지
			case WM_SIZE:
			{
				{
#pragma region Viewport Setting
					{
						D3D11_VIEWPORT Viewport = D3D11_VIEWPORT();

						Viewport.Width = LOWORD(lParameter);
						Viewport.Height = HIWORD(lParameter);
					
						DeviceContext->RSSetViewports(1, &Viewport);
					}
#pragma endregion

#pragma region Swap Chain Setting
					if (RenderTargetView)
					{
						RenderTargetView->Release();

						MUST(SwapChain->ResizeBuffers
						(
							1,								// 버퍼의 개수
							LOWORD(lParameter),				// 윈도우 창의 WIDTH
							HIWORD(lParameter),				// 윈도우 창의 HEIGHT
							DXGI_FORMAT_B8G8R8A8_UNORM,		// 1로 정규화
							0
						));
					}
#pragma endregion

#pragma region Create RenderTargetView
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
#pragma endregion
				}

				return 0;
			}
			// 메세지는 들어오는데 case가 지정되지 않은 메세지
			default:
			{
				// 처리가 필요하기 때문에 그 메세지를 default 처리하는 함수
				return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
			}
		}
		return 0;
	}
}
#pragma region 속성 설정 ***

// 속성 -> 링커 -> 시스템 -> 하위 시스템 -> '창'으로 변경

// 속성 -> 구성 속성 -> 고급 -> 문자 집합 -> '멀티바이트 문자 집합 사용'으로 변경

// 속성 -> 링커 -> 모든 속성 -> 추가 종속성 -> 편집 -> d3d11.lib 추가    

#pragma endregion
