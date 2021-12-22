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
		ID3D11Device		   * Device;			// GPU���� ���� �ڿ��� ���� �� �ִ� ��ü
		ID3D11DeviceContext	   * DeviceContext;		// Lendering Pipeline�� �ڿ��� �����Ͽ� ����� �� �ְ� �ϴ� ��ü
		IDXGISwapChain         * SwapChain;			// �� ���� ������ �����ư��� Rendering�� ȿ���� �����ִ� ��ü
		ID3D11RenderTargetView * RenderTargetView;	// �׸� ������ �����ϴ� ��ü

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

				// Texture �ڿ������� ���Ǹ�, �� �� ���ۺ��� ������ ������ �Ÿ�(Byte)
				size.cx * (BPP / 8)
			};

			ID3D11Texture2D* Texture2D = nullptr;

			MUST(Device->CreateTexture2D(&Descriptor, &Subresource, &Texture2D));
			{
				// handle�� �������̱� ������ �����Ҵ����� ����
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
					// device�� ���� descriptor���� ��� �ִ� Descriptor
					DXGI_SWAP_CHAIN_DESC Descriptor = DXGI_SWAP_CHAIN_DESC();

					Descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
					Descriptor.SampleDesc.Count = 1;
					Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
					Descriptor.BufferCount = 1;

					// Hwnd�� Width�� Height ������ �ڵ����� ������ ����
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

					// Sprite Animation�� �����ϱ� ���� 2ȸ Buffer ����
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
				// BlendState : 2���� Layer�� ȥ���Ͽ� �������� ���� (���İ��� ����)
				// RenderTarget : �̹����� ������ ���� / �ִ� 8������ ���� ����
				{
					D3D11_BLEND_DESC Descriptor = D3D11_BLEND_DESC();
					
					// Src  : PS�� ����ϴ� RGB (ȥ���� ����)
					// Dest : �̹� RenderTarget�� �����Ǿ��ִ� ����
#pragma region Blend �Ű����� ���� ***
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

	������ �� ��� : Dst + Src

	SrcBlendAlpha  : ���� ���� �ȼ��� ������ (0,0,0,0)
	DestBlendAlpha : ����� ���� (1,1,1,1)
*/
#pragma endregion
					Descriptor.RenderTarget->BlendEnable			= true;							// Bend Ȱ��ȭ (�� ȥ�� ���θ� ����)
					Descriptor.RenderTarget->SrcBlend				= D3D11_BLEND_SRC_ALPHA;		// RGBA�� ��� ���� ȥ���� ������ ���� (src  ȥ�� ����)
					Descriptor.RenderTarget->DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;	// RGBA�� ��� ���� ȥ���� ������ ���� (dest ȥ�� ����)
					Descriptor.RenderTarget->BlendOp				= D3D11_BLEND_OP_ADD;			// Blend ó�� ���� ����
					
					Descriptor.RenderTarget->SrcBlendAlpha			= D3D11_BLEND_ZERO;				// ���� ���� �ȼ��� ������ ����	
					Descriptor.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ONE;				// Dest�� ��� ����
					Descriptor.RenderTarget->BlendOpAlpha			= D3D11_BLEND_OP_ADD;			// Blend ó�� ���� ����
					Descriptor.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL; // �����ϴ� ���� ���� ���
					// RenderTarget�� 1���� ���

					ID3D11BlendState* BlendState = nullptr;

					MUST(Device->CreateBlendState(&Descriptor, &BlendState));

					// �̹� �׷��� �̹��� ���� ������ �̹����� ȥ���ϱ� ������ OM Stage�� ����
					// SampleMask : ȥ�յ� �̹����� Multy Sampling�� ����
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
 - Layout, Buffer ����
 - Topology ���� : ���� ���� ������ ��� ����ϴ��� ����

VS
 - Shader : GPU������ �ϴ� ����ü
 - Matrix�� �̿��Ͽ� ���� ���� 3���� ���� ����

RS
 - Projection : ���ٰ� ����
 - �������� �����Ͽ� ������ ä���ִ� �۾� ����
 - Ŭ���� / ������̽� �ø�
 - 3D�� 2D��ǥ�� �ٽ� �ٲ��� (Viewport ����)

PS
 - �� ����
 - SV_Target

OM
 - ���� ���
 - RenderTarget View ����

***
�ڿ� ����ü
Buffer  : ���� �ڷ��� (VIC)
Tecture : �̹��� �ڿ� (RenderTargetView : ����(OM), ShaderResourceView : �б�(PS)) 

ShaderResourcView : PS���� Free.png��� �̹����� '�а�' ���� ������ �Ŀ� RenderTargetView�� ����
RenderTargetView  : OM�ּ� ���������� ������� �̹����� '����' ȭ�鿡 ���

***
PNG vs JPEG
JPEG : �뷮�� ������ �� ��ŭ ������ �Ǿ� �սǵ� �����Ͱ� �߻�
PNG  : �뷮�� ������ ���� ��ŭ ������ �ս��� �ּ�ȭ

***
MAP(�Լ�)
 - �Ϲ������� CPU�� GPU ������ �۾� ��踦 ���� �ִµ� �Ͻ������� �����ִ� ���� ����
 - ���� ���¿��� GPU�� ������ �� �ִ� ���̵������ SubResource�� ���
*/
#pragma endregion