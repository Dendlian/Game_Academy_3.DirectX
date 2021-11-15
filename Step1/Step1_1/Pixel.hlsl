#include "Layout.hlsli"

namespace Shader
{
    const Texture2D Resource : register(T0);
    // register : �ε�� ������ RS�� �Ѿ�� Ŭ���ε��� ���� (Bitmap ����)
    // RS���� �޾ƿ� Bitmap�� �ȼ�ȭ
    // T0 : tecture 0��°�� �ѱ�

    Layout::Color Pixel(const Layout::Pixel Input) : SV_Target // OM���� ����뵵�� �ѱ�ڴٴ� �ǹ̸� ����
    {
        Layout::Color Output =
        {
            Resource.Load(int3(Input.TexCoord.x, Input.TexCoord.y, 0))
        };

        if (Output.a == 0.0f)
            discard;
     
        return Output;
     }
}

