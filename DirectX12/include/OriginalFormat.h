#pragma once
#include "PCH.h"


struct OriginalCbuffer {
	DirectX::XMMATRIX w;//���[���h�s��
	DirectX::XMMATRIX wvp;//���[���h����ˉe�܂ł̕ϊ��s��
	DirectX::XMFLOAT4 lightDir;//���C�g����
	//DirectX::XMFLOAT4 eye;//���_�ʒu
	//DirectX::XMFLOAT4 ambient;//����
	DirectX::XMFLOAT4 diffuse;//�g�U���ˌ�
	//DirectX::XMFLOAT4 specular;//���ʔ��ˌ�
	OriginalCbuffer()
	{
		ZeroMemory(this, sizeof(OriginalCbuffer));
	}
};


struct OriginalVertex {
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;

	OriginalVertex()
	{
		Pos = DirectX::XMFLOAT3(0, 0, 0);
		Normal = DirectX::XMFLOAT3(0, 0, 0);
	}
};

struct OriginalMaterial {
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
};

class OriginalFormat {

	ID3D12Device* m_pDevice;
	ID3D12CommandAllocator* m_pCommandAllocator;
	ID3D12CommandQueue* m_pQueue;
	ID3D12GraphicsCommandList* m_pCommandList;
	ID3D12Fence* m_pFence;
	UINT64 m_FenceValue;
	ID3D12PipelineState* m_pPSO;

	ID3D12Resource* m_ConstantBuffer;
	ID3D12DescriptorHeap* m_CBVHeap;
	UINT m_CBSize;//�R���X�^���g�o�b�t�@�T�C�Y

	ID3D12Resource* m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
	ID3D12Resource** m_IndexBuffer;
	D3D12_INDEX_BUFFER_VIEW* m_IndexBufferView;





	OriginalVertex* m_Vertex;		//���_���
	OriginalMaterial* m_Material;	//�}�e���A�����
	int* m_Indices;					//�C���f�b�N�X���

	int NumVertex = 0;				//���_����
	int NumMaterial = 0;			//�}�e���A������
	int NumIndices[100] = { 0 };	//�C���f�b�N�X����

	int* NumAnim;					//�A�j���[�V�����@�ő�t���[��

	float GetValue(const char* str, int offset);
	bool LoadMesh();
public:
	OriginalFormat();
	~OriginalFormat();
	bool Init(ID3D12Device* pDevice, ID3D12CommandAllocator* pCommandAllocator,
		ID3D12CommandQueue* pQueue, ID3D12GraphicsCommandList* pCmdList, ID3D12Fence* pFence,
		UINT64 FenceValue, ID3D12PipelineState* pPSO);

	void Render(DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj, DirectX::XMFLOAT4& light, DirectX::XMFLOAT3& eye);

};