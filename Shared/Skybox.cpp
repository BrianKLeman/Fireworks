
#include "SkyBox.h"
#include "Camera.h"

SkyBox::SkyBox(Camera camera):
scaler(1.f),vPos(0.0,0.0,-5.0f),l_g_pd3dDevice(0),
mpVertexBuffer(0), mCamera(camera)
{
	D3DXMATRIX tran;
	D3DXMatrixIdentity(&tran);
	D3DXMatrixTranslation(&tran, vPos.x, vPos.y, vPos.z);
	D3DXMATRIX scale;
	D3DXMatrixIdentity(&scale);
	D3DXMatrixScaling(&scale,scaler,scaler,scaler);
	D3DXMatrixMultiply(&tranScale,&scale,&tran );

	ppCompilationErrors = new LPD3DXBUFFER;
}


//-----------------------------------------------------------------------------------------
//methods
//-----------------------------------------------------------------------------------------
void SkyBox::init(LPDIRECT3DDEVICE9 device)
{
	l_g_pd3dDevice = device;

	//D3DXLoadMeshFromX("./textures/cube.x", D3DXMESH_MANAGED, l_g_pd3dDevice, nullptr, nullptr, nullptr, nullptr, &m_mesh);
	D3DXCreateSphere(l_g_pd3dDevice, 9.8f, 20, 20, &m_mesh, nullptr);
	m_mesh->GetVertexBuffer(&mpVertexBuffer);
	m_mesh->GetIndexBuffer(&mpIndexBuffer);

	D3DXCreateCubeTextureFromFile(l_g_pd3dDevice, "./textures/nightsky.dds", &m_cubeTexture);
	setupEffect();
	

}

HRESULT SkyBox::RenderWithEffect()
{
		
		//create worldviewproj matrix
	    mCamera.SetFieldOfView(45.f);
		D3DXMATRIX view = mCamera.GetViewMatrix();
		D3DXMATRIX proj =  mCamera.GetProjectionMatrix();

		//create modelworld matrix
		D3DXMATRIX object, objectWorld;
		D3DXMatrixIdentity(&object); D3DXMatrixIdentity(&objectWorld);

		D3DXMATRIX objectWorldView;
		D3DXMatrixMultiply( &objectWorldView, &objectWorld, &view	);
		SetObjectWorldViewTransfrom(objectWorldView);
		this->l_g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &proj);
		//create ModelWorldViewProj
		D3DXMATRIX  WorldViewProjectionMatrix;	
		D3DXMatrixMultiply( &WorldViewProjectionMatrix, &objectWorldView, &proj);
		
		setModelWorldViewProjMatrix(WorldViewProjectionMatrix);		

	   	// Render the contents of the vertex buffer.
		l_g_pd3dDevice -> SetFVF(m_mesh->GetFVF());		
	    l_g_pd3dDevice -> SetStreamSource(0, mpVertexBuffer, 0, m_mesh->GetNumBytesPerVertex());
		l_g_pd3dDevice->SetIndices(mpIndexBuffer);
		l_g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		UINT nbPasses(0);	
		SetBackground();
		myEffect->CommitChanges();
		auto result = myEffect->Begin(&nbPasses,0);

		for(unsigned int i = 0; i < nbPasses; i++)
		{
			myEffect->BeginPass(i);

	        l_g_pd3dDevice -> DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_mesh->GetNumVertices(),0, m_mesh->GetNumFaces());			
			
			myEffect->EndPass();
		}

		myEffect->End();
		
		return result;
}

// to create the special effect for the rubiks cube
HRESULT SkyBox::createEffect(void)
{	
	return D3DXCreateEffectFromFile(l_g_pd3dDevice,"effect/CubeMap.fx",NULL,NULL,NULL ,NULL,&myEffect,ppCompilationErrors);
}

// to create the special effect for the rubiks cube
HRESULT SkyBox::setEffectTechnique(void)
{
	HRESULT result;
	D3DXHANDLE toEffectTechnique;
	result = myEffect->FindNextValidTechnique(NULL,&toEffectTechnique);
	if(result == D3D_OK)
	{
		result = myEffect->SetTechnique(toEffectTechnique);		
	} 

	return result;
}

// to create the special effect for the rubiks cube
void SkyBox::getHandlesToParameters(void)
{	
	ObjWorldViewProjMatrixHandleOfEffect = myEffect->GetParameterByName(NULL,"ObjWorldViewProj");
	if(ObjWorldViewProjMatrixHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	objWorldViewMatrixHandleOfEffect = myEffect->GetParameterByName(NULL,"ObjWorldView");
	if(WorldViewProjMatrixHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	m_backgroundHandle = myEffect->GetParameterByName(NULL, "background");
	if (m_backgroundHandle == NULL) MessageBox(NULL, "failed to get handle to background param", "effect failed", MB_OK);

}


HRESULT SkyBox::setupEffect()
{
	HRESULT result;

	result = createEffect();
	
	if(result == D3D_OK)
	{
		getHandlesToParameters();	
		return setEffectTechnique();		
	}
	return result;
}

void SkyBox::setModelWorldViewProjMatrix(D3DXMATRIX matrix)
{
	if(myEffect->SetMatrix(ObjWorldViewProjMatrixHandleOfEffect,&matrix) != D3D_OK) MessageBox(NULL,"SET MATRIX FAILED","MATRIX",MB_OK);
}

void SkyBox::SetObjectWorldViewTransfrom( D3DXMATRIX matIn )
{
	if(myEffect->SetMatrix(objWorldViewMatrixHandleOfEffect,&matIn)!=D3D_OK) MessageBox(NULL,"SET ORIENTATION MATRIX FAILED","MATRIX",MB_OK) ;
}

void SkyBox::SetBackground()
{
	if (myEffect->SetTexture(m_backgroundHandle, m_cubeTexture) != D3D_OK) MessageBox(NULL, "Set cube map failed", "CubeMap", MB_OK);
}

SkyBox::~SkyBox()
{
	m_cubeTexture->Release(); m_cubeTexture = nullptr;

	// Release the other resources...
	if (mpVertexBuffer != nullptr) mpVertexBuffer->Release();

	mpVertexBuffer = nullptr;
	m_mesh->Release(); m_mesh = nullptr;
	myEffect->Release(); myEffect = nullptr;

}