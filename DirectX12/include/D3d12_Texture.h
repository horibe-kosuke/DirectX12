#pragma once
#include <WICTextureLoader.h>	//jpg png bmp �ǂݍ��ݗp
#include "PCH.h"
#pragma comment(lib,"DirectXTK12.lib")
#define TEXTURE_MAX 1
struct TextureHandle {
	ID3D12Resource* pResource;				//���\�[�X
	D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;	//CPU�f�B�X�N���v�^�n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;	//GPU�f�B�X�N���v�^�n���h��
};

class D3d12_Texture
{
	UINT m_cbvSrvStride;
	ID3D12Resource* m_pTexture;
	ID3D12Resource* m_pStagingTexture;
	ID3D12DescriptorHeap* m_srvHeap;//�V�F�[�_�[���\�[�X�q�[�v

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

