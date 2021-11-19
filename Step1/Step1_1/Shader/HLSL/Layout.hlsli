namespace Layout
{
	struct Vertex
	{
		float4 Position	: POSITION;
		float4 TexCoord	: TEXCOORD;
	};
	
	// Vertex Shader���� ����
	// System Value : RS���� ����� �� �ִ� ������ ����
	struct Pixel
	{
		float4 Position : SV_POSITION; 
		float4 TexCoord	: COLOR;
	};
	
    typedef float4 Color;
}