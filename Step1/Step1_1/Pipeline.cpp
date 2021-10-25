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