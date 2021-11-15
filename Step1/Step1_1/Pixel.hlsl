#include "Layout.hlsli"

namespace Shader
{
    const Texture2D Resource : register(T0);
    // register : 로드된 파일이 RS로 넘어가서 클리핑등을 적용 (Bitmap 형식)
    // RS에서 받아온 Bitmap을 픽셀화
    // T0 : tecture 0번째에 넘김

    Layout::Color Pixel(const Layout::Pixel Input) : SV_Target // OM으로 쓰기용도로 넘기겠다는 의미를 설정
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

