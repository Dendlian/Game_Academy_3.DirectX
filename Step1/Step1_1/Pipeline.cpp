/*
==NOTE==
 // cominterface : ��ǻ�Ͱ� �ڵ������� ����� ������ (I)
   - release : ���� ��û��
   - description : ���� ��û��

 // Rendering PipeLine : �������� �ϱ� ���� �ܰ�
   - IA -> VS -> RS -> PS -> OM 
*/

#include <d3d11.h>
namespace Pipeline
{
	namespace
	{	
		ID3D11Device				* Device;				// �ڿ��� ����� ���
		ID3D11DeviceContext			* DeviceContext;		// ������ ������������ �ܰ�
		IDXGISwapChain				* SwapChain;			// ������ �۾��� �����ϸ� ����
		ID3D11RenderTargetView		* RenderTargetView;		// �ڿ��� �׸��� ���
	}

	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
			case WM_CREATE:
			{
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