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


    // �ı䶥��Ϊ�ĸ������������.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    
    // ��������ϵ�еĶ��㷨��.
    float3 N = mul(input.normal, (float3x3)worldMatrix);
	output.worldnormal= N;
	
	//��������ϵ����λ��
    worldPosition = mul(input.position, worldMatrix);
	output.worldposition =  worldPosition;

	//�����������͸߹�ϵ��
	output.Ks = input.Ks;
	output.Kd = input.Kd;

    return output;
}

