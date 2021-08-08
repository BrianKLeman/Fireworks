#pragma once
//-----------------------------------------------------------------------------
// RUBIKS CUBE AUTHOR: BRIAN LEMAN, SID 16024894 D.O.B. 22/01/1985 START DATE: 30/09/2009

#include <d3dx9math.h>

class Transform
{
public:
	Transform(void);
	Transform( D3DXVECTOR3 position);
	~Transform(void);

	D3DXMATRIX 	mOrientation; 
	D3DXVECTOR3 mPosition;
};

