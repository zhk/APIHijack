// Direct3D-Hook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "apihijack.h"

#include <d3d10.h>
#include <d3dx10.h>
#include "enums.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//-------------------------------------------------------------------------
// D3D10

// Function pointer types.
typedef HRESULT (WINAPI *D3D10CreateDevice_t)(IDXGIAdapter *pAdapter
							  , D3D10_DRIVER_TYPE DriverType
							  , HMODULE Software
							  , UINT Flags
							  , UINT SDKVersion
							  , ID3D10Device **ppDevice
							  );

typedef HRESULT (WINAPI *D3D10CreateDeviceAndSwapChain_t)(IDXGIAdapter *pAdapter
							, D3D10_DRIVER_TYPE DriverType
							, HMODULE Software
							, UINT Flags
							, UINT SDKVersion
							, DXGI_SWAP_CHAIN_DESC *pSwapChainDesc
							, IDXGISwapChain **ppSwapChain
							, ID3D10Device **ppDevice
							);

// Function prototype.
HRESULT WINAPI MyD3D10CreateDevice(IDXGIAdapter *pAdapter
							, D3D10_DRIVER_TYPE DriverType
							, HMODULE Software
							, UINT Flags
							, UINT SDKVersion
							, ID3D10Device **ppDevice
							);

HRESULT WINAPI MyD3D10CreateDeviceAndSwapChain(IDXGIAdapter *pAdapter
							, D3D10_DRIVER_TYPE DriverType
							, HMODULE Software
							, UINT Flags
							, UINT SDKVersion
							, DXGI_SWAP_CHAIN_DESC *pSwapChainDesc
							, IDXGISwapChain **ppSwapChain
							, ID3D10Device **ppDevice
							);

SDLLHook D3D10_Hook = 
{
	"D3D10.DLL",
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "D3D10CreateDevice", MyD3D10CreateDevice},
		{ "D3D10CreateDeviceAndSwapChain", MyD3D10CreateDeviceAndSwapChain},
		{ NULL, NULL }
	}
};

// Hook functions.
HRESULT WINAPI MyD3D10CreateDevice(IDXGIAdapter *pAdapter
						  , D3D10_DRIVER_TYPE DriverType
						  , HMODULE Software
						  , UINT Flags
						  , UINT SDKVersion
						  , ID3D10Device **ppDevice
						  )
{
	MessageBox_Debug("MyD3D10CreateDevice called");
	OutputDebugString( "Direct3D-Hook: MyD3D10CreateDevice called.\n" );

	D3D10CreateDevice_t old_func = (D3D10CreateDevice_t) D3D10_Hook.Functions[D3DFN_D3D10CreateDevice].OrigFn;
	HRESULT ret = old_func(pAdapter, DriverType, Software, Flags, SDKVersion, ppDevice);

	// ppDevice

	return ret;
}

HRESULT WINAPI MyD3D10CreateDeviceAndSwapChain(IDXGIAdapter *pAdapter
						  , D3D10_DRIVER_TYPE DriverType
						  , HMODULE Software
						  , UINT Flags
						  , UINT SDKVersion
						  , DXGI_SWAP_CHAIN_DESC *pSwapChainDesc
						  , IDXGISwapChain **ppSwapChain
						  , ID3D10Device **ppDevice
						  )
{
	MessageBox_Debug("MyD3D10CreateDeviceAndSwapChain called");
	OutputDebugString( "Direct3D-Hook: MyD3D10CreateDeviceAndSwapChain called.\n" );

	D3D10CreateDeviceAndSwapChain_t old_func = (D3D10CreateDeviceAndSwapChain_t) D3D10_Hook.Functions[D3DFN_D3D10CreateDeviceAndSwapChain].OrigFn;
	HRESULT ret = old_func(pAdapter, DriverType, Software, Flags, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice);

	// ppSwapChain
	// ppDevice

	return ret;
}


#ifdef _MANAGED
#pragma managed(pop)
#endif

