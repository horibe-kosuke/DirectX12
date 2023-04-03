#pragma once
#include <DirectXMath.h>
#include "PCH.h"
#include "Cbuffer.h"
//#include "Obj_Mesh.h"
//#include "FBX.h"

//頂点バッファ
struct Vertex {
	DirectX::XMFLOAT3 Pos;
	//DirectX::XMFLOAT2 UV;
};

//定数バッファに送るもの
struct alignas(256) Transform {
	DirectX::XMMATRIX	World;//ワールド行列
	DirectX::XMMATRIX	View;//ビュー行列
	DirectX::XMMATRIX	Proj;//射影行列
};



//定数バッファ
//template<class T>
//struct ConstantBufferView {
//	D3D12_CONSTANT_BUFFER_VIEW_DESC Desc;               // 定数バッファ
//	D3D12_CPU_DESCRIPTOR_HANDLE     HandleCPU;          // CPUディスクリプタハンドル
//	D3D12_GPU_DESCRIPTOR_HANDLE     HandleGPU;          // GPUディスクリプタハンドルで
//	T* pBuffer;											// バッファ先頭へのポインタ.
//};



class D3d12_Shader
{

public:
	D3d12_Shader();
	~D3d12_Shader();

	bool ConstantBuffer(ID3D12Device*& pDevice, ID3D12DescriptorHeap*& CBV_SRV_UAV_Heap,
		ID3D12Resource*(& CBV_SRV_UAV)[ConstantBufferCount], UINT& CB_Size, UINT& CBV_SRV_UAV_Increment);	//定数バッファ生成

	bool DepthBuffer(ID3D12Device*& pDevice, ID3D12Resource*& DepthBuffer, ID3D12DescriptorHeap*& DSV_Heap);//深度バッファ

	bool RootSignature(ID3D12Device*& pDevice, ID3D12RootSignature*& pRootSignature);	//ルートシグネチャ生成

	bool PipelineState(ID3D12Device*& pDevice, ID3D12RootSignature*& pRootSignature, ID3D12PipelineState*& pPipelineState);	//パイプラインステート生成

	void ViewPort(D3D12_VIEWPORT& ViewPort, D3D12_RECT& ScissorRect);		//ビューポート生成

	bool VertexBuffer(ID3D12Device*& pDevice, ID3D12Resource*& VertexBuffer, D3D12_VERTEX_BUFFER_VIEW& VertexBufferView);	//頂点バッファ生成

	bool IndexBuffer(ID3D12Device*& pDevice, ID3D12Resource*& IndexBuffer, D3D12_INDEX_BUFFER_VIEW& IndexBufferView);		//インデックスバッファ生成
};

