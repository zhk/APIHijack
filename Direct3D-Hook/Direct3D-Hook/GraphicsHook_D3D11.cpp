// Direct3D-Hook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "apihijack.h"

#include <d3d11.h>
#include <d3dx11.h>
#include "enums.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//-------------------------------------------------------------------------
// D3D11

// Function pointer types.
typedef HRESULT (WINAPI *D3D11CreateDevice_t)(
	IDXGIAdapter *pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	const D3D_FEATURE_LEVEL *pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	ID3D11Device **ppDevice,
	D3D_FEATURE_LEVEL *pFeatureLevel,
	ID3D11DeviceContext **ppImmediateContext
	);

typedef HRESULT (WINAPI *D3D11CreateDeviceAndSwapChain_t)(
	IDXGIAdapter *pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	const D3D_FEATURE_LEVEL *pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
	IDXGISwapChain **ppSwapChain,
	ID3D11Device **ppDevice,
	D3D_FEATURE_LEVEL *pFeatureLevel,
	ID3D11DeviceContext **ppImmediateContext
	);

// Function prototype.
HRESULT WINAPI MyD3D11CreateDevice(
	IDXGIAdapter *pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	const D3D_FEATURE_LEVEL *pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	ID3D11Device **ppDevice,
	D3D_FEATURE_LEVEL *pFeatureLevel,
	ID3D11DeviceContext **ppImmediateContext
	);

HRESULT WINAPI MyD3D11CreateDeviceAndSwapChain(
	IDXGIAdapter *pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	const D3D_FEATURE_LEVEL *pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
	IDXGISwapChain **ppSwapChain,
	ID3D11Device **ppDevice,
	D3D_FEATURE_LEVEL *pFeatureLevel,
	ID3D11DeviceContext **ppImmediateContext
	);

SDLLHook D3D11_Hook = 
{
	"D3D11.DLL",
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "D3D11CreateDevice", MyD3D11CreateDevice},
		{ "D3D11CreateDeviceAndSwapChain", MyD3D11CreateDeviceAndSwapChain},
		{ NULL, NULL }
	}
};

// Hook functions.
HRESULT WINAPI MyD3D11CreateDevice(
								   IDXGIAdapter *pAdapter,
								   D3D_DRIVER_TYPE DriverType,
								   HMODULE Software,
								   UINT Flags,
								   const D3D_FEATURE_LEVEL *pFeatureLevels,
								   UINT FeatureLevels,
								   UINT SDKVersion,
								   ID3D11Device **ppDevice,
								   D3D_FEATURE_LEVEL *pFeatureLevel,
								   ID3D11DeviceContext **ppImmediateContext
								   )
{
	MessageBox_Debug("MyD3D11CreateDevice called");
	OutputDebugString( "Direct3D-Hook: MyD3D11CreateDevice called.\n" );

	D3D11CreateDevice_t old_func = (D3D11CreateDevice_t) D3D11_Hook.Functions[D3DFN_D3D11CreateDevice].OrigFn;
	HRESULT ret = old_func(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels
		, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);

	// ppDevice

	return ret;
}

HRESULT WINAPI MyD3D11CreateDeviceAndSwapChain(
	IDXGIAdapter *pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	const D3D_FEATURE_LEVEL *pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
	IDXGISwapChain **ppSwapChain,
	ID3D11Device **ppDevice,
	D3D_FEATURE_LEVEL *pFeatureLevel,
	ID3D11DeviceContext **ppImmediateContext
	)
{
	MessageBox_Debug("MyD3D11CreateDeviceAndSwapChain called");
	OutputDebugString( "Direct3D-Hook: MyD3D11CreateDeviceAndSwapChain called.\n" );

	D3D11CreateDeviceAndSwapChain_t old_func = (D3D11CreateDeviceAndSwapChain_t) D3D11_Hook.Functions[D3DFN_D3D11CreateDeviceAndSwapChain].OrigFn;
	HRESULT ret = old_func(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels
		, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

	// ppSwapChain
	// ppDevice

	return ret;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

