namespace Layout
{
	struct Vertex
	{
		float4 Position	: POSITION;
		float4 TexCoord	: TEXCOORD;
	};
	
	// vertex shader���� ����
	struct Pixel
	{
		float4 Position : SV_POSITION; 
		// System Value : RS���� ����� �� �ִ� ������ ����
		float4 TexCoord	: COLOR;
	};
	
    typedef float4 Color;
}