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

class D3DWindow
{
	
protected:
	HINSTANCE mInst; //window instance
	HINSTANCE mpInst;
	HWND mHwnd;


protected:
	LPDIRECT3D9					g_pD3D = NULL; // The D3D object
	LPDIRECT3DDEVICE9      mpD3DDevice = NULL; // The rendering device
	LPDIRECTINPUT8			g_pDI = NULL; // The Direct Input object
	LPDIRECTINPUTDEVICE8	g_pDIMouseDevice = NULL; // The Direct Input mouse device.
	LPDIRECTINPUTDEVICE8 mpKeyboardDevice = NULL; // The Direct Input keyboard device.
	FLOAT mScreenWidth = 800.f, mScreenHeight = 600.f;
	FLOAT mMouseX, mMouseY;
	LPDIRECT3DSURFACE9 gpPrimaryRenderTarget;
	virtual void OnCleanUp() {}
	virtual void OnProcessKeyboardInput(char* buffer) {};
	virtual void OnUpdate() {};
public:


	D3DWindow(HINSTANCE hInst, HINSTANCE pInstance);
	// Function Prototypes
	void Show()
	{
		ShowWindow(mHwnd, SW_SHOWDEFAULT);
		UpdateWindow(mHwnd);
	}
	void WINAPI CleanUpDirectInput();
	void CleanUp();
	HRESULT SetupD3D();
	virtual bool OnSetupD3D() { return true; };
	void Render();
	virtual void OnRender() {};

	BOOL WINAPI SetupDirectInput();
	void WINAPI ProcessKeyboardInput();
	void Update() { OnUpdate(); }
	static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

