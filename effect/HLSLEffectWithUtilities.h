#pragma once
#include "HLSLEffect.h"

#define D3DFVF_CUSTOMVERTEX2 (D3DFVF_XYZW | D3DFVF_NORMAL | D3DFVF_TEX1 |  D3DFVF_TEXCOORDSIZE2(0))

class CHLSLEffectWithUtilities :
public CHLSLEffect
{
public:
CHLSLEffectWithUtilities(LPDIRECT3DDEVICE9 device);
~CHLSLEffectWithUtilities(void);

struct CUSTOMVERTEX
{
    D3DXVECTOR4 position;	// Position 
	D3DXVECTOR3 normal;		// Normal
	FLOAT u, v;				// Texture co-ordinates.
	
};
struct vertex_info
{
	int nbTriangles;
	LPDIRECT3DVERTEXBUFFER9 l_pVertexBuffer; // Buffers to hold faces
	
};
bool RenderWithEffect(vertex_info vertices_structure);

private:
	CHLSLEffectWithUtilities(void);
};

