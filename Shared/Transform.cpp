#include "Transform.h"

Transform::Transform( D3DXVECTOR3 position)
: 
	mPosition(position)
{	
	D3DXMatrixIdentity(&mOrientation);
}

Transform::Transform( void )
{
	D3DXMatrixIdentity(&mOrientation);
}


Transform::~Transform(void)
{	
}
