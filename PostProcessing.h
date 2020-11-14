#include <windows.h>
#include <d3dx9.h>

#define D3DFVF_CUSTOMVERTEXPP (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(1))

struct CUSTOMVERTEXPP
{
	D3DXVECTOR3 position;	// Position
	D3DXVECTOR2 texCoord;	// Texture co-ordinates.
};

extern HRESULT CreateQuadVertexBuffer(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9* pVB);