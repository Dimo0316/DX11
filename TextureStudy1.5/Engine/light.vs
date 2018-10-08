////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
	float4 Kd: DIFFUSE;
	float4 Ks: SPECULAR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 worldposition: POSITION;
	float3 worldnormal: NORMAL;
	float4 Kd: DIFFUSE;
	float4 Ks: SPECULAR;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;


    // 改变顶点为四个分量其次坐标.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    
    // 世界坐标系中的顶点法向.
    float3 N = mul(input.normal, (float3x3)worldMatrix);
	output.worldnormal= N;
	
	//世界坐标系顶点位置
    worldPosition = mul(input.position, worldMatrix);
	output.worldposition =  worldPosition;

	//顶点的漫反射和高光系数
	output.Ks = input.Ks;
	output.Kd = input.Kd;

    return output;
}

