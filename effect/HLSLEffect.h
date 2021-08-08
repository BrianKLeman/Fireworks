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
// Define a macro to represent the key detection predicate.
#define KEYDOWN(name, key) (name[key] & 0x80)	

class CHLSLEffect
{
	public:
		CHLSLEffect(LPDIRECT3DDEVICE9 device);
		~CHLSLEffect(void);
		enum ENUM_PARAM_TYPE { P_MATRIX,P_VECTOR4, P_FLOAT,P_INT,P_TEXTURE };
		bool SetParameter(const char* param_name,void* param,ENUM_PARAM_TYPE P_TYPE);
		bool createEffect(const char * filename);
		ID3DXEffect* getEffect();
		bool setEffectTechnique(void);
		
		const LPD3DXBUFFER* GetErrors();
	protected:
		CHLSLEffect() {}; // Hide the default constructor.
		LPDIRECT3DDEVICE9       l_g_pd3dDevice;
		ID3DXEffect*			myEffect;				
		LPD3DXBUFFER* 			ppCompilationErrors;
};

