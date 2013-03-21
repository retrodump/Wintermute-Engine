// contributed by cremen
// http://forum.dead-code.org/index.php?topic=3879


float4x4 Wm: World;                 // World matrix for object
float4x4 WvpM: WorldViewProjection;    // World * View * Projection matrix
float4 	LightPos: ActorLightPos;

float4 BorderColor = { 1.0, 0.0, 0.0, 0.7 };
float BorderWidth = 5.0;

texture g_MeshTexture : DiffuseMap;

// Data structure for Vertex Shaders
struct invs_data {
    float4 Position	: POSITION;
    float3 Normal	: NORMAL;
    float2 UV		: TEXCOORD0;
};

// Data structure for Pixel color shader
struct outvs_data {
    float4 Position	: POSITION;
    float2 UV		: TEXCOORD0;
	float4 Diffuse  : COLOR0;
};

outvs_data VertShader(invs_data IN){
	float4 pos = mul(IN.Position, WvpM);
	float3 lightDir = normalize(LightPos-pos);

	outvs_data OUT = (outvs_data)0;
	OUT.UV = IN.UV;
	OUT.Diffuse = dot( IN.Normal, lightDir );
	OUT.Position = pos;
	return OUT;
}

sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float4 PixShader(outvs_data IN): COLOR0 {
	float4 col = max(0.4, IN.Diffuse) * tex2D(MeshTextureSampler, IN.UV);
	return float4( col.xyz, 1.0);
}

float4 GlowVertShader(invs_data IN): POSITION{
	float4 pos = mul(IN.Position, WvpM);
	float4 nrm = normalize(mul(IN.Normal, WvpM));
	return pos + nrm * BorderWidth;
}
float4 GlowPixShader(float4 pos: POSITION): COLOR0 {
	return BorderColor;
}

technique Main{
	pass glowPass{
		VertexShader = compile vs_2_0 GlowVertShader();
		PixelShader = compile ps_2_0 GlowPixShader();
		ZWriteEnable=false;
		//ZTest=true;
		CullMode = None;
	}
	
	pass colorPass{
		VertexShader = compile vs_2_0 VertShader();
		PixelShader = compile ps_2_0 PixShader();
		ZEnable=true;
		ZWriteEnable=true;
		//CullMode = None;
	}
	
}
