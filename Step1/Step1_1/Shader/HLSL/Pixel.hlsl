#include "Layout.hlsli"

namespace Shader
{
    // register : 로드된 파일이 RS로 넘어가서 클리핑등을 적용 (Bitmap 형식)
    // T0 : tecture 0번째에 넘김
    // RS에서 받아온 Bitmap을 픽셀화
    const Texture2D Resource : register(T0);

    // SV_Target : OM으로 쓰기용도로 넘기겠다는 의미를 설정
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

/* 속성 설정
속성 -> HlSL 컴파일러 -> 진입점 이름 : main -> Shader::Pixel
                      -> 셰이더 형식 : 픽셀 셰이더(/ps)
                      -> 셰이더 모델 : Shader Model5.0(/5_0)

속성 -> 모든 속성 -> 헤더 변수 이름 : Bytecode
                  -> 헤더 파일 이름 : $(ProjectDir)Shader/Bytecode/Pixel.h
*/
