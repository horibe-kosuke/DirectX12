#pragma once
#include "D3d12_Init.h"
#include "D3d12_Shader.h"
#include "OriginalFormat.h"
class D3d12
{
	//HRESULT hr;	//COM�����m�F

	//�I���W�i���t�H�[�}�b�g�ǂݍ���
	OriginalFormat* m_OF;


	//DirectX12�ŕK�v�Ȃ���
	ID3D12Device* m_pDevice;									//�f�o�C�X
	IDXGISwapChain3* m_pSwapChain;								//�X���b�v�`�F�C��
	ID3D12CommandQueue* m_pCommandQueue;						//�R�}���h�L���[
	ID3D12CommandAllocator* m_pCommandAllocator;				//�R�}���h�A���P�[�^
	ID3D12GraphicsCommandList* m_pCommandList;				    //�R�}���h���X�g
	ID3D12DescriptorHeap* m_pHeapRTV;							//�f�B�X�N���v�^�q�[�v(�����_�[�^�[�Q�b�g�r���[)
	ID3D12Fence* m_pFence;										//�t�F���X

	ID3D12PipelineState* m_pPipelineState;						//�p�C�v���C���X�e�[�g
	ID3D12RootSignature* m_pRootSignature;						//���[�g�V�O�l�`��

	D3D12_VIEWPORT                  m_ViewPort = {};            //�r���[�|�[�g
	D3D12_RECT                      m_ScissorRect = {};         //�V�U�[��`

	UINT64 m_FenceValue;	//�t�F���X�l

	UINT m_CBV_SRV_UAV_Increment;	//CBV(�C���N�������g�T�C�Y)
	UINT m_RTV_Increment;	//RTV(�C���N�������g�T�C�Y)
	UINT m_DSV_Increment;	//DSV(�C���N�������g�T�C�Y)

	UINT m_CB_Size;			//�萔�o�b�t�@�̃T�C�Y

	//RTV(�����_�[�^�[�Q�b�g�r���[)
	ID3D12Resource* m_RenderTargets[FrameCount];	//�����_�[�^�[�Q�b�g
	ID3D12DescriptorHeap* m_RTV_Heap;				//�f�B�X�N���v�^�q�[�v(�����_�[�^�[�Q�b�g)			

	//DSV(�[�x�X�e���V���r���[)
	ID3D12Resource* m_DepthBuffer;					//�[�x�X�e���V���r���[
	ID3D12DescriptorHeap* m_DSV_Heap;				//�f�B�X�N���v�^�q�[�v(�[�x�X�e���V���r���[)

	//CBV_SRV_UAV(�萔�o�b�t�@�[�r���[///�V�F�[�_�[���\�[�X�r���[///�����w�肳��Ă��Ȃ��A�N�Z�X�r���[)
	ID3D12Resource* m_CBV_SRV_UAV[ConstantBufferCount];
	ID3D12DescriptorHeap* m_CBV_SRV_UAV_Heap;		//�f�B�X�N���v�^�q�[�v(CBV_SRV_UAV)

	//�萔�o�b�t�@�ɑ������
	//FbxCbuffer m_Cbuffer;

	//VERTEX(���_)
	ID3D12Resource* m_VertexBuffer;					//���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;	//���_�o�b�t�@�r���[

	//INDEX(�C���f�b�N�X)
	ID3D12Resource* m_IndexBuffer;					//�C���f�b�N�X�o�b�t�@
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;		//�C���f�b�N�X�o�b�t�@�r���[


	//! @brief �f�o�b�O���C���[
	void Debug();


	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;

public:
	static bool KeyState;
	DirectX::XMFLOAT3 Pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 Rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 Scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	D3d12();
	~D3d12();

	void Init(HWND& hwnd, HINSTANCE& hInstance);//DirectX12�̏�����
	bool Update();
	void Render_2D();
	void Render_3D();	//�`��
	void WaitGPU(); //���������@GPU�̏�������������܂ő҂�
};

