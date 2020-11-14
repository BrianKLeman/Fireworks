#pragma once


#include <d3dx9.h>

class CHLSLEffect
{
	public:
		CHLSLEffect(LPDIRECT3DDEVICE9 device);
		~CHLSLEffect(void);
		enum ENUM_PARAM_TYPE { P_MATRIX,P_VECTOR4, P_FLOAT,P_INT,P_TEXTURE };
		bool setParameter(const char* param_name,void* param,ENUM_PARAM_TYPE P_TYPE);
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

