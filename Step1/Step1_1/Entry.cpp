#pragma region CPU / GPU
/*
==Note==
 // CPU : ����� ��ġ, ��� ��ġ, ������ġ ��ǻ���� ���ҽ��� ó�����ִ� �߾� ó�� ��ġ
   - ���� ���� (���������� ����) : ������ ������ ó���� �� GOOD
 
 // GPU : �׷��� ó�� ��ġ
   - ���� ���� (���ÿ� ����) : �뷮�� ������ ������ ó���� �� GOOD
   - ������ �ȼ� �ϳ��ϳ��� ���� ������ �ʿ��ϹǷ� �뷮�� ������ �䱸
 
 // DirectX : GPU ����� ����� �� �ִ� API
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