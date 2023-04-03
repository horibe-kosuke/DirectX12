#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define WINDOW_WIDTH  960	//ウィンドウの横幅
#define WINDOW_HEIGHT 640	//ウィンドウの縦幅

#define FrameCount 2
#define ConstantBufferCount 5

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <iostream>
#include <assert.h>
#include <DirectXMath.h>
#include <vector>
#include "d3dx12.h"

#include "Relese.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment( lib, "d3dcompiler.lib" )

static HINSTANCE g_hInstance;







