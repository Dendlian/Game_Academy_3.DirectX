#include <Windows.h>
#include "Input.h"

// 0x8000 : 입력하지 않은 상태에서 호출시점에 입력하고 있는 경우

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
                // 이전에 눌리지 않았다면 수행
                if (not Pressed(code))
                {
                    // State.Pressed[4] = State.Pressed[4] | 0x4000
                    // State.Pressed[4] = 0000 0000 0000 0000 | 0100 0000 0000 0000 = 0100 0000 0000 0000
                    // | 대신 ^를 써도 결과값이 같기 때문에 가독성을 위해 변환
                    State.Pressed[code >> 0x4] = State.Pressed[code >> 0x4] | (0x8000 >> (code & 0xf));
                    State.Changed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
                    // (^)xor : 다른 값이면 0, 같은 값이면 1을 반환
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
                // 저장한 값의 모든 메모리를 0으로 변환
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
                // LOWORD, HIWORD : 창 안에서 마우스가 작업하고 있는 좌표
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

#pragma region Direct X 공부
/*
 1. Pipeline 단계별 진행방식
 2. 교차 판정
 3. Matrix

 (1) 충돌처리
 (2) 프로젝션, 메트릭스
 (3) 게임 프레임워크
*/
#pragma endregion

