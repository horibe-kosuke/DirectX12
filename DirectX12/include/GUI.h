#pragma once
#include "PCH.h"
#include <tchar.h>
//#include <gdiplus.h>	
#include <OleCtl.h>
//#pragma comment(lib,"Gdiplus.lib")
#pragma comment(lib, "winmm.lib")


#define GUI_NAME TEXT("GUI")
#define GUI_AddWindow 0



const LPCTSTR strItem[] = {
	TEXT("Test0") ,
	TEXT("Test1") ,
	TEXT("Test2") ,
	TEXT("Test3") ,
	TEXT("Test4")
};

const LPCTSTR Text[3] = {
	TEXT("�ʒu"),
	TEXT("��]"),
	TEXT("�X�P�[��"),
};

class GUI
{


	const enum Edit {
		POS_X = 0,	//�ʒu	X���W
		POS_Y,		//�ʒu	Y���W
		POS_Z,		//�ʒu	Z���W

		ROT_X,		//��]	X���W
		ROT_Y,		//��]	Y���W
		ROT_Z,		//��]	Z���W

		SCALE_X,	//�X�P�[��	X���W
		SCALE_Y,	//�X�P�[��	Y���W
		SCALE_Z,	//�X�P�[��	Z���W
	};



	static HWND ComboBox;

	static float* Pos[3];		//���݈ʒu
	static HWND Pos_EditBox[3];	//�ʒu�̃G�f�B�b�g�{�b�N�X

	static float* Rot[3];
	static HWND Rot_EditBox[3];	//��]�̃G�f�B�b�g�{�b�N�X

	static float* Scale[3];
	static HWND Scale_EditBox[3];



	static LRESULT WndProc(HWND hwnd, UINT uint, WPARAM wp, LPARAM lp);
	LRESULT MainProc(HWND hwnd, UINT uint, WPARAM wp, LPARAM lp);


	void GuiText(HDC hdc);
	void AddEditBox(HWND& hwnd, LPARAM lp);
	void AddWidget(HWND hwnd);

	void UpdateEdit(WPARAM wp);

public:
	static HWND m_hwnd;
	static bool UpdateEditState;


	GUI();
	~GUI();
	void WindowInit(HINSTANCE& hinstance, DirectX::XMFLOAT3* pos, DirectX::XMFLOAT3* rot, DirectX::XMFLOAT3* scale);
	void AddMenus(HWND hwnd);//���j���[�𐶐�
	void AddWindow();

	void Run();
	void Destroy();
};

