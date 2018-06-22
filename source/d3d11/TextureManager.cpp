#include "TextureManager.h"
#include "log.hpp"
#include <dxgi1_5.h>

TextureManager TextureManager::instance;

constexpr bool linear = true;

void TextureManager::CreateHDRSwapChain(DXGI_SWAP_CHAIN_DESC* desc, std::function<HRESULT(DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**)> createSwapChainLambda)
{
	DXGI_SWAP_CHAIN_DESC descCopy = *desc;
	descCopy.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	if constexpr(linear)
	{
		descCopy.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	}
	else
	{
		descCopy.BufferDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
	}

	HRESULT hr = createSwapChainLambda(&descCopy, (IDXGISwapChain**)&hdrSwapChain);

	LOG(INFO) << "HDR swapchain created with result " << hr;

	if constexpr(linear)
	{
		hr = hdrSwapChain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709);
	}
	else
	{
		hr = hdrSwapChain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
	}
	
	LOG(INFO) << "HDR colorspace set with result " << hr;

}




HRESULT TextureManager::PresentHDR(IDXGISwapChain * sdrSwapchain, UINT sync, UINT flags)
{
	// copy hdr rtv into swapchain buffer?
	ID3D11Device *device;
	sdrSwapchain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

	ID3D11DeviceContext *context;

	device->GetImmediateContext(&context);

	ID3D11Resource * backbuffer;
	ID3D11Resource * sdrBackbuffer;
	

	InitResources(device);
	int rtvIndex = 1;
	CopyHDR(context, m_computeShader, texData[rtvIndex].srv[0].get(), m_backbufferUAVs[hdrSwapChain->GetCurrentBackBufferIndex()], 3840, 2160);

	return hdrSwapChain->Present(sync, flags);
}

void TextureManager::AddTexture(ID3D11Texture2D * tex)
{
	//LOG(INFO) << "Tracking Texture" << (intptr_t)tex;
	allTextures.push_back(tex);
}

static void QueryColorspaces(IDXGISwapChain * swapchain)
{
	IDXGISwapChain * oldSwapChain;
	swapchain->QueryInterface(__uuidof(IDXGISwapChain4), (void**)&oldSwapChain);
	IDXGISwapChain4 * swapchain4 = (IDXGISwapChain4*)swapchain;
	UINT support;
	for (int i = 0; i <= 24; i++)
	{
		auto hr = swapchain4->CheckColorSpaceSupport((DXGI_COLOR_SPACE_TYPE)i, &support);
		LOG(INFO) << " HDR SUPPPPORT " << i << " " << support << " " << hr;
	}
}

void TextureManager::AddRTV(ID3D11RenderTargetView * rtv)
{
	// get srv
	D3D11_RENDER_TARGET_VIEW_DESC desc;
	rtv->GetDesc(&desc);
	
	if (desc.Format == DXGI_FORMAT_R11G11B10_FLOAT )
	{
		LOG(INFO) << "Tracking HDR Render Target " << (intptr_t)rtv;
		allRenderTargets.push_back(rtv);
		ID3D11Resource *resource;
		rtv->GetResource(&resource);
		ID3D11Device *device;
		rtv->GetDevice(&device);
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R11G11B10_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 0;
		srvDesc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView *srv;
		auto res = device->CreateShaderResourceView(resource, NULL, &srv);
		LOG(INFO) << "SRV Creation " << res;

		reshade::d3d11::d3d11_tex_data data;
		data.texture = nullptr;
		data.rtv[0] = rtv;
		data.srv[0] = srv;

		data.rtv[1] = nullptr;
		data.srv[1] = nullptr;

		texData.push_back(data);
	}


}

void TextureManager::CopyHDR( ID3D11DeviceContext * context, ID3D11ComputeShader *shader, ID3D11ShaderResourceView *srv, ID3D11UnorderedAccessView * uav, UINT textureX, UINT textureY)
{
	context->CSSetShader(shader, NULL, 0);
	ID3D11ShaderResourceView * srvs[1] = { srv };
	ID3D11UnorderedAccessView * uavs[1] = { uav };
	context->CSSetShaderResources(0, 1, srvs);
	context->CSSetUnorderedAccessViews(0, 1, uavs, NULL);
	context->Dispatch(textureX / 8, textureY / 8, 1);
}


void TextureManager::InitResources( ID3D11Device * device)
{
	#include "hdrShaders.h"
	if (!m_inited)
	{
		// create CS shader.
		auto HR = device->CreateComputeShader(copyShader, sizeof(copyShader), NULL, &m_computeShader);
		LOG(INFO) << "Shader HR: " << HR;

		// create backbuffer UAVs
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		ID3D11Resource * backbuffer;
		HR = hdrSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&backbuffer);
		LOG(INFO) << "BB 0 HR: " << HR;

		HR = device->CreateUnorderedAccessView(backbuffer, &uavDesc, &m_backbufferUAVs[0]);
		LOG(INFO) << "UAV 0 HR: " << HR;

		HR = hdrSwapChain->GetBuffer(1, __uuidof(ID3D11Resource), (void**)&backbuffer);
		LOG(INFO) << "BB 1 HR: " << HR;

		HR = device->CreateUnorderedAccessView(backbuffer, &uavDesc, &m_backbufferUAVs[1]);
		LOG(INFO) << "UAV 1 HR: " << HR;

		m_inited = true;
	}
	
}


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}
