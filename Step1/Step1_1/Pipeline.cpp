/*
==NOTE==
 // cominterface : 컴퓨터가 자동적으로 만드는 데이터 (I)
   - release : 삭제 신청서
   - description : 생성 요청서

 // Rendering PipeLine : 랜더링을 하기 위한 단계
   - IA -> VS -> RS -> PS -> OM 
*/

#include <d3d11.h>
namespace Pipeline
{
	namespace
	{	
		ID3D11Device				* Device;				// 자원을 만드는 장소
		ID3D11DeviceContext			* DeviceContext;		// 랜더링 파이프라인의 단계
		IDXGISwapChain				* SwapChain;			// 나눠진 작업을 교대하며 실행
		ID3D11RenderTargetView		* RenderTargetView;		// 자원을 그리는 장소
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