#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* _spriteCommon, uint32_t textureIndex) {
    HRESULT result{};
    assert(_spriteCommon);
    spriteCommon = _spriteCommon;
    
    // テクスチャサイズをイメージに合わせる
    if (textureIndex != UINT32_MAX) {
        this->textureIndex = textureIndex;
        AdjustTextureSize();
        size = textureSize;
    }

    // UV
    {
        ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex);

        if (textureBuffer) {
            // テクスチャ情報取得
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
    // 座標
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

        // 頂点データ
        vertices[LB].pos = { left, bottom,0.0f};
        vertices[LT].pos = { left,    top,0.0f};
        vertices[RB].pos = {right, bottom,0.0f};
        vertices[RT].pos = {right,    top,0.0f};
    }

	// 頂点データ
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

    // 頂点バッファの設定
    D3D12_HEAP_PROPERTIES heapProp{};   // ヒープ設定
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
    // リソース設定
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // 頂点データ全体のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 頂点バッファの生成
    ID3D12Resource* vertBuff;
    result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
        &heapProp, // ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff)
    );
    assert(SUCCEEDED(result));

    // 転送
    Vertex* vertMap = nullptr;
    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));
    // 全頂点に対して
    for (int i = 0; i < _countof(vertices); i++) {
        vertMap[i] = vertices[i];   // 座標をコピー
    }
    // 繋がりを解除
    vertBuff->Unmap(0, nullptr);

    
    // GPU仮想アドレス
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    // 頂点バッファのサイズ
    vbView.SizeInBytes = sizeVB;
    // 頂点１つ分のデータサイズ
    vbView.StrideInBytes = sizeof(vertices[0]);

    {
        // 定数バッファのヒープ設定
        D3D12_HEAP_PROPERTIES heapProp{};
        heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
        // 定数バッファのリソース設定
        D3D12_RESOURCE_DESC resdesc{};
        resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resdesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
        resdesc.Height = 1;
        resdesc.DepthOrArraySize = 1;
        resdesc.MipLevels = 1;
        resdesc.SampleDesc.Count = 1;
        resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // 定数バッファの生成
        result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resdesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constBufferMaterial)
        );
        assert(SUCCEEDED(result));

        // 定数バッファのマッピング
        result = constBufferMaterial->Map(0, nullptr, (void**)&constMapMaterial);
        assert(SUCCEEDED(result));
        constMapMaterial->color = color;
    }

    {
        D3D12_HEAP_PROPERTIES heapProp{};
        heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
        // 定数バッファのリソース設定
        D3D12_RESOURCE_DESC resdesc{};
        resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
        resdesc.Height = 1;
        resdesc.DepthOrArraySize = 1;
        resdesc.MipLevels = 1;
        resdesc.SampleDesc.Count = 1;
        resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // 定数バッファの生成
        result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resdesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constBufferTransform)
        );
        assert(SUCCEEDED(result));

        // 定数バッファのマッピング
        result = constBufferTransform->Map(0, nullptr, (void**)&constMapTransform);
        assert(SUCCEEDED(result));

        //ワールド
        XMMATRIX matWorld;
        matWorld = XMMatrixIdentity();

        // 回転
        XMMATRIX matRot;
        matRot = XMMatrixIdentity();
        matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
        matWorld *= matRot;

        // 平行移動
        XMMATRIX matTrans;
        matTrans = XMMatrixTranslation(position.x, position.y, 0.0f);
        matWorld *= matTrans;
            
        // 射影変換
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
            // テクスチャ情報取得
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
    // 座標
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

        // 頂点データ
        vertices[LB].pos = { left, bottom,0.0f };
        vertices[LT].pos = { left,    top,0.0f };
        vertices[RB].pos = { right, bottom,0.0f };
        vertices[RT].pos = { right,    top,0.0f };
    }
    // 転送
    Vertex* vertMap = nullptr;
    HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));
    // 全頂点に対して
    for (int i = 0; i < _countof(vertices); i++) {
        vertMap[i] = vertices[i];   // 座標をコピー
    }
    // 繋がりを解除
    vertBuff->Unmap(0, nullptr);

    constMapMaterial->color = color;

    //ワールド
    XMMATRIX matWorld;
    matWorld = XMMatrixIdentity();

    // 回転
    XMMATRIX matRot;
    matRot = XMMatrixIdentity();
    matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
    matWorld *= matRot;

    // 平行移動
    XMMATRIX matTrans;
    matTrans = XMMatrixTranslation(position.x, position.y, 0.0f);
    matWorld *= matTrans;

    // 射影変換
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

    // テクスチャコマンド
    spriteCommon->SetTextureCommands(textureIndex);

    // 頂点バッファビューの設定コマンド
    spriteCommon->GetDirectXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
    // 定数バッファビューの設定コマンド
    spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBufferMaterial->GetGPUVirtualAddress());
    // CBV
    spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBufferTransform->GetGPUVirtualAddress());

    // 描画コマンド
    spriteCommon->GetDirectXCommon()->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Sprite::AdjustTextureSize(){
    ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex);
    assert(textureBuffer);

    D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

    textureSize.x = static_cast<float>(resDesc.Width);
    textureSize.y = static_cast<float>(resDesc.Height);

}

