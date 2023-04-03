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
		printf("�f�o�C�X����		:ERROR\n");
		return false;
	}
	else {
		printf("�f�o�C�X����		:OK\n");
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
		printf("�R�}���h�L���[		:ERROR\n");
		return false;
	}
	else {
		printf("�R�}���h�L���[		:OK\n");
	}

	return true;
}

bool D3d12_Init::CommandList(ID3D12Device*& pDevice, ID3D12CommandAllocator*& pCommandAllocator, ID3D12GraphicsCommandList*& pCommandList)
{

	//�R�}���h�A���P�[�^����
	hr = pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&pCommandAllocator)
	);
	if (FAILED(hr)) {
		printf("�R�}���h�A���P�[�^	:ERROR\n");
		return false;
	}
	else {
		printf("�R�}���h�A���P�[�^	:OK\n");
	}



	//�R�}���h���X�g����
	hr = pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		pCommandAllocator,
		nullptr,
		IID_PPV_ARGS(&pCommandList)
	);

	if (FAILED(hr)) {
		printf("�R�}���h���X�g		:ERROR\n");
		return false;
	}
	else {
		printf("�R�}���h���X�g		:OK\n");
	}



	return true;


}

bool D3d12_Init::SwapChain(ID3D12CommandQueue*& pCommandQueue, IDXGISwapChain3*& pSwapChain) {

	//�t�@�N�g���[�̐���
	IDXGIFactory4* pFactory = nullptr;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
	if (FAILED(hr)) {
		printf("�t�@�N�g��		:ERROR\n");
	}
	else {
		printf("�t�@�N�g��		:OK\n");
	}

	//�X���b�v�`�F�C���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = FrameCount;
	desc.Width = WINDOW_WIDTH;
	desc.Height = WINDOW_HEIGHT;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;


	//�X���b�v�`�F�C���̎擾
	hr = pFactory->CreateSwapChainForHwnd(pCommandQueue, m_hwnd, &desc, NULL, NULL, (IDXGISwapChain1**)&pSwapChain);

	if (FAILED(hr)) {
		printf("�X���b�v�`�F�C��	:ERROR\n");
		pFactory->Release();
		return false;
	}
	else {
		printf("�X���b�v�`�F�C��	:OK\n");
	}

	//�j��
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

	//�f�B�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FrameCount;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	//�f�B�X�N���v�^�q�[�v����
	hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&RTV_Heap));
	if (FAILED(hr)) {
		printf("�f�B�X�N���v�^�q�[�v	:ERROR\n");
		return false;
	}
	else {
		printf("�f�B�X�N���v�^�q�[�v	;OK\n");
	}

	//D3D12_CPU_DESCRIPTOR_HANDLE handle = RTV_Heap->GetCPUDescriptorHandleForHeapStart();
	//incrementsize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = RTV_Heap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < FrameCount; i++) {
		hr = pSwapChain->GetBuffer(i, IID_PPV_ARGS(&RenderTargets[i]));
		if (FAILED(hr)) {
			printf("�����_�[�^�[�Q�b�g[%d]	:ERROR\n", i);
			return false;
		}
		else {
			printf("�����_�[�^�[�Q�b�g[%d]	:OK\n", i);
		}

		D3D12_RENDER_TARGET_VIEW_DESC ViewDesc = {};
		ViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		ViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		ViewDesc.Texture2D.MipSlice = 0;
		ViewDesc.Texture2D.PlaneSlice = 0;


		//�����_�[�^�[�Q�b�g�r���[�̐���
		//pDevice->CreateRenderTargetView(RenderTargets[i], NULL, handle);
		pDevice->CreateRenderTargetView(RenderTargets[i], NULL, rtvHandle);
		//rtvHandle.Offset(1,RTV_Increment);
		rtvHandle.ptr += RTV_Increment;
	}

	return true;
}

bool D3d12_Init::Fence(ID3D12Device*& pDevice, ID3D12Fence*& pFence, UINT64& FenceValue) {

	//�t�F���X�J�E���^�[������
	hr = pDevice->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&pFence)
	);

	if (FAILED(hr)) {
		printf("�t�F���X		:ERROR\n");
		return false;
	}
	else {
		printf("�t�F���X		:OK\n");
	}

	FenceValue = 1;
	return true;

}



