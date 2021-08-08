#include "GlowEffect.h"

#define D3DFVF_CUSTOMVERTEXPP (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(1))



// Glow 1
void GlowEffect::ProcessGlow1(UINT nbPasses)
{
	HR(mpDevice9->SetRenderTarget(0, mpTextureSurfaceGlow1));
	HR(mpDevice9->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0));
	HR(mpDevice9->SetFVF(D3DFVF_CUSTOMVERTEXPP));
	HR(mpDevice9->SetStreamSource(0, mpPostProcessingPlane, 0, sizeof(CUSTOMVERTEXPP)));
	HR(mpDevice9->BeginScene());
	HR(mpGlowEffect->getEffect()->Begin(&nbPasses, 0));
	HR(mpGlowEffect->getEffect()->BeginPass(0));
	HR(mpDevice9->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 2));
	HR(mpGlowEffect->getEffect()->EndPass());
	HR(mpGlowEffect->getEffect()->End());
	HR(mpDevice9->EndScene());
}

void GlowEffect::ProcessGlow2(UINT nbPasses)
{
	//// Glow frame
	HR(mpDevice9->SetRenderTarget(0, mpTextureSurfaceGlow2));
	HR(mpDevice9->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0));
	HR(mpDevice9->SetFVF(D3DFVF_CUSTOMVERTEXPP));
	HR(mpDevice9->SetStreamSource(0, mpPostProcessingPlane, 0, sizeof(CUSTOMVERTEXPP)));
	HR(mpDevice9->BeginScene());
	HR(mpGlowEffect->getEffect()->Begin(&nbPasses, 0));
	// Glow 2
	HR(mpGlowEffect->getEffect()->BeginPass(1));
	HR(mpDevice9->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 2));
	HR(mpGlowEffect->getEffect()->EndPass());
	HR(mpGlowEffect->getEffect()->End());
	HR(mpDevice9->EndScene());
}

// Pass 4
void GlowEffect::ComposeTextures(LPDIRECT3DTEXTURE9 pTextureA, LPDIRECT3DTEXTURE9 pTextureB, LPDIRECT3DTEXTURE9 pTextureOut)
{
	// Composition.
	LPDIRECT3DSURFACE9 pSurface = nullptr;
	pTextureOut->GetSurfaceLevel(0, &pSurface);
	HR(mpDevice9->SetRenderTarget(0, pSurface));

	this->mpGlowEffect->SetParameter("gBackgroundTexture", pTextureA, CHLSLEffect::P_TEXTURE);
	mpGlowEffect->SetParameter("gForegroundTexture", pTextureB, CHLSLEffect::P_TEXTURE);
	UINT nbPasses = 0;
	mpDevice9->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	HR(mpDevice9->SetFVF(D3DFVF_CUSTOMVERTEXPP));
	HR(mpDevice9->SetStreamSource(0, mpPostProcessingPlane, 0, sizeof(CUSTOMVERTEXPP)));

	// Combine Glow and fireworks.
	HR(mpDevice9->BeginScene());
	HR(mpGlowEffect->getEffect()->Begin(&nbPasses, 0));
	HR(mpGlowEffect->getEffect()->BeginPass(3));
	mpDevice9->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 2);
	HR(mpGlowEffect->getEffect()->EndPass());
	HR(mpGlowEffect->getEffect()->End());
	HR(mpDevice9->EndScene());
}

void GlowEffect::DisplayTexture(LPDIRECT3DTEXTURE9 pTexture, LPD3DXSPRITE sprite)
{
	LPDIRECT3DSURFACE9 primaryRenderTarget;
	HR(mpDevice9->SetRenderTarget(0, primaryRenderTarget));
	// Clear the backbuffer to a black color
	HR(mpDevice9->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0));
	HR(mpDevice9->BeginScene());
	HR(sprite->Begin(D3DXSPRITE_ALPHABLEND));
	D3DXVECTOR3 pos;
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
	HR(sprite->Draw(pTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA(255, 255, 255, 255)));
	HR(sprite->End());
	HR(mpDevice9->EndScene());
}

void GlowEffect::SetUpEffect(float screenWidth, float screenHeight)
{
	D3DXHANDLE technique = mpGlowEffect->getEffect()->GetTechniqueByName("Main");
	HR(mpGlowEffect->getEffect()->SetTechnique(technique));
	HR(mpGlowEffect->getEffect()->ValidateTechnique(technique));

	D3DXHANDLE viewportSize = mpGlowEffect->getEffect()->GetParameterByName(nullptr, "ViewportSize");
	FLOAT viewPortSize[] = { screenWidth, screenHeight };
	HR(mpGlowEffect->getEffect()->SetFloatArray(viewportSize, viewPortSize, 2));

	D3DXHANDLE viewportOffset = mpGlowEffect->getEffect()->GetParameterByName(nullptr, "ViewportOffset");
	FLOAT viewPortOffset[] = { 0.0, 0.0f };
	HR(mpGlowEffect->getEffect()->SetFloatArray(viewportOffset, viewPortOffset, 2));

	HR(mpGlowEffect->getEffect()->CommitChanges());
}

void GlowEffect::PostProcessTexture(LPDIRECT3DTEXTURE9 pTextureOut, LPDIRECT3DTEXTURE9 pTextureIn, float sceneContribution, float glowRadius, float glowStrength, float screenWidth, float screenHeight)
{
	D3DXHANDLE technique = mpGlowEffect->getEffect()->GetTechniqueByName("Main");
	HR(mpGlowEffect->getEffect()->SetTechnique(technique));
	HR(mpGlowEffect->getEffect()->ValidateTechnique(technique));

	mpGlowEffect->SetParameter("gSceneTexture", mpIntermediaryTexture, CHLSLEffect::P_TEXTURE);

	{
		D3DXHANDLE handle = mpGlowEffect->getEffect()->GetParameterByName(nullptr, "gSceneness");
		HR(mpGlowEffect->getEffect()->SetFloat(handle, sceneContribution));
	}

	{
		D3DXHANDLE handle = mpGlowEffect->getEffect()->GetParameterByName(nullptr, "gGlowness");
		HR(mpGlowEffect->getEffect()->SetFloat(handle, glowStrength));
	}

	{
		D3DXHANDLE handle = mpGlowEffect->getEffect()->GetParameterByName(nullptr, "gGlowSpan");
		HR(mpGlowEffect->getEffect()->SetFloat(handle, glowRadius));
	}

	D3DXHANDLE viewportSize = mpGlowEffect->getEffect()->GetParameterByName(nullptr, "ViewportSize");
	FLOAT viewPortSize[] = { screenWidth, screenHeight };
	HR(mpGlowEffect->getEffect()->SetFloatArray(viewportSize, viewPortSize, 2));
	D3DXHANDLE viewportOffset = mpGlowEffect->getEffect()->GetParameterByName(nullptr, "ViewportOffset");
	FLOAT viewPortOffset[] = { 0.0, 0.0f };
	HR(mpGlowEffect->getEffect()->SetFloatArray(viewportOffset, viewPortOffset, 2));
	HR(mpGlowEffect->getEffect()->CommitChanges());
	UINT nbPasses(0);
	ProcessGlow1(nbPasses);
	mpGlowEffect->SetParameter("gGlowMap1", mpGlowTexture1, CHLSLEffect::P_TEXTURE);
	LPDIRECT3DSURFACE9 pSurface = nullptr;
	pTextureOut->GetSurfaceLevel(0, &pSurface);
	HR(mpDevice9->SetRenderTarget(0, pSurface));
	ProcessGlow2(nbPasses);
	mpGlowEffect->SetParameter("gGlowMap2", mpGlowTexture2, CHLSLEffect::P_TEXTURE);
	// Glow frame
	pTextureOut->GetSurfaceLevel(0, &pSurface);
	HR(mpDevice9->SetRenderTarget(0, pSurface))
		HR(mpDevice9->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));
	HR(mpDevice9->SetFVF(D3DFVF_CUSTOMVERTEXPP));
	HR(mpDevice9->SetStreamSource(0, mpPostProcessingPlane, 0, sizeof(CUSTOMVERTEXPP)));
	HR(mpDevice9->BeginScene());
	HR(mpGlowEffect->getEffect()->Begin(&nbPasses, 0));
	// Glow 2
	HR(mpGlowEffect->getEffect()->BeginPass(2));
	HR(mpDevice9->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 2));
	HR(mpGlowEffect->getEffect()->EndPass());
	HR(mpGlowEffect->getEffect()->End());
	HR(mpDevice9->EndScene());
}

// Uses Passes 1 and 2 of the glow effect.
void GlowEffect::PostProcessWithGlow(LPDIRECT3DTEXTURE9 pTextureOut, void (*pRenderFunc)(), float sceneContribution, float glowRadius, float glowStrength, float screenWidth, float screenHeight)
{
	RenderToTexture(pRenderFunc, mpIntermediaryTexture, mpDevice9);
	PostProcessTexture(pTextureOut, mpIntermediaryTexture, sceneContribution, glowRadius, glowStrength, screenWidth, screenHeight);
	PostProcessTexture(mpIntermediaryTexture, pTextureOut, sceneContribution, glowRadius / 2.f, glowStrength / 2.f, screenWidth, screenHeight);
	PostProcessTexture(pTextureOut, mpIntermediaryTexture, sceneContribution, glowRadius / 4.f, glowStrength / 4.f, screenWidth, screenHeight);
}
