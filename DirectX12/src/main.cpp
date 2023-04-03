#include <Windows.h>
#include <iostream>
#include "App.h"
int WINAPI WinMain(_In_ HINSTANCE hInstance,	//インスタンスハンドル
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,					//文字型
	_In_ int nCmdShow)
{

	//コンソール表示
	AllocConsole();
	FILE* fp;

	freopen_s(&fp, "CONOUT$", "w", stdout);	//書き込み
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);	//読み込み
	setlocale(LC_ALL, "Japanese");			//日本語ワイド文字対応

	App* app = new App(hInstance);
	g_hInstance = hInstance;
	if (app->Init() == false) {
		delete app;
		return 0;
	}
	if (fp != NULL) {
		fclose(fp);
	}
	app->Run();
	delete app;

	return 0;
}