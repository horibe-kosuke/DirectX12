#pragma once
#include <WICTextureLoader.h>	//jpg png bmp 読み込み用
#include "PCH.h"
#pragma comment(lib,"DirectXTK12.lib")
#define TEXTURE_MAX 1
struct TextureHandle {
	ID3D12Resource* pResource;				//リソース
	D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;	//CPUディスクリプタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;	//GPUディスクリプタハンドル
};

class D3d12_Texture
{
	UINT m_cbvSrvStride;
	ID3D12Resource* m_pTexture;
	ID3D12Resource* m_pStagingTexture;
	ID3D12DescriptorHeap* m_srvHeap;//シェーダーリソースヒープ

	TextureHandle m_Texture[TEXTURE_MAX];

	const UINT TextureWidth = 256;
	const UINT TextureHeight = 256;
public:
	D3d12_Texture();
	~D3d12_Texture();
	bool Texture(ID3D12Device* pDevice, ID3D12CommandAllocator* pCommandAllocator,
		ID3D12CommandQueue* pQueue, ID3D12GraphicsCommandList* pCmdList, ID3D12Fence* pFence,
		UINT64 FenceValue, ID3D12PipelineState* pPSO, const wchar_t& FileName);
};

