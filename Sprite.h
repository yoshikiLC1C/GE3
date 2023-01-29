#pragma once
#include <DirectXMath.h>
#include "SpriteCommon.h"


class Sprite
{
public: //�����o�֐�
	void Initialize(SpriteCommon* spriteCommon);

	// �`��
	void Draw();

private:
	SpriteCommon* spriteCommon = nullptr; 

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

};

