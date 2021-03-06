#pragma once
#include <vector>
#include <functional>

#include "d3d11_device.hpp"
#include "d3d11_runtime.hpp"
#include "dxgi/dxgi_swapchain.hpp"
#include "renderdoc_app.h"

struct IDXGISwapChain4;

union BigBool
{
	bool b;
	uint32_t u;
};

struct ShaderConstants
{
	float brightnessScale;
	float gamma;
	BigBool enabled;
	float wcgScale;
};

constexpr auto size = sizeof(ShaderConstants);

static_assert(sizeof(ShaderConstants) == 16);

class TextureManager
{
public:
	static TextureManager instance;

	bool start = false;

	std::vector<ID3D11RenderTargetView*> allRenderTargets;
	std::vector<ID3D11Texture2D*> allTextures;
	std::vector<reshade::d3d11::d3d11_tex_data> texData;
	DXGISwapChain * hdrSwapChain;
	void CreateHDRSwapChain(DXGI_SWAP_CHAIN_DESC* desc, D3D11Device * device, const std::shared_ptr<reshade::runtime> &runtime, std::function<HRESULT(DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**)> createSwapChainLambda);

	HRESULT PresentHDR(DXGISwapChain * sdrSwapchain, UINT sync, UINT flags);

	void AddTexture(ID3D11Texture2D* tex);
	void AddRTV(ID3D11RenderTargetView* rtv);
	void RunHDRShaders(ID3D11DeviceContext * context, ID3D11ComputeShader * shader, ID3D11ShaderResourceView ** srvs, ID3D11UnorderedAccessView * uav, UINT textureX, UINT textureY);
	void InitResources(ID3D11Device * device);

	int firstBBIndex, secondBBIndex;

	ShaderConstants constants =
	{
		1.0f,
		2.2f,
		false,
		1.0f,
	};



	TextureManager();
	~TextureManager();

private:
	bool m_inited = false;
	ID3D11ComputeShader * m_computeShader;
	ID3D11UnorderedAccessView * m_backbufferUAVs[2];
	ID3D11Buffer * m_constantBuffer;
	ID3D11Texture2D * m_backbuffer;
	RENDERDOC_API_1_1_2 *m_rdoc;


};

