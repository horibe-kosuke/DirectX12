#include "GUI.h"

//static
bool GUI::UpdateEditState;
HWND GUI::m_hwnd;
HWND GUI::ComboBox;

HWND GUI::Pos_EditBox[3];
float* GUI::Pos[3];

HWND GUI::Rot_EditBox[3];
float* GUI::Rot[3];

HWND GUI::Scale_EditBox[3];
float* GUI::Scale[3];


GUI::GUI()
{

}

GUI::~GUI()
{
}

void GUI::WindowInit(HINSTANCE& hinstance, DirectX::XMFLOAT3* pos, DirectX::XMFLOAT3* rot, DirectX::XMFLOAT3* scale)
{

	Pos[0] = &pos->x;
	Pos[1] = &pos->y;
	Pos[2] = &pos->z;

	Rot[0] = &rot->x;
	Rot[1] = &rot->y;
	Rot[2] = &rot->z;

	Scale[0] = &scale->x;
	Scale[1] = &scale->y;
	Scale[2] = &scale->z;
}


//ウィンドウプロシージャ
LRESULT CALLBACK GUI::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	GUI* window = (GUI*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (window != nullptr) {	//取得できなかった場合
		if (msg == WM_CREATE) {
			window = (GUI*)((LPCREATESTRUCT)lp)->lpCreateParams;
			if (window) {
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)lp);
				return window->MainProc(hwnd, msg, wp, lp);
			}
		}
	}
	else {	//
		return window->MainProc(hwnd, msg, wp, lp);
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}

LRESULT GUI::MainProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

	TCHAR str[256];

	HDC hdc;
	PAINTSTRUCT ps;
	HFONT hFont;
	HWND Get;
	HWND hForcus = GetFocus();
	switch (msg)
	{
	case WM_CREATE:
		//m_IExplorer.AddWindow();
		AddEditBox(hwnd, lp);
		SetTimer(hwnd, 1, 100, NULL);

		ComboBox = CreateWindow(
			TEXT("COMBOBOX"), NULL,
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWNLIST,
			0, 0, 300, 300, hwnd, (HMENU)1,
			((LPCREATESTRUCT)(lp))->hInstance, NULL
		);

		for (int i = 0; i < 5; i++) {
			SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)strItem[i]);
		}
		break;

	case WM_QUIT:

		break;
	case WM_KEYDOWN:
		//IsDialogMessageがある場合 TAB ESC ENTER は設定できない
		switch (wp) {

		case 'Q':
			printf("AA\n");
			break;
		case VK_ESCAPE:
			printf("AA\n");
			break;
		}
		break;

	case WM_COMMAND:

		//エディットボックスに変更があったら更新
		UpdateEdit(wp);

		if (HIWORD(wp) == EN_SETFOCUS) {
			UpdateEditState = false;
			SendMessage((HWND)wp, EM_SETSEL, 0, -1);
		}


		//コンボボックス
		if (HIWORD(wp) == CBN_SELCHANGE) {

		}


		break;

	case WM_DESTROY:
		Destroy();

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hFont = CreateFont(
			30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			VARIABLE_PITCH | FF_ROMAN, NULL
		);
		SelectObject(hdc, hFont);
		GuiText(hdc);

		EndPaint(hwnd, &ps);

		break;

	case WM_MOVING:
		break;
	case WM_SIZE:
		break;

	case WM_KILLFOCUS:
		break;

	case WM_SETFOCUS:
		SetFocus(m_hwnd);

		break;

	case WM_TIMER:
		if (UpdateEditState == true) {
			for (int i = 0; i < 3; i++) {
				swprintf_s(str, 256, TEXT("%.2f"), *Pos[i]);
				SetWindowText(Pos_EditBox[i], str);

				swprintf_s(str, 256, TEXT("%.2f"), *Rot[i]);
				SetWindowText(Rot_EditBox[i], str);

				swprintf_s(str, 256, TEXT("%.2f"), *Scale[i]);
				SetWindowText(Scale_EditBox[i], str);
			}
		}
		break;

	case WM_ACTIVATE:

		break;

	case WM_GETDLGCODE:

		break;
	default:

		break;
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}

void GUI::AddMenus(HWND hwnd)
{
	HMENU DebugTool = CreateMenu();
	HMENU Tool = CreateMenu();
	AppendMenu(DebugTool, MF_STRING, NULL, L"デバッグ用ツール");

	AppendMenu(Tool, MF_POPUP, (UINT_PTR)DebugTool, L"ツール");

	SetMenu(hwnd, Tool);

}

void GUI::AddWidget(HWND hwnd)
{
}

void GUI::AddWindow()
{

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = GUI::WndProc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = g_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GUI_NAME;

	RegisterClass(&wc);

	m_hwnd = CreateWindow(
		GUI_NAME,
		GUI_NAME,
		WS_OVERLAPPEDWINDOW,
		1000,
		300,
		800,
		600,
		0,
		0,
		g_hInstance,
		0
	);

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

}

void GUI::AddEditBox(HWND& hwnd, LPARAM lp)
{

	TCHAR* str = new TCHAR[256];

	//位置エディットボックス生成
	{
		// X座標のエディットボックス
		swprintf_s(str, 256, TEXT("%.2f"), *Pos[0]);
		Pos_EditBox[0] = CreateWindow(
			TEXT("EDIT"), str,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT,
			170, 35, 80, 25, hwnd, (HMENU)POS_X,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
		);

		// Y座標のエディットボックス
		swprintf_s(str, 256, TEXT("%.2f"), *Pos[1]);
		Pos_EditBox[1] = CreateWindow(
			TEXT("EDIT"), str,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT,
			290, 35, 80, 25, hwnd, (HMENU)POS_Y,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
		);

		// Z座標のエディットボックス
		swprintf_s(str, 256, TEXT("%.2f"), *Pos[2]);
		Pos_EditBox[2] = CreateWindow(
			TEXT("EDIT"), str,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT,
			410, 35, 80, 25, hwnd, (HMENU)POS_Z,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
		);
	}

	//回転エディットボックス生成
	{
		swprintf_s(str, 256, TEXT("%.2f"), *Rot[0]);
		Rot_EditBox[0] = CreateWindow(
			TEXT("EDIT"), str,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT,
			170, 95, 80, 25, hwnd, (HMENU)ROT_X,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
		);

		swprintf_s(str, 256, TEXT("%.2f"), *Rot[1]);
		Rot_EditBox[0] = CreateWindow(
			TEXT("EDIT"), str,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT,
			290, 95, 80, 25, hwnd, (HMENU)ROT_Y,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
		);

		swprintf_s(str, 256, TEXT("%.2f"), *Rot[2]);
		Rot_EditBox[0] = CreateWindow(
			TEXT("EDIT"), str,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT,
			410, 95, 80, 25, hwnd, (HMENU)ROT_Z,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
		);
	}

	//スケールエディットボックス生成
	{
		swprintf_s(str, 256, TEXT("%.2f"), *Scale[0]);
		Scale_EditBox[0] = CreateWindow(
			TEXT("EDIT"), str,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT,
			170, 155, 80, 25, hwnd, (HMENU)SCALE_X,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
		);

		swprintf_s(str, 256, TEXT("%.2f"), *Scale[1]);
		Scale_EditBox[1] = CreateWindow(
			TEXT("EDIT"), str,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT,
			290, 155, 80, 25, hwnd, (HMENU)SCALE_Y,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
		);

		swprintf_s(str, 256, TEXT("%.2f"), *Scale[2]);
		Scale_EditBox[2] = CreateWindow(
			TEXT("EDIT"), str,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT,
			410, 155, 80, 25, hwnd, (HMENU)SCALE_Z,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
		);
	}

	SetFocus(Pos_EditBox[0]);

	delete[] str;
}


void GUI::UpdateEdit(WPARAM wp) {
	TCHAR str[256] = L"";
	float pos[3] = {};
	float scale[3] = {};
	float rot[3] = {};
	if (LOWORD(wp) < 12 && HIWORD(wp) == EN_UPDATE) {
		switch (LOWORD(wp)) {
		case POS_X:
			GetWindowText(Pos_EditBox[0], str, 256);
			if (Pos_EditBox[0] != NULL) {
				pos[0] = _wtof(str);
				*Pos[0] = pos[0];
			}

			break;

		case POS_Y:
			GetWindowText(Pos_EditBox[1], str, 256);
			if (Pos_EditBox[1] != NULL) {
				pos[1] = _wtof(str);
				*Pos[1] = pos[1];
			}
			break;

		case POS_Z:
			GetWindowText(Pos_EditBox[2], str, 256);
			if (Pos_EditBox[2] != NULL) {
				pos[2] = _wtof(str);
				*Pos[2] = pos[2];
			}
			break;

		case ROT_X:
			GetWindowText(Rot_EditBox[0], str, 256);
			if (Rot_EditBox[0] != NULL) {
				rot[0] = _wtof(str);
				*Rot[0] = rot[0];
			}
			break;

		case ROT_Y:
			GetWindowText(Rot_EditBox[1], str, 256);
			if (Rot_EditBox[1] != NULL) {
				rot[1] = _wtof(str);
				*Rot[1] = rot[1];
			}
			break;

		case ROT_Z:
			GetWindowText(Rot_EditBox[2], str, 256);
			if (Rot_EditBox[2] != NULL) {
				rot[2] = _wtof(str);
				*Rot[2] = rot[2];
			}
			break;


		case SCALE_X:
			GetWindowText(Scale_EditBox[0], str, 256);
			if (Scale_EditBox[0] != NULL) {
				scale[0] = _wtof(str);
				*Scale[0] = scale[0];
			}
			break;

		case SCALE_Y:
			GetWindowText(Scale_EditBox[1], str, 256);
			if (Scale_EditBox[1] != NULL) {
				scale[1] = _wtof(str);
				*Scale[1] = scale[1];
			}
			break;

		case SCALE_Z:
			GetWindowText(Scale_EditBox[2], str, 256);
			if (Scale_EditBox[2] != NULL) {
				scale[2] = _wtof(str);
				*Scale[2] = scale[2];
			}
			break;
		}

	}
}
void GUI::GuiText(HDC hdc)
{
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(0, 0, 0));
	TextOut(hdc, 10, 30, Text[0], lstrlen(Text[0]));

	TextOut(hdc, 10, 90, Text[1], lstrlen(Text[1]));

	TextOut(hdc, 10, 150, Text[2], lstrlen(Text[2]));


	for (int i = 0; i < 3; i++) {
		SetTextColor(hdc, RGB(255, 100, 100));
		TextOut(hdc, 140, 30 + (i * 60), TEXT("X"), lstrlen(TEXT("X")));

		SetTextColor(hdc, RGB(0, 180, 0));
		TextOut(hdc, 260, 30 + (i * 60), TEXT("Y"), lstrlen(TEXT("Y")));

		SetTextColor(hdc, RGB(150, 150, 255));
		TextOut(hdc, 380, 30 + (i * 60), TEXT("Z"), lstrlen(TEXT("Z")));
	}
}

void GUI::Run() {
	MSG msg = {};

	if (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE) == TRUE)
	{
		if (!IsDialogMessage(m_hwnd, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


}





void GUI::Destroy()
{
	DestroyWindow(m_hwnd);
	UnregisterClass(GUI_NAME, g_hInstance);
}
