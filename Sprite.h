#pragma once
#include <DirectXMath.h>
#include "SpriteCommon.h"


class Sprite
{
// 構造体
public:

	//頂点データ
	struct Vertex{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial {
		DirectX::XMFLOAT4 color; // 色 (RGBA)
	};

	// 定数バッファ用データ構造体（３Ｄ変換行列）
	struct ConstBufferDataTransform {
		DirectX::XMMATRIX mat;   // ３Ｄ変換行列
	};

	enum VertexNumber {
		LB,
		LT,
		RB,
		RT,
	};

public: //メンバ関数
	void Initialize(SpriteCommon* spriteCommon, uint32_t textureIndex = UINT32_MAX);

	// 更新
	void Update();

	// 描画
	void Draw();

	// Getter
	const DirectX::XMFLOAT4 GetColor() const { return color; }

	const uint32_t GetTextureIndex() const { return textureIndex; }

	const DirectX::XMFLOAT2& GetPosition() const { return position; }
	const float& GetRotationZ() const { return rotationZ; }
	const DirectX::XMFLOAT2& GetSize() const { return size; }
	const DirectX::XMFLOAT2& GetAnchorPoint() const { return anchorPoint; }

	const DirectX::XMFLOAT2& GetTextureLeftTop() const { return textureLeftTop; }
	const DirectX::XMFLOAT2& GetTextureSize() const { return textureSize; }

	const bool& GetIsFlipX() const { return IsFlipX; }
	const bool& GetIsFlipY() const { return IsFlipY; }
	const bool& GetIsInvisible() const { return IsInvisible; }


	// Setter
	void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }

	void SetTextureIndex(uint32_t index) { this->textureIndex = index; }

	void SetPosition(const DirectX::XMFLOAT2& position) { this->position = position; }
	void SetRotationZ(const float& rotationZ) { this->rotationZ = rotationZ; }
	void SetSize(const DirectX::XMFLOAT2& size) { this->size = size; }
	void SetAnchorPoint(const DirectX::XMFLOAT2& anchorPoint) { this->anchorPoint = anchorPoint; }

	void SetTextureLeftTop(const DirectX::XMFLOAT2& textureLeftTop) { this->textureLeftTop = textureLeftTop; }
	void SetTextureSize(const DirectX::XMFLOAT2& textureSize) { this->textureSize = textureSize; }

	void SetIsFlipX(const bool& isFlipX) { this->IsFlipX = isFlipX; }
	void SetIsFlipY(const bool& isFlipY) { this->IsFlipY = isFlipY; }
	void SetIsInvisible(const bool& isInvisible) { this->IsInvisible = isInvisible; }

private:
	// テクスチャサイズ
	void AdjustTextureSize();

private:
	SpriteCommon* spriteCommon = nullptr; 

	// テクスチャ番号
	uint32_t textureIndex = 0;

	//スプライト情報
	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	DirectX::XMFLOAT2 size = { 100.0f,100.0f };
	DirectX::XMFLOAT2 anchorPoint = { 0.0f,0.0f };

	

	float rotationZ = 0.0f;
	DirectX::XMFLOAT2 position = { 0.0f,0.0f };

	DirectX::XMFLOAT2 textureLeftTop = { 0.0f,0.0f };
	DirectX::XMFLOAT2 textureSize = { 100.0f,100.0f };

	bool IsFlipX = false;
	bool IsFlipY = false;

	bool IsInvisible = false;

	Vertex vertices[4];

	
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBufferMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> constBufferTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
};

