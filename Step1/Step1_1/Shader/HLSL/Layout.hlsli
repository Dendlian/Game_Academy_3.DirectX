namespace Layout
{
	struct Vertex
	{
		float4 Position	: POSITION;
		float4 TexCoord	: TEXCOORD;
	};
	
	// Vertex Shader에서 수행
	// System Value : RS에서 사용할 수 있는 값으로 설정
	struct Pixel
	{
		float4 Position : SV_POSITION; 
		float4 TexCoord	: COLOR;
	};
	
    typedef float4 Color;
}