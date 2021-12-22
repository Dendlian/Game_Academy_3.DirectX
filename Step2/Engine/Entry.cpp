#include <Windows.h>
// Entry.cpp : 창에 대한 설정
#pragma region Win procedure
/*
 // 메세지를 받는 방식
	- GetMessage   : 대기 타임 O (메세지가 없으면 false를 반환)
	    (Update)   : Timer 함수를 통해 지속적으로 함수를 호출하기 때문에 속도면에서 낮은 효율성

	- PeekMessage  : 대기 타임 X (메세지가 없으면 조건문을 통해 update)
				   : 호출 속도면에서 높은 효율성
*/
#pragma endregion 

// WIN API 사용 (Windows.h)
namespace Engine
{
	LRESULT CALLBACK Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
}

int APIENTRY WinMain
(
	_In_ HINSTANCE const hInstance,
	_In_opt_ HINSTANCE const hPrevInstance,
	_In_ LPSTR const lpCmdLine,
	_In_ int const nShowCmd
)
{
	// hWindow   : 창이 가지는 정보를 담고 있는 Instance
	// handle(h) : 프로세서 / 프로그램이 실행되는 메모리 주소를 가리키는 요소
	HWND hWindow = nullptr;

	{
		// wndClass : 만들 창의 설계도
		WNDCLASSEX wndClass = WNDCLASSEX();
		
		wndClass.cbSize        = sizeof(WNDCLASSEX);

		// 메세지 처리 기반 함수 설정 ***
		wndClass.lpfnWndProc   = Engine::Procedure;

		// Window를 만들기 위한 프로그램의 주소를 받기 위한 설정
		wndClass.hInstance     = hInstance;
		wndClass.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
		wndClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		
		// winClass의 이름 설정
		wndClass.lpszClassName = "Window";

		// 클래스 생성
		wndClass.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);

		RegisterClassEx(&wndClass);
	}

	{
		// 화면에 띄울 창(객체)을 생성 ***
		CREATESTRUCT window = CREATESTRUCT();
		
		window.lpszClass = "Window";
		window.lpszName = "Game";
		window.style = WS_CAPTION | WS_SYSMENU;
		window.cx = 1280;
		window.cy = 720;
		window.hInstance = hInstance;

		// 창이 화면의 중앙에 오도록 위치 조정
		{
			RECT rect = RECT();

			rect.right = window.cx;
			rect.bottom = window.cy;

			AdjustWindowRectEx(&rect, window.style, static_cast<bool>(window.hMenu), window.dwExStyle);

			window.cx = rect.right - rect.left;
			window.cy = rect.bottom - rect.top;

			window.x = (GetSystemMetrics(SM_CXSCREEN) - window.cx) / 2;
			window.y = (GetSystemMetrics(SM_CYSCREEN) - window.cy) / 2;
		}

		// 창 최종 생성
		hWindow = CreateWindowEx
		(
			window.dwExStyle, 
			window.lpszClass,
			window.lpszName, 
			window.style,    
			window.x,        
			window.y,        
			window.cx,
			window.cy,
			window.hwndParent,
			window.hMenu,
			window.hInstance,
			window.lpCreateParams
		);

		ShowWindow(hWindow, SW_RESTORE);
	}
	
	{
		// msg : 메세지를 담기 위한 구조체
		MSG msg = MSG();

		while (true)
		{
			// PeekMessage : 메세지가 들어오지 않을 시 false를 반환
			// Getmessage  : WM_QUIT 메세지가 들어왔을 때 false를 반환
			if (PeekMessage(&msg, HWND(), WM_NULL, WM_NULL, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					return static_cast<int>(msg.wParam);
			
				// 메세지 처리 함수로 msg를 전송하는 함수
				DispatchMessage(&msg);
			}
			else
			{
				// 메세지가 들어오지 않을 때 WM_APP(사용자가 정의한 메세지를 처리하는 장소)로 이동
				SendMessage(hWindow, WM_APP, 0, 0);
			}
		}
	}
}
