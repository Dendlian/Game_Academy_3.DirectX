#include "Layout.hlsli"

namespace Shader
{
	Layout::Pixel Vertex(const Layout::Vertex Input)
	{
		Layout::Pixel Output =
		{
			Input.Position,
			Input.TexCoord
		};
		return Output;
	}
}

// �Ӽ� -> HlSL �����Ϸ� -> ������ �̸� : main -> Shader::Vertex
//						 -> ���̴� ���� : ������ ���̴�(/vs)
//						 -> ���̴� �� : Shader Model5.0(/5_0)

// �Ӽ� -> ��� �Ӽ� -> ������� �̸� : Bytecode