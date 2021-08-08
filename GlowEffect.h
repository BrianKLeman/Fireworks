#pragma once
#include "Util.h"
#include "./effect/HLSLEffect.h"

class GlowEffect
{

public:
	GlowEffect(LPDIRECT3DDEVICE9 device9, D3DVIEWPORT9 vp)
		: mpDevice9(device9)
	{

		HR(D3DXCreateTexture(device9, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &mpGlowTexture1));
		HR(D3DXCreateTexture(device9, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &mpGlowTexture2));
		HR(D3DXCreateTexture(device9, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &mpIntermediaryTexture));
		HR(D3DXCreateTexture(mpDevice9, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &mpPingTexture));
		HR(D3DXCreateTexture(mpDevice9, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &mpPongTexture));
		HR(mpGlowTexture1->GetSurfaceLevel(0, &mpTextureSurfaceGlow1));
		HR(mpGlowTexture2->GetSurfaceLevel(0, &mpTextureSurfaceGlow2));
		HR(mpPingTexture->GetSurfaceLevel(0, &mpPingSurface));
		HR(mpPongTexture->GetSurfaceLevel(0, &mpPongSurface));
		CreateQuadVertexBuffer(mpDevice9, &mpPostProcessingPlane);
		mpGlowEffect = new CHLSLEffect(device9);
		mpGlowEffect->createEffect("./effect/glow.fx");		
	}
	void PostProcessWithGlow(LPDIRECT3DTEXTURE9 pTextureOut, void (*pRenderFunc)(), float sceneContribution, float glowRadius, float glowStrength, float screenWidth, float screenHeight);
private:
	LPDIRECT3DDEVICE9 mpDevice9 = nullptr;
	LPDIRECT3DTEXTURE9 mpGlowTexture1 = nullptr, mpGlowTexture2 = nullptr, mpIntermediaryTexture = nullptr;
	LPDIRECT3DSURFACE9 mpPrimaryRenderTarget, mpPongSurface, mpPingSurface;
	LPDIRECT3DSURFACE9 mpTextureSurface = nullptr, mpTextureSurfaceGlow1 = nullptr, mpTextureSurfaceGlow2 = nullptr;
	LPDIRECT3DTEXTURE9 mpPingTexture = nullptr, mpPongTexture = nullptr;
	LPDIRECT3DVERTEXBUFFER9 mpPostProcessingPlane;

	void SetUpEffect(float screenWidth, float screenHeight);
	void PostProcessTexture(LPDIRECT3DTEXTURE9 pTextureOut, LPDIRECT3DTEXTURE9 pTextureIn, float sceneContribution, float glowRadius, float glowStrength, float screenWidth, float screenHeight);
	// Glow 1
	void ProcessGlow1(UINT nbPasses);

	void ProcessGlow2(UINT nbPasses);

	// Pass 4
	void ComposeTextures(LPDIRECT3DTEXTURE9 pTextureA, LPDIRECT3DTEXTURE9 pTextureB, LPDIRECT3DTEXTURE9 pTextureOut);

	void DisplayTexture(LPDIRECT3DTEXTURE9 pTexture, LPD3DXSPRITE sprite);

	CHLSLEffect* mpGlowEffect;
};

