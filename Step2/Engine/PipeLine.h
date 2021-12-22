#pragma once

namespace Rendering::Pipeline
{
    namespace Texture
    {
        void Create(struct Handle *& handle, SIZE const size, BYTE const *const data);
        void Render(struct Handle *& handle, RECT const area);
        void Delete(struct Handle *& handle);
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