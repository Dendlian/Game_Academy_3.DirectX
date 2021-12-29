#include <Windows.h>
#include "Game.h"

// namespace : 같은 프로젝트 내에서 자유롭게 접근 가능
namespace Rendering
{
    void Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
}

namespace Input
{
    void Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
}

namespace Time
{
    void Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
}
namespace Engine
{

    namespace { Game* Portfolio; }

    LRESULT Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
    {
        switch (uMessage)
        {
            case WM_CREATE:
            {
                Rendering::Procedure(hWindow, uMessage, wParameter, lParameter);
                
                (Portfolio->Initialize())->Start();
                
                return 0;
            }
            case WM_APP:
            {
                if (Portfolio->Update())
                    CloseWindow(hWindow);

                Input::Procedure(hWindow, uMessage, wParameter, lParameter);
                Rendering::Procedure(hWindow, uMessage, wParameter, lParameter);
                Time::Procedure(hWindow, uMessage, wParameter, lParameter);
                return 0;
            }
            case WM_DESTROY:
            {
                Portfolio->End();
                delete Portfolio;

                Rendering::Procedure(hWindow, uMessage, wParameter, lParameter);
                PostQuitMessage(0);
                return 0;
            } 
            case WM_MOUSEWHEEL  : case WM_MOUSEHWHEEL : case WM_MOUSEMOVE :    
            case WM_SYSKEYDOWN  : case WM_LBUTTONDOWN : case WM_LBUTTONUP :     
            case WM_SYSKEYUP    : case WM_RBUTTONDOWN : case WM_RBUTTONUP :    
            case WM_KEYDOWN     : case WM_MBUTTONDOWN : case WM_MBUTTONUP :     
            case WM_KEYUP       : case WM_XBUTTONDOWN : case WM_XBUTTONUP :     
#pragma region 키 입력 키워드 정리
/*
WM_MOUSEWHEEL       : 마우스 휠 회전
WM_MOUSEHWHEEL      : 마우스 휠 회전(2)

WM_MOUSEMOVE        : 마우스 이동

WM_KEYUP            : 키보드 키 입력
WM_KEYDOWN          : 키보드 키 제거

WM_SYSKEYUP         : alt 키 입력
WM_SYSKEYDOWN       : alt 키 제거

WM_LBUTTONUP        : 좌클릭 입력
WM_LBUTTONDOWN      : 좌클릭 제거
WM_RBUTTONUP        : 우클릭 입력
WM_RBUTTONDOWN      : 우클릭 제거

WM_MBUTTONUP        : 휠버튼 입력
WM_MBUTTONDOWN      : 휠버튼 제거

WM_XBUTTONUP        : (마우스) 단축버튼 입력
WM_XBUTTONDOWN      : (마우스) 단축버튼 제거
*/
#pragma endregion 
            
            {
                Input::Procedure(hWindow, uMessage, wParameter, lParameter);
            }
            case WM_SIZE:
            {
                Rendering::Procedure(hWindow, uMessage, wParameter, lParameter);
                return 0;
            }
            default:
            {
                return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
            }
            return 0;
        }
    }
}
