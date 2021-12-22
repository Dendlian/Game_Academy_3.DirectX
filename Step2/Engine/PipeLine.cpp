#include <d3d11.h>
#include <cassert>
#include "PipeLine.h"

#pragma region Console
// #pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma endregion

#ifndef  _DEBUG
#define MUST(Expression) (		(		  (Expression)))
#else
#define MUST(Expression) (assert(SUCCEEDED(Expression)))
#endif

namespace Rendering::Pipeline
{
	namespace
	{
		ID3D11Device		   * Device;			// GPU에서 쓰일 자원을 만들 수 있는 객체
		ID3D11DeviceContext	   * DeviceContext;		// Lendering Pipeline에 자원을 연결하여 사용할 수 있게 하는 객체
		IDXGISwapChain         * SwapChain;			// 두 개의 연산을 번갈아가며 Rendering의 효율을 높혀주는 객체
		ID3D11RenderTargetView * RenderTargetView;	// 그릴 공간을 설정하는 객체

		namespace Buffer
		{
			ID3D11Buffer* Vertex;
			ID3D11Buffer* Constant[3];

			template<typename Data>
			void Update(ID3D11Buffer * const buffer, Data const & data)
			{
				D3D11_MAPPED_SUBRESOURCE Subresource = D3D11_MAPPED_SUBRESOURCE();

				MUST(DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Subresource));
				{
					memcpy_s(Subresource.pData, Subresource.RowPitch, data, sizeof(data));
				}
				DeviceContext->Unmap(buffer, 0);
			}
		}
	}

	namespace Texture
	{
		struct Handle final
		{
			ID3D11ShaderResourceView* ShaderResourceView = nullptr;
		};

		void Create(Handle*& handle, SIZE const size, BYTE const* const data)
		{
			D3D11_TEXTURE2D_DESC const Descriptor
			{
				static_cast<UINT>(size.cx),
				static_cast<UINT>(size.cy),
				1,
				1,
				DXGI_FORMAT_B8G8R8A8_UNORM,
				1,
				0,
				D3D11_USAGE_IMMUTABLE,
				D3D11_BIND_SHADER_RESOURCE,
			};

			UINT constexpr BPP = 32;

			D3D11_SUBRESOURCE_DATA const Subresource
			{
				data,

				// Texture 자원에서만 사용되며, 한 줄 시작부터 다음줄 까지의 거리(Byte)
				size.cx * (BPP / 8)
			};

			ID3D11Texture2D* Texture2D = nullptr;

			MUST(Device->CreateTexture2D(&Descriptor, &Subresource, &Texture2D));
			{
				// handle은 포인터이기 때문에 동적할당으로 생성
				MUST(Device->CreateShaderResourceView(Texture2D, nullptr, &(handle = new Handle)->ShaderResourceView));
			}
			Texture2D->Release();
		}

		void Render(Handle*& handle, RECT const area)
		{	
			DeviceContext->PSSetShaderResources(0, 1, &handle->ShaderResourceView);
			{
				float const Cooredinates[4][2]
				{
					{ static_cast<float>(area.left),	static_cast<float>(area.top)    },
					{ static_cast<float>(area.right),	static_cast<float>(area.top)	},
					{ static_cast<float>(area.left),	static_cast<float>(area.bottom)	},
					{ static_cast<float>(area.right),	static_cast<float>(area.bottom)	}
				};

				Buffer::Update(Buffer::Vertex, Cooredinates);
			}
			DeviceContext->Draw(4,0);
		}

		void Delete(Handle*& handle)
		{
			handle->ShaderResourceView->Release();

			delete handle;
		}
	}

	namespace Transform
	{
		template<Type type>
		void Update(Matrix const& matrix)
		{
			Buffer::Update(Buffer::Constant[static_cast<UINT>(type)], matrix);
		}

		template void Update<Type::Former>(Matrix const& matrix);
		template void Update<Type::Latter>(Matrix const& matrix);
	}

	void CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{

		switch (uMessage)
		{
			case WM_CREATE :
			{
#pragma region Create SwapChain
				{
					// device에 대한 descriptor까지 담고 있는 Descriptor
					DXGI_SWAP_CHAIN_DESC Descriptor = DXGI_SWAP_CHAIN_DESC();

					Descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
					Descriptor.SampleDesc.Count = 1;
					Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
					Descriptor.BufferCount = 1;

					// Hwnd의 Width와 Height 정보를 자동으로 가져와 설정
					Descriptor.OutputWindow = hWindow;
					Descriptor.Windowed = true;

					MUST(D3D11CreateDeviceAndSwapChain
					(
						nullptr,
						D3D_DRIVER_TYPE_HARDWARE,
						nullptr,
						0,
						nullptr,
						0,
						D3D11_SDK_VERSION,
						&Descriptor,
						&SwapChain,
						&Device,
						nullptr,
						&DeviceContext
					));
				}
#pragma endregion

#pragma region Vertex Shader Setting
				{
					#include "Shader/Bytecode/Vertex.h"
					{
						D3D11_INPUT_ELEMENT_DESC Descriptor[2] =
						{
							{ "POSITION" , 0, DXGI_FORMAT_R32G32_FLOAT, 0 },
							{ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT, 1 }
						};
						
						ID3D11InputLayout* InputLayout = nullptr;

						 MUST(Device->CreateInputLayout(Descriptor, 2 , Bytecode, sizeof(Bytecode), &InputLayout));
						 
						 DeviceContext->IASetInputLayout(InputLayout);
						
						 InputLayout->Release();
					}

					{
						ID3D11VertexShader* VertexShader = nullptr;

						MUST(Device->CreateVertexShader(Bytecode, sizeof(Bytecode), nullptr, &VertexShader));

						DeviceContext->VSSetShader(VertexShader, nullptr, 0);

						VertexShader->Release();
					}
				}
#pragma endregion   

#pragma region Pixel Shader Setting
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

#pragma region Vertex Buffer Setting
				{
					DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
				}

				{
					float const Coordinates[4][2]
					{
						{ -0.5f, +0.5f },
						{ +0.5f, +0.5f },
						{ -0.5f, -0.5f },
						{ +0.5f, -0.5f }
					};

					D3D11_BUFFER_DESC const Descriptor
					{
						sizeof(Coordinates),
						D3D11_USAGE_IMMUTABLE,
						D3D11_BIND_VERTEX_BUFFER,
						0
					};

					D3D11_SUBRESOURCE_DATA const Subresource
					{
						Coordinates
					};

					ID3D11Buffer* Buffer = nullptr;

					// Sprite Animation을 생성하기 위해 2회 Buffer 생성
					MUST(Device->CreateBuffer(&Descriptor, &Subresource, &Buffer));

					UINT const Stride = sizeof(*Coordinates);
					UINT const Offset = 0;

					DeviceContext->IASetVertexBuffers(0, 1, &Buffer, &Stride, &Offset);

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

					for (UINT u = 0; u < 2; ++u)
						MUST(Device->CreateBuffer(&Descriptor, nullptr, &Buffer::Constant[u]));

					DeviceContext->VSSetConstantBuffers(0, 2, Buffer::Constant);
				}
#pragma endregion

#pragma region BlendState Setting
				// BlendState : 2개의 Layer를 혼합하여 반투명도를 구현 (알파값을 조정)
				// RenderTarget : 이미지를 보여줄 공간 / 최대 8개까지 생성 가능
				{
					D3D11_BLEND_DESC Descriptor = D3D11_BLEND_DESC();
					
					// Src  : PS가 출력하는 RGB (혼합할 색상)
					// Dest : 이미 RenderTarget에 생성되어있는 색상
#pragma region Blend 매개변수 설정 ***
/*
D3D11_BLEND_ZERO				= 1,		// (0,0,0,0)          
D3D11_BLEND_ONE					= 2,        // (1,1,1,1)
D3D11_BLEND_SRC_COLOR			= 3,      	// (R,G,B,A)
D3D11_BLEND_INV_SRC_COLOR		= 4,		// (1-R, 1-G, 1-B, 1-A)
D3D11_BLEND_SRC_ALPHA			= 5,		// (A,A,A,A)
D3D11_BLEND_INV_SRC_ALPHA		= 6,		// (1-A, 1-A, 1_A, 1-A)
D3D11_BLEND_DEST_ALPHA			= 7,
D3D11_BLEND_INV_DEST_ALPHA		= 8,
D3D11_BLEND_DEST_COLOR			= 9,
D3D11_BLEND_INV_DEST_COLOR		= 10,
D3D11_BLEND_SRC_ALPHA_SAT		= 11,
D3D11_BLEND_BLEND_FACTOR		= 14,
D3D11_BLEND_INV_BLEND_FACTOR	= 15,
D3D11_BLEND_SRC1_COLOR			= 16,
D3D11_BLEND_INV_SRC1_COLOR		= 17,
D3D11_BLEND_SRC1_ALPHA			= 18,
D3D11_BLEND_INV_SRC1_ALPHA		= 19		

	Dst(0.25, 0.75, 0.40, 0.70)
	Src(0.70, 0.10, 0.30, 0.50)

	Dst = (0.25, 0.75, 0.40) * (1 - 0.5)
	Src = (0.70, 0.10, 0.30) * (0.5)

	불투명 색 결과 : Dst + Src

	SrcBlendAlpha  : 새로 쓰일 픽셀의 불투명도 (0,0,0,0)
	DestBlendAlpha : 배경을 유지 (1,1,1,1)
*/
#pragma endregion
					Descriptor.RenderTarget->BlendEnable			= true;							// Bend 활성화 (색 혼합 여부를 결정)
					Descriptor.RenderTarget->SrcBlend				= D3D11_BLEND_SRC_ALPHA;		// RGBA중 어떠한 값에 혼합할 것인지 설정 (src  혼합 비율)
					Descriptor.RenderTarget->DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;	// RGBA중 어떠한 값에 혼합할 것인지 설정 (dest 혼합 비율)
					Descriptor.RenderTarget->BlendOp				= D3D11_BLEND_OP_ADD;			// Blend 처리 연산 설정
					
					Descriptor.RenderTarget->SrcBlendAlpha			= D3D11_BLEND_ZERO;				// 새로 쓰일 픽셀의 불투명도 설정	
					Descriptor.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ONE;				// Dest의 배경 유지
					Descriptor.RenderTarget->BlendOpAlpha			= D3D11_BLEND_OP_ADD;			// Blend 처리 연산 설정
					Descriptor.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL; // 배제하는 색상 없이 사용
					// RenderTarget을 1개만 사용

					ID3D11BlendState* BlendState = nullptr;

					MUST(Device->CreateBlendState(&Descriptor, &BlendState));

					// 이미 그려진 이미지 위에 반투명 이미지를 혼합하기 때문에 OM Stage에 연결
					// SampleMask : 혼합된 이미지에 Multy Sampling을 설정
					DeviceContext->OMSetBlendState(BlendState, nullptr, D3D11_DEFAULT_SAMPLE_MASK);

					BlendState->Release();
				}
#pragma endregion
				return;
			}
			case WM_APP :
			{

#pragma region SwapChain Event
				{
					MUST(SwapChain->Present(0, 0));

					float const Color[4] = { 1.0f , 1.0f , 1.0f , 1.0f };

					DeviceContext->ClearRenderTargetView(RenderTargetView, Color);
				}
#pragma endregion
				return;
			}
			case WM_DESTROY :
			{
#pragma region Release
				RenderTargetView->Release();

				Buffer::Vertex->Release();

				for (UINT u = 0; u < 2; ++u)
					Buffer::Constant[u]->Release();

				SwapChain->Release();
				DeviceContext->Release();
				Device->Release();

				return;
			}
#pragma endregion
			case WM_SIZE :
			{
				{
#pragma region Viewport Setting
					{
						D3D11_VIEWPORT Viewport = D3D11_VIEWPORT();

						Viewport.Width  = LOWORD(lParameter);
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
							1,
							LOWORD(lParameter),
							HIWORD(lParameter),
							DXGI_FORMAT_B8G8R8A8_UNORM,
							0
						));
					}
#pragma endregion

#pragma region Create RenderTargetView
					{
						ID3D11Texture2D* texture2D = nullptr;
						MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&texture2D)));

						MUST(Device->CreateRenderTargetView(texture2D, nullptr, &RenderTargetView));
						texture2D->Release();

						DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
					}
#pragma endregion
				}
				return;
			}
		}
	}
}

#pragma region DirectX Review
/*
IA
 - Layout, Buffer 생성
 - Topology 생성 : 점에 대한 정보를 어떻게 사용하는지 설정

VS
 - Shader : GPU연산을 하는 집합체
 - Matrix를 이용하여 점에 대한 3차원 공간 생성

RS
 - Projection : 원근감 설정
 - 폴리곤을 생성하여 공간을 채워주는 작업 실행
 - 클리핑 / 백슾페이스 컬링
 - 3D를 2D좌표로 다시 바꿔줌 (Viewport 설정)

PS
 - 색 추출
 - SV_Target

OM
 - 최종 출력
 - RenderTarget View 설정

***
자원 전달체
Buffer  : 숫자 자료형 (VIC)
Tecture : 이미지 자원 (RenderTargetView : 쓰기(OM), ShaderResourceView : 읽기(PS)) 

ShaderResourcView : PS에서 Free.png라는 이미지를 '읽고' 색을 추출한 후에 RenderTargetView로 전달
RenderTargetView  : OM애서 최종적으로 만들어진 이미지를 '쓰고' 화면에 출력

***
PNG vs JPEG
JPEG : 용량이 작지만 그 만큼 압축이 되어 손실된 데이터가 발생
PNG  : 용량이 많지만 ㄱ그 만큼 데이터 손실을 최소화

***
MAP(함수)
 - 일반적으로 CPU와 GPU 사이의 작업 경계를 막고 있는데 일시적으로 열어주는 역할 수행
 - 열린 상태에서 GPU에 전달할 수 있는 가이드라인인 SubResource를 사용
*/
#pragma endregion