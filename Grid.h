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

class Grid
{
public:
	Grid(LPDIRECT3DDEVICE9 device)
		: mpDevice(device)
	{
	
	}

private:
	LPDIRECT3DDEVICE9 mpDevice = nullptr;
};

