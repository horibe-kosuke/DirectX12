#pragma once
#include "PCH.h"


struct OriginalCbuffer {
	DirectX::XMMATRIX w;//ワールド行列
	DirectX::XMMATRIX wvp;//ワールドから射影までの変換行列
	DirectX::XMFLOAT4 lightDir;//ライト方向
	//DirectX::XMFLOAT4 eye;//視点位置
	//DirectX::XMFLOAT4 ambient;//環境光
	DirectX::XMFLOAT4 diffuse;//拡散反射光
	//DirectX::XMFLOAT4 specular;//鏡面反射光
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
	UINT m_CBSize;//コンスタントバッファサイズ

	ID3D12Resource* m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
	ID3D12Resource** m_IndexBuffer;
	D3D12_INDEX_BUFFER_VIEW* m_IndexBufferView;





	OriginalVertex* m_Vertex;		//頂点情報
	OriginalMaterial* m_Material;	//マテリアル情報
	int* m_Indices;					//インデックス情報

	int NumVertex = 0;				//頂点総数
	int NumMaterial = 0;			//マテリアル総数
	int NumIndices[100] = { 0 };	//インデックス総数

	int* NumAnim;					//アニメーション　最大フレーム

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