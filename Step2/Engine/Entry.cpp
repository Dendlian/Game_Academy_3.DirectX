#include <Windows.h>
// Entry.cpp : â�� ���� ����
#pragma region Win procedure
/*
 // �޼����� �޴� ���
	- GetMessage   : ��� Ÿ�� O (�޼����� ������ false�� ��ȯ)
	    (Update)   : Timer �Լ��� ���� ���������� �Լ��� ȣ���ϱ� ������ �ӵ��鿡�� ���� ȿ����

	- PeekMessage  : ��� Ÿ�� X (�޼����� ������ ���ǹ��� ���� update)
				   : ȣ�� �ӵ��鿡�� ���� ȿ����
*/
#pragma endregion 

// WIN API ��� (Windows.h)
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
	// hWindow   : â�� ������ ������ ��� �ִ� Instance
	// handle(h) : ���μ��� / ���α׷��� ����Ǵ� �޸� �ּҸ� ����Ű�� ���
	HWND hWindow = nullptr;

	{
		// wndClass : ���� â�� ���赵
		WNDCLASSEX wndClass = WNDCLASSEX();
		
		wndClass.cbSize        = sizeof(WNDCLASSEX);

		// �޼��� ó�� ��� �Լ� ���� ***
		wndClass.lpfnWndProc   = Engine::Procedure;

		// Window�� ����� ���� ���α׷��� �ּҸ� �ޱ� ���� ����
		wndClass.hInstance     = hInstance;
		wndClass.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
		wndClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		
		// winClass�� �̸� ����
		wndClass.lpszClassName = "Window";

		// Ŭ���� ����
		wndClass.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);

		RegisterClassEx(&wndClass);
	}

	{
		// ȭ�鿡 ��� â(��ü)�� ���� ***
		CREATESTRUCT window = CREATESTRUCT();
		
		window.lpszClass = "Window";
		window.lpszName = "Game";
		window.style = WS_CAPTION | WS_SYSMENU;
		window.cx = 1280;
		window.cy = 720;
		window.hInstance = hInstance;

		// â�� ȭ���� �߾ӿ� ������ ��ġ ����
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

		// â ���� ����
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
		// msg : �޼����� ��� ���� ����ü
		MSG msg = MSG();

		while (true)
		{
			// PeekMessage : �޼����� ������ ���� �� false�� ��ȯ
			// Getmessage  : WM_QUIT �޼����� ������ �� false�� ��ȯ
			if (PeekMessage(&msg, HWND(), WM_NULL, WM_NULL, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					return static_cast<int>(msg.wParam);
			
				// �޼��� ó�� �Լ��� msg�� �����ϴ� �Լ�
				DispatchMessage(&msg);
			}
			else
			{
				// �޼����� ������ ���� �� WM_APP(����ڰ� ������ �޼����� ó���ϴ� ���)�� �̵�
				SendMessage(hWindow, WM_APP, 0, 0);
			}
		}
	}
}
