#include "Util.h"


HRESULT CreateQuadVertexBuffer( LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9* ppVB )
{
	// Calculate the number of vertices required, and the size of the buffer to hold them.
	int Vertices = 6;
	int BufferSize = Vertices * sizeof(CUSTOMVERTEXPP);

	// Create the vertex buffer.
	if (FAILED(pDevice -> CreateVertexBuffer(BufferSize, 0, D3DFVF_CUSTOMVERTEXPP, D3DPOOL_DEFAULT, ppVB, NULL)))
	{
		return E_FAIL; // if the vertex buffer could not be created.
	}

	// Fill the buffer with appropriate vertices to describe the cube...

	// Create a pointer to the first vertex in the buffer.
	CUSTOMVERTEXPP *pVertices;
	if (FAILED((*ppVB) -> Lock(0, 0, (void**)&pVertices, 0)))
	{
		return E_FAIL;  // if the pointer to the vertex buffer could not be established.
	}

	// Fill the vertex buffers with data...
	const float length = 1.f;
	//        1--------------2
	//       /|             /|
	//      / |            / |
	//     /  |           /  |
	//    0---+----------3   |
	//    |   5----------+---6
	//    |  /           |  /
	//    | /            | /
	//    |/             |/
	//    4--------------7

	D3DXVECTOR3 cubeVertices[8] = 
	{
		D3DXVECTOR3(-length, length, 0.f),
		D3DXVECTOR3(-length, length, 0.f),
		D3DXVECTOR3( length, length, 0.f),
		D3DXVECTOR3( length, length, 0.f),

		D3DXVECTOR3(-length,-length, 0.f),
		D3DXVECTOR3(-length,-length, 0.f),
		D3DXVECTOR3( length,-length, 0.f),
		D3DXVECTOR3( length,-length, 0.f)
	};	

	D3DXVECTOR2 texCoords[4] =
	{
		D3DXVECTOR2(0.f, 0.f), // Top Left Corner = 0
		D3DXVECTOR2(1.f, 0.f), // Top Right Corner = 1
		D3DXVECTOR2(0.f,1.f), // Bottom Left Corner = 2
		D3DXVECTOR2(1.f,1.f)  // Bottom Right Corner = 3
	};
	enum TEX_COORDS { TEXCOORD_TOPLEFT = 0, TEXCOORD_TOPRIGHT = 1, TEXCOORD_BOTTOMLEFT = 2, TEXCOORD_BOTTOMRIGHT = 3};

	int vertexIndex = 0;
	// Side 1 - Front face
	pVertices[vertexIndex].position = cubeVertices[0];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPLEFT];
	++vertexIndex;
	pVertices[vertexIndex].position = cubeVertices[3];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	++vertexIndex;
	pVertices[vertexIndex].position = cubeVertices[4];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	++vertexIndex;
	pVertices[vertexIndex].position = cubeVertices[3];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	++vertexIndex;
	pVertices[vertexIndex].position = cubeVertices[7];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMRIGHT];
	++vertexIndex;
	pVertices[vertexIndex].position = cubeVertices[4];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];

	return (*ppVB)->Unlock();
}

void RenderToTexture(void (*pRenderFunc)(), LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DDEVICE9 pDevice9)
{
	LPDIRECT3DSURFACE9 pSurface = nullptr;
	pTexture->GetSurfaceLevel(0, &pSurface);
	LPDIRECT3DSURFACE9 pOriginalRenderTarget;
	HR(pDevice9->GetRenderTarget(0, &pOriginalRenderTarget));
	HR(pDevice9->SetRenderTarget(0, pSurface));

	// Begin the scene
	HR(pDevice9->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0));

	if (SUCCEEDED(pDevice9->BeginScene()))
	{
		pRenderFunc();
		// End the scene.		
		HR(pDevice9->EndScene());
	}

	HR(pDevice9->SetRenderTarget(0, pOriginalRenderTarget));

}