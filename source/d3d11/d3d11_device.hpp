/*
 * Copyright (C) 2014 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause OR MIT
 */

#pragma once

#include "dxgi/dxgi_device.hpp"
#include "d3d11_impl_device.hpp"

struct D3D11On12Device;
struct D3D11DeviceContext;

// VUGGER_ADDON
struct DECLSPEC_UUID("3FF202D4-AC63-4AF0-9D74-0F69ADC521FA") D3D11ShaderResourceView final : ID3D11ShaderResourceView1, public reshade::d3d11::shader_resource_view_impl
{
	DECLARE_MEM(D3D11ShaderResourceView, 4096)

	D3D11ShaderResourceView(struct D3D11Device *device, const reshade::api::resource_view_desc &desc, ID3D11ShaderResourceView * original);		// VUGGER_ADDON
	D3D11ShaderResourceView(struct D3D11Device *device, const reshade::api::resource_view_desc &desc, ID3D11ShaderResourceView1 * original);	// VUGGER_ADDON

	#pragma region IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj) override final
	{
		if (ppvObj == nullptr)
			return E_POINTER;

		if (check_and_upgrade_interface(riid))
		{
			AddRef();
			*ppvObj = this;
			return S_OK;
		}

		return _orig->QueryInterface(riid, ppvObj);
	}
	ULONG   STDMETHODCALLTYPE AddRef() override final
	{
		_orig->AddRef();
		return InterlockedIncrement(&_ref);
	}
	ULONG   STDMETHODCALLTYPE Release() override final;
	#pragma endregion
	#pragma region ID3D11DeviceChild
	void    STDMETHODCALLTYPE GetDevice(ID3D11Device **ppDevice) override final;
	HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData) override final { return _orig->GetPrivateData(guid, pDataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid, UINT DataSize, const void *pData) override final { return _orig->SetPrivateData(guid, DataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown *pData) override final { return _orig->SetPrivateDataInterface(guid, pData); }
	#pragma endregion
	#pragma region ID3D11View
	void STDMETHODCALLTYPE GetResource(ID3D11Resource **ppResource) override final { _orig->GetResource(ppResource); }
	#pragma endregion
	#pragma region ID3D11ShaderResourceView
	void STDMETHODCALLTYPE GetDesc(D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc) override final { _orig->GetDesc(pDesc); }
	#pragma endregion
	#pragma region ID3D11ShaderResourceView1
	void STDMETHODCALLTYPE GetDesc1(D3D11_SHADER_RESOURCE_VIEW_DESC1 * pDesc1) override final { assert(_interface_version >= 1); static_cast<ID3D11ShaderResourceView1 *>(_orig)->GetDesc1(pDesc1); }
	#pragma endregion

	bool check_and_upgrade_interface(REFIID riid)
	{
		if (riid == __uuidof(this) ||
			riid == __uuidof(IUnknown) ||
			riid == __uuidof(ID3D11DeviceChild) ||
			riid == __uuidof(ID3D11View) ||
			riid == __uuidof(ID3D11ShaderResourceView))
			return true;

		return false;
	}

	ULONG _ref = 1;
	unsigned int _interface_version = 0;
	D3D11Device *const _device = nullptr;
	//ID3D11ShaderResourceView *_orig = nullptr;
};
DECLARE_MEM_STATIC(D3D11ShaderResourceView, 4096)


struct DECLSPEC_UUID("ED73DC39-8A00-4264-B6B5-E6DAABBE79B5") D3D11UnorderedAccessView final : ID3D11UnorderedAccessView1, public reshade::d3d11::unordered_access_view_impl
{
	DECLARE_MEM(D3D11UnorderedAccessView, 256)

	D3D11UnorderedAccessView(struct D3D11Device *device, const reshade::api::resource_view_desc &desc, ID3D11UnorderedAccessView * original);		// VUGGER_ADDON
	D3D11UnorderedAccessView(struct D3D11Device *device, const reshade::api::resource_view_desc &desc, ID3D11UnorderedAccessView1 *original);		// VUGGER_ADDON

	#pragma region IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj) override final
	{
		if (ppvObj == nullptr)
			return E_POINTER;

		if (check_and_upgrade_interface(riid))
		{
			AddRef();
			*ppvObj = this;
			return S_OK;
		}

		return _orig->QueryInterface(riid, ppvObj);
	}
	ULONG   STDMETHODCALLTYPE AddRef() override final
	{
		_orig->AddRef();
		return InterlockedIncrement(&_ref);
	}
	ULONG   STDMETHODCALLTYPE Release() override final;
	#pragma endregion
	#pragma region ID3D11DeviceChild
	void    STDMETHODCALLTYPE GetDevice(ID3D11Device **ppDevice) override final;
	HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData) override final { return _orig->GetPrivateData(guid, pDataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid, UINT DataSize, const void *pData) override final { return _orig->SetPrivateData(guid, DataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown *pData) override final { return _orig->SetPrivateDataInterface(guid, pData); }
	#pragma endregion
	#pragma region ID3D11View
	void STDMETHODCALLTYPE GetResource(ID3D11Resource **ppResource) override final { _orig->GetResource(ppResource); }
	#pragma endregion
	#pragma region ID3D11UnorderedAccessView
	void STDMETHODCALLTYPE GetDesc(D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc) override final { _orig->GetDesc(pDesc); }
	#pragma endregion
	#pragma region ID3D11UnorderedAccessView1
	void STDMETHODCALLTYPE GetDesc1(D3D11_UNORDERED_ACCESS_VIEW_DESC1 *pDesc1) override final { assert(_interface_version >= 1); static_cast<ID3D11UnorderedAccessView1 *>(_orig)->GetDesc1(pDesc1); }
	#pragma endregion

	bool check_and_upgrade_interface(REFIID riid)
	{
		if (riid == __uuidof(this) ||
			riid == __uuidof(IUnknown) ||
			riid == __uuidof(ID3D11DeviceChild) ||
			riid == __uuidof(ID3D11View) ||
			riid == __uuidof(ID3D11UnorderedAccessView))
			return true;

		return false;
	}

	ULONG _ref = 1;
	unsigned int _interface_version = 0;
	D3D11Device *const _device = nullptr;
	//ID3D11UnorderedAccessView *_orig = nullptr;
};
DECLARE_MEM_STATIC(D3D11UnorderedAccessView, 256)


struct DECLSPEC_UUID("32ED0AD5-5462-4AC6-BA63-4B7641FE1B3E") D3D11RenderTargetView final : ID3D11RenderTargetView1, public reshade::d3d11::render_target_view_impl
{
	DECLARE_MEM(D3D11RenderTargetView, 256)

	D3D11RenderTargetView(struct D3D11Device *device, const reshade::api::resource_view_desc &desc, ID3D11RenderTargetView * original);		// VUGGER_ADDON
	D3D11RenderTargetView(struct D3D11Device *device, const reshade::api::resource_view_desc &desc, ID3D11RenderTargetView1 *original);		// VUGGER_ADDON

	#pragma region IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj) override final
	{
		if (ppvObj == nullptr)
			return E_POINTER;

		if (check_and_upgrade_interface(riid))
		{
			AddRef();
			*ppvObj = this;
			return S_OK;
		}

		return _orig->QueryInterface(riid, ppvObj);
	}
	ULONG   STDMETHODCALLTYPE AddRef() override final
	{
		_orig->AddRef();
		return InterlockedIncrement(&_ref);
	}
	ULONG   STDMETHODCALLTYPE Release() override final;
	#pragma endregion
	#pragma region ID3D11DeviceChild
	void    STDMETHODCALLTYPE GetDevice(ID3D11Device **ppDevice) override final;
	HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData) override final { return _orig->GetPrivateData(guid, pDataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid, UINT DataSize, const void *pData) override final { return _orig->SetPrivateData(guid, DataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown *pData) override final { return _orig->SetPrivateDataInterface(guid, pData); }
	#pragma endregion
	#pragma region ID3D11View
	void STDMETHODCALLTYPE GetResource(ID3D11Resource **ppResource) override final { _orig->GetResource(ppResource); }
	#pragma endregion
	#pragma region ID3D11RenderTargetView
	void STDMETHODCALLTYPE GetDesc(D3D11_RENDER_TARGET_VIEW_DESC *pDesc) override final { _orig->GetDesc(pDesc); }
	#pragma endregion
	#pragma region ID3D11RenderTargetView1
	void STDMETHODCALLTYPE GetDesc1(D3D11_RENDER_TARGET_VIEW_DESC1 *pDesc1) override final { assert(_interface_version >= 1); static_cast<ID3D11RenderTargetView1 *>(_orig)->GetDesc1(pDesc1); }
	#pragma endregion

	bool check_and_upgrade_interface(REFIID riid)
	{
		if (riid == __uuidof(this) ||
			riid == __uuidof(IUnknown) ||
			riid == __uuidof(ID3D11DeviceChild) ||
			riid == __uuidof(ID3D11View) ||
			riid == __uuidof(ID3D11RenderTargetView))
			return true;

		return false;
	}

	ULONG _ref = 1;
	unsigned int _interface_version = 0;
	D3D11Device *const _device = nullptr;
	//ID3D11RenderTargetView *_orig = nullptr;
};
DECLARE_MEM_STATIC(D3D11RenderTargetView, 256)

struct DECLSPEC_UUID("E12919C2-54FA-4FB9-9938-6DC0F3257008") D3D11DepthStencilView final : ID3D11DepthStencilView, public reshade::d3d11::depth_stencil_view_impl
{
	DECLARE_MEM(D3D11DepthStencilView, 256)

	D3D11DepthStencilView(struct D3D11Device *device, const reshade::api::resource_view_desc &desc, ID3D11DepthStencilView * original);			// VUGGER_ADDON

	#pragma region IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj) override final
	{
		if (ppvObj == nullptr)
			return E_POINTER;

		if (check_and_upgrade_interface(riid))
		{
			AddRef();
			*ppvObj = this;
			return S_OK;
		}

		return _orig->QueryInterface(riid, ppvObj);
	}
	ULONG   STDMETHODCALLTYPE AddRef() override final
	{
		_orig->AddRef();
		return InterlockedIncrement(&_ref);
	}
	ULONG   STDMETHODCALLTYPE Release() override final;
	#pragma endregion
	#pragma region ID3D11DeviceChild
	void    STDMETHODCALLTYPE GetDevice(ID3D11Device **ppDevice) override final;
	HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData) override final { return _orig->GetPrivateData(guid, pDataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid, UINT DataSize, const void *pData) override final { return _orig->SetPrivateData(guid, DataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown *pData) override final { return _orig->SetPrivateDataInterface(guid, pData); }
	#pragma endregion
	#pragma region ID3D11View
	void STDMETHODCALLTYPE GetResource(ID3D11Resource **ppResource) override final { _orig->GetResource(ppResource); }
	#pragma endregion
	#pragma region ID3D11DepthStencilView
	void STDMETHODCALLTYPE GetDesc(D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc) override final { _orig->GetDesc(pDesc); }
	#pragma endregion

	bool check_and_upgrade_interface(REFIID riid)
	{
		if (riid == __uuidof(this) ||
			riid == __uuidof(IUnknown) ||
			riid == __uuidof(ID3D11DeviceChild) ||
			riid == __uuidof(ID3D11View) ||
			riid == __uuidof(ID3D11DepthStencilView))
			return true;

		return false;
	}

	ULONG _ref = 1;
	unsigned int _interface_version = 0;
	D3D11Device *const _device = nullptr;
	//ID3D11DepthStencilView *_orig = nullptr;
};
DECLARE_MEM_STATIC(D3D11DepthStencilView, 256)

struct DECLSPEC_UUID("FC08B47C-A36C-408A-A5E7-A707527E0FF1") D3D11SamplerState final : ID3D11SamplerState, public reshade::d3d11::sampler_impl
{
	DECLARE_MEM(D3D11SamplerState, 32)

	D3D11SamplerState(struct D3D11Device *device, const reshade::api::sampler_desc &desc, ID3D11SamplerState * original);

	#pragma region IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj) override final
	{
		if (ppvObj == nullptr)
			return E_POINTER;

		if (check_and_upgrade_interface(riid))
		{
			AddRef();
			*ppvObj = this;
			return S_OK;
		}

		return _orig->QueryInterface(riid, ppvObj);
	}
	ULONG   STDMETHODCALLTYPE AddRef() override final
	{
		_orig->AddRef();
		return InterlockedIncrement(&_ref);
	}
	ULONG   STDMETHODCALLTYPE Release() override final;
	#pragma endregion
	#pragma region ID3D11DeviceChild
	void    STDMETHODCALLTYPE GetDevice(ID3D11Device **ppDevice) override final;
	HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData) override final { return _orig->GetPrivateData(guid, pDataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid, UINT DataSize, const void *pData) override final { return _orig->SetPrivateData(guid, DataSize, pData); }
	HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown *pData) override final { return _orig->SetPrivateDataInterface(guid, pData); }
	#pragma endregion
	#pragma region ID3D11SamplerState
	void STDMETHODCALLTYPE GetDesc(D3D11_SAMPLER_DESC *pDesc) override final { _orig->GetDesc(pDesc); }
	#pragma endregion

	bool check_and_upgrade_interface(REFIID riid)
	{
		if (riid == __uuidof(this) ||
			riid == __uuidof(IUnknown) ||
			riid == __uuidof(ID3D11DeviceChild) ||
			riid == __uuidof(ID3D11SamplerState))
			return true;

		return false;
	}

	ULONG _ref = 1;
	unsigned int _interface_version = 0;
	D3D11Device *const _device = nullptr;
	//ID3D11SamplerState *_orig = nullptr;
};
DECLARE_MEM_STATIC(D3D11SamplerState, 32)
// VUGGER_ADDON

struct DECLSPEC_UUID("72299288-2C68-4AD8-945D-2BFB5AA9C609") D3D11Device final : DXGIDevice, ID3D11Device5, public reshade::d3d11::device_impl
{
	D3D11Device(IDXGIDevice1 *original_dxgi_device, ID3D11Device *original);

	#pragma region IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj) override;
	ULONG   STDMETHODCALLTYPE AddRef() override;
	ULONG   STDMETHODCALLTYPE Release() override;
	#pragma endregion
	#pragma region ID3D11Device
	HRESULT STDMETHODCALLTYPE CreateBuffer(const D3D11_BUFFER_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Buffer **ppBuffer) override;
	HRESULT STDMETHODCALLTYPE CreateTexture1D(const D3D11_TEXTURE1D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture1D **ppTexture1D) override;
	HRESULT STDMETHODCALLTYPE CreateTexture2D(const D3D11_TEXTURE2D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture2D **ppTexture2D) override;
	HRESULT STDMETHODCALLTYPE CreateTexture3D(const D3D11_TEXTURE3D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture3D **ppTexture3D) override;
	HRESULT STDMETHODCALLTYPE CreateShaderResourceView(ID3D11Resource *pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D11ShaderResourceView **ppSRView) override;
	HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(ID3D11Resource *pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc, ID3D11UnorderedAccessView **ppUAView) override;
	HRESULT STDMETHODCALLTYPE CreateRenderTargetView(ID3D11Resource *pResource, const D3D11_RENDER_TARGET_VIEW_DESC *pDesc, ID3D11RenderTargetView **ppRTView) override;
	HRESULT STDMETHODCALLTYPE CreateDepthStencilView(ID3D11Resource *pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D11DepthStencilView **ppDepthStencilView) override;
	HRESULT STDMETHODCALLTYPE CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout **ppInputLayout) override;
	HRESULT STDMETHODCALLTYPE CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader) override;
	HRESULT STDMETHODCALLTYPE CreateGeometryShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader) override;
	HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D11_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, const UINT *pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader) override;
	HRESULT STDMETHODCALLTYPE CreatePixelShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader **ppPixelShader) override;
	HRESULT STDMETHODCALLTYPE CreateHullShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11HullShader **ppHullShader) override;
	HRESULT STDMETHODCALLTYPE CreateDomainShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11DomainShader **ppDomainShader) override;
	HRESULT STDMETHODCALLTYPE CreateComputeShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11ComputeShader **ppComputeShader) override;
	HRESULT STDMETHODCALLTYPE CreateClassLinkage(ID3D11ClassLinkage **ppLinkage) override;
	HRESULT STDMETHODCALLTYPE CreateBlendState(const D3D11_BLEND_DESC *pBlendStateDesc, ID3D11BlendState **ppBlendState) override;
	HRESULT STDMETHODCALLTYPE CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D11DepthStencilState **ppDepthStencilState) override;
	HRESULT STDMETHODCALLTYPE CreateRasterizerState(const D3D11_RASTERIZER_DESC *pRasterizerDesc, ID3D11RasterizerState **ppRasterizerState) override;
	HRESULT STDMETHODCALLTYPE CreateSamplerState(const D3D11_SAMPLER_DESC *pSamplerDesc, ID3D11SamplerState **ppSamplerState) override;
	HRESULT STDMETHODCALLTYPE CreateQuery(const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery) override;
	HRESULT STDMETHODCALLTYPE CreatePredicate(const D3D11_QUERY_DESC *pPredicateDesc, ID3D11Predicate **ppPredicate) override;
	HRESULT STDMETHODCALLTYPE CreateCounter(const D3D11_COUNTER_DESC *pCounterDesc, ID3D11Counter **ppCounter) override;
	HRESULT STDMETHODCALLTYPE CreateDeferredContext(UINT ContextFlags, ID3D11DeviceContext **ppDeferredContext) override;
	HRESULT STDMETHODCALLTYPE OpenSharedResource(HANDLE hResource, REFIID ReturnedInterface, void **ppResource) override;
	HRESULT STDMETHODCALLTYPE CheckFormatSupport(DXGI_FORMAT Format, UINT *pFormatSupport) override;
	HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels) override;
	void    STDMETHODCALLTYPE CheckCounterInfo(D3D11_COUNTER_INFO *pCounterInfo) override;
	HRESULT STDMETHODCALLTYPE CheckCounter(const D3D11_COUNTER_DESC *pDesc, D3D11_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength) override;
	HRESULT STDMETHODCALLTYPE CheckFeatureSupport(D3D11_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize) override;
	HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData) override;
	HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid, UINT DataSize, const void *pData) override;
	HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown *pData) override;
	UINT    STDMETHODCALLTYPE GetCreationFlags() override;
	HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason() override;
	void    STDMETHODCALLTYPE GetImmediateContext(ID3D11DeviceContext **ppImmediateContext) override;
	HRESULT STDMETHODCALLTYPE SetExceptionMode(UINT RaiseFlags) override;
	UINT    STDMETHODCALLTYPE GetExceptionMode() override;
	D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel() override;
	#pragma endregion
	#pragma region ID3D11Device1
	void    STDMETHODCALLTYPE GetImmediateContext1(ID3D11DeviceContext1 **ppImmediateContext) override;
	HRESULT STDMETHODCALLTYPE CreateDeferredContext1(UINT ContextFlags, ID3D11DeviceContext1 **ppDeferredContext) override;
	HRESULT STDMETHODCALLTYPE CreateBlendState1(const D3D11_BLEND_DESC1 *pBlendStateDesc, ID3D11BlendState1 **ppBlendState);
	HRESULT STDMETHODCALLTYPE CreateRasterizerState1(const D3D11_RASTERIZER_DESC1 *pRasterizerDesc, ID3D11RasterizerState1 **ppRasterizerState) override;
	HRESULT STDMETHODCALLTYPE CreateDeviceContextState(UINT Flags, const D3D_FEATURE_LEVEL *pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, REFIID EmulatedInterface, D3D_FEATURE_LEVEL *pChosenFeatureLevel, ID3DDeviceContextState **ppContextState) override;
	HRESULT STDMETHODCALLTYPE OpenSharedResource1(HANDLE hResource, REFIID returnedInterface, void **ppResource) override;
	HRESULT STDMETHODCALLTYPE OpenSharedResourceByName(LPCWSTR lpName, DWORD dwDesiredAccess, REFIID returnedInterface, void **ppResource) override;
	#pragma endregion
	#pragma region ID3D11Device2
	void    STDMETHODCALLTYPE GetImmediateContext2(ID3D11DeviceContext2 **ppImmediateContext) override;
	HRESULT STDMETHODCALLTYPE CreateDeferredContext2(UINT ContextFlags, ID3D11DeviceContext2 **ppDeferredContext) override;
	void    STDMETHODCALLTYPE GetResourceTiling(ID3D11Resource *pTiledResource, UINT *pNumTilesForEntireResource, D3D11_PACKED_MIP_DESC *pPackedMipDesc, D3D11_TILE_SHAPE *pStandardTileShapeForNonPackedMips, UINT *pNumSubresourceTilings, UINT FirstSubresourceTilingToGet, D3D11_SUBRESOURCE_TILING *pSubresourceTilingsForNonPackedMips) override;
	HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels1(DXGI_FORMAT Format, UINT SampleCount, UINT Flags, UINT *pNumQualityLevels) override;
	#pragma endregion
	#pragma region ID3D11Device3
	HRESULT STDMETHODCALLTYPE CreateTexture2D1(const D3D11_TEXTURE2D_DESC1 *pDesc1, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture2D1 **ppTexture2D) override;
	HRESULT STDMETHODCALLTYPE CreateTexture3D1(const D3D11_TEXTURE3D_DESC1 *pDesc1, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture3D1 **ppTexture3D) override;
	HRESULT STDMETHODCALLTYPE CreateRasterizerState2(const D3D11_RASTERIZER_DESC2 *pRasterizerDesc, ID3D11RasterizerState2 **ppRasterizerState) override;
	HRESULT STDMETHODCALLTYPE CreateShaderResourceView1(ID3D11Resource *pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC1 *pDesc1, ID3D11ShaderResourceView1 **ppSRView1) override;
	HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView1(ID3D11Resource *pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC1 *pDesc1, ID3D11UnorderedAccessView1 **ppUAView1) override;
	HRESULT STDMETHODCALLTYPE CreateRenderTargetView1(ID3D11Resource *pResource, const D3D11_RENDER_TARGET_VIEW_DESC1 *pDesc1, ID3D11RenderTargetView1 **ppRTView1) override;
	HRESULT STDMETHODCALLTYPE CreateQuery1(const D3D11_QUERY_DESC1 *pQueryDesc1, ID3D11Query1 **ppQuery1) override;
	void    STDMETHODCALLTYPE GetImmediateContext3(ID3D11DeviceContext3 **ppImmediateContext) override;
	HRESULT STDMETHODCALLTYPE CreateDeferredContext3(UINT ContextFlags, ID3D11DeviceContext3 **ppDeferredContext) override;
	void    STDMETHODCALLTYPE WriteToSubresource(ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch) override;
	void    STDMETHODCALLTYPE ReadFromSubresource(void *pDstData, UINT DstRowPitch, UINT DstDepthPitch, ID3D11Resource *pSrcResource, UINT SrcSubresource, const D3D11_BOX *pSrcBox) override;
	#pragma endregion
	#pragma region ID3D11Device4
	HRESULT STDMETHODCALLTYPE RegisterDeviceRemovedEvent( HANDLE hEvent, DWORD *pdwCookie) override;
	void    STDMETHODCALLTYPE UnregisterDeviceRemoved(DWORD dwCookie) override;
	#pragma endregion
	#pragma region ID3D11Device5
	HRESULT STDMETHODCALLTYPE OpenSharedFence(HANDLE hFence, REFIID ReturnedInterface, void **ppFence) override;
	HRESULT STDMETHODCALLTYPE CreateFence(UINT64 InitialValue, D3D11_FENCE_FLAG Flags, REFIID ReturnedInterface, void **ppFence) override;
	#pragma endregion

	bool check_and_upgrade_interface(REFIID riid);

	using device_impl::_orig;

	LONG _ref = 1;
	unsigned int _interface_version = 0;
	D3D11On12Device *_d3d11on12_device = nullptr;
	D3D11DeviceContext *_immediate_context = nullptr;
};
