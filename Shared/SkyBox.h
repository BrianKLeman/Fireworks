//check the class has not already been declared
#ifndef BOX_HEADER
#define BOX_HEADER

//include directx header file for vector operations
#define DIRECTINPUT_VERSION 0x0800
#define D3D_DEBUG_INFO	// Enable debugging information, so the .NET environment can help you.

//-----------------------------------------------------------------------------
// Include these files
#include <windows.h>	// Windows library (for window functions, menus, dialog boxes, etc)
#include <d3dx9.h>		// Direct 3D library (for all Direct 3D functions)
#include <math.h>		//for sin, cos, tan and sqrt
#include "Camera.h"

class SkyBox
{
public:
	SkyBox(Camera camera);
	~SkyBox();

	void init(LPDIRECT3DDEVICE9 device);
	
	void SetObjectWorldViewTransfrom( D3DXMATRIX matIn);
	void SetBackground();
	// to create the special effect for the rubiks cube
	HRESULT createEffect(void);

	//set the parameters
	void getHandlesToParameters();

	//set the modelviewproj matrix
	void setModelWorldViewProjMatrix(D3DXMATRIX matrix);	

	//set effect technique
	HRESULT SkyBox::setEffectTechnique(void);
	
	HRESULT setupEffect();
	//render geometry with effect
	HRESULT RenderWithEffect();
private:
	Camera mCamera;
	D3DXMATRIX tranScale;
	float scaler;
	D3DXVECTOR3 vPos;
	LPD3DXMESH m_mesh;
	LPDIRECT3DDEVICE9       l_g_pd3dDevice; // The rendering device
	LPDIRECT3DVERTEXBUFFER9 mpVertexBuffer; // Buffers to hold faces
	LPDIRECT3DINDEXBUFFER9  mpIndexBuffer;
	LPDIRECT3DCUBETEXTURE9 m_cubeTexture;
	ID3DXEffect* myEffect;				//the shader for the cube
	LPD3DXBUFFER* ppCompilationErrors; 	//A buffer to hold the compilation errors
	D3DXHANDLE				ObjWorldViewProjMatrixHandleOfEffect;
	D3DXHANDLE				WorldViewProjMatrixHandleOfEffect;
	D3DXHANDLE				objWorldViewMatrixHandleOfEffect;
	D3DXHANDLE				m_backgroundHandle;
};
#endif