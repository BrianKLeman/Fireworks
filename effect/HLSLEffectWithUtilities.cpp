#include "HLSLEffectWithUtilities.h"


CHLSLEffectWithUtilities::CHLSLEffectWithUtilities(LPDIRECT3DDEVICE9 device)
	:CHLSLEffect(device)
{
}


CHLSLEffectWithUtilities::~CHLSLEffectWithUtilities(void)
{
}

bool CHLSLEffectWithUtilities::RenderWithEffect(vertex_info vertices_structure)
{
		// Render the contents of the vertex buffer.
		l_g_pd3dDevice -> SetFVF(D3DFVF_CUSTOMVERTEX2);
		l_g_pd3dDevice -> SetStreamSource(0, vertices_structure.mpVertexBuffer, 0, sizeof(CUSTOMVERTEX));

		UINT nbPasses(0);

		HRESULT result = myEffect->Begin(&nbPasses,0);
		for(unsigned int i = 0; i < nbPasses; i++)
		{
			myEffect->BeginPass(i);
			l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertices_structure.nbTriangles);
			myEffect->EndPass();
		}

		myEffect->End();		
		return SUCCEEDED(result);
}