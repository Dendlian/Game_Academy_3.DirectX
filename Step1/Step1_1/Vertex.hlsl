#include "Layout.hlsli"

namespace Shader
{
	namespace Resource
	{
		cbuffer Transform : register(B0) { matrix World;	  } // 4x4 행렬을 만들어주는 키워드
		cbuffer Transform : register(B1) { matrix View;		  }
		cbuffer Transform : register(B2) { matrix Projection; }
	}

	Layout::Pixel Vertex(const Layout::Vertex Input)
	{
		Layout::Pixel Output =
		{
			Input.Position,
			Input.TexCoord
		};

		//cbuffer
		// RS로 보내주는
		Output.Position = mul(Output.Position, Resource::World);
		Output.Position = mul(Output.Position, Resource::View);
		Output.Position = mul(Output.Position, Resource::Projection);

		return Output;
	}
}

// 속성 -> HlSL 컴파일러 -> 진입점 이름 : main -> Shader::Vertex
//						-> 셰이더 형식 : 꼭지점 셰이더(/vs)
//						-> 셰이더 모델 : Shader Model5.0(/5_0)

// 속성 -> 모든 속성 -> 헤더변수 이름 : Bytecode