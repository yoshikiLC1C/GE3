#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* _spriteCommon, uint32_t textureIndex) {
    HRESULT result{};
    assert(_spriteCommon);
    spriteCommon = _spriteCommon;
    
    // �e�N�X�`���T�C�Y���C���[�W�ɍ��킹��
    if (textureIndex != UINT32_MAX) {
        this->textureIndex = textureIndex;
        AdjustTextureSize();
        size = textureSize;
    }

    // UV
    {
        ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex);

        if (textureBuffer) {
            // �e�N�X�`�����擾
            D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

            float tex_left = textureLeftTop.x / resDesc.Width;
            float tex_right = (textureLeftTop.x + textureSize.x) / resDesc.Width;
            float tex_top = textureLeftTop.y / resDesc.Height;
            float tex_bottom = (textureLeftTop.y + textureSize.y) / resDesc.Height;

            vertices[LB].uv = { tex_left,tex_bottom };
            vertices[LT].uv = { tex_left,tex_top };
            vertices[RB].uv = { tex_right,tex_bottom };
            vertices[RT].uv = { tex_right,tex_top };
        }
    }
    // ���W
    {
        float left = (0.0f - anchorPoint.x) * size.x;
        float right = (1.0f - anchorPoint.x) * size.x;
        float top = (0.0f - anchorPoint.y) * size.y;
        float bottom = (2.0f - anchorPoint.y) * size.y;

        if (IsFlipX) {
            left = -left;
            right = -right;
        }
        if (IsFlipY) {
            top = -top;
            bottom = -bottom;
        }

        // ���_�f�[�^
        vertices[LB].pos = { left, bottom,0.0f};
        vertices[LT].pos = { left,    top,0.0f};
        vertices[RB].pos = {right, bottom,0.0f};
        vertices[RT].pos = {right,    top,0.0f};
    }

	// ���_�f�[�^
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

    // ���_�o�b�t�@�̐ݒ�
    D3D12_HEAP_PROPERTIES heapProp{};   // �q�[�v�ݒ�
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // ���_�o�b�t�@�̐���
    ID3D12Resource* vertBuff;
    result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
        &heapProp, // �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff)
    );
    assert(SUCCEEDED(result));

    // �]��
    Vertex* vertMap = nullptr;
    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));
    // �S���_�ɑ΂���
    for (int i = 0; i < _countof(vertices); i++) {
        vertMap[i] = vertices[i];   // ���W���R�s�[
    }
    // �q���������
    vertBuff->Unmap(0, nullptr);

    
    // GPU���z�A�h���X
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    // ���_�o�b�t�@�̃T�C�Y
    vbView.SizeInBytes = sizeVB;
    // ���_�P���̃f�[�^�T�C�Y
    vbView.StrideInBytes = sizeof(vertices[0]);

    {
        // �萔�o�b�t�@�̃q�[�v�ݒ�
        D3D12_HEAP_PROPERTIES heapProp{};
        heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
        // �萔�o�b�t�@�̃��\�[�X�ݒ�
        D3D12_RESOURCE_DESC resdesc{};
        resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resdesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
        resdesc.Height = 1;
        resdesc.DepthOrArraySize = 1;
        resdesc.MipLevels = 1;
        resdesc.SampleDesc.Count = 1;
        resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // �萔�o�b�t�@�̐���
        result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resdesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constBufferMaterial)
        );
        assert(SUCCEEDED(result));

        // �萔�o�b�t�@�̃}�b�s���O
        result = constBufferMaterial->Map(0, nullptr, (void**)&constMapMaterial);
        assert(SUCCEEDED(result));
        constMapMaterial->color = color;
    }

    {
        D3D12_HEAP_PROPERTIES heapProp{};
        heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
        // �萔�o�b�t�@�̃��\�[�X�ݒ�
        D3D12_RESOURCE_DESC resdesc{};
        resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
        resdesc.Height = 1;
        resdesc.DepthOrArraySize = 1;
        resdesc.MipLevels = 1;
        resdesc.SampleDesc.Count = 1;
        resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // �萔�o�b�t�@�̐���
        result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resdesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constBufferTransform)
        );
        assert(SUCCEEDED(result));

        // �萔�o�b�t�@�̃}�b�s���O
        result = constBufferTransform->Map(0, nullptr, (void**)&constMapTransform);
        assert(SUCCEEDED(result));

        //���[���h
        XMMATRIX matWorld;
        matWorld = XMMatrixIdentity();

        // ��]
        XMMATRIX matRot;
        matRot = XMMatrixIdentity();
        matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
        matWorld *= matRot;

        // ���s�ړ�
        XMMATRIX matTrans;
        matTrans = XMMatrixTranslation(position.x, position.y, 0.0f);
        matWorld *= matTrans;
            
        // �ˉe�ϊ�
        XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
            0.0f, WinApp::window_width,
            WinApp::window_height, 0.0f,
            0.0f, 1.0f
        );

        constMapTransform->mat = matWorld * matProjection;

    }
}

void Sprite::Update() {
    
    // UV
    {
        ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex);

        if (textureBuffer) {
            // �e�N�X�`�����擾
            D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

            float tex_left = textureLeftTop.x / resDesc.Width;
            float tex_right = (textureLeftTop.x + textureSize.x) / resDesc.Width;
            float tex_top = textureLeftTop.y / resDesc.Height;
            float tex_bottom = (textureLeftTop.y + textureSize.y) / resDesc.Height;

            vertices[LB].uv = { tex_left,tex_bottom };
            vertices[LT].uv = { tex_left,tex_top };
            vertices[RB].uv = { tex_right,tex_bottom };
            vertices[RT].uv = { tex_right,tex_top };
        }
    }
    // ���W
    {
        float left = (0.0f - anchorPoint.x) * size.x;
        float right = (1.0f - anchorPoint.x) * size.x;
        float top = (0.0f - anchorPoint.y) * size.y;
        float bottom = (2.0f - anchorPoint.y) * size.y;

        if (IsFlipX) {
            left = -left;
            right = -right;
        }
        if (IsFlipY) {
            top = -top;
            bottom = -bottom;
        }

        // ���_�f�[�^
        vertices[LB].pos = { left, bottom,0.0f };
        vertices[LT].pos = { left,    top,0.0f };
        vertices[RB].pos = { right, bottom,0.0f };
        vertices[RT].pos = { right,    top,0.0f };
    }
    // �]��
    Vertex* vertMap = nullptr;
    HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));
    // �S���_�ɑ΂���
    for (int i = 0; i < _countof(vertices); i++) {
        vertMap[i] = vertices[i];   // ���W���R�s�[
    }
    // �q���������
    vertBuff->Unmap(0, nullptr);

    constMapMaterial->color = color;

    //���[���h
    XMMATRIX matWorld;
    matWorld = XMMatrixIdentity();

    // ��]
    XMMATRIX matRot;
    matRot = XMMatrixIdentity();
    matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
    matWorld *= matRot;

    // ���s�ړ�
    XMMATRIX matTrans;
    matTrans = XMMatrixTranslation(position.x, position.y, 0.0f);
    matWorld *= matTrans;

    // �ˉe�ϊ�
    XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
        0.0f, WinApp::window_width,
        WinApp::window_height, 0.0f,
        0.0f, 1.0f
    );

    constMapTransform->mat = matWorld * matProjection;
}

void Sprite::Draw() {

    if (IsInvisible) {
        return;
    }

    // �e�N�X�`���R�}���h
    spriteCommon->SetTextureCommands(textureIndex);

    // ���_�o�b�t�@�r���[�̐ݒ�R�}���h
    spriteCommon->GetDirectXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
    // �萔�o�b�t�@�r���[�̐ݒ�R�}���h
    spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBufferMaterial->GetGPUVirtualAddress());
    // CBV
    spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBufferTransform->GetGPUVirtualAddress());

    // �`��R�}���h
    spriteCommon->GetDirectXCommon()->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Sprite::AdjustTextureSize(){
    ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex);
    assert(textureBuffer);

    D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

    textureSize.x = static_cast<float>(resDesc.Width);
    textureSize.y = static_cast<float>(resDesc.Height);

}

