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

#pragma endregion

#pragma region 最初のシーン初期化
    
    Sprite* sprite = nullptr;
    sprite = new Sprite();
    sprite->Initialize(spriteCommon);

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

#pragma endregion

        // 描画前処理
        dxCommon->PreDraw();
        
#pragma region 最初のシーン描画
        spriteCommon->PreDraw();
        sprite->Draw();

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
