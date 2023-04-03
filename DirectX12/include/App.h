#pragma once
#include <Windows.h>
#include "D3D12_App.h"
#include "GUI.h"
#define CLASS_NAME TEXT("GAME")
class App
{
	//GUI
	//static HWND hPush;


	//�E�B���h�E�\���ŕK�v�Ȃ���
	HWND				m_hwnd;						//���C���E�B���h�E�n���h��
	HINSTANCE			m_hInstance;					//�C���X�^���X�n���h��


	//DirectX12�̏���
	D3d12 D3D12;


	//GUI
	GUI m_Gui;
public:
	App(HINSTANCE Instance);						//�R���X�g���N�^

	~App();											//�f�X�g���N�^
	bool Init();									//������
	void Run();										//�N��
	static LRESULT WndProc(HWND hwnd, UINT uint, WPARAM wp, LPARAM lp);
	LRESULT MainProc(HWND hwnd, UINT uint, WPARAM wp, LPARAM lp);

};

