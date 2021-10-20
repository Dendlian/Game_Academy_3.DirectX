/*
==NOTE==
 // cominterface : ��ǻ�Ͱ� �ڵ������� ����� ������ (I)
   - release : ���� ��û��
   - description : ���� ��û��

 // Rendering PipeLine : �������� �ϱ� ���� �ܰ�
   - IA -> VS -> RS -> PS -> OM 
*/

#include <d3d11.h>
#include <cassert>

namespace Pipeline
{
	namespace
	{	
		ID3D11Device				* Device;				// �ڿ��� ����� ���
		ID3D11DeviceContext			* DeviceContext;		// ������ ������������ �ܰ�
		IDXGISwapChain				* SwapChain;			// ������ �۾��� �����ϸ� ����
															// �� �� ����ü�� ��ũ������ ����
		ID3D11RenderTargetView		* RenderTargetView;		// �ڿ��� �׸��� ���
	}
	


	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
			case WM_CREATE:
			{
				DXGI_SWAP_CHAIN_DESC Descriptor = DXGI_SWAP_CHAIN_DESC( );
				Descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
				

				Descriptor.SampleDesc.Count = 1;
				// �ȼ��� ����(��ܽ� �̹���)�� ���� : ��Ƽ�ٸ����
				
				Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				// ������ Ÿ���� ����ϴ� �뵵�� ���

				Descriptor.BufferCount = 1;
				// ������ ��밳��
				// 0 : ���� ��� X
				// 1 : ���� 2�� ���

				return 0;
			}
			case WM_APP:
			{
				return 0;
			}
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
			case WM_SIZE:
			{
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