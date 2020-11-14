//-----------------------------------------------------------------------------
// Author: BRIAN LEMAN, D.O.B. START DATE: 30/09/2009
// Code in this file is partly from Programming For Games Module at SHU
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#define D3D_DEBUG_INFO
#define DEBUG_VS
#define DEBUG_PS 
//-----------------------------------------------------------------------------
// Include these files
#include <windows.h>	// Windows library (for window functions, menus, dialog boxes, etc)
#include <d3dx9.h>		// Direct 3D library (for all Direct 3D functions)
#include <dinput.h>		// Direct Input library (for Direct Input functions)
#include <DxErr.h>
#include "resource.h"
#include "Particles.h"
#include "effect/HLSLEffect.h"
#include "PostProcessing.h"
#include <ctime>
#include "Trace.h"

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
//-----------------------------------------------------------------------------
// Global variables
HINSTANCE inst; //window instance
HWND hwnd;

D3DXMATRIX matView;
D3DXMATRIX matProj;
LPDIRECT3D9				g_pD3D = nullptr; // The D3D object
LPDIRECT3DDEVICE9		g_pd3dDevice = nullptr; // The rendering device
LPDIRECTINPUT8			g_pDI = nullptr; // The Direct Input object
LPDIRECTINPUTDEVICE8	g_pDIMouseDevice = nullptr; // The Direct Input mouse device.
LPDIRECTINPUTDEVICE8	g_pDIKeyboardDevice	= nullptr; // The Direct Input keyboard device.

// Global variables
D3DXMATRIX TranslateMat;
D3DMATERIAL9 Mtl;
D3DXVECTOR3 vCamera(00.0f, 0.0f, 10.0f);
D3DXVECTOR3 vLookat(0.0f, 0.0f, 0.0f);
double dt(0.05);
D3DXVECTOR3 vVelocity(0.0,0.0,0.0);
D3DXVECTOR3 vG(0.0,-9.81f,0.0);
//angle of the camera from the origin
double angle(90.0);
bool useShaders(false);
// Function Prototypes
void WINAPI CleanUpDirectInput();
void CleanUp();
D3DXMATRIX orientation;
//fire works
#include "effect/HLSLEffectWithUtilities.h"
#include "High Performance Timer/HighPerformanceTimer.h"
CHighPerformanceTimer* timer;

CHLSLEffectWithUtilities* effect2(nullptr);
IDirect3DTexture9* g_pDiffuseTexture = nullptr,*bgTexture = nullptr, *bgForeground = nullptr;
CHLSLEffectWithUtilities::vertex_info vertices_struct, vertices_struct_background;
D3DXVECTOR3 colour(1.0f,1.0f,1.0f);
D3DXMATRIX matViewFW, matProjFW;
D3DXVECTOR3 vCameraFW(00.0f, 00.0f, -10.0f);
D3DXVECTOR3 vLookatFW(0.0f, 00.0f, 0.0f);

 LPD3DXSPRITE gp_sprite = nullptr;
 LPD3DXLINE gp_line = nullptr;
 FLOAT screenWidth = 800.f, screenHeight = 600.f;
 LPDIRECT3DTEXTURE9 gpFireworksTexture = nullptr, gpFireTexture = nullptr, gpGlowTexture1 = nullptr, gpGlowTexture2 = nullptr, gpIntermediaryTexture = nullptr;
 LPDIRECT3DTEXTURE9 gpPingTexture = nullptr, gpPongTexture = nullptr;
 LPDIRECT3DSURFACE9 gpTextureSurface = nullptr, gpTextureSurfaceGlow1 = nullptr, gpTextureSurfaceGlow2 = nullptr;
 LPDIRECT3DSURFACE9 gpPingSurface = nullptr, gpPongSurface = nullptr;
 LPDIRECT3DSURFACE9 gpPrimaryRenderTarget;
 CHLSLEffect* gGlowEffect = nullptr;
 LPDIRECT3DVERTEXBUFFER9 gpPostProcessingPlane = nullptr;

 FLOAT gFireGlowRadius = 3.f;
 FLOAT gFireGlowStrength = 4.f;
 FLOAT gFireWorksGlowRadius = 2.f;
 FLOAT gFireWorksGlowStrength = 4.f;	

bool SetupParticleHLSLEffectAndGeometry()
{	
	HR(D3DXCreateTextureFromFile(g_pd3dDevice, "./effect/galaxy.png", &g_pDiffuseTexture));
	if(g_pDiffuseTexture == nullptr) return false;
	D3DXCreateTextureFromFile(g_pd3dDevice, "./effect/galaxy.png", &bgTexture);
	if(bgTexture == nullptr) return false;
	D3DXCreateTextureFromFile(g_pd3dDevice, "./effect/sky.png", &bgForeground);
	if(bgForeground == nullptr) return false;
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMATRIX WorldViewProjection;
	D3DXMatrixMultiply(&WorldViewProjection,&world,&matViewFW);
	D3DXMatrixMultiply(&WorldViewProjection,&WorldViewProjection,&matProjFW);
	vertices_struct.nbTriangles = 2;
	int BufferSize = 6 * sizeof(CHLSLEffectWithUtilities::CUSTOMVERTEX);
	
	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice -> CreateVertexBuffer(BufferSize, 0, D3DFVF_CUSTOMVERTEX2, D3DPOOL_DEFAULT, &vertices_struct.l_pVertexBuffer, nullptr)))
	{
		return false; // if the vertex buffer could not be created.
	}

	// Create a pointer to the first vertex in the buffer.
	CHLSLEffectWithUtilities::CUSTOMVERTEX* pVertices = new CHLSLEffectWithUtilities::CUSTOMVERTEX[6]; //*pVertices;
    if (FAILED(vertices_struct.l_pVertexBuffer -> Lock(0, 0, (void**)&pVertices, 0)))
    {
		return false;  // if the pointer to the vertex buffer could not be established.
	}

	// Fill the vertex buffers with data...
	float width, height;width = 1.f; height = 1.f;

	// Side 1 - Front face
	pVertices[0].position.x = width;	// Vertex co-ordinate.
	pVertices[0].position.y = height;
	pVertices[0].position.z = 0;
	pVertices[0].position.w = 1;
	pVertices[0].u		  = 0;
	pVertices[0].v		  = 0;
	pVertices[0].normal.x = 0;	
	pVertices[0].normal.y = 0;
	pVertices[0].normal.z = -1;

	pVertices[1].position.x = -width;
	pVertices[1].position.y = height;
	pVertices[1].position.z = 0;
	pVertices[1].position.w = 1;
	pVertices[1].u		  = 1;
	pVertices[1].v		  = 0;
	pVertices[1].normal.x = 0;
	pVertices[1].normal.y = 0;
	pVertices[1].normal.z = -1;

	pVertices[2].position.x = -width;
	pVertices[2].position.y = -height;
	pVertices[2].position.z = 0;
	pVertices[2].position.w = 1;
	pVertices[2].u		  = 1;
	pVertices[2].v	      = 1;
	pVertices[2].normal.x = 0;
	pVertices[2].normal.y = 0;
	pVertices[2].normal.z = -1;

	pVertices[3].position.x = +width;
	pVertices[3].position.y = +height;
	pVertices[3].position.z = 0;
	pVertices[3].position.w = 1;
	pVertices[3].u		  = 0;
	pVertices[3].v	      = 0;
	pVertices[3].normal.x = 0;
	pVertices[3].normal.y = 0;
	pVertices[3].normal.z = -1;

	pVertices[4].position.x = -width;
	pVertices[4].position.y = -height;
	pVertices[4].position.z = 0;
	pVertices[4].position.w = 1;
	pVertices[4].u		  = 1;
	pVertices[4].v	      = 1;
	pVertices[4].normal.x = 0;
	pVertices[4].normal.y = 0;
	pVertices[4].normal.z = -1;

	pVertices[5].position.x = width;
	pVertices[5].position.y = -height;
	pVertices[5].position.z = 0;
	pVertices[5].position.w = 1;
	pVertices[5].u		  = 0;
	pVertices[5].v	      = 1;
	pVertices[5].normal.x = 0;
	pVertices[5].normal.y = 0;
	pVertices[5].normal.z = -1;

	// Unlock the vertex buffer...
	vertices_struct.l_pVertexBuffer -> Unlock();
		
	return true;
}

// Initialise Direct 3D.
HRESULT SetupD3D(HWND hWnd)
{
    // Create the D3D object, return failure if this can't be done.
    if (nullptr == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                    D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                    &d3dpp, &g_pd3dDevice)))
	{
		MessageBox(hWnd,"Error","Error - Direct3D Device Hardware Acceleration Failed",MB_OK);
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
		{
			MessageBox(hWnd,"Error","Error - Direct3D Device Software Failed",MB_OK);

			return E_FAIL;
		}
	}

	// Enable the lighting.
	g_pd3dDevice -> SetRenderState(D3DRS_LIGHTING, FALSE);
    // Turn on D3D depth buffer to ensure polygons are rendered correctly according to their depth
	g_pd3dDevice -> SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    // Turn on D3D backface culling to ensure both faces of triangles are rendered (clockwise & anti-clockwise)
    g_pd3dDevice -> SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	if (FAILED(D3DXCreateSprite(g_pd3dDevice, &gp_sprite)))
	{
		return E_FAIL;
	}	

	if (FAILED(D3DXCreateLine(g_pd3dDevice, &gp_line)))
		return E_FAIL;

	useShaders = SetupParticleHLSLEffectAndGeometry();;
	if(!useShaders) MessageBox(nullptr,"Shaders failed to initialize.","Program is closing.",MB_OK);

	gGlowEffect = new CHLSLEffect(g_pd3dDevice);
	if (!gGlowEffect->createEffect("./effect/glow.fx"))
	{
		return E_FAIL;
	}		
	
	D3DVIEWPORT9 vp;
	g_pd3dDevice->GetViewport(&vp);
	HR(D3DXCreateTexture( g_pd3dDevice, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &gpFireworksTexture));
	HR(D3DXCreateTexture( g_pd3dDevice, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &gpFireTexture));
	HR(D3DXCreateTexture( g_pd3dDevice, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &gpGlowTexture1));
	HR(D3DXCreateTexture( g_pd3dDevice, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &gpGlowTexture2));
	HR(D3DXCreateTexture( g_pd3dDevice, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &gpIntermediaryTexture));
	HR(D3DXCreateTexture(g_pd3dDevice, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &gpPingTexture));
	HR(D3DXCreateTexture(g_pd3dDevice, vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &gpPongTexture));
	HR(gpFireworksTexture->GetSurfaceLevel(0, &gpTextureSurface));	
	HR(gpGlowTexture1->GetSurfaceLevel(0, &gpTextureSurfaceGlow1));
	HR(gpGlowTexture2->GetSurfaceLevel(0, &gpTextureSurfaceGlow2));
	HR(gpPingTexture->GetSurfaceLevel(0, &gpPingSurface));
	HR(gpPongTexture->GetSurfaceLevel(0, &gpPongSurface));
	HR(g_pd3dDevice->GetRenderTarget(0, &gpPrimaryRenderTarget))
    return CreateQuadVertexBuffer(g_pd3dDevice, &gpPostProcessingPlane);
}

//-----------------------------------------------------------------------------
// Terminate Direct Input and clean up (always unacquire device before calling Release())
void WINAPI CleanUpDirectInput()
{
    if (g_pDI != nullptr)
    {
        // Release the mouse.
		if (g_pDIMouseDevice != nullptr)
        {
            g_pDIMouseDevice -> Unacquire();
            g_pDIMouseDevice -> Release();
            g_pDIMouseDevice = nullptr;
        }

		// Release the keyboard
		if (g_pDIKeyboardDevice != nullptr)
        {
            g_pDIKeyboardDevice -> Unacquire();
            g_pDIKeyboardDevice -> Release();
            g_pDIKeyboardDevice = nullptr;
        }

		// Release the Direct Input Object.
		g_pDI -> Release();
        g_pDI = nullptr;
    }
}

//-----------------------------------------------------------------------------
// Release (delete) all the resources used by this program.
VOID CleanUp()
{
    CleanUpDirectInput();

    if (g_pd3dDevice != nullptr)
	{
		g_pd3dDevice -> Release();
		g_pd3dDevice = nullptr;
	}

    if (g_pD3D != nullptr)	
	{
		g_pD3D -> Release();
		g_pD3D = nullptr;
	}

	delete[] mParticles;
}

//-----------------------------------------------------------------------------
// Set up the view and projection matrices.
void SetupViewMatrices()
{
	// Set up the view matrix.
	// This defines which way the 'camera' will look at, and which way is up.
    D3DXVECTOR3 vUpVector(0.0f, 1.0f, 0.0f);    
    D3DXMatrixLookAtRH( &matView, &vCamera, &vLookat, &vUpVector);     
	D3DXMatrixOrthoRH(&matProj, screenWidth, screenHeight, 5.0f, -5.0f);
}

//------------------------------------------------------------------------------
void UpdateParticleSystems()
{
	float dt = timer->getTimeElapsedFromStart();
	timer->reset();
	Update(dt);
	UpdateParticles(dt);
}

void RenderFire()
{	
	D3DXVECTOR3 centre(0.5f, 0.5f, 0.f);
	D3DXMATRIX I; D3DXMATRIX T;
	D3DXVECTOR3 zero = D3DXVECTOR3(0.f,0.f,0.f);
		
	gp_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);

	StructParticle* p = mParticles;	
	D3DXVECTOR3 pos(0.f,0.f,0.f);
	for (TIndex i = 0; i < FIRE_WORKS_LIMIT*particlesPerRocket; ++i, ++p)
	{				
		if (!p->mAlive)
			continue;

		pos.x = p->p.x;
		pos.y = screenHeight-p->p.y;			
		pos.z = p->p.z;
		gp_sprite->Draw(g_pDiffuseTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA((int)p->colour.x, (int)p->colour.y, (int)p->colour.z, (int)p->colour.w));
	}

	D3DXVECTOR3 pf;
	gp_sprite->End();

}

void RenderFireWorks()
{	
	
	gp_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);
	
	StructParticle* p = &(mParticles[FIRE_WORKS_LIMIT*particlesPerRocket]);	
	D3DXVECTOR3 pos(0.f,0.f,0.f);
	for (TIndex i = FIRE_WORKS_LIMIT*particlesPerRocket; i < MAX_PARTICLES; ++i, ++p)
	{				
			if (!p->mAlive)
				continue;

			pos.x = p->p.x;
			pos.y = screenHeight-p->p.y;			
			pos.z = 0.f;
			gp_sprite->Draw(g_pDiffuseTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA((int)p->colour.x, (int)p->colour.y, (int)p->colour.z, 255));
	}
	gp_sprite->End();
}

void RenderToTexture(void (*pFunc)(), LPDIRECT3DTEXTURE9 pTexture);
void ProcessGlow1( UINT nbPasses );
void ProcessGlow2( UINT nbPasses );

void SetEffectTexture(LPDIRECT3DTEXTURE9 pTexture, const char * textureName)
{
	D3DXHANDLE textureHandle = gGlowEffect->getEffect()->GetParameterByName(nullptr, textureName);
	HR(gGlowEffect->getEffect()->SetTexture( textureHandle, pTexture));
	HR(gGlowEffect->getEffect()->CommitChanges());
}

void SetUpEffect()
{
	D3DXHANDLE technique = gGlowEffect->getEffect()->GetTechniqueByName("Main");
	HR(gGlowEffect->getEffect()->SetTechnique(technique));
	HR(gGlowEffect->getEffect()->ValidateTechnique(technique));

	D3DXHANDLE viewportSize = gGlowEffect->getEffect()->GetParameterByName(nullptr, "ViewportSize");
	FLOAT viewPortSize[] = {screenWidth, screenHeight} ;
	HR(gGlowEffect->getEffect()->SetFloatArray( viewportSize, viewPortSize, 2));

	D3DXHANDLE viewportOffset = gGlowEffect->getEffect()->GetParameterByName(nullptr, "ViewportOffset");
	FLOAT viewPortOffset[] = {0.0, 0.0f} ;
	HR(gGlowEffect->getEffect()->SetFloatArray( viewportOffset, viewPortOffset, 2));

	HR(gGlowEffect->getEffect()->CommitChanges());
}

void PostProcessTexture(LPDIRECT3DTEXTURE9 pTextureOut, LPDIRECT3DTEXTURE9 pTextureIn, float sceneContribution, float glowRadius, float glowStrength)
{
	D3DXHANDLE technique = gGlowEffect->getEffect()->GetTechniqueByName("Main");
	HR(gGlowEffect->getEffect()->SetTechnique(technique));
	HR(gGlowEffect->getEffect()->ValidateTechnique(technique));

	SetEffectTexture(gpIntermediaryTexture, "gSceneTexture");

	{
		D3DXHANDLE handle = gGlowEffect->getEffect()->GetParameterByName(nullptr, "gSceneness");
		HR(gGlowEffect->getEffect()->SetFloat(handle, sceneContribution));
	}

	{
		D3DXHANDLE handle = gGlowEffect->getEffect()->GetParameterByName(nullptr, "gGlowness");
		HR(gGlowEffect->getEffect()->SetFloat(handle, glowStrength));
	}

	{
		D3DXHANDLE handle = gGlowEffect->getEffect()->GetParameterByName(nullptr, "gGlowSpan");
		HR(gGlowEffect->getEffect()->SetFloat(handle, glowRadius));
	}

	D3DXHANDLE viewportSize = gGlowEffect->getEffect()->GetParameterByName(nullptr, "ViewportSize");
	FLOAT viewPortSize[] = { screenWidth, screenHeight };
	HR(gGlowEffect->getEffect()->SetFloatArray(viewportSize, viewPortSize, 2));
	D3DXHANDLE viewportOffset = gGlowEffect->getEffect()->GetParameterByName(nullptr, "ViewportOffset");
	FLOAT viewPortOffset[] = { 0.0, 0.0f };
	HR(gGlowEffect->getEffect()->SetFloatArray(viewportOffset, viewPortOffset, 2));
	HR(gGlowEffect->getEffect()->CommitChanges());
	UINT nbPasses(0);
	ProcessGlow1(nbPasses);
	SetEffectTexture(gpGlowTexture1, "gGlowMap1");
	LPDIRECT3DSURFACE9 pSurface = nullptr;
	gpGlowTexture2->GetSurfaceLevel(0, &pSurface);
	HR(g_pd3dDevice->SetRenderTarget(0, pSurface));
	ProcessGlow2(nbPasses);
	SetEffectTexture(gpGlowTexture2, "gGlowMap2");
	// Glow frame
	pTextureOut->GetSurfaceLevel(0, &pSurface);
	HR(g_pd3dDevice->SetRenderTarget(0, pSurface))
	HR(g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));
	HR(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEXPP));
	HR(g_pd3dDevice->SetStreamSource(0, gpPostProcessingPlane, 0, sizeof(CUSTOMVERTEXPP)));
	HR(g_pd3dDevice->BeginScene());
	HR(gGlowEffect->getEffect()->Begin(&nbPasses, 0));
	// Glow 2
	HR(gGlowEffect->getEffect()->BeginPass(2));
	HR(g_pd3dDevice->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 2));
	HR(gGlowEffect->getEffect()->EndPass());
	HR(gGlowEffect->getEffect()->End());
	HR(g_pd3dDevice->EndScene());
}

// Uses Passes 1 and 2 of the glow effect.
void PostProcessWithGlow(LPDIRECT3DTEXTURE9 pTextureOut, void (*pRenderFunc)(), float sceneContribution, float glowRadius, float glowStrength)
{	
	RenderToTexture(pRenderFunc, gpIntermediaryTexture);
	PostProcessTexture(pTextureOut, gpIntermediaryTexture, sceneContribution, glowRadius, glowStrength);
	PostProcessTexture(gpIntermediaryTexture, pTextureOut, sceneContribution, glowRadius / 2.f, glowStrength / 2.f);
	PostProcessTexture(pTextureOut, gpIntermediaryTexture, sceneContribution, glowRadius / 4.f, glowStrength / 4.f);
}

void DisplayTexture(LPDIRECT3DTEXTURE9 pTexture);

void ApplyACESToneMapping()
{

}
void Render()
{	
	UpdateParticleSystems();

	PostProcessWithGlow(gpFireworksTexture, RenderFireWorks, 1.f, gFireWorksGlowRadius, gFireWorksGlowStrength);
	PostProcessWithGlow(gpFireTexture, RenderFire, 0.f, gFireGlowRadius, gFireGlowStrength);
	HR(g_pd3dDevice->SetRenderTarget(0, gpPrimaryRenderTarget));

	SetupViewMatrices();
	// Clear the backbuffer to a black color
	HR(g_pd3dDevice -> Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));

	HR(g_pd3dDevice->BeginScene());

	HR(gp_sprite->Begin(D3DXSPRITE_ALPHABLEND));
	D3DXVECTOR3 pos;
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
	HR(gp_sprite->Draw(bgForeground, nullptr, nullptr, &pos, D3DCOLOR_RGBA(255, 255, 255, 255)));	
	HR(gp_sprite->Draw(gpFireworksTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA(255, 255, 255, 255)));
	HR(gp_sprite->Draw(gpFireTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA(255, 255, 255, 255)));
	HR(gp_sprite->End());
	HR(g_pd3dDevice->EndScene());

    // Present the backbuffer to the display.
    g_pd3dDevice -> Present(nullptr, nullptr, nullptr, nullptr);
}

//------------------------------------------------------------------------------
// Initialise Direct Input (& terminate initialisation if an error is detected.)
BOOL WINAPI SetupDirectInput(HINSTANCE g_hinst, HWND g_hwndMain)
{
    // Create the DirectInput object.
    HRESULT hr = DirectInput8Create(g_hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDI, nullptr);
    if FAILED(hr)
		return FALSE;
	
	// Initialise the keyboard...
    // Retrieve a pointer to an IDirectInputDevice8 interface.
    hr = g_pDI -> CreateDevice(GUID_SysKeyboard, &g_pDIKeyboardDevice, nullptr);
    if FAILED(hr)
    {
        CleanUpDirectInput();
        return FALSE;
    }

    // Set the data format using the predefined mouse data format.
    hr = g_pDIKeyboardDevice -> SetDataFormat(&c_dfDIKeyboard);
    if FAILED(hr)
    {
        CleanUpDirectInput(); // Terminate Direct Input initialisation if an error is detected.
        return FALSE;
    }

    // Set the cooperative level
    hr = g_pDIKeyboardDevice -> SetCooperativeLevel(g_hwndMain,
													DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if FAILED(hr)
    {
        CleanUpDirectInput();
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
// Process mouse inputs.
void WINAPI ProcessMouseInput()
{
    DIMOUSESTATE2 MouseState;      // DirectInput mouse state structure

    if (g_pDIMouseDevice != nullptr)
	{
		// Get the input's device state and store it in 'MouseState'
	    ZeroMemory(&MouseState, sizeof(MouseState));
		HRESULT hr;
		hr = g_pDIMouseDevice -> GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState);
		if (FAILED(hr))
		{
			// DirectInput may be telling us that the input stream has been
			// interrupted.  We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done.
			// We just re-acquire and try again.

			// If input is lost, then try to acquire and keep trying until acquired...
			hr = g_pDIMouseDevice -> Acquire();
			while (hr == DIERR_INPUTLOST)
			{
				hr = g_pDIMouseDevice -> Acquire();
			}
			hr = g_pDIMouseDevice -> GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState);
		}	
	}
}

//------------------------------------------------------------------------------------------
// Process keyboard inputs...
void WINAPI ProcessKeyboardInput()
{
	static bool tabPressed(false),pg_down_pressed(false),pg_up_pressed(false),backspacePressed(false),deletePressed(false);

	// Define a macro to represent the key detection predicate.
	#define KEYDOWN(name, key) (name[key] & 0x80)	
	
 	// Create a buffer (memory space) to contain the key press data from the keyboard.
	char     buffer[256];
	HRESULT  hr;

    // Clear the keyboard data buffer - just in case.
	ZeroMemory(buffer, 256);

    // What is the current state of the keyboard?
	hr = g_pDIKeyboardDevice -> GetDeviceState(sizeof(buffer),(LPVOID)&buffer);
	if FAILED(hr)
	{
		// If this failed, the device has probably been lost.
		// Check for (hr == DIERR_INPUTLOST) and attempt to reacquire it here.
		hr = g_pDIKeyboardDevice -> Acquire();
        while (hr == DIERR_INPUTLOST)
		{
			hr = g_pDIKeyboardDevice -> Acquire();
		}
		hr = g_pDIKeyboardDevice -> GetDeviceState(sizeof(buffer),(LPVOID)&buffer);
	}	
}


//-----------------------------------------------------------------------------
// The window's message handling function.
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
		case WM_MOUSEMOVE:
		{
			mousex = (float)LOWORD( lParam );
			mousey = screenHeight - (float)HIWORD( lParam );
			break;
		}
		case WM_SIZE:
		{					
			screenWidth = (float)LOWORD( lParam );
			screenHeight = (float)HIWORD( lParam );
			break;
		}
        case WM_DESTROY:
		{
            // Respond to a Windows destroy event.
			// Usually generated by clicking on the close box on the window.
            PostQuitMessage(0);
            return 0;
		}
		case WM_KEYDOWN:
		{
			const float delta = 0.1f;
			if (wParam == 0x51)
			{
				gFireGlowRadius+=delta;
			}
			else if (wParam == 0x41)
			{
				gFireGlowRadius-=delta;
			}
			else if (wParam == 0x57)
			{
				gFireGlowStrength += delta;
			}
			else if (wParam == 0x53)
			{
				gFireGlowStrength -= delta;
			}

			if (wParam == 0x59)
			{
				gFireWorksGlowRadius+=delta;
			}
			else if (wParam == 0x48)
			{
				gFireWorksGlowRadius-=delta;
			}
			else if (wParam == 0x55)
			{
				gFireWorksGlowStrength += delta;
			}
			else if (wParam == 0x4A)
			{
				gFireWorksGlowStrength -= delta;
			}

			if (wParam == VK_SPACE)
			{
				TRACE( "Fireworks strength (%f) radius (%f) \n\r Fire strength (%f) radius (%f)", gFireWorksGlowStrength, gFireWorksGlowRadius, gFireGlowStrength, gFireWorksGlowRadius);
			}
		}
	}

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// WinMain() - The application's entry point.
// This sort of procedure is mostly standard, and could be used in most DirectX applications.
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	mParticles = new StructParticle[MAX_PARTICLES];
	Init();
	timer = new CHighPerformanceTimer(0);

	// Register the window class.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MsgProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = sizeof(LONG_PTR);
	wc.hInstance     = hInst;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName  =  nullptr;
	wc.hCursor       = nullptr;
	wc.lpszClassName = "Fireworks";
	
    RegisterClassEx(&wc);
	HWND hWnd = CreateWindow( "Fireworks", "Fireworks", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 999, 545, nullptr, nullptr, hInst, nullptr);
    
	D3DXMatrixIdentity(&orientation);
	hwnd = hWnd;
	inst = hInst;
	SetupViewMatrices();
	SetupRockets();
	
    if (SUCCEEDED(SetupD3D(hWnd)))
    {        
		// Initialise Direct Input and acquire the keyboard
		if (SUCCEEDED(SetupDirectInput(hInst, hWnd)))
		{				
			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
						
					Render();
					ProcessKeyboardInput();
					ProcessMouseInput();
				}
			}            
        }
    }
	
	CleanUp();
    UnregisterClass("Fireworks", wc.hInstance);
    return 0;
}

void RenderToTexture(void (*pRenderFunc)(), LPDIRECT3DTEXTURE9 pTexture)
{	
	LPDIRECT3DSURFACE9 pSurface = nullptr;
	pTexture->GetSurfaceLevel(0, &pSurface);
	HR(g_pd3dDevice->SetRenderTarget(0, pSurface));

	// Begin the scene
	HR(g_pd3dDevice -> Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0));

	if (SUCCEEDED(g_pd3dDevice -> BeginScene()))
	{
		pRenderFunc();
		// End the scene.		
		HR(g_pd3dDevice -> EndScene());
	}

	HR(g_pd3dDevice->SetRenderTarget(0, gpPrimaryRenderTarget));
}

// Glow 1
void ProcessGlow1( UINT nbPasses )
{
	HR(g_pd3dDevice->SetRenderTarget(0, gpTextureSurfaceGlow1));
	HR(g_pd3dDevice -> Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0));
	HR(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEXPP));
	HR(g_pd3dDevice->SetStreamSource(0, gpPostProcessingPlane, 0, sizeof(CUSTOMVERTEXPP)));
	HR(g_pd3dDevice->BeginScene());
	HR(gGlowEffect->getEffect()->Begin(&nbPasses,0));
	HR(gGlowEffect->getEffect()->BeginPass(0));
	HR(g_pd3dDevice->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 2));
	HR(gGlowEffect->getEffect()->EndPass());
	HR(gGlowEffect->getEffect()->End());
	HR(g_pd3dDevice->EndScene());
}

void ProcessGlow2( UINT nbPasses )
{	
	//// Glow frame
	HR(g_pd3dDevice->SetRenderTarget(0, gpTextureSurfaceGlow2));
	HR(g_pd3dDevice -> Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0));
	HR(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEXPP));
	HR(g_pd3dDevice->SetStreamSource(0, gpPostProcessingPlane, 0, sizeof(CUSTOMVERTEXPP)));
	HR(g_pd3dDevice->BeginScene());
	HR(gGlowEffect->getEffect()->Begin(&nbPasses, 0));
	// Glow 2
	HR(gGlowEffect->getEffect()->BeginPass(1));
	HR(g_pd3dDevice->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 2));
	HR(gGlowEffect->getEffect()->EndPass());
	HR(gGlowEffect->getEffect()->End());
	HR(g_pd3dDevice->EndScene());
}

// Pass 4
void ComposeTextures(  LPDIRECT3DTEXTURE9 pTextureA, LPDIRECT3DTEXTURE9 pTextureB, LPDIRECT3DTEXTURE9 pTextureOut)
{
	// Composition.
	LPDIRECT3DSURFACE9 pSurface = nullptr;
	pTextureOut->GetSurfaceLevel(0, &pSurface);
	HR(g_pd3dDevice->SetRenderTarget(0, pSurface));

	SetEffectTexture(pTextureA, "gBackgroundTexture");
	SetEffectTexture(pTextureB, "gForegroundTexture");
	UINT nbPasses = 0;
	g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	HR(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEXPP));
	HR(g_pd3dDevice->SetStreamSource(0, gpPostProcessingPlane, 0, sizeof(CUSTOMVERTEXPP)));

	// Combine Glow and fireworks.
	HR(g_pd3dDevice->BeginScene());
	HR(gGlowEffect->getEffect()->Begin(&nbPasses, 0));
	HR(gGlowEffect->getEffect()->BeginPass(3));
	g_pd3dDevice->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 2);
	HR(gGlowEffect->getEffect()->EndPass());
	HR(gGlowEffect->getEffect()->End());
	HR(g_pd3dDevice -> EndScene());
}

void DisplayTexture(LPDIRECT3DTEXTURE9 pTexture)
{	
	HR(g_pd3dDevice->SetRenderTarget(0, gpPrimaryRenderTarget));
	SetupViewMatrices();
	// Clear the backbuffer to a black color
	HR(g_pd3dDevice -> Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0));
	HR(g_pd3dDevice->BeginScene());
	HR(gp_sprite->Begin(D3DXSPRITE_ALPHABLEND));
	D3DXVECTOR3 pos;
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
	HR(gp_sprite->Draw(pTexture, nullptr, nullptr, &pos, D3DCOLOR_RGBA(255, 255, 255, 255)));	
	HR(gp_sprite->End());
	HR(g_pd3dDevice->EndScene());
}
