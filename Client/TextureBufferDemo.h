#pragma once
#include "IExecute.h"

class TextureBufferDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

private:
	shared_ptr<Shader>				_shader;
	
private:
	// CS()에서 Texture을 가공한 Texturer을 받는 SRV
	ComPtr<ID3D11ShaderResourceView> MakeComputeShaderTexture();
};

