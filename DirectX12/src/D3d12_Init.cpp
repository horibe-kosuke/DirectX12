#include "D3d12_Init.h"


D3d12_Init::D3d12_Init(HWND& hwnd, HINSTANCE& hInstance)
	:hr(NULL)
	, m_hwnd(hwnd)
	, m_hInstance(hInstance)
{

}

D3d12_Init::~D3d12_Init()
{
	hr = NULL;

}



bool D3d12_Init::Device(ID3D12Device*& pDevice) {
	hr = D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&pDevice));
	if (FAILED(hr)) {
		printf("デバイス生成		:ERROR\n");
		return false;
	}
	else {
		printf("デバイス生成		:OK\n");
	}

	return true;
}

bool D3d12_Init::CommandQueue(ID3D12Device*& pDevice, ID3D12CommandQueue*& pCommandQueue) {
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	hr = pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&pCommandQueue));
	if (FAILED(hr)) {
		printf("コマンドキュー		:ERROR\n");
		return false;
	}
	else {
		printf("コマンドキュー		:OK\n");
	}

	return true;
}

bool D3d12_Init::CommandList(ID3D12Device*& pDevice, ID3D12CommandAllocator*& pCommandAllocator, ID3D12GraphicsCommandList*& pCommandList)
{

	//コマンドアロケータ生成
	hr = pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&pCommandAllocator)
	);
	if (FAILED(hr)) {
		printf("コマンドアロケータ	:ERROR\n");
		return false;
	}
	else {
		printf("コマンドアロケータ	:OK\n");
	}



	//コマンドリスト生成
	hr = pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		pCommandAllocator,
		nullptr,
		IID_PPV_ARGS(&pCommandList)
	);

	if (FAILED(hr)) {
		printf("コマンドリスト		:ERROR\n");
		return false;
	}
	else {
		printf("コマンドリスト		:OK\n");
	}



	return true;


}

bool D3d12_Init::SwapChain(ID3D12CommandQueue*& pCommandQueue, IDXGISwapChain3*& pSwapChain) {

	//ファクトリーの生成
	IDXGIFactory4* pFactory = nullptr;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
	if (FAILED(hr)) {
		printf("ファクトリ		:ERROR\n");
	}
	else {
		printf("ファクトリ		:OK\n");
	}

	//スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = FrameCount;
	desc.Width = WINDOW_WIDTH;
	desc.Height = WINDOW_HEIGHT;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;


	//スワップチェインの取得
	hr = pFactory->CreateSwapChainForHwnd(pCommandQueue, m_hwnd, &desc, NULL, NULL, (IDXGISwapChain1**)&pSwapChain);

	if (FAILED(hr)) {
		printf("スワップチェイン	:ERROR\n");
		pFactory->Release();
		return false;
	}
	else {
		printf("スワップチェイン	:OK\n");
	}

	//破棄
	pFactory->Release();

	return true;

}

bool D3d12_Init::RenderTarget(ID3D12Device*& pDevice,
	IDXGISwapChain3*& pSwapChain,
	ID3D12Resource* (&RenderTargets)[FrameCount],
	ID3D12DescriptorHeap*& RTV_Heap,
	UINT& RTV_Increment
)
{

	//ディスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FrameCount;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	//ディスクリプタヒープ生成
	hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&RTV_Heap));
	if (FAILED(hr)) {
		printf("ディスクリプタヒープ	:ERROR\n");
		return false;
	}
	else {
		printf("ディスクリプタヒープ	;OK\n");
	}

	//D3D12_CPU_DESCRIPTOR_HANDLE handle = RTV_Heap->GetCPUDescriptorHandleForHeapStart();
	//incrementsize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = RTV_Heap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < FrameCount; i++) {
		hr = pSwapChain->GetBuffer(i, IID_PPV_ARGS(&RenderTargets[i]));
		if (FAILED(hr)) {
			printf("レンダーターゲット[%d]	:ERROR\n", i);
			return false;
		}
		else {
			printf("レンダーターゲット[%d]	:OK\n", i);
		}

		D3D12_RENDER_TARGET_VIEW_DESC ViewDesc = {};
		ViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		ViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		ViewDesc.Texture2D.MipSlice = 0;
		ViewDesc.Texture2D.PlaneSlice = 0;


		//レンダーターゲットビューの生成
		//pDevice->CreateRenderTargetView(RenderTargets[i], NULL, handle);
		pDevice->CreateRenderTargetView(RenderTargets[i], NULL, rtvHandle);
		//rtvHandle.Offset(1,RTV_Increment);
		rtvHandle.ptr += RTV_Increment;
	}

	return true;
}

bool D3d12_Init::Fence(ID3D12Device*& pDevice, ID3D12Fence*& pFence, UINT64& FenceValue) {

	//フェンスカウンター初期化
	hr = pDevice->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&pFence)
	);

	if (FAILED(hr)) {
		printf("フェンス		:ERROR\n");
		return false;
	}
	else {
		printf("フェンス		:OK\n");
	}

	FenceValue = 1;
	return true;

}



