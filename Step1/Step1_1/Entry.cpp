#pragma region CPU / GPU
/*
==Note==
 // CPU : 입출력 장치, 기억 장치, 연산장치 컴퓨터의 리소스를 처리해주는 중앙 처리 장치
   - 직렬 연산 (순차적으로 연산) : 복잡한 연산을 처리할 때 GOOD
 
 // GPU : 그래픽 처리 장치
   - 병렬 연산 (동시에 연산) : 대량의 연산을 빠르게 처리할 때 GOOD
   - 렌더링 픽셀 하나하나에 대한 연산이 필요하므로 대량의 연산을 요구
 
 // DirectX : GPU 기능을 사용할 수 있는 API
*/

/*
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
		CREATESTRUCT Window = CREATESTRUCT();

		Window.lpszClass = "Window";
		Window.lpszName = "Game";
		Window.style = WS_CAPTION | WS_SYSMENU;
		Window.cx = 500;
		Window.cy = 500;
		Window.hInstance = hInstance;
		
		hWindow CreateWindowEx
		(
			Window.dwExstyle,
			Window.lpszClass,
			Wincow.lpszName,
			Window.style,
			Window.x,
			Window.y,
			Window.cx,
			Window.cy
		);
	}
}