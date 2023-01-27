#pragma once
#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>
#include "WinApp.h"


class Input
{
public:
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	void Initialize(WinApp* winApp);
	void Update();

	/// <summary>
	/// 入力
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0等)</param>
	/// <returns>押されているか</returns>
	bool TriggerKey(BYTE keyNumber);


private: // メンバ変数
	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;

	// DirectInputの初期化
	ComPtr<IDirectInput8> directInput;

	// 全キーの状態
	BYTE key[256] = {};

	// 前回の全キー状態
	BYTE keyPre[256] = {};

	//WindowsAPI
	WinApp* winApp = nullptr;

};

