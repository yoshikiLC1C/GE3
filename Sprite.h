#pragma once
#include <DirectXMath.h>
#include "SpriteCommon.h"


class Sprite
{
public: //メンバ関数
	void Initialize(SpriteCommon* spriteCommon);

	// 描画
	void Draw();

private:
	SpriteCommon* spriteCommon = nullptr; 

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

};

