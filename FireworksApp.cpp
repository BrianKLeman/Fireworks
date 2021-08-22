#include "./FireworksApp.h"
#include "Particles.h"
#include "Util.h"

float FireworksApp::screenWidth = 800.f;
float FireworksApp::screenHeight = 600.f;
LPDIRECT3DTEXTURE9 FireworksApp::mpDiffuseTexture = nullptr;
LPD3DXSPRITE FireworksApp::gp_sprite = nullptr;
bool FireworksApp::OnSetupD3D()
{

	mpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	mpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	mpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	if (FAILED(D3DXCreateSprite(mpD3DDevice, &gp_sprite)))
	{
		return E_FAIL;
	}

	if(!SetupParticleHLSLEffectAndGeometry())
		MessageBox(nullptr, "Shaders failed to initialize.", "Program is closing.", MB_OK);

	D3DVIEWPORT9 vp;
	mpD3DDevice->GetViewport(&vp);
	HR(mpD3DDevice->GetRenderTarget(0, &gpPrimaryRenderTarget));
	HR(D3DXCreateTexture(mpD3DDevice, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &mpFireworksTexture));
	HR(D3DXCreateTexture(mpD3DDevice, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &mpFireTexture));
	HR(mpFireworksTexture->GetSurfaceLevel(0, &mpTextureSurface));
	HR(mpD3DDevice->GetRenderTarget(0, &gpPrimaryRenderTarget));

	mpGlowEffect = new GlowEffect(mpD3DDevice, vp);

	mSkybox = new SkyBox(mCamera);
	mSkybox->init(mpD3DDevice);

	return true;
}


bool FireworksApp::SetupParticleHLSLEffectAndGeometry()
{
	HR(D3DXCreateTextureFromFile(mpD3DDevice, "./effect/galaxy.png", &mpDiffuseTexture));
	if (mpDiffuseTexture == nullptr) return false;

	D3DXCreateTextureFromFile(mpD3DDevice, "./effect/sky.png", &mpForeground);
	if (mpForeground == nullptr) return false;

	return true;
}

void FireworksApp::RenderFire()
{
	D3DXVECTOR3 centre(0.5f, 0.5f, 0.f);
	D3DXMATRIX I; D3DXMATRIX T;
	D3DXVECTOR3 zero = D3DXVECTOR3(0.f, 0.f, 0.f);

	gp_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);

	StructParticle* p = mParticles;
	D3DXVECTOR3 pos(0.f, 0.f, 0.f);
	for (TIndex i = 0; i < FIRE_WORKS_LIMIT * particlesPerRocket; ++i, ++p)
	{
		if (!p->mAlive)
			continue;
		
		pos.x = p->p.x;
		pos.y = screenHeight - p->p.y;
		pos.z = p->p.z;
		gp_sprite->Draw(mpDiffuseTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA((int)p->colour.x, (int)p->colour.y, (int)p->colour.z, (int)p->colour.w));
	}

	D3DXVECTOR3 pf;
	gp_sprite->End();

}

void FireworksApp::RenderFireWorks()
{
	gp_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);

	StructParticle* p = &(mParticles[FIRE_WORKS_LIMIT * particlesPerRocket]);
	D3DXVECTOR3 pos(0.f, 0.f, 0.f);
	for (TIndex i = FIRE_WORKS_LIMIT * particlesPerRocket; i < MAX_PARTICLES; ++i, ++p)
	{
		if (!p->mAlive)
			continue;

		pos.x = p->p.x;
		pos.y = screenHeight - p->p.y;
		pos.z = 0.f;
		gp_sprite->Draw(mpDiffuseTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA((int)p->colour.x, (int)p->colour.y, (int)p->colour.z, 255));
	}
	gp_sprite->End();
}
void FireworksApp::OnRender()
{
		RenderToTexture(RenderFireWorks, mpFireworksTexture, mpD3DDevice);
		RenderToTexture(RenderFire, mpFireTexture, mpD3DDevice);
		mpGlowEffect->PostProcessWithGlow(mpFireworksTexture, RenderFireWorks, 1.f, gFireWorksGlowRadius, gFireWorksGlowStrength, mScreenWidth,mScreenHeight);
		mpGlowEffect->PostProcessWithGlow(mpFireTexture, RenderFire, 0.f, gFireGlowRadius, gFireGlowStrength, mScreenWidth, mScreenHeight);
		HR(mpD3DDevice->SetRenderTarget(0, gpPrimaryRenderTarget));

		// Clear the backbuffer to a black color
		HR(mpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 0, 0), 1.0f, 0));

		HR(mpD3DDevice->BeginScene());
		
		mSkybox->RenderWithEffect();
		HR(gp_sprite->Begin(D3DXSPRITE_ALPHABLEND));
		D3DXVECTOR3 pos;
		pos.x = 0.f;
		pos.y = 0.f;
		pos.z = 0.f;
		//HR(gp_sprite->Draw(mpForeground, nullptr, nullptr, &pos, D3DCOLOR_RGBA(255, 255, 255, 255)));
		HR(gp_sprite->Draw(mpFireworksTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA(255, 255, 255, 255)));
		HR(gp_sprite->Draw(mpFireTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA(255, 255, 255, 255)));
		HR(gp_sprite->End());
		HR(mpD3DDevice->EndScene());

		// Present the backbuffer to the display.
		mpD3DDevice->Present(NULL, NULL, NULL, NULL);

}

void FireworksApp::OnProcessKeyboardInput(char* buffer)
{	
	const float delta = 0.1f;
	if (KEYDOWN(buffer, DIK_W))
	{
		gFireGlowRadius += delta;
	}
		
	if (KEYDOWN(buffer, DIK_S))
	{
		gFireGlowRadius -= delta;
	}
		
	if (KEYDOWN(buffer, DIK_A))
	{
		gFireGlowStrength += delta;
	}

	if (KEYDOWN(buffer, DIK_D))
	{
		gFireGlowStrength -= delta;
	}

	if (KEYDOWN(buffer, DIK_I))
	{
		gFireWorksGlowRadius += delta;
	}

	if (KEYDOWN(buffer, DIK_K))
	{
		gFireWorksGlowRadius -= delta;
	}

	if (KEYDOWN(buffer, DIK_J))
	{
		gFireWorksGlowStrength += delta;
	}

	if (KEYDOWN(buffer, DIK_L))
	{
		gFireWorksGlowStrength -= delta;
	}
}

void FireworksApp::showControls()
{
	//MessageBox(mHwnd, "Keyboard Configuration\n\nCURSOR:\tRotate Whole Cube\nSPACE:\tReset View\n\nTab:\tCycle Through Rotatable Sections/Complete Rotation\n\tPage Up Or Page Down Keys:\tRotate\n\nR:\tHOLD to rapidly rotate sections\n\nQ:\tTo Quit", "Controls", MB_OK | MB_ICONINFORMATION);
}

void FireworksApp::OnCleanUp()
{
	delete mSkybox;
	delete mpFireworksTexture;
	delete mpFireTexture;
	delete mpForeground;
	delete mpGlowEffect;
	delete mpDiffuseTexture;
}

void FireworksApp::OnUpdate()
{
	UpdateParticleSystems();
}

void FireworksApp::UpdateParticleSystems()
{
	float dt = mTimer.TotalElapsedSeconds();
	mTimer.Reset();
	UpdateParticles(dt);
}
