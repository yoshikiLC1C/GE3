#pragma once
#include <Windows.h>

class WinApp
{
public: // �ÓI�����o�ϐ�

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // �����o�֐�

	void Intialize();

	// �I��
	void Finalize();

	// ���b�Z�[�W�̏���
	bool ProcessMessage();

	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }

private:
	// �E�B���h�E�n���h��
	HWND hwnd = nullptr;
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};


public: // �萔
	// �E�B���h�E�T�C�Y����
	static const int window_width = 1280;
	// �E�B���h�E�T�C�Y�c��
	static const int window_height = 720;


};

