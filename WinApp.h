#pragma once
#include <Windows.h>

class WinApp
{
public: // 静的メンバ変数

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数

	void Intialize();

	// 終了
	void Finalize();

	// メッセージの処理
	bool ProcessMessage();

	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }

private:
	// ウィンドウハンドル
	HWND hwnd = nullptr;
	// ウィンドウクラスの設定
	WNDCLASSEX w{};


public: // 定数
	// ウィンドウサイズ横幅
	static const int window_width = 1280;
	// ウィンドウサイズ縦幅
	static const int window_height = 720;


};

