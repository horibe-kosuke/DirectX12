#pragma once
#include <DirectXMath.h>
#include "PCH.h"
#include "Cbuffer.h"
//#include "Obj_Mesh.h"
//#include "FBX.h"

//���_�o�b�t�@
struct Vertex {
	DirectX::XMFLOAT3 Pos;
	//DirectX::XMFLOAT2 UV;
};

//�萔�o�b�t�@�ɑ������
struct alignas(256) Transform {
	DirectX::XMMATRIX	World;//���[���h�s��
	DirectX::XMMATRIX	View;//�r���[�s��
	DirectX::XMMATRIX	Proj;//�ˉe�s��
};



//�萔�o�b�t�@
//template<class T>
//struct ConstantBufferView {
//	D3D12_CONSTANT_BUFFER_VIEW_DESC Desc;               // �萔�o�b�t�@
//	D3D12_CPU_DESCRIPTOR_HANDLE     HandleCPU;          // CPU�f�B�X�N���v�^�n���h��
//	D3D12_GPU_DESCRIPTOR_HANDLE     HandleGPU;          // GPU�f�B�X�N���v�^�n���h����
//	T* pBuffer;											// �o�b�t�@�擪�ւ̃|�C���^.
//};



class D3d12_Shader
{

public:
	D3d12_Shader();
	~D3d12_Shader();

	bool ConstantBuffer(ID3D12Device*& pDevice, ID3D12DescriptorHeap*& CBV_SRV_UAV_Heap,
		ID3D12Resource*(& CBV_SRV_UAV)[ConstantBufferCount], UINT& CB_Size, UINT& CBV_SRV_UAV_Increment);	//�萔�o�b�t�@����

	bool DepthBuffer(ID3D12Device*& pDevice, ID3D12Resource*& DepthBuffer, ID3D12DescriptorHeap*& DSV_Heap);//�[�x�o�b�t�@

	bool RootSignature(ID3D12Device*& pDevice, ID3D12RootSignature*& pRootSignature);	//���[�g�V�O�l�`������

	bool PipelineState(ID3D12Device*& pDevice, ID3D12RootSignature*& pRootSignature, ID3D12PipelineState*& pPipelineState);	//�p�C�v���C���X�e�[�g����

	void ViewPort(D3D12_VIEWPORT& ViewPort, D3D12_RECT& ScissorRect);		//�r���[�|�[�g����

	bool VertexBuffer(ID3D12Device*& pDevice, ID3D12Resource*& VertexBuffer, D3D12_VERTEX_BUFFER_VIEW& VertexBufferView);	//���_�o�b�t�@����

	bool IndexBuffer(ID3D12Device*& pDevice, ID3D12Resource*& IndexBuffer, D3D12_INDEX_BUFFER_VIEW& IndexBufferView);		//�C���f�b�N�X�o�b�t�@����
};

