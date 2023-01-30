#pragma once
#include <DirectXMath.h>
#include "SpriteCommon.h"


class Sprite
{
// �\����
public:

	//���_�f�[�^
	struct Vertex{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
	struct ConstBufferDataMaterial {
		DirectX::XMFLOAT4 color; // �F (RGBA)
	};

	// �萔�o�b�t�@�p�f�[�^�\���́i�R�c�ϊ��s��j
	struct ConstBufferDataTransform {
		DirectX::XMMATRIX mat;   // �R�c�ϊ��s��
	};

	enum VertexNumber {
		LB,
		LT,
		RB,
		RT,
	};

public: //�����o�֐�
	void Initialize(SpriteCommon* spriteCommon, uint32_t textureIndex = UINT32_MAX);

	// �X�V
	void Update();

	// �`��
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
	// �e�N�X�`���T�C�Y
	void AdjustTextureSize();

private:
	SpriteCommon* spriteCommon = nullptr; 

	// �e�N�X�`���ԍ�
	uint32_t textureIndex = 0;

	//�X�v���C�g���
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
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBufferMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> constBufferTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
};

