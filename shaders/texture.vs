cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    row_major float4x4 World : WORLD;
    uint InstanceId : SV_InstanceID;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normalW : NORMAL;
    float2 tex : TEXCOORD0;
};

PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    

    // Change the position vector to be 4 units for proper matrix calculations.
    output.position = mul(input.position, input.World);
    output.normalW = mul(input.normal, (float3x3)input.World);

    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = input.tex;

    return output;
}