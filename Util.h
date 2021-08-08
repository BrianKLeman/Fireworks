#pragma once
#pragma once
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#define D3D_DEBUG_INFO	// Enable debugging information, so the .NET environment can help you.
#define DEBUG_VS
#define DEBUG_PS 
#include <windows.h>

#include <d3dx9.h>
#include <dinput.h>
#include <memory>
#include <DxErr.h>
#if defined(DEBUG) | defined(_DEBUG)
#include <DxErr.h>
#ifndef HR
#define HR(x)                                      \
{                                                  \
	HRESULT hr = x;                                \
{                                              \
	\
}                                              \
}
#endif

#else
#ifndef HR
#define HR(x) x;
#endif
#endif 
#define D3DFVF_CUSTOMVERTEXPP (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(1))

struct CUSTOMVERTEXPP
{
	D3DXVECTOR3 position;	// Position
	D3DXVECTOR2 texCoord;	// Texture co-ordinates.
};

extern HRESULT CreateQuadVertexBuffer(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9* pVB);
extern void RenderToTexture(void (*pRenderFunc)(), LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DDEVICE9 pDevice9);