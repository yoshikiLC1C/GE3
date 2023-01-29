#pragma once
#include <d3d12.h>
#include <wrl.h>

#include "DirectXCommon.h"

class SpriteCommon
{
public: //�����o�֐�
	void Initialize(DirectXCommon* _dxCommon);

	// �`��
	void PreDraw();

	DirectXCommon* GetDirectXCommon() { return dxCommon; }

private://�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	// �p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

};

