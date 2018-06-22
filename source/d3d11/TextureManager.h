#pragma once
#include <vector>
#include <functional>

#include "d3d11_device.hpp"
#include "d3d11_runtime.hpp"


struct IDXGISwapChain4;

class TextureManager
{
public:
	static TextureManager instance;

	bool start = false;

	std::vector<ID3D11RenderTargetView*> allRenderTargets;
	std::vector<ID3D11Texture2D*> allTextures;
	std::vector<reshade::d3d11::d3d11_tex_data> texData;
	IDXGISwapChain4 * hdrSwapChain;
	void CreateHDRSwapChain(DXGI_SWAP_CHAIN_DESC* desc, std::function<HRESULT(DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**)> createSwapChainLambda);

	HRESULT PresentHDR(IDXGISwapChain * sdrSwapchain, UINT sync, UINT flags);

	

	void AddTexture(ID3D11Texture2D* tex);
	void AddRTV(ID3D11RenderTargetView* rtv);

	void CopyHDR(ID3D11DeviceContext * context, ID3D11ComputeShader * shader, ID3D11ShaderResourceView * srv, ID3D11UnorderedAccessView * uav, UINT textureX, UINT textureY);

	void InitResources(ID3D11Device * device);




	TextureManager();
	~TextureManager();

private:
	bool m_inited = false;
	ID3D11ComputeShader * m_computeShader;
	ID3D11UnorderedAccessView * m_backbufferUAVs[2];

};

