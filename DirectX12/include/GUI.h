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
	TEXT("位置"),
	TEXT("回転"),
	TEXT("スケール"),
};

class GUI
{


	const enum Edit {
		POS_X = 0,	//位置	X座標
		POS_Y,		//位置	Y座標
		POS_Z,		//位置	Z座標

		ROT_X,		//回転	X座標
		ROT_Y,		//回転	Y座標
		ROT_Z,		//回転	Z座標

		SCALE_X,	//スケール	X座標
		SCALE_Y,	//スケール	Y座標
		SCALE_Z,	//スケール	Z座標
	};



	static HWND ComboBox;

	static float* Pos[3];		//現在位置
	static HWND Pos_EditBox[3];	//位置のエディットボックス

	static float* Rot[3];
	static HWND Rot_EditBox[3];	//回転のエディットボックス

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
	void AddMenus(HWND hwnd);//メニューを生成
	void AddWindow();

	void Run();
	void Destroy();
};

