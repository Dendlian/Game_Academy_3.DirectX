#pragma once

namespace Rendering::Pipeline
{
    namespace String
    {
        // 폰트의 경로를 얻는 함수 (HFONT : 폰트의 Handle 값)
        // LPCSTR : 32비트 문자열 상수로 들어오는 string
        void Render(HFONT const hFont, LPCSTR const string, COLORREF const color, SIZE const size, POINT const center);
    }

    namespace Texture
    {
        void Create(struct Handle *& handle, SIZE const size, BYTE const *const data);
        void Render(struct Handle const * const & handle, RECT const area);
        void Delete(struct Handle const * const & handle);
    }

    namespace Transform
    {
        // Formor : 크기, 위치, 회전
        // Latter : 카메라 투영
        enum class Type
        {
            Former,
            Latter
        };
    
        using Matrix = float[4][4];
        
        template<Type type>
        void Update(Matrix const& matrix);
    }
}