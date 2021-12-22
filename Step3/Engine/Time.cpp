#include "Time.h"
#include <Windows.h>
#include <chrono>

namespace Time
{
    namespace
    {
        // steady_clock : 시스템에 있는 독립적인 시계 체제
        // system_clock : 윈도우에 있는 종속적인 시계 체제
        // time_point : 시각을 표현하는 객체
        std::chrono::steady_clock::time_point const Started = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point       Updated = std::chrono::steady_clock::now();

        // 경과 시간
        float Elapsed = float();
        
        // 프레임마다 호출되는 시간
        float Delta   = float();
    }

    namespace Get
    {
        float Elapsed() { return Time::Elapsed; }
        float Delta()   { return Time::Delta; }
    }

    void CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
    {
        switch (uMessage)
        {
        case WM_APP:
            {
                using namespace std::chrono;

                Elapsed = duration_cast<duration<float>>(steady_clock::now() - Started).count();
                Delta = duration_cast<duration<float>>(steady_clock::now() - Updated).count();

                Updated = steady_clock::now();
                return;
            }
        }
    }
}
