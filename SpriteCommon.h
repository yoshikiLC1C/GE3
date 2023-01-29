#pragma once
#include <d3d12.h>
#include <wrl.h>

#include "DirectXCommon.h"

class SpriteCommon
{
public: //メンバ関数
	void Initialize(DirectXCommon* _dxCommon);

	// 描画
	void PreDraw();

	DirectXCommon* GetDirectXCommon() { return dxCommon; }

private://メンバ変数
	DirectXCommon* dxCommon = nullptr;
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

};

