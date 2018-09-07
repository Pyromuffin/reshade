#include "TextureManager.h"
#include "log.hpp"
#include <dxgi1_5.h>
#include "../dxgi/dxgi.hpp"
#include "hook_manager.hpp"

TextureManager TextureManager::instance;

constexpr bool linear = false;


void TextureManager::CreateHDRSwapChain(DXGI_SWAP_CHAIN_DESC* desc, D3D11Device * device, const std::shared_ptr<reshade::runtime> &runtime, std::function<HRESULT(DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**)> createSwapChainLambda)
{
	DXGI_SWAP_CHAIN_DESC descCopy = *desc;
	descCopy.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	descCopy.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_SHADER_INPUT;
	descCopy.Flags = 0;
	descCopy.Windowed = true;
	descCopy.BufferCount = 2;
	descCopy.BufferDesc.RefreshRate.Numerator = 98;
	descCopy.BufferDesc.RefreshRate.Denominator = 1;
	descCopy.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	descCopy.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;


	if constexpr(linear)
	{
		descCopy.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	}
	else
	{
		descCopy.BufferDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
	}

	IDXGISwapChain *swapchain;
	IDXGISwapChain4 *swapchain4;
	auto hr = createSwapChainLambda(&descCopy, (IDXGISwapChain**)&swapchain);
	hdrSwapChain = new DXGISwapChain(device, swapchain, runtime);
	hdrSwapChain->QueryInterface(__uuidof(IDXGISwapChain4),(void**)&swapchain4);
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

std::unordered_map<ID3D11Resource*, ID3D11ShaderResourceView *> srvMap;

HRESULT TextureManager::PresentHDR(DXGISwapChain * sdrSwapchain, UINT sync, UINT flags)
{
	ID3D11Device *device;
	sdrSwapchain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

	ID3D11DeviceContext *context;

	device->GetImmediateContext(&context);

	ID3D11Resource * backbuffer;
	ID3D11Resource * sdrBackbuffer;
	

	InitResources(device);
	int rtvIndex = firstBBIndex;
	ID3D11ShaderResourceView * srvs[2];
	srvs[0] = texData[rtvIndex].srv[0].get();

	ID3D11Resource * currentBB;
	auto hr = sdrSwapchain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&currentBB);
	DXGI_SWAP_CHAIN_DESC swapDesc;
	sdrSwapchain->GetDesc(&swapDesc);

	if (srvMap.count(currentBB) > 0)
	{
		srvs[1] = srvMap[currentBB];
	}
	else
	{
		// create backbuffer view
		if (false) // swapDesc.BufferUsage & DXGI_USAGE_SHADER_INPUT)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = -1;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Format = swapDesc.BufferDesc.Format; // srgb?

			ID3D11ShaderResourceView * srv;
			device->CreateShaderResourceView(currentBB, &desc, &srv);
			srvMap[currentBB] = srv;
			srvs[1] = srvMap[currentBB];
		}
		else
		{
			D3D11_TEXTURE2D_DESC texdesc = {};
			texdesc.Width = 3840;
			texdesc.Height = 2160;
			texdesc.ArraySize = texdesc.MipLevels = 1;
			texdesc.Format = swapDesc.BufferDesc.Format;
			texdesc.SampleDesc = { 1, 0 };
			texdesc.Usage = D3D11_USAGE_DEFAULT;
			texdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			device->CreateTexture2D(&texdesc, nullptr, &m_backbuffer);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = -1;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Format = swapDesc.BufferDesc.Format; // srgb?

			ID3D11ShaderResourceView * srv;
			device->CreateShaderResourceView(m_backbuffer, &desc, &srv);
			srvMap[currentBB] = srv;
			srvs[1] = srvMap[currentBB];
		}
	}

	if ( true) //(swapDesc.BufferUsage & DXGI_USAGE_SHADER_INPUT) == 0)
	{
		context->CopyResource(m_backbuffer, currentBB);
	}

	RunHDRShaders(context, m_computeShader, srvs, m_backbufferUAVs[hdrSwapChain->GetCurrentBackBufferIndex()], 3840, 2160);
	auto ret = hdrSwapChain->_orig->Present(sync, flags);

	static bool capturing = false;
	static bool endCapture = false;
	if (endCapture)
	{
		m_rdoc->EndFrameCapture(nullptr, nullptr);
		endCapture = false;
	}

	if (capturing)
	{
		m_rdoc->SetActiveWindow(device, swapDesc.OutputWindow);
		m_rdoc->StartFrameCapture(nullptr, nullptr);
		capturing = false;
		endCapture = true;
	}

	return ret;
}

void TextureManager::AddTexture(ID3D11Texture2D * tex)
{
	//LOG(INFO) << "Tracking Texture" << (intptr_t)tex;
	//allTextures.push_back(tex);
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
	DXGI_FORMAT HDRFormats[] =
	{
		//DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R11G11B10_FLOAT,
		DXGI_FORMAT_R10G10B10A2_UNORM,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	};



	D3D11_RENDER_TARGET_VIEW_DESC desc;
	rtv->GetDesc(&desc);
	ID3D11Resource *resource;
	rtv->GetResource(&resource);
	ID3D11Texture2D *tex = (ID3D11Texture2D *)resource;

	D3D11_TEXTURE2D_DESC texDesc;
	tex->GetDesc(&texDesc);
	int width = texDesc.Width;
	int fourkWidth = 3840;


	for (int i = 0; i < 3; i++)
	{
		auto format = HDRFormats[i];

		if (desc.Format == format && width == fourkWidth)
		{
			LOG(INFO) << "Tracking HDR Render Target " << (intptr_t)rtv;
			allRenderTargets.push_back(rtv);
		
			ID3D11Device *device;
			rtv->GetDevice(&device);
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 0;
			srvDesc.Texture2D.MostDetailedMip = 0;

			ID3D11ShaderResourceView *srv;
			auto res = device->CreateShaderResourceView(resource, NULL, &srv);
			LOG(INFO) << "SRV Creation " << res;

			reshade::d3d11::d3d11_tex_data data;
			data.texture = (ID3D11Texture2D *) resource;
			data.rtv[0] = rtv;
			data.srv[0] = srv;

			data.rtv[1] = nullptr;
			data.srv[1] = nullptr;

			texData.push_back(data);
		}
	}
}

void TextureManager::RunHDRShaders( ID3D11DeviceContext * context, ID3D11ComputeShader *shader, ID3D11ShaderResourceView ** srvs, ID3D11UnorderedAccessView * uav, UINT textureX, UINT textureY)
{
	ID3D11RenderTargetView *rtvs[8];
	ID3D11DepthStencilView *dsv[1];

	context->OMGetRenderTargets(8, rtvs, dsv);
	context->OMSetRenderTargets(0, nullptr, nullptr);

	// save off rendering state
	ID3D11ComputeShader *oldShader;
	ID3D11ShaderResourceView *oldViews[8];
	ID3D11UnorderedAccessView *oldUavs[8];
	ID3D11Buffer *oldBuffers[8];

	context->CSGetShader(&oldShader, nullptr, nullptr);
	context->CSGetShaderResources(0, 8, oldViews);
	context->CSGetUnorderedAccessViews(0, 8, oldUavs);
	context->CSGetConstantBuffers(0, 8, oldBuffers);


	context->CSSetShader(shader, NULL, 0);
	//ID3D11ShaderResourceView * srvs[1] = { srv };
	ID3D11UnorderedAccessView * uavs[1] = { uav };
	context->CSSetShaderResources(0, 2, srvs);
	context->CSSetUnorderedAccessViews(0, 1, uavs, NULL);


	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	context->Map(m_constantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &constants, sizeof(constants));
	context->Unmap(m_constantBuffer, 0);

	context->CSSetConstantBuffers(0, 1, &m_constantBuffer);
	context->Dispatch(textureX / 8, textureY / 8, 1);

	// restore rendering state.
	context->CSSetShader(oldShader, nullptr, 0);
	context->CSSetShaderResources(0, 8, oldViews);
	context->CSSetUnorderedAccessViews(0, 8, oldUavs, NULL);
	context->CSSetConstantBuffers(0, 8, oldBuffers);
	context->OMSetRenderTargets(8, rtvs, dsv[0]);
}

typedef int RENDERDOC_GetAPI(RENDERDOC_Version version, void **outAPIPointers);
RENDERDOC_API_1_1_2* LoadRenderDoc()
{
	HMODULE mod = LoadLibraryA("C:\\Program Files\\RenderDoc\\renderdoc.dll");
	if(mod)
	{
		auto proc = (RENDERDOC_GetAPI*)GetProcAddress(mod, "RENDERDOC_GetAPI");
		RENDERDOC_API_1_1_2 *rdoc = nullptr;
		proc(eRENDERDOC_API_Version_1_1_2, (void**)&rdoc);
		rdoc->LaunchReplayUI(1, nullptr);
		return rdoc;
	}
	else
	{
		LOG(INFO) << GetLastError();
	}
	
	return nullptr;
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
		uavDesc.Format = linear ? DXGI_FORMAT_R16G16B16A16_FLOAT : DXGI_FORMAT_R10G10B10A2_UNORM;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		ID3D11Resource * backbuffer;
		HR = hdrSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&backbuffer);
		LOG(INFO) << "BB 0 HR: " << HR;

		HR = device->CreateUnorderedAccessView(backbuffer, &uavDesc, &m_backbufferUAVs[0]);
		LOG(INFO) << "UAV 0 HR: " << HR;

		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(ShaderConstants);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;
		HR = device->CreateBuffer(&cbDesc, nullptr, &m_constantBuffer);
		LOG(INFO) << "CB HR: " << HR;

		IDXGISwapChain4 *swap4;
		hdrSwapChain->QueryInterface(__uuidof(IDXGISwapChain4), (void**)&swap4);
		//hdrSwapChain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);


		//m_rdoc = LoadRenderDoc();
		m_inited = true;
	}
	
}



TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}
