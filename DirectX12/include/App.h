#pragma once
#include <Windows.h>
#include "D3D12_App.h"
#include "GUI.h"
#define CLASS_NAME TEXT("GAME")
class App
{
	//GUI
	//static HWND hPush;


	//ウィンドウ表示で必要なもの
	HWND				m_hwnd;						//メインウィンドウハンドル
	HINSTANCE			m_hInstance;					//インスタンスハンドル


	//DirectX12の処理
	D3d12 D3D12;


	//GUI
	GUI m_Gui;
public:
	App(HINSTANCE Instance);						//コンストラクタ

	~App();											//デストラクタ
	bool Init();									//初期化
	void Run();										//起動
	static LRESULT WndProc(HWND hwnd, UINT uint, WPARAM wp, LPARAM lp);
	LRESULT MainProc(HWND hwnd, UINT uint, WPARAM wp, LPARAM lp);

};

