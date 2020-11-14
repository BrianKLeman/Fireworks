


//created by Brian Leman

Texture2D diffuseTexture;
sampler DiffuseTexture = sampler_state
{ 
	MinFilter = LINEAR; 
	MagFilter = LINEAR; 
	Texture	  = (diffuseTexture);
};

float4x4 WorldViewProj : WorldViewProjection;
float4x4 debugMatrix = { 1.0f,0.0f,0.0f,-1.0f,
						 0.0f,1.0f,0.0f,-1.0f,
						 0.0f,0.0f,1.052f,8.42f,
						 0.0f,0.0f,1.0f,9.0f };
float4 translate;
float4 brightness = float4(1.0f,1.0f,1.0f,1.0f);

//create an output structure containing the normal in the texture coordinate
struct VS_OUT{
	float4 pos			: POSITION;
	float2 diffTexture	: TEXCOORD0;
	float3 col			: COLOR0;
};
float3 t_colour;

VS_OUT mainVS(in float4 pos : POSITION,  in float3 N:NORMAL,in float2 text : TEXCOORD0){
	
	
	VS_OUT vs_out;
	
	vs_out.pos = mul(pos+float4(translate.xyz,0.0f), WorldViewProj);
	
	vs_out.diffTexture = text;
	vs_out.col = float3(brightness.xyz);
	return vs_out;
}

struct Pixel
{
	float4 colour : COLOR;
	float  mask	  : STENCIL;
	
};

float4 mainPS(in VS_OUT vs) : COLOR {
	
	float3 textureComponent = tex2D(DiffuseTexture,float2(vs.diffTexture.x,vs.diffTexture.y));
	
	return float4(textureComponent.xyz,1.0f);
}

BlendState blendAdditive //does not work
{
    BlendEnable[0]  = true;
    DestBlend		= Dest_Color;
    SrcBlend		= Src_Color;
    BlendOp			= Add;
    
};

technique technique0 {
	
	pass p0 {
		
		AlphaBlendEnable=false;
		Lighting = FALSE;
		ZEnable = false; // We want z-buffering disabled
		
	
		
		Sampler[0] = DiffuseTexture;
		
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
		

	}
}