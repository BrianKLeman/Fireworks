#pragma once
#include <math.h>
#include "Transform.h"

class Camera
{
private:
	float mFov = 45.f;
	float mzNear = 5.f;
	float mzFar = -5.f;
	float mWidth = 800.f;
	float mHeight = 600.f;
	float mAspect = mWidth / mHeight;
	Transform mT;
	Transform mLookAt;
	bool mIsOrthographic = false;
public:
	Camera(Transform& t)
		: mT(t), mLookAt(D3DXVECTOR3(0.f,0.f,0.f))
	{

	}

    Camera(Transform& t,float nearClip, float farClip, float width, float height)
		: mT(t), mzFar(far), mzNear(nearClip), mWidth(width), mHeight(height)
	{

	}

	float GetFieldOfView() const
	{
		return mFov;
	}

	void SetFieldOfView(float angle)
	{
		//mFov = (float)fmax(fmin(180.f, angle), 0.f); //clamp to range 10f to 180f		
	}
	
	void LookAt(Transform& t)
	{	
		mT = t;
	}

	D3DXMATRIX GetProjectionMatrix()
	{
		D3DXMATRIX projection;
		if (mIsOrthographic == false)
			D3DXMatrixPerspectiveFovRH(&projection, mFov, mAspect, mzNear, mzFar);
		else
			D3DXMatrixOrthoRH(&projection, mWidth, mHeight, mzNear, mzFar);

		return projection;
	}

	D3DXMATRIX GetViewMatrix()
	{
		D3DXMATRIX view;

		D3DXMatrixLookAtRH(&view, &(mT.mPosition), &(mLookAt.mPosition), new D3DXVECTOR3(0.f, 1.f, 0.f));
		return view;
	}

};

