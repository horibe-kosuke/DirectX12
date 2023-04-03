#pragma once
#include "D3d12_Init.h"
#include "D3d12_Shader.h"
#include "OriginalFormat.h"
class D3d12
{
	//HRESULT hr;	//COM生成確認

	//オリジナルフォーマット読み込み
	OriginalFormat* m_OF;


	//DirectX12で必要なもの
	ID3D12Device* m_pDevice;									//デバイス
	IDXGISwapChain3* m_pSwapChain;								//スワップチェイン
	ID3D12CommandQueue* m_pCommandQueue;						//コマンドキュー
	ID3D12CommandAllocator* m_pCommandAllocator;				//コマンドアロケータ
	ID3D12GraphicsCommandList* m_pCommandList;				    //コマンドリスト
	ID3D12DescriptorHeap* m_pHeapRTV;							//ディスクリプタヒープ(レンダーターゲットビュー)
	ID3D12Fence* m_pFence;										//フェンス

	ID3D12PipelineState* m_pPipelineState;						//パイプラインステート
	ID3D12RootSignature* m_pRootSignature;						//ルートシグネチャ

	D3D12_VIEWPORT                  m_ViewPort = {};            //ビューポート
	D3D12_RECT                      m_ScissorRect = {};         //シザー矩形

	UINT64 m_FenceValue;	//フェンス値

	UINT m_CBV_SRV_UAV_Increment;	//CBV(インクリメントサイズ)
	UINT m_RTV_Increment;	//RTV(インクリメントサイズ)
	UINT m_DSV_Increment;	//DSV(インクリメントサイズ)

	UINT m_CB_Size;			//定数バッファのサイズ

	//RTV(レンダーターゲットビュー)
	ID3D12Resource* m_RenderTargets[FrameCount];	//レンダーターゲット
	ID3D12DescriptorHeap* m_RTV_Heap;				//ディスクリプタヒープ(レンダーターゲット)			

	//DSV(深度ステンシルビュー)
	ID3D12Resource* m_DepthBuffer;					//深度ステンシルビュー
	ID3D12DescriptorHeap* m_DSV_Heap;				//ディスクリプタヒープ(深度ステンシルビュー)

	//CBV_SRV_UAV(定数バッファービュー///シェーダーリソースビュー///順序指定されていないアクセスビュー)
	ID3D12Resource* m_CBV_SRV_UAV[ConstantBufferCount];
	ID3D12DescriptorHeap* m_CBV_SRV_UAV_Heap;		//ディスクリプタヒープ(CBV_SRV_UAV)

	//定数バッファに送るもの
	//FbxCbuffer m_Cbuffer;

	//VERTEX(頂点)
	ID3D12Resource* m_VertexBuffer;					//頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;	//頂点バッファビュー

	//INDEX(インデックス)
	ID3D12Resource* m_IndexBuffer;					//インデックスバッファ
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;		//インデックスバッファビュー


	//! @brief デバッグレイヤー
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

	void Init(HWND& hwnd, HINSTANCE& hInstance);//DirectX12の初期化
	bool Update();
	void Render_2D();
	void Render_3D();	//描画
	void WaitGPU(); //同期処理　GPUの処理が完了するまで待つ
};

