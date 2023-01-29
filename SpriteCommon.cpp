#include "SpriteCommon.h"

#include <d3dcompiler.h>
#include <cassert>
#include <string>

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;

void SpriteCommon::Initialize(DirectXCommon* _dxCommon) {

    HRESULT result{};
    assert(_dxCommon);
    dxCommon = _dxCommon;

    ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
    ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
    ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"Resources/shaders/SpriteVS.hlsl",  // �V�F�[�_�t�@�C����
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
        "main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
        0,
        &vsBlob, &errorBlob);
    // �G���[�Ȃ�
    if (FAILED(result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"Resources/shaders/SpritePS.hlsl",   // �V�F�[�_�t�@�C����
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
        "main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
        0,
        &psBlob, &errorBlob);
    // �G���[�Ȃ�
    if (FAILED(result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }

    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
       { // xyz���W(1�s�ŏ������ق������₷��)
           "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
           D3D12_APPEND_ALIGNED_ELEMENT,
           D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
       },
    };

    // �O���t�B�b�N�X�p�C�v���C���ݒ�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
    // �V�F�[�_�[�̐ݒ�
    pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
    // �T���v���}�X�N�̐ݒ�
    pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
    // ���X�^���C�U�̐ݒ�
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �J�����O���Ȃ�
    //pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // �w�ʂ��J�����O
    pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
    //pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ���C���[�t���[��
    pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
    // �u�����h�X�e�[�g
    pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA�S�Ẵ`�����l����`��
    // ���_���C�A�E�g�̐ݒ�
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
    // �}�`�̌`��ݒ�
    pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // ���̑��̐ݒ�
    pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
    pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
    pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O
    
    // ���[�g�V�O�l�`��
    // ���[�g�V�O�l�`���̐ݒ�
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    // ���[�g�V�O�l�`���̃V���A���C�Y
    ID3DBlob* rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    assert(SUCCEEDED(result));
    result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
    assert(SUCCEEDED(result));
    rootSigBlob->Release();
    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    pipelineDesc.pRootSignature = rootSignature.Get();
    // �p�C�v�����X�e�[�g�̐���
    result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
    assert(SUCCEEDED(result));

}


void SpriteCommon::PreDraw() {

    // �v���~�e�B�u�`��̐ݒ�R�}���h
    dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
    // �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
    dxCommon->GetCommandList()->SetPipelineState(pipelineState.Get());
    dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
}