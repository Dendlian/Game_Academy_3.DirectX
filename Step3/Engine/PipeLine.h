#pragma once

namespace Rendering::Pipeline
{
    namespace String
    {
        // ��Ʈ�� ��θ� ��� �Լ� (HFONT : ��Ʈ�� Handle ��)
        // LPCSTR : 32��Ʈ ���ڿ� ����� ������ string
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
        // Formor : ũ��, ��ġ, ȸ��
        // Latter : ī�޶� ����
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