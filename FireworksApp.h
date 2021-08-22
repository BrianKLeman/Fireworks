#pragma once
#include "./Shared/Window.h"
#include "./Shared/Camera.h"
#include "effect/HLSLEffectWithUtilities.h"
#include "Shared/Timer.h"
#include "GlowEffect.h"
#include "./Shared/SkyBox.h"
class FireworksApp :
    public D3DWindow
{
public:
	FireworksApp(HINSTANCE hInstance, HINSTANCE hPrevInstance)
		: D3DWindow(hInstance, hPrevInstance)
	{
	}
	

	static LPDIRECT3DTEXTURE9 mpDiffuseTexture;
	static LPD3DXSPRITE gp_sprite;
	static FLOAT screenWidth, screenHeight;
private:
	D3DXVECTOR3 vCameraFW = D3DXVECTOR3(00.0f, 00.0f, 10.0f);
	D3DXVECTOR3 vLookatFW = D3DXVECTOR3(0.0f, 00.0f, 0.0f);
	// Global variables
	Camera mCamera = Camera(Transform(vCameraFW));

	static void RenderFire();
	static void RenderFireWorks();
	void UpdateParticleSystems();
	bool SetupParticleHLSLEffectAndGeometry();
	LPDIRECT3DTEXTURE9 mpFireworksTexture = nullptr, mpFireTexture = nullptr;	
	LPDIRECT3DSURFACE9 mpPrimaryRenderTarget, mpTextureSurface;
	LPDIRECT3DTEXTURE9 mpForeground = nullptr;
	Timer mTimer;
	FLOAT gFireGlowRadius = 3.f;
	FLOAT gFireGlowStrength = 4.f;
	FLOAT gFireWorksGlowRadius = 2.f;
	FLOAT gFireWorksGlowStrength = 4.f;
	GlowEffect* mpGlowEffect = nullptr;
	SkyBox* mSkybox;
protected:
	bool OnSetupD3D() override;
	void OnRender() override;
	void OnProcessKeyboardInput(char* buffer) override;
	void showControls();
	virtual void OnCleanUp() override;
	void OnUpdate() override;
};

