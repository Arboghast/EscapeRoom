/*

This effect file is designed for rending bitmap fonts output by
AngelCode Bitmap Font Generator. It is capable of rendering
from 32bit textures that pack colored icons together with outlined
characters into one texture, where the icons use all 32bits and the
characters only use 8bits each.

*/

shared float4x4 g_mWorld : WORLD;      // World matrix
shared float4x4 g_mView  : VIEW;       // View matrix
shared float4x4 g_mProj  : PROJECTION; // Projection matrix

texture  g_txScene : TEXTURE0;

sampler2D g_samScene =
sampler_state
{
    Texture = <g_txScene>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};


void VertScene( float4 vPos         : POSITION,
				float4 vColor       : COLOR0,
				int4   vChnl        : BLENDINDICES0,
                float2 vTex0        : TEXCOORD0,
                out float4 oDiffuse : COLOR0,
                out int4   oChnl    : TEXCOORD1,
                out float4 oPos     : POSITION,
                out float2 oTex0    : TEXCOORD0 )
{
    // Transform the position from object space to homogeneous projection space
    oPos = mul( vPos, g_mWorld );
    oPos = mul( oPos, g_mView );
    oPos = mul( oPos, g_mProj );

    // Just copy the texture coordinate and color 
    oDiffuse = vColor;
    oChnl = vChnl;
    oTex0 = vTex0;
}


float4 PixWithoutOutline( float4 color : COLOR0,
                          int4   chnl  : TEXCOORD1,
                          float2 tex0  : TEXCOORD0 ) : COLOR0
{
    float4 pixel = tex2D(g_samScene, tex0);
    
    // Are we rendering a colored image, or 
    // a character from only one of the channels
    if( dot(vector(1,1,1,1), chnl) )
    {
        // Get the pixel value
		float val = dot(pixel, chnl);
		
        pixel.rgb = 1;
        pixel.a   = val;
    }
    
	return pixel * color;
}

float4 PixWithOutline( float4 color : COLOR0,
                       int4   chnl  : TEXCOORD1,
                       float2 tex0  : TEXCOORD0 ) : COLOR0
{
    float4 pixel = tex2D(g_samScene, tex0);
    
    // Are we rendering a colored image, or 
    // a character from only one of the channels
    if( dot(vector(1,1,1,1), chnl) )
    {
        // Get the pixel value
		float val = dot(pixel, chnl);
		
        // A value above .5 is part of the actual character glyph
        // A value below .5 is part of the glyph outline
		pixel.rgb = val > 0.5 ? 2*val-1 : 0;
		pixel.a   = val > 0.5 ? 1 : 2*val;
    }
    
	return pixel * color;
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique RenderWithOutline
{
    pass P0
    {
		// Turn on alpha blending
		AlphaBlendEnable = true;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		
		// Set the vertex and pixel shaders
        VertexShader = compile vs_2_0 VertScene();
        PixelShader  = compile ps_2_0 PixWithOutline();
    }
}

technique RenderWithoutOutline
{
    pass P0
    {
		// Turn on alpha blending
		AlphaBlendEnable = true;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		
		// Set the vertex and pixel shaders
        VertexShader = compile vs_2_0 VertScene();
        PixelShader  = compile ps_2_0 PixWithoutOutline();
    }
}
