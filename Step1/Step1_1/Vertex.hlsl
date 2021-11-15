#include "Layout.hlsli"

namespace Shader
{
	namespace Resource
	{
		cbuffer Transform : register(B0) { matrix World;	  } // 4x4 ����� ������ִ� Ű����
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
		// RS�� �����ִ�
		Output.Position = mul(Output.Position, Resource::World);
		Output.Position = mul(Output.Position, Resource::View);
		Output.Position = mul(Output.Position, Resource::Projection);

		return Output;
	}
}

// �Ӽ� -> HlSL �����Ϸ� -> ������ �̸� : main -> Shader::Vertex
//						-> ���̴� ���� : ������ ���̴�(/vs)
//						-> ���̴� �� : Shader Model5.0(/5_0)

// �Ӽ� -> ��� �Ӽ� -> ������� �̸� : Bytecode