/*  
==NOTE==
 // cominterface : 컴퓨터가 자동적으로 만드는 데이터 (I)
   - release : 삭제 신청서
   - description : 생성 요청서

   // CPU에 대해서 연산하는 코드 cpp
   // GPU를 동작하는 코드 hlsl
   

 // Rendering PipeLine : 랜더링을 하기 위한 단계
   - IA -> VS -> RS -> PS -> OM (DeviceContext에서 적용)
   - IA -> VS -> HS -> TS -> DS -> GS -> SO -> RS -> PS -> OM
   
 // IA (Input Assembler Stage)
   - 정점에 대한 정보를 입력하는 단계
   - Layout을 설정

 // VS (Vwetex Shader Stage)
   - Shader : GPU 연산을 할 수 있는 함수의 집합체
   - IA에게 받은 데이터들을 통하여 쉐이더 연산을 처리

 // RS (Rasterizer Stage) <- GPU 
   - 정점 정보를 화면에 출력하기 위해 레스터화 이미지(폴리곤)로 변환하는 단계
   - 정점을 바탕으로 픽셀로 변환하는 단계

 // PS (Pixel Shader) <- GPU
   - 랜더링 대상 픽셀들의 색을 계산하는 단계
 
 // OM (Output Merge) 
   - RenderTargets 설정
   - 최종적으로 출력하기 위해 랜더링하는 단계

 Local Space    
 World Space    
 View Space	     
 Viewport Space 
 
 Projection : 3D를 2D 화면에 투영하기 위한 작업
 Back Space Calling : 앞 화면에 가려 보이지 않는 뒤 화면에 대한 연산을 진행하지 않고 삭제
 Cleeping : View Soace 밖의 화면에 대한 연산을 진행하지 않고 삭제
 ViewPort : 3D인 View Space를 윈도우 상(2D)에 옮기기 위해 변환하는 작업
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

namespace Pipeline
{
	namespace
	{	
		ID3D11Device				* Device;				// 자원을 만드는 장소
		ID3D11DeviceContext			* DeviceContext;		// 랜더링 파이프라인의 단계
		IDXGISwapChain				* SwapChain;			// 나눠진 작업을 교대하며 실행
															// 위 두 구조체의 디스크립션을 포함
		ID3D11RenderTargetView		* RenderTargetView;		// 자원을 그리는 장소
	}
	
	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
			case WM_CREATE:
			{
				DXGI_SWAP_CHAIN_DESC Descriptor		= DXGI_SWAP_CHAIN_DESC( );
				Descriptor.BufferDesc.Format		= DXGI_FORMAT_B8G8R8A8_UNORM;
				Descriptor.SampleDesc.Count			= 1;
				// 픽셀의 단점(계단식 이미지)을 개선 : 안티앨리어싱
				Descriptor.BufferUsage				= DXGI_USAGE_RENDER_TARGET_OUTPUT;
				// 렌더링 타겟을 출력하는 용도로 사용
				Descriptor.BufferCount				= 1;
				// 버퍼의 사용개수
				// 0 : 버퍼 사용 X
				// 1 : 버퍼 2개 사용
				Descriptor.OutputWindow				= hWindow;
				Descriptor.Windowed					= true;
				// 창모드 사용여부

				HRESULT hr = D3D11CreateDeviceAndSwapChain
				(
					nullptr,					// 어뎁터 미사용
					D3D_DRIVER_TYPE_HARDWARE,	// 드라이브 타입으로 하드웨어를 설정하여 하드웨어의 도움을 받아 랜더링하도록 설정
					nullptr,					// 추가적인 소프트웨어 타입 기능 미사용
					0,							// 플래그 미사용
					nullptr,					// Direct X의 버전 호환성을 기본으로 설정
					0,							// 설정된 버전 호환성을 모두 사용
					D3D11_SDK_VERSION,			// 컴퓨터에게 어떤 실행 버전을 사용할지 통보
					&Descriptor,				// 설명서의 주소
					&SwapChain,					// 저장할 공간의 주소 (SWAP)
					&Device,					// 저장할 공간의 주소 (DEVICE)
					nullptr,					// Direct X의 버전 호환성을 기본으로 설정
					&DeviceContext				// 저장할 공간의 주소 (DEVICECONTEXT)
				);
				assert(SUCCEEDED(hr));			// 프로그램이 잘 만들어졌는지 확인

				return 0;
			}
			case WM_APP: 
			{
				static float element = 0.0000f;
				static float delta = 0.001f;
				float const Color[4] = { element, element, 1.0f, 1.0f };
				DeviceContext->ClearRenderTargetView(RenderTargetView, Color); // 작업할 렌더 영역(스크린 전체)의 초기화 (색 입히기)
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
							1,								// 버퍼의 개수
							LOWORD(lParameter),				// 윈도우 창의 WIDTH
							HIWORD(lParameter),				// 윈도우 창의 HEIGHT
							DXGI_FORMAT_B8G8R8A8_UNORM,
							0
						);
					}
					{
						ID3D11Texture2D* texture2D = nullptr;
						MUST(SwapChain->GetBuffer(0,IID_PPV_ARGS(&texture2D))); // HRESULT를 define으로 묶어서 설정하는 방법

						HRESULT hr = Device->CreateRenderTargetView(texture2D, nullptr, &RenderTargetView); // GPU에서 그리기 때문에 CPU에서 GPU로 텍스처를 반환
						assert(SUCCEEDED(hr));
						texture2D->Release();

						DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr); // 작업할 영역 = 1
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




// 속성 -> 링커 -> 시스템 -> 하위 시스템 -> '창'으로 변경
// 속성 -> 링커 -> 모든 속성 -> 추가 종속성 -> 편집 -> d3d11.lib 추가    
// 속성 -> 구성 속성 -> 고급 -> 문자 집합 -> '멀티바이트 문자 집합 사용'으로 변경