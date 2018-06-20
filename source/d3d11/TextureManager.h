#pragma once
#include <vector>
#include "d3d11_device.hpp"
#include "d3d11_runtime.hpp"


class TextureManager
{
public:
	static TextureManager instance;

	std::vector<ID3D11RenderTargetView*> allRenderTargets;
	std::vector<ID3D11Texture2D*> allTextures;
	std::vector<reshade::d3d11::d3d11_tex_data> texData;
	void AddTexture(ID3D11Texture2D* tex);
	void AddRTV(ID3D11RenderTargetView* rtv);



	TextureManager();
	~TextureManager();
};

