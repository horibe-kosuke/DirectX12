#pragma once
#include"PCH.h"
class D3d12_Init
{
	HWND		m_hwnd;			//メインウィンドウハンドル
	HINSTANCE	m_hInstance;	//インスタンスハンドル
	HRESULT		hr;				//COM生成確認
public:
	D3d12_Init(HWND& hwnd, HINSTANCE& hInstance);
	~D3d12_Init();

	//! @brief			デバイス生成
	//! @param pDevice	デバイス
	//! @return true	成功
	//! @return false	失敗
	bool Device(ID3D12Device*& pDevice);

	//! @brief					コマンドキュー生成
	//! @param  pDevice			デバイス
	//! @param  pCommandQueue	コマンドキュー
	//! @return true			成功
	bool CommandQueue(ID3D12Device*& pDevice, ID3D12CommandQueue*& pCommandQueue);



	//! @brief						コマンドリスト、コマンドアロケータ生成
	//! @param pDevice				デバイス
	//! @param pCommandAllocator	コマンドアロケータ
	//! @param pCommandList			コマンドリスト
	//! @return 
	bool CommandList(ID3D12Device*& pDevice, ID3D12CommandAllocator*& pCommandAllocator, ID3D12GraphicsCommandList*& pCommandList);

	//! @brief					スワップチェイン生成
	//! @param pCommandQueue	コマンドキュー
	//! @param pSwapChain		スワップチェイン
	//! @return 
	bool SwapChain(ID3D12CommandQueue*& pCommandQueue, IDXGISwapChain3*& pSwapChain);

	//! @brief					レンダーターゲット生成
	//! @param pDevice			デバイス
	//! @param pSwapChain		スワップチェイン	
	//! @param RenderTargets	レンダーターゲット
	//! @param RTV_Heap			レンダーターゲットヒープ
	//! @param RTV_Increment	レンダーターゲットインクリメント
	//! @return 
	bool RenderTarget(ID3D12Device*& pDevice, IDXGISwapChain3*& pSwapChain,
		ID3D12Resource* (&RenderTargets)[FrameCount], ID3D12DescriptorHeap*& RTV_Heap, UINT& RTV_Increment);

	//! @brief				フェンス生成
	//! @param pDevice		デバイス
	//! @param pFence		フェンス
	//! @param FenceValue	フェンス値
	//! @return 
	bool Fence(ID3D12Device*& pDevice, ID3D12Fence*& pFence, UINT64& FenceValue);

};

