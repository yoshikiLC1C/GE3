#pragma once
#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>


class Input
{
public:
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	void Initialize(HINSTANCE hinstance,HWND hwnd);
	void Update();



private: // メンバ変数
// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;
};

