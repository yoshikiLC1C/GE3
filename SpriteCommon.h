#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>

#include "DirectXCommon.h"
#include <string>


class SpriteCommon
{

public: //メンバ関数
	void Initialize(DirectXCommon* _dxCommon);

	// 描画
	void PreDraw();

	void PostDraw();

	// テクスチャ読み込み
	void LoadTexture(uint32_t index, const std::string& fileName);

	// 描画用テクスチャコマンド
	void SetTextureCommands(uint32_t index);

	DirectXCommon* GetDirectXCommon() { return dxCommon; }
	ID3D12Resource* GetTextureBuffer(uint32_t index) { return texBuff[index].Get(); }

// 静的メンバ変数
private:
	static const size_t kMaxSRVCount = 2056;

	static std::string kDefaultTextureDirectoryPath;

private://メンバ変数
	DirectXCommon* dxCommon = nullptr;
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	// テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> texBuff;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

};

