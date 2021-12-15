#include <Windows.h>
namespace Pipeline
{
	// WPARAM : Ű������ Ư�� Ű�� �����ų� ���콺�� ��� Ű�� ���ȴ��� Ȯ���ϴ� ����
	// LPARAM : ��뷮�� ���� �ʿ�� �ϴ� ���� X, Y�� ��ǥ���� �����ϴ� ����
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

		// �� â�� �ߴ� ������
		Class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		Class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		Class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		Class.lpszClassName = "Window";
		
		// Ÿ��Ʋ �ٿ� �ߴ� ������
		Class.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

		RegisterClassEx(&Class);
	}

	{
		// Windowâ�� �⺻������ ������ ���� ����
		CREATESTRUCT window = CREATESTRUCT();

		// Windowâ�� ����� ���� Class ����
		window.lpszClass = "Window";
		window.lpszName = "Game";
		window.style = WS_CAPTION | WS_SYSMENU;
		window.cx = 500;
		window.cy = 500;
		window.hInstance = hInstance;
		
#pragma region Rect�� ���� ���簢���� ����� 2���� ��� ***
/*
1. LeftTop, RightBottom �� ���� ��ǥ�� �����Ͽ� �׸���		
2. LeftTop �� ���� ��ǥ�� ����, ���� ���̸� �����Ͽ� �׸���
(���� ����� ����� 1��)
*/
#pragma endregion 

		// Windowâ�� �⺻������ �����߱� ������ �׸� ��ǥ�� ����
		{
			RECT rect = RECT();
			rect.right = window.cx;
			rect.bottom = window.cy;

			// Rect�� ���� â�� ���� ��ǥ�� �����ϰ� �� ���� window�� ����
			// CREATESTRUCT�� ��ü �� hMenu : �߰����� �޴��μ� �⺻�� = '������� ����'
			AdjustWindowRectEx(&rect, window.style, static_cast<bool>(window.hMenu), window.dwExStyle);

			// cx = 500, cy = 500���� �ٽ� �����ϴ� ���� : Ÿ��Ʋ �ٸ� ������ ũ�Ⱑ (500, 500)�̱� ������ �� ����
			window.cx = rect.right - rect.left;
			window.cy = rect.bottom - rect.top;

			// Windowâ�� ������� �� �߾ӿ� ��ġ
			window.x = (GetSystemMetrics(SM_CXSCREEN) - window.cx) / 2;
			window.y = (GetSystemMetrics(SM_CYSCREEN) - window.cy) / 2;
		}

		// ���������� Windowâ�� ����� �Լ�
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

		// ���� â�� ���
		// SW_RESTORE : ȭ���� �ִ�(��)ȭ �ߴٰ� �ٽ� �ǵ����� â�� ������ ũ�⸦ ����
		ShowWindow(hWindow, SW_RESTORE); 
	}

	// Pipeline : Window Procedure�� ���� �޼��� ó���� �޴� �Լ��� ����
	// MSG : �޼����� ��� �ް� ó������ ����
	{
		// MSG �ʱ�ȭ
		MSG msg = MSG();

		// true : GetMessage �̻��
		while (true) 
		{
			// PeekMessage : ���ð��� ������, �޼����� �ִٸ� True, ���ٸ� False�� ��ȯ
			if (PeekMessage(&msg, HWND(), WM_NULL, WM_NULL, PM_REMOVE))
			{
				// �޼����� ���� �� �� �޼����� WM_QUIT�̶�� return 0, �� ���α׷� ����
				// msg.wParam : �߰� �޼����� ���� ���� (long type)
				if (msg.message == WM_QUIT)
					return static_cast<int>(msg.wParam);
				
				// ���� msg�� Window Procedure�� ����
				// �� Window Procedure�� DispatchMessage�� ���� �� �� �޼����� �ް� ó��
				DispatchMessage(&msg);
			}
			else
			{
				// �޼����� ���� �� Pipeline�� WM_APP�� ���� / WM_APP : ����� ���� �޼���
				SendMessage(hWindow, WM_APP, 0, 0);
			}
		}
	}
}