#include <Windows.h>
#include "Game.h"

// namespace : ���� ������Ʈ ������ �����Ӱ� ���� ����
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
#pragma region Ű �Է� Ű���� ����
/*
WM_MOUSEWHEEL       : ���콺 �� ȸ��
WM_MOUSEHWHEEL      : ���콺 �� ȸ��(2)

WM_MOUSEMOVE        : ���콺 �̵�

WM_KEYUP            : Ű���� Ű �Է�
WM_KEYDOWN          : Ű���� Ű ����

WM_SYSKEYUP         : alt Ű �Է�
WM_SYSKEYDOWN       : alt Ű ����

WM_LBUTTONUP        : ��Ŭ�� �Է�
WM_LBUTTONDOWN      : ��Ŭ�� ����
WM_RBUTTONUP        : ��Ŭ�� �Է�
WM_RBUTTONDOWN      : ��Ŭ�� ����

WM_MBUTTONUP        : �ٹ�ư �Է�
WM_MBUTTONDOWN      : �ٹ�ư ����

WM_XBUTTONUP        : (���콺) �����ư �Է�
WM_XBUTTONDOWN      : (���콺) �����ư ����
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
