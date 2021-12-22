#include <Windows.h>
#include "Input.h"

// 0x8000 : �Է����� ���� ���¿��� ȣ������� �Է��ϰ� �ִ� ���

// bool KeyManager::IsStayKeyDown(int key)
// {
//     if (GetAsyncKeyState(key) & 0x8000) return true;
//     return false;
// }

namespace Input
{
    namespace
    {
        class
        {
        public :
            void Down(WPARAM const code)
            {   
                // ������ ������ �ʾҴٸ� ����
                if (not Pressed(code))
                {
                    // State.Pressed[4] = State.Pressed[4] | 0x4000
                    // State.Pressed[4] = 0000 0000 0000 0000 | 0100 0000 0000 0000 = 0100 0000 0000 0000
                    // | ��� ^�� �ᵵ ������� ���� ������ �������� ���� ��ȯ
                    State.Pressed[code >> 0x4] = State.Pressed[code >> 0x4] | (0x8000 >> (code & 0xf));
                    State.Changed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
                    // (^)xor : �ٸ� ���̸� 0, ���� ���̸� 1�� ��ȯ
                }
            }

            void Up(WPARAM const code)
            {
                // State.Pressed[4] = State.Pressed[4] ^ 0x4000 (^ : nor)
                State.Pressed[code >> 0x4] = State.Pressed[code >> 0x4] ^ (0x8000 >> (code & 0xf));
                State.Changed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
            }

            bool Changed(WPARAM const code)
            {
                return State.Changed[code >> 0x4] & (0x8000 >> (code & 0xf));
            }

            bool Pressed(WPARAM const code)
            {
                /*
                A -> 65 -> 0100 0001
                code >> 0x4 = 0100 0001 >> 0100 = 0000 0100 -> 4
              
                0x8000 >> (code & 0xf) = 1000 0000 0000 0000 >> (0100 0001 & 0000 1111) 
                = 1000 0000 0000 0000 >> 0000 0001 = 1000 0000 0000 0000 >> 1 = 0100 0000 0000 0000

                State.Pressed[4] & 0100 0000 0000 0000
                = State.Pressed[4] & 0x4000
                */
                return State.Pressed[code >> 0x4] & (0x8000 >> (code & 0xf));
            }

            void Update()
            {
                // ������ ���� ��� �޸𸮸� 0���� ��ȯ
                // 0000 0000 0000 0000
                ZeroMemory(State.Changed, sizeof(State.Changed));
            }

        private:
            struct
            {
                USHORT Changed[16];
                USHORT Pressed[16];
            }State;
        }Key;

        POINT Cursor;
        POINT Wheel;
    }

    void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
    {
        switch (uMessage)
        {
            case WM_APP:
            {
                Key.Update();
             
                Wheel = POINT();
                
                return;
            }
            case WM_MOUSEWHEEL  : { Wheel.y += GET_WHEEL_DELTA_WPARAM(wParameter) / WHEEL_DELTA; return; }
            case WM_MOUSEHWHEEL : { Wheel.x += GET_WHEEL_DELTA_WPARAM(wParameter) / WHEEL_DELTA; return; }
            case WM_MOUSEMOVE   : 
            {
                // LOWORD, HIWORD : â �ȿ��� ���콺�� �۾��ϰ� �ִ� ��ǥ
                Cursor.x = static_cast<USHORT>(LOWORD(lParameter));
                Cursor.y = static_cast<USHORT>(HIWORD(lParameter));
                return;
            }
            case WM_SYSKEYDOWN  : { Key.Down(wParameter); return; }
            case WM_LBUTTONDOWN : { Key.Down(VK_LBUTTON); return; }
            case WM_RBUTTONDOWN : { Key.Down(VK_RBUTTON); return; }
            case WM_MBUTTONDOWN : { Key.Down(VK_MBUTTON); return; }
            case WM_KEYDOWN     : { Key.Down(wParameter); return; }
            case WM_XBUTTONDOWN : 
            {
                switch (GET_XBUTTON_WPARAM(wParameter))
                {
                case XBUTTON1:Key.Down(VK_XBUTTON1); return;
                case XBUTTON2:Key.Down(VK_XBUTTON2); return;
                }         
            }
            case WM_SYSKEYUP    : { Key.Up(wParameter); return; }
            case WM_LBUTTONUP   : { Key.Up(VK_LBUTTON); return; }
            case WM_RBUTTONUP   : { Key.Up(VK_RBUTTON); return; }
            case WM_MBUTTONUP   : { Key.Up(VK_MBUTTON); return; }
            case WM_KEYUP       : { Key.Up(wParameter); return; }
            case WM_XBUTTONUP   :
            {
                switch (GET_XBUTTON_WPARAM(wParameter))
                {
                case XBUTTON1:Key.Up(VK_XBUTTON1); return;
                case XBUTTON2:Key.Up(VK_XBUTTON2); return;
                }
            }
        }
    }
}

#pragma region Direct X ����
/*
 1. Pipeline �ܰ躰 ������
 2. ���� ����
 3. Matrix

 (1) �浹ó��
 (2) ��������, ��Ʈ����
 (3) ���� �����ӿ�ũ
*/
#pragma endregion

