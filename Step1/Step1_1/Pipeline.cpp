/*  
==NOTE==
 // cominterface : ��ǻ�Ͱ� �ڵ������� ����� ������
   - release : ���� ��û��
   - description : ���� ��û��

   // CPU�� ���ؼ� �����ϴ� �ڵ� cpp
   // GPU�� �����ϴ� �ڵ� hlsl
   
 // Rendering PipeLine : �������� �ϱ� ���� �ܰ�
   - IA -> VS -> RS -> PS -> OM (DeviceContext���� ����)
   - IA -> VS -> HS -> TS -> DS -> GS -> SO -> RS -> PS -> OM
   
 // IA (Input Assembler Stage)
   - ������ ���� ������ �Է��ϴ� �ܰ�
   - Layout�� ����

 // VS (Vertex Shader Stage)
   - Shader : GPU ������ �� �� �ִ� �Լ��� ����ü
   - IA���� ���� �����͵��� ���Ͽ� ���̴� ������ ó��

 // RS (Rasterizer Stage) <- GPU 
   - ���� ������ ȭ�鿡 ����ϱ� ���� ���ؽ��� ��ȯ�Ͽ� ������(������ȭ �̹���) ����
   - ������ �������� �������� �ȼ��� ��ȯ�ϴ� �ܰ�(�����Ͷ�����)

 // PS (Pixel Shader) <- GPU
   - ������ ��� �ȼ����� ���� ����ϴ� �ܰ�
 
 // OM (Output Merge) 
   - RenderTargets ����
   - ���������� ����ϱ� ���� �������ϴ� �ܰ�

 Local Space    
 World Space    
 View Space	     
 Viewport Space 
 
 Projection : 3D�� 2D ȭ�鿡 �����ϱ� ���� �۾�
 Back Space Culling : �� ȭ�鿡 ���� ������ �ʴ� �� ȭ�鿡 ���� ������ �������� �ʰ� ����
 Cleeping : View Soace ���� ȭ�鿡 ���� ������ �������� �ʰ� ����
 ViewPort : 3D�� View Space�� ������ ��(2D)�� �ű�� ���� ��ȯ�ϴ� �۾�
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
// GPU���� �ٷ� �� �ִ� ����
 // Texture
 // Buffer : �׸��� �׸� �� �⺻������ ��, ��, �ﰢ������ ���
  // ������ �׸� �� ������ ��ǥ���� �ð�������� ����
   
    - vertex : �� �� ����� ������ �ٽ� ��� �Ұ�
    - index : ������ ������ �Ű� ������ ���� ����
    - constant
*/
#pragma endregion

namespace Pipeline
{
	namespace
	{	
		ID3D11Device				* Device;				// �ڿ��� ����� ���
		ID3D11DeviceContext			* DeviceContext;		// ������ ������������ �ܰ�
		IDXGISwapChain				* SwapChain;			// ������ �۾��� �����ϸ� ����
															// �� �� ����ü�� ��ũ������ ����
		ID3D11RenderTargetView		* RenderTargetView;		// �ڿ��� �׸��� ���
		
		namespace Buffer
		{
			ID3D11Buffer* Vertex;	// ������ �Է��� ����
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
					Descriptor.SampleDesc.Count = 1;
					// �ȼ��� ����(��ܽ� �̹���)�� ���� : ��Ƽ�ٸ����
					Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
					// ������ Ÿ���� ����ϴ� �뵵�� ���
					Descriptor.BufferCount = 1;
					// ������ ��밳��
					// 0 : ���� ��� X
					// 1 : ���� 2�� ���
					Descriptor.OutputWindow = hWindow;
					Descriptor.Windowed = true;
					// â��� ��뿩��

					HRESULT hr = D3D11CreateDeviceAndSwapChain
					(
						nullptr,					// ��� �̻��
						D3D_DRIVER_TYPE_HARDWARE,	// ����̺� Ÿ������ �ϵ��� �����Ͽ� �ϵ������ ������ �޾� �������ϵ��� ����
						nullptr,					// �߰����� ����Ʈ���� Ÿ�� ��� �̻��
						0,							// �÷��� �̻��
						nullptr,					// Direct X�� ���� ȣȯ���� �⺻���� ����
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

				{
					{
						D3D11_INPUT_ELEMENT_DESC Descriptor[] =
						{
							D3D11_INPUT_ELEMENT_DESC(),
							D3D11_INPUT_ELEMENT_DESC()
						};
						Descriptor[0].SemanticName			= "POSITION";
						Descriptor[0].SemanticIndex			= 0;
						Descriptor[0].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
						Descriptor[0].InputSlot				= 0; // IA�ܰ迡�� �ĺ��ϴ� �������� �����ϴ� �κ�
						Descriptor[0].AlignedByteOffset 	= 0; // POSITION�� ���� ���� ���� ����
						Descriptor[0].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA; // ���� �������� ���� ����
						Descriptor[0].InstanceDataStepRate	= 0; // ���ۿ��� �׸� �ν��Ͻ��� ���� ���� (���� �����Ͷ�� ������ 0)
					
						Descriptor[1].SemanticName			= "COLOR";
						Descriptor[1].SemanticIndex			= 0;
						Descriptor[1].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
						Descriptor[1].InputSlot				= 0; 
						Descriptor[1].AlignedByteOffset		= 16; 
						Descriptor[1].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA; 
						Descriptor[1].InstanceDataStepRate	= 0; 
					}
				}


				// Vertex Buffer
				{ // CPU���� �۾���..
					struct Vertex
					{
						float position[4];
						float Color[4];
					};
				
					Vertex const Vertices[4]
					{
						{{ -0.5f, +0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
						{{ +0.5f, +0.5f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
						{{ -0.5f, -0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
						{{ +0.5f, -0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
					};
			
					D3D11_BUFFER_DESC Descriptor = D3D11_BUFFER_DESC();		// CPU���� ����ϴ� �ڵ带 GPU���� ��� �����ϵ��� ��ȯ (������ ���)
					Descriptor.ByteWidth = sizeof(Vertices);				// ������ ��ü ũ�⸦ ����
					Descriptor.Usage = D3D11_USAGE_IMMUTABLE;				// GPU���� ���� �� �ֵ��� ��� ����
					Descriptor.BindFlags = 0;								// CPU������ ���� ������ ������ ���� (���̳��Ϳ��� CPU�� ���� ����� �� �ϱ� ������ 0���� ����)
					Descriptor.CPUAccessFlags = 0;
					Descriptor.MiscFlags = 0;
					Descriptor.StructureByteStride = 0;

					D3D11_SUBRESOURCE_DATA SubResource = D3D11_SUBRESOURCE_DATA();
					SubResource.pSysMem = Vertices;
					SubResource.SysMemPitch = 0;
					SubResource.SysMemSlicePitch = 0;

					MUST(Device->CreateBuffer(&Descriptor, &SubResource, &Buffer::Vertex));

					UINT const Stride = sizeof(Vertex);
					UINT const Offset = 0;

					DeviceContext->IASetVertexBuffers(0, 1, &Buffer::Vertex, &Stride, &Offset);
				}

				// PrimitiveTopology
				{
					// ������ �����͸� �ﰢ�� ������� �ؼ��ϵ��� ����
					DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				}

				// Index Buffer
				{
					typedef USHORT Index[3];
					Index const Indices[2]
					{
						{0, 1, 2},
						{3, 2, 1}
					};

					D3D11_BUFFER_DESC Descriptor = D3D11_BUFFER_DESC();

					Descriptor.ByteWidth			= sizeof(Indices);
					Descriptor.Usage				= D3D11_USAGE_IMMUTABLE;
					Descriptor.BindFlags			= D3D11_BIND_INDEX_BUFFER;
					Descriptor.CPUAccessFlags		= 0;
					Descriptor.MiscFlags			= 0;
					Descriptor.StructureByteStride	= 0;
				
					D3D11_SUBRESOURCE_DATA SubResource = D3D11_SUBRESOURCE_DATA();
					SubResource.pSysMem = Indices;
					SubResource.SysMemPitch = 0;
					SubResource.SysMemSlicePitch = 0;

					MUST(Device->CreateBuffer(&Descriptor, &SubResource, &Buffer::Index));

					DeviceContext->IASetIndexBuffer(Buffer::Index, DXGI_FORMAT_R16_UINT, 0);

				}

				return 0;
			}
			case WM_APP: 
			{
				static float element = 0.0000f;
				static float delta = 0.001f;
				float const Color[4] = { element, element, 1.0f, 1.0f };
				DeviceContext->ClearRenderTargetView(RenderTargetView, Color); // �۾��� ���� ����(��ũ�� ��ü)�� �ʱ�ȭ (�� ������)
				MUST(SwapChain->Present(1, 0));

				element += delta;

				cout << element << endl;

				if (element < 0.0f || 1.0f <= element)
					delta *= -1;

				return 0;
			}
			case WM_DESTROY:
			{
				RenderTargetView->Release();
				Buffer::Vertex->Release();
				Buffer::Index->Release();
				SwapChain->Release();
				Device->Release();
				DeviceContext->Release();
				PostQuitMessage(0);
				return 0;
			}
			case WM_SIZE:
			{
				{
					if (RenderTargetView)
					{
						RenderTargetView->Release();

						SwapChain->ResizeBuffers
						(
							1,								// ������ ����
							LOWORD(lParameter),				// ������ â�� WIDTH
							HIWORD(lParameter),				// ������ â�� HEIGHT
							DXGI_FORMAT_B8G8R8A8_UNORM,
							0
						);
					}
					{
						ID3D11Texture2D* texture2D = nullptr;
						MUST(SwapChain->GetBuffer(0,IID_PPV_ARGS(&texture2D))); // HRESULT�� define���� ��� �����ϴ� ���

						HRESULT hr = Device->CreateRenderTargetView(texture2D, nullptr, &RenderTargetView); // GPU���� �׸��� ������ CPU���� GPU�� �ؽ�ó�� ��ȯ
						assert(SUCCEEDED(hr));
						texture2D->Release();

						DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr); // �۾��� ���� = 1
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

// �Ӽ� -> ��Ŀ -> �ý��� -> ���� �ý��� -> 'â'���� ����
// �Ӽ� -> ��Ŀ -> ��� �Ӽ� -> �߰� ���Ӽ� -> ���� -> d3d11.lib �߰�    
// �Ӽ� -> ���� �Ӽ� -> ��� -> ���� ���� -> '��Ƽ����Ʈ ���� ���� ���'���� ����