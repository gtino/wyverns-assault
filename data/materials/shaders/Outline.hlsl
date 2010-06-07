float4x4 WorldViewProjection;
//float3 EyePosition; //Camera Position

//Here's the important value. It determins the thickness of the outline.
//The value is completely dependent on the size of the model.
//My model is very tiny so my outine is very tiny.
//You may need to increase this or better yet, calculate it based on the distance
//between your camera and your model.
float offset;

//Sampler for the color texture applid to the model
sampler TextureSampler: register(s0);


//Input for the vertex Shader
//There are two vertex shaders both take the same input
struct VS_INPUT
{
	float4 Position		: POSITION0;
	float3 Normal		: NORMAL0;
	float2 Texcoord		: TEXCOORD0;

};

//Output for the outline vertex shader
struct VS_OUTPUT2
{
	float4 Position			: POSITION0;
	float4 Normal			: TEXCOORD1;
};


//The outline vertex shader
//This tranforms the model and
//"peaks" the surface (scales it out on it's normal) 
VS_OUTPUT2 outline_vp(VS_INPUT Input)
{
	//float3 ObjectPosition = mul(Input.Position, WorldViewProjection);
	
	VS_OUTPUT2 Output;
	Output.Normal			= mul(Input.Normal, WorldViewProjection);
	Output.Position			= mul(WorldViewProjection, Input.Position)+(mul(offset, mul(WorldViewProjection, Input.Normal)));
	
	return Output;
}

//This is the ouline pixel shader. It just outputs unlit black.
float4 black_fp() : COLOR0
{
   return float4(0.0f, 0.0f, 0.0f, 1.0f);
}
