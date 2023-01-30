#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>

#include "DirectXCommon.h"
#include <string>


class SpriteCommon
{

public: //�����o�֐�
	void Initialize(DirectXCommon* _dxCommon);

	// �`��
	void PreDraw();

	void PostDraw();

	// �e�N�X�`���ǂݍ���
	void LoadTexture(uint32_t index, const std::string& fileName);

	// �`��p�e�N�X�`���R�}���h
	void SetTextureCommands(uint32_t index);

	DirectXCommon* GetDirectXCommon() { return dxCommon; }
	ID3D12Resource* GetTextureBuffer(uint32_t index) { return texBuff[index].Get(); }

// �ÓI�����o�ϐ�
private:
	static const size_t kMaxSRVCount = 2056;

	static std::string kDefaultTextureDirectoryPath;

private://�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	// �p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	// �e�N�X�`���o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> texBuff;
	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

};

