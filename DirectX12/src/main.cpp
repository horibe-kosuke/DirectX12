#include <Windows.h>
#include <iostream>
#include "App.h"
int WINAPI WinMain(_In_ HINSTANCE hInstance,	//�C���X�^���X�n���h��
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,					//�����^
	_In_ int nCmdShow)
{

	//�R���\�[���\��
	AllocConsole();
	FILE* fp;

	freopen_s(&fp, "CONOUT$", "w", stdout);	//��������
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);	//�ǂݍ���
	setlocale(LC_ALL, "Japanese");			//���{�ꃏ�C�h�����Ή�

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