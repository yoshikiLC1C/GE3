#include "SpriteCommon.h"

#include <d3dcompiler.h>
#include <cassert>
#include <string>
#include <DirectXTex.h>

#include <d3dx12.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

std::string SpriteCommon::kDefaultTextureDirectoryPath = "Resources/";

void SpriteCommon::Initialize(DirectXCommon* _dxCommon) {

    HRESULT result{};
    assert(_dxCommon);
    dxCommon = _dxCommon;

    
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    srvHeapDesc.NumDescriptors = kMaxSRVCount;

    result = dxCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
    assert(SUCCEEDED(result));


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
       { // uv���W(1�s�ŏ������ق������₷��)
           "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
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
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
    blenddesc.BlendEnable = true; // �u�����h��L���ɂ���
    //����
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;    // ���Z
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;      // �\�[�X�̒l��100% �g��
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;    // �f�X�g�̒l��  0% �g��
    // ����������
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;             // ���Z
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;         // �\�[�X�̃A���t�@�l
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;    // 1.0f-�\�[�X�̃A���t�@�l
    // ���_���C�A�E�g�̐ݒ�
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
    // �}�`�̌`��ݒ�
    pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // ���̑��̐ݒ�
    pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
    pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
    pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O
    
    // �f�X�N���v�^�����W�̐ݒ�
    D3D12_DESCRIPTOR_RANGE descriptorRange{};
    descriptorRange.NumDescriptors = 1;         //��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
    descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange.BaseShaderRegister = 0;     //�e�N�X�`�����W�X�^0��
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    
    // ���[�g�p�����[�^�̐ݒ�
    D3D12_ROOT_PARAMETER rootParams[3] = {};
    // �萔�o�b�t�@0��
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // ���
    rootParams[0].Descriptor.ShaderRegister = 0;                   // �萔�o�b�t�@�ԍ�
    rootParams[0].Descriptor.RegisterSpace = 0;                    // �f�t�H���g�l
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // �S�ẴV�F�[�_���猩���� 
    // �e�N�X�`�����W�X�^0��
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   //���
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
    rootParams[1].DescriptorTable.NumDescriptorRanges = 1;              //�f�X�N���v�^�����W��
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;       //�S�ẴV�F�[�_���猩����
    // �萔�o�b�t�@1��
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // ���
    rootParams[2].Descriptor.ShaderRegister = 1;                   // �萔�o�b�t�@�ԍ�
    rootParams[2].Descriptor.RegisterSpace = 0;                    // �f�t�H���g�l
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // �S�ẴV�F�[�_���猩����

    // �e�N�X�`���T���v���[�̐ݒ�
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //���J��Ԃ��i�^�C�����O�j
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //�c�J��Ԃ��i�^�C�����O�j
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //���s�J��Ԃ��i�^�C�����O�j
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;  //�{�[�_�[�̎��͍�
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                   //�S�ă��j�A���
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                 //�~�b�v�}�b�v�ő�l
    samplerDesc.MinLOD = 0.0f;                                              //�~�b�v�}�b�v�ŏ��l
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;           //�s�N�Z���V�F�[�_����̂ݎg�p�\

    // ���[�g�V�O�l�`��
    // ���[�g�V�O�l�`���̐ݒ�
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams; //���[�g�p�����[�^�̐擪�A�h���X
    rootSignatureDesc.NumParameters = _countof(rootParams);        //���[�g�p�����[�^��
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;
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

    // �f�X�N���v�^�q�[�v
    ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
    dxCommon->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void SpriteCommon::PostDraw(){

}

void SpriteCommon::LoadTexture(uint32_t index, const std::string& fileName){

    std::string fullPath = kDefaultTextureDirectoryPath + fileName;

    int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);

    std::vector<wchar_t> wfilePath(filePathBufferSize);
    MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath.data(), filePathBufferSize);

    TexMetadata metadata{};
    ScratchImage scratchImg{};

    HRESULT result = LoadFromWICFile(
        wfilePath.data(),
        WIC_FLAGS_NONE,
        &metadata, scratchImg
    );
    assert(SUCCEEDED(result));

    ScratchImage mipChain{};
    result = GenerateMipMaps(
        scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
        TEX_FILTER_DEFAULT, 0, mipChain
    );
    if (SUCCEEDED(result)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }

    metadata.format = MakeSRGB(metadata.format);

    //�e�N�X�`��
    D3D12_HEAP_PROPERTIES textureHeapProp{};
    textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = (UINT)metadata.height;
    textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
    textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
    textureResourceDesc.SampleDesc.Count = 1;

    result = dxCommon->GetDevice()->CreateCommittedResource(
        &textureHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&texBuff[index])
    );

    // �]��
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        const Image* img = scratchImg.GetImage(i, 0, 0);

        result = texBuff[index]->WriteToSubresource(
            (UINT)i,
            nullptr,
            img->pixels,
            (UINT)img->rowPitch,
            (UINT)img->slicePitch
        );
        assert(SUCCEEDED(result));
    }

    // SRV�쐬
    //D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension - D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;


    UINT descriptorhandleIncrementSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    dxCommon->GetDevice()->CreateShaderResourceView
    (
        texBuff[index].Get(),
        &srvDesc,
        CD3DX12_CPU_DESCRIPTOR_HANDLE(
            srvHeap->GetCPUDescriptorHandleForHeapStart(),
            index,
            descriptorhandleIncrementSize)
    );

}

void SpriteCommon::SetTextureCommands(uint32_t index)
{
    //SRV�q�[�v
    UINT descriptorhandleIncrementSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(
            srvHeap->GetGPUDescriptorHandleForHeapStart(),
            index,
            descriptorhandleIncrementSize)
    );
}
