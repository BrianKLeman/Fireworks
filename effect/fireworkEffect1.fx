


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
	vs_out.col = t_colour*brightness.xyz;
	return vs_out;
}

struct Pixel
{
	float4 colour : COLOR;
	float  mask	  : STENCIL;
	
};

float4 mainPS(in VS_OUT vs) : COLOR {
	float alpha = 1.0;
	float3 textureComponent = tex2D(DiffuseTexture,float2(vs.diffTexture.x,vs.diffTexture.y));

	textureComponent *= t_colour*float4(brightness.xyz,1.0);
	
	//if(alpha == 0.0f) clip(-1.0f);
	return float4(1.0,1.0,1.0,1.0);
}

technique technique0 {
	//SetBlendState[0](blendAdditive,float4(0.0f,0.0f,0.0f,0.0f),0xFFFFFFFF);
	pass p0 {		
		AlphaBlendEnable=true;
		Lighting = FALSE;
		ZEnable = false; // We want z-buffering disabled		
		DestBlend	= DestAlpha;
    	SrcBlend	= SrcAlpha;
    	BlendOp		= Add;
		Sampler[0] = DiffuseTexture;
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}