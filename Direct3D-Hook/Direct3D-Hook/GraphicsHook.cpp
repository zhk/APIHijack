// Direct3D-Hook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "GraphicsHook.h"
#include "apihijack.h"

#include "enums.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HINSTANCE hDLL;

void MessageBox_Debug(LPCSTR str)
{
#if _DEBUG
	MessageBox(0, str, "Direct3D Hook", MB_OK | MB_ICONINFORMATION);
#endif
}

//-------------------------------------------------------------------------
// DXUT uses GetProcAddress(), so hook it.
typedef FARPROC (WINAPI *GetProcAddress_t)(HMODULE hModule,LPCSTR lpProcName);
FARPROC WINAPI MyGetProcAddress(HMODULE hModule,LPCSTR lpProcName);
SDLLHook Kernel32_Hook = 
{
	"Kernel32.dll",
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "GetProcAddress", MyGetProcAddress},
		{ NULL, NULL }
	}
};

extern SDLLHook D3D9_Hook;
extern SDLLHook D3D10_Hook;
extern SDLLHook D3D11_Hook;
extern SDLLHook OpenGL_Hook;

FARPROC WINAPI MyGetProcAddress(HMODULE hModule,LPCSTR lpProcName)
{
	GetProcAddress_t old_func = (GetProcAddress_t) Kernel32_Hook.Functions[0].OrigFn;

	if(_strnicmp(lpProcName, "Direct3DCreate9", strlen(lpProcName)) == 0)
	{
		// d3d9
		static HMODULE hModD3D9 = LoadLibrary( "d3d9.dll" );
		if(hModD3D9)
			D3D9_Hook.Functions[D3DFN_Direct3DCreate9].OrigFn = old_func(hModD3D9, lpProcName);

		return (FARPROC)(D3D9_Hook.Functions[D3DFN_Direct3DCreate9].HookFn);
	}
	//-------------------------------------------------------------------------------
	else if(_strnicmp(lpProcName, "D3D10CreateDevice", strlen(lpProcName)) == 0)
	{
		// d3d10
		static HMODULE hModD3D10 = LoadLibrary( "d3d10.dll" );
		if(hModD3D10)
			D3D10_Hook.Functions[D3DFN_D3D10CreateDevice].OrigFn = old_func(hModD3D10, lpProcName);

		return (FARPROC)(D3D10_Hook.Functions[D3DFN_D3D10CreateDevice].HookFn);
	}
	else if(_strnicmp(lpProcName, "D3D10CreateDeviceAndSwapChain", strlen(lpProcName)) == 0)
	{
		// d3d10
		static HMODULE hModD3D10 = LoadLibrary( "d3d10.dll" );
		if(hModD3D10)
			D3D10_Hook.Functions[D3DFN_D3D10CreateDeviceAndSwapChain].OrigFn = old_func(hModD3D10, lpProcName);

		return (FARPROC)(D3D10_Hook.Functions[D3DFN_D3D10CreateDeviceAndSwapChain].HookFn);
	}
	//-------------------------------------------------------------------------------
	else if(_strnicmp(lpProcName, "D3D10CreateDevice1", strlen(lpProcName)) == 0)
	{
		// d3d10.1
		//static HMODULE hModD3D101 = LoadLibrary( "d3d10_1.dll" );
		MessageBox_Debug("GetProcAddress: D3D10CreateDevice1");
	}
	else if(_strnicmp(lpProcName, "D3D10CreateDeviceAndSwapChain1", strlen(lpProcName)) == 0)
	{
		// d3d10.1
		//static HMODULE hModD3D101 = LoadLibrary( "d3d10_1.dll" );
		MessageBox_Debug("GetProcAddress: D3D10CreateDeviceAndSwapChain1");
	}
	//-------------------------------------------------------------------------------
	else if(_strnicmp(lpProcName, "D3D11CreateDevice", strlen(lpProcName)) == 0)
	{
		// d3d11
		//static HMODULE hModD3D11 = LoadLibrary( "d3d11.dll" );
		MessageBox_Debug("GetProcAddress: D3D11CreateDevice");
	}
	else if(_strnicmp(lpProcName, "D3D11CreateDeviceAndSwapChain", strlen(lpProcName)) == 0)
	{
		// d3d11
		//static HMODULE hModD3D11 = LoadLibrary( "d3d11.dll" );
		MessageBox_Debug("GetProcAddress: D3D11CreateDeviceAndSwapChain");
	}
	//-------------------------------------------------------------------------------
	else if(_strnicmp(lpProcName, "glBegin", strlen(lpProcName)) == 0)
	{
		// opengl32
		//static HMODULE hModD3D11 = LoadLibrary( "opengl32.dll" );
		MessageBox_Debug("GetProcAddress: glBegin");
	}

	return old_func(hModule, lpProcName);
}

//-------------------------------------------------------------------------

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)  // When initializing....
	{
		hDLL = hModule;

		// We don't need thread notifications for what we're doing.  Thus, get
		// rid of them, thereby eliminating some of the overhead of this DLL
		DisableThreadLibraryCalls( hModule );
		
		char targetProcess[512];
		DWORD count = 512;

		HKEY appKey = 0;
		if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Direct3D-Hook", 0, KEY_QUERY_VALUE,
			&appKey))
		{
			if(ERROR_SUCCESS == RegQueryValueEx(appKey, 0, 0, 0, reinterpret_cast<BYTE*>(targetProcess), &count))
			{
				char process[512];
				GetModuleFileName(GetModuleHandle(0), process, sizeof(process));
				PathStripPath(process);
				
				if(_strnicmp(targetProcess, process, 512) == 0)
				{
					HookAPICalls(&D3D9_Hook);
					HookAPICalls(&D3D10_Hook);
					HookAPICalls(&D3D11_Hook);

					HookAPICalls(&OpenGL_Hook);

					HookAPICalls(&Kernel32_Hook);
				}
				else
				{
					//MessageBox_Debug(process);
				}
			}

			RegCloseKey(appKey);
		}
	}

	return TRUE;
}

// This segment must be defined as SHARED in the .DEF
#pragma data_seg (".HookSection")		
// Shared instance for all processes.
HHOOK hHook = NULL;	
#pragma data_seg ()

// This hook does nothing, but let the dll attached to later process
D3DHOOK_API LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
    return CallNextHookEx( hHook, nCode, wParam, lParam); 
}

D3DHOOK_API void InstallHook()
{
    OutputDebugString( "D3DHOOK hook installed.\n" );
    hHook = SetWindowsHookEx( WH_CBT, HookProc, hDLL, 0 ); 
}

D3DHOOK_API void RemoveHook()
{
    OutputDebugString( "D3DHOOK hook removed.\n" );
    UnhookWindowsHookEx( hHook );
}


#ifdef _MANAGED
#pragma managed(pop)
#endif

