#include <cassert>
#include <d3d11.h>
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
		ID3D11Device		   * Device;			
		ID3D11DeviceContext	   * DeviceContext;		
		IDXGISwapChain         * SwapChain;			

		namespace Buffer
		{
			ID3D11Buffer* Vertex;
			ID3D11Buffer* Constant[2];

			template<typename Data>
			void Update(ID3D11Buffer* const buffer, Data const& data)
			{
				D3D11_MAPPED_SUBRESOURCE Subresource = D3D11_MAPPED_SUBRESOURCE();

				MUST(DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Subresource));
				{
					memcpy_s(Subresource.pData, Subresource.RowPitch, data, sizeof(data));
				}
				DeviceContext->Unmap(buffer, 0);
			};
		}
		ID3D11RenderTargetView* RenderTargetView;
	}

	namespace String
	{
		void Render(HFONT const hFont, LPCSTR const string, COLORREF const color, SIZE const size, POINT const center)
		{
			IDXGISurface1* Surface = nullptr;

			MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&Surface)));
			{
				HDC hdc = HDC();

				MUST(Surface->GetDC(false, &hdc));
				{
					if (hFont != HFONT())
						SelectObject(hdc, hFont);

					SetTextColor(hdc, color);

					RECT area
					{
						center.x - size.cx / 2,
						center.y - size.cy / 2,
						center.x + size.cx / 2,
						center.y + size.cy / 2
					};

					// ~'\0' : -1을 의미하며 문자열이 들어올때 길이를 계산
					DrawText(hdc, string, ~'\0', &area, 0);
				}
				MUST(Surface->ReleaseDC(nullptr));
			}
			Surface->Release();
			
			DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
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
				size.cx * (BPP / 8)
			};

			ID3D11Texture2D* Texture2D = nullptr;

			MUST(Device->CreateTexture2D(&Descriptor, &Subresource, &Texture2D));
			{
				MUST(Device->CreateShaderResourceView(Texture2D, nullptr, &(handle = new Handle)->ShaderResourceView));
			}
			Texture2D->Release();
		}

		void Render(Handle const * const & handle, RECT const area)
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

		void Delete(Handle const * const & handle)
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

	void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
			case WM_CREATE :
			{
#pragma region Create SwapChain
				{
					DXGI_SWAP_CHAIN_DESC Descriptor = DXGI_SWAP_CHAIN_DESC();

					Descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
					Descriptor.SampleDesc.Count = 1;
					Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
					Descriptor.BufferCount = 1;

					// Hwnd의 Width와 Height 정보를 자동으로 가져와 설정
					Descriptor.OutputWindow = hWindow;
					Descriptor.Windowed = true;
					Descriptor.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;

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
						D3D11_INPUT_ELEMENT_DESC Descriptor[2]
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
				{
					D3D11_BLEND_DESC Descriptor = D3D11_BLEND_DESC();

					Descriptor.RenderTarget->BlendEnable			= true;							
					Descriptor.RenderTarget->SrcBlend				= D3D11_BLEND_SRC_ALPHA;		
					Descriptor.RenderTarget->DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;	
					Descriptor.RenderTarget->BlendOp				= D3D11_BLEND_OP_ADD;			
					Descriptor.RenderTarget->SrcBlendAlpha			= D3D11_BLEND_ZERO;				
					Descriptor.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ONE;				
					Descriptor.RenderTarget->BlendOpAlpha			= D3D11_BLEND_OP_ADD;			
					Descriptor.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL; 

					ID3D11BlendState* BlendState = nullptr;

					MUST(Device->CreateBlendState(&Descriptor, &BlendState));

					DeviceContext->OMSetBlendState(BlendState, nullptr, D3D11_DEFAULT_SAMPLE_MASK);

					BlendState->Release();
				}
#pragma endregion
				return;
			}
			case WM_APP :
			{
#pragma region SwapChain Event
			
				MUST(SwapChain->Present(0, 0));

				float const Color[4] = { 1.0f , 1.0f , 1.0f , 1.0f };

				DeviceContext->ClearRenderTargetView(RenderTargetView, Color);
#pragma endregion
				return;
			}
			case WM_DESTROY :
			{
#pragma region Release

				DeviceContext->ClearState();

				RenderTargetView->Release();

				for (UINT u = 0; u < 2; ++u)
					Buffer::Constant[u]->Release();

				Buffer::Vertex->Release();

				SwapChain->Release();
				DeviceContext->Release();
				Device->Release();

				return;
			}
#pragma endregion
			case WM_SIZE :
			{
#pragma region Viewport Setting
					{
						D3D11_VIEWPORT Viewport = D3D11_VIEWPORT();

						Viewport.Width  = LOWORD(lParameter);
						Viewport.Height = HIWORD(lParameter);

						DeviceContext->RSSetViewports(1, &Viewport);
					}
#pragma endregion
					{
#pragma region Swap Chain Setting
						if (RenderTargetView != nullptr)
						{
							RenderTargetView->Release();

							MUST(SwapChain->ResizeBuffers
							(
								1,
								LOWORD(lParameter),
								HIWORD(lParameter),
								DXGI_FORMAT_B8G8R8A8_UNORM,
								// HDC와 RenderTargetView를 연결
								// Buffer를 통해 가져온 Texture2D는 Surface를 포함하는데 Surface가 가지고 있는 GetDC를 사용
								// GetDC를 사용하여 Surface에 HDC를 연결하고 RenderTargetView와 결함
								DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE
							));
						}
#pragma endregion

#pragma region Create RenderTargetView
						{
							ID3D11Texture2D* texture2D = nullptr;
							MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&texture2D)));
							{
								IDXGISurface1* Surface = nullptr;
								MUST(texture2D->QueryInterface(IID_PPV_ARGS(&Surface)));
								{
									HDC hdc = HDC();
									MUST(Surface->GetDC(false, &hdc));
									{
										// TRANSPARENT : 원래 있던 배경을 그대로 적용
										SetBkMode(hdc, TRANSPARENT);
									}
									MUST(Surface->ReleaseDC(nullptr));
								}
								Surface->Release();
							}
							{
								MUST(Device->CreateRenderTargetView(texture2D, nullptr, &RenderTargetView));

								DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
							}
							texture2D->Release();
						}
#pragma endregion
					}
				return;
			}
		}
	}
}
