#pragma once
#include"PCH.h"
class D3d12_Init
{
	HWND		m_hwnd;			//���C���E�B���h�E�n���h��
	HINSTANCE	m_hInstance;	//�C���X�^���X�n���h��
	HRESULT		hr;				//COM�����m�F
public:
	D3d12_Init(HWND& hwnd, HINSTANCE& hInstance);
	~D3d12_Init();

	//! @brief			�f�o�C�X����
	//! @param pDevice	�f�o�C�X
	//! @return true	����
	//! @return false	���s
	bool Device(ID3D12Device*& pDevice);

	//! @brief					�R�}���h�L���[����
	//! @param  pDevice			�f�o�C�X
	//! @param  pCommandQueue	�R�}���h�L���[
	//! @return true			����
	bool CommandQueue(ID3D12Device*& pDevice, ID3D12CommandQueue*& pCommandQueue);



	//! @brief						�R�}���h���X�g�A�R�}���h�A���P�[�^����
	//! @param pDevice				�f�o�C�X
	//! @param pCommandAllocator	�R�}���h�A���P�[�^
	//! @param pCommandList			�R�}���h���X�g
	//! @return 
	bool CommandList(ID3D12Device*& pDevice, ID3D12CommandAllocator*& pCommandAllocator, ID3D12GraphicsCommandList*& pCommandList);

	//! @brief					�X���b�v�`�F�C������
	//! @param pCommandQueue	�R�}���h�L���[
	//! @param pSwapChain		�X���b�v�`�F�C��
	//! @return 
	bool SwapChain(ID3D12CommandQueue*& pCommandQueue, IDXGISwapChain3*& pSwapChain);

	//! @brief					�����_�[�^�[�Q�b�g����
	//! @param pDevice			�f�o�C�X
	//! @param pSwapChain		�X���b�v�`�F�C��	
	//! @param RenderTargets	�����_�[�^�[�Q�b�g
	//! @param RTV_Heap			�����_�[�^�[�Q�b�g�q�[�v
	//! @param RTV_Increment	�����_�[�^�[�Q�b�g�C���N�������g
	//! @return 
	bool RenderTarget(ID3D12Device*& pDevice, IDXGISwapChain3*& pSwapChain,
		ID3D12Resource* (&RenderTargets)[FrameCount], ID3D12DescriptorHeap*& RTV_Heap, UINT& RTV_Increment);

	//! @brief				�t�F���X����
	//! @param pDevice		�f�o�C�X
	//! @param pFence		�t�F���X
	//! @param FenceValue	�t�F���X�l
	//! @return 
	bool Fence(ID3D12Device*& pDevice, ID3D12Fence*& pFence, UINT64& FenceValue);

};

