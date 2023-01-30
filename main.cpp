#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE,HINSTANCE, LPSTR, int) {

#pragma region 基礎システム初期化

    WinApp* winApp = nullptr;
    winApp = new WinApp();
    winApp->Initialize();
    
    DirectXCommon* dxCommon = nullptr;
    dxCommon = new DirectXCommon();
    dxCommon ->Initialize(winApp);

    // 入力初期化
    Input* input = nullptr;
    input = new Input();
    input -> Initialize(winApp);
   
    SpriteCommon* spriteCommon = nullptr;
    spriteCommon = new SpriteCommon();
    spriteCommon->Initialize(dxCommon);
    spriteCommon->LoadTexture(0, "texture.png");
    spriteCommon->LoadTexture(1, "reimu.png");



#pragma endregion

#pragma region 最初のシーン初期化
    
    Sprite* sprite = nullptr;
    sprite = new Sprite();
    sprite->Initialize(spriteCommon, 0);
    Sprite* sprite1 = nullptr;
    sprite1 = new Sprite();
    sprite1->Initialize(spriteCommon, 1);

    DirectX::XMFLOAT2 pos = sprite->GetPosition();
    DirectX::XMFLOAT2 pos1 = sprite1->GetPosition();
    DirectX::XMFLOAT2 size = sprite->GetSize();
    DirectX::XMFLOAT2 size1 = sprite1->GetSize();
    pos = { 0,360 };
    pos1 = { 0,0 };
    size = { 100,100 };
    size1 = { 200,100 };

#pragma endregion

    // ゲームループ
    while (true) {

#pragma region 基礎システム更新

        // Windowsのメッセージ処理
        if (winApp->ProcessMessage()) {
            break;
        }
        // 入力の更新
        input->Update();

#pragma endregion

#pragma region 最初のシーン更新
        
        
        pos.x += 3;

        sprite->SetPosition(pos);
        sprite1->SetPosition(pos1);
        sprite->SetSize(size);
        sprite1->SetSize(size1);

        sprite->Update();
        sprite1->Update();


#pragma endregion

        // 描画前処理
        dxCommon->PreDraw();
        
#pragma region 最初のシーン描画
        spriteCommon->PreDraw();
        sprite->Draw();
        sprite1->Draw();
        spriteCommon->PostDraw();

#pragma endregion

        // 描画後処理
        dxCommon->PostDraw();

    }

#pragma region 最初のシーン終了

    delete sprite;

#pragma endregion

#pragma region 基礎システム終了

    delete spriteCommon;

    // 入力解放
    delete input;
    // DirectX解放
    delete dxCommon;
    // ウィンドウクラスを登録解除
    winApp->Finalize();
    // WindowsAPI解放
    delete winApp;
    
#pragma endregion
    return 0;
}
