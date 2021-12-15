#include <Windows.h>
namespace Pipeline
{
	// WPARAM : 키보드의 특정 키가 눌리거나 마우스의 어느 키가 눌렸는지 확인하는 인자
	// LPARAM : 사용량이 많고 필요로 하는 값인 X, Y의 좌표값을 저장하는 인자
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

		// 밑 창에 뜨는 아이콘
		Class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		Class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		Class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		Class.lpszClassName = "Window";
		
		// 타이틀 바에 뜨는 아이콘
		Class.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

		RegisterClassEx(&Class);
	}

	{
		// Window창을 기본값으로 설정한 변수 선언
		CREATESTRUCT window = CREATESTRUCT();

		// Window창을 만들기 위한 Class 생성
		window.lpszClass = "Window";
		window.lpszName = "Game";
		window.style = WS_CAPTION | WS_SYSMENU;
		window.cx = 500;
		window.cy = 500;
		window.hInstance = hInstance;
		
#pragma region Rect를 통해 직사각형을 만드는 2가지 방법 ***
/*
1. LeftTop, RightBottom 두 개의 좌표를 설정하여 그리기		
2. LeftTop 한 개의 좌표와 가로, 세로 길이를 설정하여 그리기
(현재 사용할 방식은 1번)
*/
#pragma endregion 

		// Window창을 기본값으로 설정했기 때문에 그릴 좌표를 지정
		{
			RECT rect = RECT();
			rect.right = window.cx;
			rect.bottom = window.cy;

			// Rect를 통해 창을 만들 좌표를 지정하고 그 값을 window에 적용
			// CREATESTRUCT의 객체 중 hMenu : 추가적인 메뉴로서 기본값 = '사용하지 않음'
			AdjustWindowRectEx(&rect, window.style, static_cast<bool>(window.hMenu), window.dwExStyle);

			// cx = 500, cy = 500에서 다시 설정하는 이유 : 타이틀 바를 포함한 크기가 (500, 500)이기 때문에 재 설정
			window.cx = rect.right - rect.left;
			window.cy = rect.bottom - rect.top;

			// Window창을 모니터의 정 중앙에 배치
			window.x = (GetSystemMetrics(SM_CXSCREEN) - window.cx) / 2;
			window.y = (GetSystemMetrics(SM_CYSCREEN) - window.cy) / 2;
		}

		// 실질적으로 Window창을 만드는 함수
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

		// 만든 창을 출력
		// SW_RESTORE : 화면을 최대(소)화 했다가 다시 되돌리면 창이 원래의 크기를 유지
		ShowWindow(hWindow, SW_RESTORE); 
	}

	// Pipeline : Window Procedure를 통해 메세지 처리를 받는 함수를 생성
	// MSG : 메세지를 어떻게 받고 처리할지 설정
	{
		// MSG 초기화
		MSG msg = MSG();

		// true : GetMessage 미사용
		while (true) 
		{
			// PeekMessage : 대기시간이 없으며, 메세지가 있다면 True, 없다면 False를 반환
			if (PeekMessage(&msg, HWND(), WM_NULL, WM_NULL, PM_REMOVE))
			{
				// 메세지가 있을 때 그 메세지가 WM_QUIT이라면 return 0, 즉 프로그램 종료
				// msg.wParam : 추가 메세지에 대한 정보 (long type)
				if (msg.message == WM_QUIT)
					return static_cast<int>(msg.wParam);
				
				// 받은 msg를 Window Procedure로 전송
				// 즉 Window Procedure는 DispatchMessage가 콜이 될 때 메세지를 받고 처리
				DispatchMessage(&msg);
			}
			else
			{
				// 메세지가 없을 때 Pipeline의 WM_APP로 전송 / WM_APP : 사용자 정의 메세지
				SendMessage(hWindow, WM_APP, 0, 0);
			}
		}
	}
}