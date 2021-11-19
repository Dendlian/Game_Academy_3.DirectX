#include "Layout.hlsli"

namespace Shader
{
    // register : �ε�� ������ RS�� �Ѿ�� Ŭ���ε��� ���� (Bitmap ����)
    // T0 : tecture 0��°�� �ѱ�
    // RS���� �޾ƿ� Bitmap�� �ȼ�ȭ
    const Texture2D Resource : register(T0);

    // SV_Target : OM���� ����뵵�� �ѱ�ڴٴ� �ǹ̸� ����
    Layout::Color Pixel(const Layout::Pixel Input) : SV_Target
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

/* �Ӽ� ����
�Ӽ� -> HlSL �����Ϸ� -> ������ �̸� : main -> Shader::Pixel
                      -> ���̴� ���� : �ȼ� ���̴�(/ps)
                      -> ���̴� �� : Shader Model5.0(/5_0)

�Ӽ� -> ��� �Ӽ� -> ��� ���� �̸� : Bytecode
                  -> ��� ���� �̸� : $(ProjectDir)Shader/Bytecode/Pixel.h
*/
