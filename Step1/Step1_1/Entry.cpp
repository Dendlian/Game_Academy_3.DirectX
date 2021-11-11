#pragma region CPU / GPU
/*
==Note==
 // CPU : 입출력 장치, 기억 장치, 연산장치 컴퓨터의 리소스를 처리해주는 중앙 처리 장치
   - 직렬 연산 (순차적으로 연산) : 복잡한 연산을 처리할 때 GOOD
 
 // GPU : 그래픽 처리 장치
   - 병렬 연산 (동시에 연산) : 대량의 연산을 빠르게 처리할 때 GOOD
   - 렌더링 픽셀 하나하나에 대한 연산이 필요하므로 대량의 연산을 요구
 
 // DirectX : GPU 기능을 사용할 수 있는 API

 //
 cdcel
 __stdcall (WINAPI)
 __fastcall
 __thiscall
*/

#pragma endregion
#include <Windows.h>
namespace Pipeline
{
	LRESULT CALLBACK Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
}

int APIENTRY WinMain
(
	_In_		HINSTANCE const hInstance,
	_In_opt_	HINSTANCE const hPrevInstance,
	_In_		LPSTR	  const lpCmdLine,
	_In_		int		  const nShowCmd
)
{
	HWND hWindow = nullptr; 


	{
		WNDCLASSEX Class = WNDCLASSEX();

		Class.cbSize = sizeof(WNDCLASSEX);
		Class.lpfnWndProc = Pipeline::Procedure;
		Class.hInstance = hInstance;
		Class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		Class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		Class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		Class.lpszClassName = "Window";
		Class.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

		RegisterClassEx(&Class);
	}

	{
		CREATESTRUCT window = CREATESTRUCT();

		window.lpszClass = "Window";
		window.lpszName = "Game";
		window.style = WS_CAPTION | WS_SYSMENU;
		window.cx = 500;
		window.cy = 500;
		window.hInstance = hInstance;
		
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
		MSG msg = MSG();

		while (true) 
		{
			if (PeekMessage(&msg, HWND(), WM_NULL, WM_NULL, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					return static_cast<int>(msg.wParam);
			
				DispatchMessage(&msg);
			}
			else
			{
				SendMessage(hWindow, WM_APP, 0, 0);
			}
		}
	}
}