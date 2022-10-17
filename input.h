#pragma once
#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>


class Input
{
public:
	// namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �����o�֐�
	void Initialize(HINSTANCE hinstance,HWND hwnd);
	void Update();



private: // �����o�ϐ�
// �L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> keyboard;
};

