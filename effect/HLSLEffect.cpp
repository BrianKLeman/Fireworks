#include "HLSLEffect.h"


CHLSLEffect::CHLSLEffect(LPDIRECT3DDEVICE9 device)
{
	l_g_pd3dDevice = device;
	ppCompilationErrors = new LPD3DXBUFFER;
}


CHLSLEffect::~CHLSLEffect(void)
{
}

bool CHLSLEffect::SetParameter(const char* param_name,void* param,ENUM_PARAM_TYPE P_TYPE)
{
	if(myEffect == nullptr) return false; //FAILED if effect has not been set up

	D3DXHANDLE handle = myEffect->GetParameterByName(NULL,param_name);
	if(handle == nullptr) return false;  //FAILED TO GET HANDLE

	switch(P_TYPE)
	{
		
		case P_MATRIX: return SUCCEEDED(myEffect->SetMatrix		(handle,		  	(D3DXMATRIX*)param)); break;
		case P_VECTOR4:return SUCCEEDED(myEffect->SetVector		(handle,			(D3DXVECTOR4*)param)); break;
		case P_FLOAT:  return SUCCEEDED(myEffect->SetFloat		(handle,    			*(FLOAT*)param)); break;
		case P_INT:    return SUCCEEDED(myEffect->SetInt		(handle,	  			  *(INT*)param)); break;
		case P_TEXTURE:return SUCCEEDED(myEffect->SetTexture	(handle, reinterpret_cast<IDirect3DTexture9*>(param))); break;
		default: return false; //error
	}
	
	return false;
}

bool CHLSLEffect::createEffect(const char * filename)
{
	if(filename == nullptr || l_g_pd3dDevice == nullptr) return false;
	
	return SUCCEEDED(D3DXCreateEffectFromFile(l_g_pd3dDevice,filename,NULL,NULL,NULL ,NULL,&myEffect,ppCompilationErrors));
}

ID3DXEffect* CHLSLEffect::getEffect()
{	
	return myEffect;
}

bool CHLSLEffect::setEffectTechnique(void)
{
	HRESULT result;
	D3DXHANDLE toEffectTechnique;
	result = myEffect->FindNextValidTechnique(NULL,&toEffectTechnique);
	if(result == D3D_OK)
	{
		result = myEffect->SetTechnique(toEffectTechnique);
	} 
	return SUCCEEDED(result);
}

const LPD3DXBUFFER * CHLSLEffect::GetErrors()
{
	return ppCompilationErrors;
}
