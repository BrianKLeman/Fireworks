


float4x4 ObjWorldView;
float4x4 ObjWorldViewProj;
Texture background;
samplerCUBE backgroundCube = sampler_state
{
    Texture = (background);
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
};						 
struct VS_OUT
{
	float4 pos : POSITION;
	float3 tex : TEXCOORD2;
};


VS_OUT mainVS(in float4 pos : POSITION, in float3 N:NORMAL, in float4 colour : COLOR)
{	
	VS_OUT vs_out;	
	vs_out.pos = mul(pos, ObjWorldViewProj);
	vs_out.pos.z = vs_out.pos.w;
	vs_out.tex = pos.xyz;
	return vs_out;
}

float4 mainPS(in VS_OUT vs) : COLOR 
{	
	return float4(texCUBE(backgroundCube, vs.tex.rgb).rgb,1.0);
	//return float4(vs.tex.xyz,1.0f);
}



technique technique0 {
	pass p0 {
		// Set render states
		Lighting = FALSE;
		ZEnable = TRUE;
		Sampler[0] = backgroundCube;
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}

