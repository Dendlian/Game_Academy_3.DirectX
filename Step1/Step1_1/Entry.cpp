#pragma region CPU / GPU
/*
==Note==
 // CPU : ����� ��ġ, ��� ��ġ, ������ġ ��ǻ���� ���ҽ��� ó�����ִ� �߾� ó�� ��ġ
   - ���� ���� (���������� ����) : ������ ������ ó���� �� GOOD
 
 // GPU : �׷��� ó�� ��ġ
   - ���� ���� (���ÿ� ����) : �뷮�� ������ ������ ó���� �� GOOD
   - ������ �ȼ� �ϳ��ϳ��� ���� ������ �ʿ��ϹǷ� �뷮�� ������ �䱸
 
 // DirectX : GPU ����� ����� �� �ִ� API

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