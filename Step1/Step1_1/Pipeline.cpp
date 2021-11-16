#include <d3d11.h>
#include <iostream>
#pragma region cassert ***
/*
cassert 
 - ���带 �� �� ���α׷� ���� ���θ� �����ϴ� �������
 - ������ ������ ������ ���� �������� �Ϸ�Ǵ��� ����ġ ���� ������ �˻�
 - ��� comInterface�� �� ����������� ����(HRESULT : Long type)�� ��ȯ
 - assert ��ũ�� �Լ��� ���� HRESULT�� �˻�
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

// namespace : static ��� / ������ ���� �ȿ��� ������ ������ �ܺο��� ��� �Ұ�
// Pipeline  : Rendering�� ���� �ܰ�
namespace Pipeline 
{
	// DXGI : �׷��� �������̽�
	namespace
	{	
		ID3D11Device				* Device;				// ���� �� �ڿ��� ������ ���
		ID3D11DeviceContext			* DeviceContext;		// ���� �ڿ��� ������ ���������ο� ����
		IDXGISwapChain				* SwapChain;			// (Back Buffer) ������ �۾��� �����ϸ� ���� (���� + Scene)
		ID3D11RenderTargetView		* RenderTargetView;		// �ڿ��� �׸��� ��� ����
		ID3D11InputLayout			* InputLayout;
		
		namespace Buffer
		{
			ID3D11Buffer* Vertex;
		    //ID3D11Buffer* Index;
			ID3D11Buffer* Constant[3];

			template<typename Data>
			void Update(ID3D11Buffer* const buffer, Data const& data)
			// Data �� buffer�� ���� / buffer�� ������ data�� �ʱ�ȭ
			{
				D3D11_MAPPED_SUBRESOURCE Subresource = D3D11_MAPPED_SUBRESOURCE();
				MUST(DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Subresource));
				// Subresource�� �����ϸ� buffer�� �Բ� �ڵ����� ����
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
			// â�� ������ �� �߻��ϴ� �޼���
			case WM_CREATE:
			{
#pragma region Create SwapChain

#pragma region DXGI_SWAP_CHAIN_DESC ��ü ���� ***
/*
DXGI_SWAP_CHAIN_DESC
 - DXGI_MODE_DESC SampleDesc;
	{
	UINT Width;									// ����
    UINT Height;								// ����
    DXGI_RATIONAL RefreshRate;					// ȭ���� ���ŷ�
		{
			�������� ���ߴ� ������ : 60 ������
			0/0 : ��ǻ�Ͱ� 1�� �ؼ��ϸ� �ڵ� ���������� ����
			UINT Numerator;		// 60/1 �� 60
			UINT Denominator;	// 60/1 �� 1
		}
    DXGI_FORMAT Format;							// �������� (Buffer�� ���� �����ϸ� �Ϸ�)
    DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;	// �̹����� ǥ���ϴ� ����
		{
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED        = 0,	// �⺻�� : �� ���� X
			DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE        = 1,	// �̹����� �Ѳ����� ����
			DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST  = 2,	// ��� �̹������� ���� ����
			DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST  = 3		// �ϴ� �̹������� ���� ����
		}
    DXGI_MODE_SCALING Scaling;					// â�� Ȯ���� �� ���� ��� ����
		{
		DXGI_MODE_SCALING_UNSPECIFIED   = 0,	// �⺻�� : �� ���� X
		DXGI_MODE_SCALING_CENTERED      = 1,	// ���� ���� X : â�� Ȯ���ϸ� �����ִ� �̹��� ���
		DXGI_MODE_SCALING_STRETCHED     = 2		// ��� ���� : â�� Ȯ���ϸ� ���� �̹����� Ȯ��
		}
	}

 - DXGI_SAMPLE_DESC SampleDesc;
	// ��Ƽ�󸮾��
	// ���� ���ø� : ��� �� �ȼ��� ������ �ٽ� ��� �� ��ȯ
	// ��Ƽ ���ø� : �׵θ��� ��� �� ��ȯ
	{
	UINT Count;		// ��Ƽ ���ø��� �� �� 1���� Pixel�� �� ���� ������ ����
					// (Pixel �� ��Ƽ ���ø� ��)
	UINT Quality;	// �⺻�� = 0 : �̹����� ǰ���� �ּ����� ǰ�� ������ ����
	}

 - DXGI_USAGE BufferUsage;
	// �׸� ��ü�� �Ѱ��ִ� ����� ����
	// ������ Ÿ���� ����ϴ� �뵵�� ���
	// => DXGI_USAGE_RENDER_TARGET_OUTPUT

 - UINT BufferCount;
	// ������ ��� ����
	// 0 : ���� 0�� ���
	// 1 : ���� 2�� ���

 - HWND OutputWindow;
	// ��� â�� ������� ����

 - BOOL Windowed;
	// â��� ��뿩��

 - DXGI_SWAP_EFECT SwapEffect; 
	// Buffer���� �ѱ� �����͸� ��� ������ �������� ���θ� ����
	// �⺻�� 0 = DISCARD : �����͸� �������� �ʰ� ���
	// ex) back buffer���� ������ �� �����͸� front buffer�� �ѱ� �� ����
	{
	DXGI_SWAP_EFFECT_DISCARD			= 0,	// ������ ����
    DXGI_SWAP_EFFECT_SEQUENTIAL			= 1,	// ������ ����
    DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL	= 3, 
    DXGI_SWAP_EFFECT_FLIP_DISCARD		= 4	
	}

 - UINT Flags;
	// �߰� �ɼ�
	// �⺻�� 0 : Flag ��� X
*/
#pragma endregion				
				{
					// SwapChain�� Description ����
					DXGI_SWAP_CHAIN_DESC Descriptor = DXGI_SWAP_CHAIN_DESC();
					
					// 4����Ʈ�� FLOAT ������ ������ ����
					// Format : ������ �ѱ�� ���
					// UNORM  : 1�� ����ȭ
					Descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
					
					// �ȼ��� ����(��ܽ� �̹���)�� ���� : ��Ƽ�ٸ����
					// SampleDesc : ��Ƽ ���ϸ������ ���
					// Count : �ȼ��� ��Ƽ ���ø����� �˻��ϴ� Ƚ��
					Descriptor.SampleDesc.Count = 1;
					
					// �׸� ��ü�� �Ѱ��ִ� ����� ����
					// ������ Ÿ���� ����ϴ� �뵵�� ���
					Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
					
					// ������ ��� ����
					// 0 : ���� 0�� ���
					// 1 : ���� 2�� ���
					Descriptor.BufferCount = 1;
					
					// ��� â�� ������� ����
					Descriptor.OutputWindow = hWindow;
					
					// â��� ��뿩��
					Descriptor.Windowed = true;
				

					HRESULT hr = D3D11CreateDeviceAndSwapChain
					(
						nullptr,					// ���(Display�� ���� ���� �ý���) �̻��
						D3D_DRIVER_TYPE_HARDWARE,	// ����̺� Ÿ������ �ϵ��� ���� (�ϵ������ ������ �޾� �������ϵ��� ����)
						nullptr,					// �߰����� ����Ʈ���� Ÿ�� ��� �̻��
						0,							// �÷��� �̻��
						nullptr,					// Direct X�� ���� ȣȯ���� �⺻���� ���� // �⺻���� ������ 11_0 ~ 6_0
						0,							// ������ ���� ȣȯ���� ��� ���
						D3D11_SDK_VERSION,			// ��ǻ�Ϳ��� � ���� ������ ������� �뺸
						&Descriptor,				// ������ �ּ�
						&SwapChain,					// ������ ������ �ּ� (SWAP)
						&Device,					// ������ ������ �ּ� (DEVICE)
						nullptr,					// Direct X�� ���� ȣȯ���� �⺻���� ����
						&DeviceContext				// ������ ������ �ּ� (DEVICECONTEXT)
					);
					assert(SUCCEEDED(hr));			// ���α׷��� �� ����������� Ȯ��
				}
#pragma endregion

#pragma region Vertex Shader Setting
				{
					#include "Shader/Bytecode/Vertex.h"
					{
						D3D11_INPUT_ELEMENT_DESC Descriptor[] =
						{
							// ������ ��Ȳ�� ���� �ٲ㼭 �ﰢ���� ����� ������ �ε����� ����
							{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0},
							{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1}
							/// D3D11_INPUT_ELEMENT_DESC(),
							/// D3D11_INPUT_ELEMENT_DESC()
						};
						/// Descriptor[0].SemanticName			= "POSITION";
						/// Descriptor[0].SemanticIndex			= 0;
						/// Descriptor[0].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
						/// Descriptor[0].InputSlot				= 0; // IA�ܰ迡�� �ĺ��ϴ� �������� �����ϴ� �κ�
						/// Descriptor[0].AlignedByteOffset 	= 0; // POSITION�� ���� ���� ���� ����
						/// Descriptor[0].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA; // ���� �������� ���� ����
						/// Descriptor[0].InstanceDataStepRate	= 0; // ���ۿ��� �׸� �ν��Ͻ��� ���� ���� (���� �����Ͷ�� ������ 0)
						/// 
						/// Descriptor[1].SemanticName			= "COLOR";
						/// Descriptor[1].SemanticIndex			= 0;
						/// Descriptor[1].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
						/// Descriptor[1].InputSlot				= 1; 
						/// Descriptor[1].AlignedByteOffset		= 16; 
						/// Descriptor[1].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA; 
						/// Descriptor[1].InstanceDataStepRate	= 0; 

						// GPU���� CPU�� �ٽ� ������ �Ѱ��ִ� �����̸� CPU���� ���� �� �ִ� 2���ڵ�� ��ȯ
						// VS�ܰ迡�� Inputlayout�� �ѱ�� ����
						// VertexBuffer�� �ִ� 15���� �� , 0 ~ 15 ���� 16���� �����ϴ�
						MUST(Device->CreateInputLayout(Descriptor, 2, Bytecode, sizeof(Bytecode), &InputLayout));

						DeviceContext->IASetInputLayout(InputLayout);
					}

					{
						// Vertex shader�� ����� ���� byte code�� ���
						ID3D11VertexShader* VertexShader = nullptr;

						MUST(Device->CreateVertexShader(Bytecode, sizeof(Bytecode), nullptr, &VertexShader));
						// Bytecode : vertexshader �� ����� ���� �ʿ��ϴ�

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
				
#pragma region Vertex ���� ***			
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
					// D3D11_BUFFER_DESC Descriptor = D3D11_BUFFER_DESC();		// CPU���� ����ϴ� �ڵ带 GPU���� ��� �����ϵ��� ��ȯ (������ ���)
					// Descriptor.ByteWidth = sizeof(Vertices);					// ������ ��ü ũ�⸦ ����
					// Descriptor.Usage = D3D11_USAGE_IMMUTABLE;				// GPU���� ���� �� �ֵ��� ��� ����
					// Descriptor.BindFlags = 0;								// CPU������ ���� ������ ������ ���� (���̳��Ϳ��� CPU�� ���� ����� �� �ϱ� ������ 0���� ����)
					// Descriptor.CPUAccessFlags = 0;
					// Descriptor.MiscFlags = 0;
					// Descriptor.StructureByteStride = 0;						// input layout���� �����ϰ� �ֱ� ������ ���� ���� X
																				// �Ѱ��� �����͸� ��� �������� ������ �������� ����																		
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
					// ���긮�ҽ� : ���� ������ ���� ���� �ʱ�ȭ�ϱ� ���� ���긮�ҽ��� ���� ��� ���ۿ� ���� ������ ����
	
					ID3D11Buffer* Buffer = nullptr;

					MUST(Device->CreateBuffer(&Descriptor, &SubResource, &Buffer));

					UINT const Stride = sizeof(*Coordinates);
					UINT const Offset = 0;

					DeviceContext->IASetVertexBuffers(0, 1, &Buffer, &Stride, &Offset);
					// ���۸� ������ �߰� ���������������ο� ������ ���ִ� �۾�
					// ��ŸƮ ����, ��� ���۸� �ѱ���, ��� �ڷ���� ����������, ???

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
					// ������ �����͸� �ﰢ�� ������� �ؼ��ϵ��� ����
					DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					// list -> strip : ������ ������ �ﰢ������ �������� �ʰ� ����� �ﰢ������ ����
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
				//	// subresource : ���� ����
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
							// �޸����� �ε�Ǵ� �̹����� �ٽ� ������
							FreeImage_FlipVertical(Bitmap);

							// Buffer�� �ƴ� Texture�� ����
							// ���� �ؽ�ó�� ���� ��ũ���� �ۼ�
							D3D11_TEXTURE2D_DESC Descriptor = D3D11_TEXTURE2D_DESC();

							Descriptor.Width = FreeImage_GetWidth(Bitmap);
							Descriptor.Height = FreeImage_GetHeight(Bitmap);
							// MipLevels : �ȼ��� �ʹ� ���� �̹����� ���� ���� �ؽ�ó�� ����ϴ� ����
										// -> �����͸� ����ϹǷμ� ������ �ӵ� ����
							Descriptor.MipLevels = 1;

							Descriptor.ArraySize = 1;
							Descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

							// ��Ƽ�󸮾��
							Descriptor.SampleDesc.Count = 1;
							Descriptor.SampleDesc.Quality = 0;

							Descriptor.Usage = D3D11_USAGE_IMMUTABLE;

							// �뵵 : ���̴��� ����
							Descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;

							Descriptor.CPUAccessFlags = 0;
							Descriptor.MiscFlags = 0;

							/// ���۶� ������ : ����, ����Ʈ, ��Ƽ�󸮾�� ��

							D3D11_SUBRESOURCE_DATA SubResource = D3D11_SUBRESOURCE_DATA();

							// ���긮�ҽ�
							// ��Ʈ�� : ���� ���ϴ� ���� �����Ͱ��� �ؼ��� �� �ִ� ������ ��Ʈ(?)
							SubResource.pSysMem = FreeImage_GetBits(Bitmap);

							// �ѱ� �̹����� â�� ũ�⿡ �� �°� ����
							SubResource.SysMemPitch = FreeImage_GetPitch(Bitmap);
							// �ѱ� �̹����� ��� ���̸� ����
							SubResource.SysMemSlicePitch = 0;

							ID3D11Texture2D* Texture2D = nullptr;
							MUST(Device->CreateTexture2D(&Descriptor, &SubResource, &Texture2D));
							{
								ID3D11ShaderResourceView* ShaderResourceView = nullptr;
								MUST(Device->CreateShaderResourceView(Texture2D, nullptr, &ShaderResourceView));
								{
									// ���� �� ������ ����������(PS)�� ���� / PS���� �޾ư� ����Ÿ������ ��� 
									DeviceContext->PSSetShaderResources(0, 1, &ShaderResourceView);
									// ���° ���Կ� �ѱ���, ��� �ѱ���, ������ �ѱ���
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
			// (PeekMessage���� �޼����� �ް� ���� ���� ��)����ڰ� �����ϴ� �޼���
			case WM_APP: 
			{
#pragma region CPU Access
				{

#pragma region �ȼ�ȭ�� �̹��� �׸���
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

				// ClearRenderTargetView : �۾��� ����(��ũ�� ��ü)�� �ʱ�ȭ
				DeviceContext->ClearRenderTargetView(RenderTargetView, Color);

				// �����ϴ� ���ۿ��� ����ϴ� ���۷� �ѱ�� �۾�
				MSUT(SwapChain->Present(1,0));

				element += delta;

				std::cout << element << std::endl;

				if(element < 0.0f || 1.0f <= element)
					delta *= -1;

				������ : ������ ȭ�鿡�� ���ݾ� ������� ��ȯ�Ǵ� â ����
*/
#pragma endregion
					// D3D11_MAPPED_SUBRESOURCE Subresource = D3D11_MAPPED_SUBRESOURCE();
					// MUST(DeviceContext->Map(Buffer::Vertex, 0, D3D11_MAP_WRITE_DISCARD, 0, &Subresource)); // discard : ������ ���� �������� �ʰ� ����
					// �迭�� �Ѱ��ְ� ���� �� �迭�� ���° ���� �Ѱ��ٰ����� Map�� ���� ����
					{
						static struct
						{
							float const Width = 156;
							float const Height = 160;
						}
						Frame;

						// static�� ���� ���� �ۿ����� ���� ����
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
					// �۾��� ���� ����(��ũ�� ��ü)�� �ʱ�ȭ (�� ������)

					DeviceContext->Draw(4, 0);
					// Draw : Vertex�� ���� ������ �޴� �޼���
					// Draw�� DrawIndex�� ����
					// DRAW :�� �� ����� ������ �ٽ� ��� �Ұ�
					// DRAWINDEX: ������ ������ �Ű� ������ ���� ����

					MUST(SwapChain->Present(0, 0));
					// flag : �̹��� �Ѱ��� �� �߰� �ɼ��� ��� ����
				}
#pragma endregion
				return 0;
			}
			// â�� ������ �� �߻��ϴ� �޼���
			// WM_QUIT : ���� �޼���(��� ���α׷�) / WM_DESTROY : �Ҹ� �޼���(Only â)
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
				// WM_QUIT�� ȣ���ϴ� �Լ� : ���α׷� ����
				PostQuitMessage(0);
				return 0;
			}
			// â�� ����� ����� �� �߻��ϴ� �޼���
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
							1,								// ������ ����
							LOWORD(lParameter),				// ������ â�� WIDTH
							HIWORD(lParameter),				// ������ â�� HEIGHT
							DXGI_FORMAT_B8G8R8A8_UNORM,		// 1�� ����ȭ
							0
						));
					}
#pragma endregion

#pragma region Create RenderTargetView
					{
						ID3D11Texture2D* texture2D = nullptr;
						MUST(SwapChain->GetBuffer(0,IID_PPV_ARGS(&texture2D))); 
						// HRESULT�� define���� ��� �����ϴ� ���
						// ���ۿ��� ������ �̹����� �����ͼ� �ؽ��Ŀ� ����

						HRESULT hr = Device->CreateRenderTargetView(texture2D, nullptr, &RenderTargetView); // GPU���� �׸��� ������ CPU���� GPU�� �ؽ�ó�� ��ȯ
						assert(SUCCEEDED(hr));
						texture2D->Release();

						DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr); // �۾��� ���� = 1
					}
#pragma endregion
				}

				return 0;
			}
			// �޼����� �����µ� case�� �������� ���� �޼���
			default:
			{
				// ó���� �ʿ��ϱ� ������ �� �޼����� default ó���ϴ� �Լ�
				return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
			}
		}
		return 0;
	}
}
#pragma region �Ӽ� ���� ***

// �Ӽ� -> ��Ŀ -> �ý��� -> ���� �ý��� -> 'â'���� ����

// �Ӽ� -> ���� �Ӽ� -> ��� -> ���� ���� -> '��Ƽ����Ʈ ���� ���� ���'���� ����

// �Ӽ� -> ��Ŀ -> ��� �Ӽ� -> �߰� ���Ӽ� -> ���� -> d3d11.lib �߰�    

#pragma endregion
