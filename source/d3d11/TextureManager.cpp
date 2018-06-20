#include "TextureManager.h"
#include "log.hpp"


TextureManager TextureManager::instance;

void TextureManager::AddTexture(ID3D11Texture2D * tex)
{
	//LOG(INFO) << "Tracking Texture" << (intptr_t)tex;
	allTextures.push_back(tex);
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

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}
