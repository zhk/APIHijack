// Direct3D-Hook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "apihijack.h"

#include <d3d9.h>
#include "MyDirect3D9.h"
#include "enums.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//-------------------------------------------------------------------------
// D3D9
// Function pointer types.
typedef IDirect3D9* (WINAPI *Direct3DCreate9_t)(UINT sdk_version);

// Function prototype.
IDirect3D9* WINAPI MyDirect3DCreate9(UINT sdk_version);

// Hook structure.
SDLLHook D3D9_Hook = 
{
	"D3D9.DLL",
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "Direct3DCreate9", MyDirect3DCreate9},
		{ NULL, NULL }
	}
};

IDirect3D9* WINAPI MyDirect3DCreate9(UINT sdk_version)
{
	// Let the world know we're working.
	//MessageBeep(MB_ICONINFORMATION);
	MessageBox_Debug("MyDirect3DCreate9 called");
	OutputDebugString( "Direct3D-Hook: MyDirect3DCreate9 called.\n" );

	Direct3DCreate9_t old_func = (Direct3DCreate9_t) D3D9_Hook.Functions[D3DFN_Direct3DCreate9].OrigFn;
	IDirect3D9* d3d = old_func(sdk_version);

	return d3d? new MyDirect3D9(d3d) : 0;
}


#ifdef _MANAGED
#pragma managed(pop)
#endif

