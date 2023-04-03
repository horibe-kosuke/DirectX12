#include "App.h"


//ウィンドウプロシージャ
LRESULT CALLBACK App::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	App* window = (App*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (window != nullptr) {	//取得できなかった場合
		if (msg == WM_CREATE) {
			window = (App*)((LPCREATESTRUCT)lp)->lpCreateParams;
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

//HWND App::hPush;
LRESULT App::MainProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

	switch (msg)
	{
	case WM_CREATE:
		//m_Gui.AddMenus(hwnd);
		break;
	case WM_KEYDOWN:
		switch (wp) {

		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		case VK_RETURN:
			m_Gui.AddWindow();
			break;

		default:
			break;
		}
		break;

	case WM_KEYUP:
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case GUI_AddWindow:
			m_Gui.AddWindow();
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		break;

	case WM_MOVING:

		break;
	case WM_SIZE:
		break;

	case WM_SYSCOMMAND:
		break;

	case WM_ENTERSIZEMOVE:
		break;

	case WM_SETFOCUS:
		D3D12.KeyState = true;
		GUI::UpdateEditState = true;
		break;

	case WM_KILLFOCUS:
		D3D12.KeyState = false;
		break;
	default:
		break;
	}

	//DestroyWindow(button);
	return DefWindowProc(hwnd, msg, wp, lp);
}

App::App(HINSTANCE Instance)
	: m_hwnd(NULL)
	, m_hInstance(Instance)
{
}

App::~App()
{
	m_hwnd = NULL;
	m_hInstance = NULL;

}

bool App::Init() {


	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = App::WndProc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = g_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);


	m_hwnd = CreateWindow(
		CLASS_NAME,
		CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0,
		0,
		g_hInstance,
		0
	);



	if (!m_hwnd) {
		return false;
	}


	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	//GUI用のウィンドウ生成
	m_Gui.WindowInit(g_hInstance, &D3D12.Pos, &D3D12.Rot, &D3D12.Scale);
	m_Gui.AddWindow();
	D3D12.Init(m_hwnd, g_hInstance);
	return true;
}

void App::Run() {
	//メッセージループ
	MSG msg = {};
	while (msg.message != WM_QUIT) {

		m_Gui.Run();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			D3D12.Update();

			D3D12.Render_3D();
		}


	}

	m_Gui.Destroy();
	DestroyWindow(m_hwnd);
	UnregisterClass(CLASS_NAME, m_hInstance);

}

