#pragma once
#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include<array>
#include<string>

#include"DirectXCommon.h"

class SpriteCommon
{
public:
	//初期化
	void Initialize(DirectXCommon* _dxCommon);

	//描画
	void PreDraw();

	void PostDraw();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="index">テクスチャ番号</param>
	void LoadTexture(uint32_t index, const std::string& fileName);

	/// <summary>
	/// 描画用テクスチャコマンドの発行
	/// </summary>
	/// <param name="index">テクスチャ番号</param>
	void SetTextureCommands(uint32_t index);

	//Getter
	DirectXCommon* GetDirectXCommon() { return dxCommon; }
	ID3D12Resource* GetTextureBuffer(uint32_t index) { return texBuff[index].Get(); }
private://性的メンバ変数
	static const size_t kMaxSRVCount = 2056;

	static std::string kDefaultTextureDirectoryPath;
private://メンバ変数
	DirectXCommon* dxCommon;

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr <ID3D12RootSignature>rootSignature;


	//テクスチャブッファ
	std::array<Microsoft::WRL::ComPtr< ID3D12Resource>, kMaxSRVCount>texBuff;
	//デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
};