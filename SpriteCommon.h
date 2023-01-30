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
	//������
	void Initialize(DirectXCommon* _dxCommon);

	//�`��
	void PreDraw();

	void PostDraw();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="index">�e�N�X�`���ԍ�</param>
	void LoadTexture(uint32_t index, const std::string& fileName);

	/// <summary>
	/// �`��p�e�N�X�`���R�}���h�̔��s
	/// </summary>
	/// <param name="index">�e�N�X�`���ԍ�</param>
	void SetTextureCommands(uint32_t index);

	//Getter
	DirectXCommon* GetDirectXCommon() { return dxCommon; }
	ID3D12Resource* GetTextureBuffer(uint32_t index) { return texBuff[index].Get(); }
private://���I�����o�ϐ�
	static const size_t kMaxSRVCount = 2056;

	static std::string kDefaultTextureDirectoryPath;
private://�����o�ϐ�
	DirectXCommon* dxCommon;

	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr <ID3D12RootSignature>rootSignature;


	//�e�N�X�`���u�b�t�@
	std::array<Microsoft::WRL::ComPtr< ID3D12Resource>, kMaxSRVCount>texBuff;
	//�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
};