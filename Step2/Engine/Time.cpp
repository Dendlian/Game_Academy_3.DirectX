#include "Time.h"
#include <Windows.h>
#include <chrono>

namespace Time
{
    namespace
    {
        // steady_clock : �ý��ۿ� �ִ� �������� �ð� ü��
        // system_clock : �����쿡 �ִ� �������� �ð� ü��
        // time_point : �ð��� ǥ���ϴ� ��ü
        std::chrono::steady_clock::time_point const Started = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point       Updated = std::chrono::steady_clock::now();

        // ��� �ð�
        float Elapsed = float();
        
        // �����Ӹ��� ȣ��Ǵ� �ð�
        float Delta   = float();

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
