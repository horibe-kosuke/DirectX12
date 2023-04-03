#include "D3D12_App.h"

bool D3d12::KeyState;


#ifdef _DEBUG
void D3d12::Debug()
{
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
}
#endif
D3d12::D3d12()
	: m_pDevice(nullptr)
	, m_pSwapChain(nullptr)
	, m_pCommandQueue(nullptr)
	, m_pCommandAllocator(nullptr)
	, m_pCommandList(nullptr)
	, m_pHeapRTV(nullptr)
	, m_pFence(nullptr)
	, m_FenceValue(0)
	, m_CBV_SRV_UAV_Increment(NULL)
	, m_RTV_Increment(NULL)
	, m_DSV_Increment(NULL)
	, m_CB_Size(NULL)
	, m_RTV_Heap(nullptr)
	, m_DepthBuffer(nullptr)
	, m_DSV_Heap(nullptr)
	, m_CBV_SRV_UAV_Heap(nullptr)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, Pos(0.0f, 0.0f, 0.0f)
{
	m_VertexBufferView = {};
	m_IndexBufferView = {};
	for (DWORD i = 0; i < FrameCount; i++) {
		m_RenderTargets[i] = nullptr;
	}

	for (DWORD i = 0; i < ConstantBufferCount; i++) {
		m_CBV_SRV_UAV[i] = nullptr;
	}

}

D3d12::~D3d12()
{
	WaitGPU();
	SafeRelease(m_pDevice);
	SafeRelease(m_pSwapChain);
	SafeRelease(m_pCommandQueue);
	SafeRelease(m_pCommandAllocator);
	SafeRelease(m_pCommandList);
	SafeRelease(m_pHeapRTV);
	SafeRelease(m_pFence);
	m_FenceValue = NULL;
	m_CBV_SRV_UAV_Increment = NULL;
	m_RTV_Increment = NULL;
	m_DSV_Increment = NULL;
	m_CB_Size = NULL;
	SafeRelease(m_RTV_Heap);
	SafeRelease(m_DepthBuffer);
	SafeRelease(m_DSV_Heap);

	for (int i = 0; i < ConstantBufferCount; i++) {
		SafeRelease(m_CBV_SRV_UAV[i]);
	}

	SafeRelease(m_CBV_SRV_UAV_Heap);
	SafeRelease(m_VertexBuffer);
	SafeRelease(m_IndexBuffer);

	m_VertexBufferView = {};
	m_IndexBufferView = {};
	for (DWORD i = 0; i < FrameCount; i++) {
		m_RenderTargets[i] = nullptr;
	}

	delete m_OF;
}

void D3d12::Init(HWND& hwnd, HINSTANCE& hInstance)
{
#if _DEBUG
	Debug();
#endif
	//初期化

	D3d12_Init* D3D_Init = new D3d12_Init(hwnd, hInstance);

	D3D_Init->Device(m_pDevice);//デバイス取得

	//インクリメントサイズ取得
	m_RTV_Increment = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_CBV_SRV_UAV_Increment = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_DSV_Increment = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	D3D_Init->CommandQueue(m_pDevice, m_pCommandQueue);								//コマンドキュー取得

	D3D_Init->CommandList(m_pDevice, m_pCommandAllocator, m_pCommandList);			//コマンドリスト、コマンドアロケータ取得

	D3D_Init->SwapChain(m_pCommandQueue, m_pSwapChain);								//スワップチェイン取得

	D3D_Init->RenderTarget(m_pDevice, m_pSwapChain, m_RenderTargets,				//レンダーターゲット取得
		m_RTV_Heap, m_RTV_Increment);

	D3D_Init->Fence(m_pDevice, m_pFence, m_FenceValue);								//フェンス取得

	m_pCommandList->Close();
	delete D3D_Init;

	//シェーダ関連
	D3d12_Shader* D3D_Shader = new D3d12_Shader;
	printf("\n\nShader\n");

	D3D_Shader->ConstantBuffer(m_pDevice, m_CBV_SRV_UAV_Heap,						//定数バッファ取得
		m_CBV_SRV_UAV, m_CB_Size, m_CBV_SRV_UAV_Increment);

	D3D_Shader->DepthBuffer(m_pDevice, m_DepthBuffer, m_DSV_Heap);					//深度バッファ

	D3D_Shader->RootSignature(m_pDevice, m_pRootSignature);							//ルートシグネチャ取得

	D3D_Shader->PipelineState(m_pDevice, m_pRootSignature, m_pPipelineState);		//パイプラインステート取得

	D3D_Shader->ViewPort(m_ViewPort, m_ScissorRect);

	//D3D_Shader->VertexBuffer(m_pDevice, m_VertexBuffer, m_VertexBufferView);		//バーテックスバッファ(2D)

	//D3D_Shader->IndexBuffer(m_pDevice, m_IndexBuffer, m_IndexBufferView);			//インデックスバッファ(2D)
	delete D3D_Shader;



	//オリジナルフォーマット読み込み
	m_OF = new OriginalFormat;
	m_OF->Init(m_pDevice, m_pCommandAllocator, m_pCommandQueue, m_pCommandList, m_pFence, m_FenceValue, m_pPipelineState);
}


bool D3d12::Update() {

	if (KeyState) {
		if (GetKeyState('D') & 0x8000) {
			Pos.x += 0.1f;
		}
		if (GetKeyState('A') & 0x8000) {
			Pos.x -= 0.1f;
		}

		if (GetKeyState('W') & 0x8000) {
			Pos.z += 0.1f;
		}
		if (GetKeyState('S') & 0x8000) {
			Pos.z -= 0.1f;
		}

		if (GetKeyState('F') & 0x8000) {
			Pos.y -= 0.1f;
		}
		if (GetKeyState('R') & 0x8000) {
			Pos.y += 0.1f;
		}
	}
	return true;
}


//3D
void D3d12::Render_3D()
{
	//バックバッファ現在何枚目かを取得
	UINT BackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	//コマンドリストに書き込む前にはコマンドアロケータリセット
	m_pCommandAllocator->Reset();

	//コマンドリストリセット
	m_pCommandList->Reset(m_pCommandAllocator, m_pPipelineState);

	//コマンドリストにコマンドを書き込む

	//バックバッファのトランジションをレンダーターゲットモードにする
	// リソースバリアの設定.
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_RenderTargets[BackBufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	//リソースバリア
	m_pCommandList->ResourceBarrier(1, &barrier);

	//バックバッファをレンダーターゲットにセット
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_Handle(m_RTV_Heap->GetCPUDescriptorHandleForHeapStart(), BackBufferIndex, m_RTV_Increment);

	D3D12_CPU_DESCRIPTOR_HANDLE Dsv_Handle = m_DSV_Heap->GetCPUDescriptorHandleForHeapStart();
	//D3D12_CPU_DESCRIPTOR_HANDLE rtv_Handle = {};
	//D3D12_CPU_DESCRIPTOR_HANDLE rtv_Heap = m_RTV_Heap->GetCPUDescriptorHandleForHeapStart();
	//rtv_Handle.ptr += static_cast<SIZE_T>(rtv_Heap.ptr + BackBufferIndex * m_RTV_Increment);

	m_pCommandList->OMSetRenderTargets(1, &rtv_Handle, false, &Dsv_Handle);

	//ビューポートセット
	m_pCommandList->RSSetViewports(1, &m_ViewPort);
	m_pCommandList->RSSetScissorRects(1, &m_ScissorRect);

	//画面クリア
	const float ClearColor[] = { 0.5f,0.5f,0.5f,1.0f };
	m_pCommandList->ClearRenderTargetView(rtv_Handle, ClearColor, 0, nullptr);
	m_pCommandList->ClearDepthStencilView(Dsv_Handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//ルートシグネチャセット
	m_pCommandList->SetGraphicsRootSignature(m_pRootSignature);

	//World View Proiection 変換
	//ワールドトランスフォーム
	DirectX::XMMATRIX pitch;
	world = DirectX::XMMatrixIdentity();

	//拡大・縮小
	world.r[0].m128_f32[0] = Scale.x * -1;
	world.r[1].m128_f32[1] = Scale.y;
	world.r[2].m128_f32[2] = Scale.z * -1;

	//回転
	world *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(Rot.x * 3.14 / 180));
	world *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(Rot.y * 3.14 / 180));
	world *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(Rot.z * 3.14 / 180));

	//ビュートランスフォーム
	DirectX::XMFLOAT3 eye(Pos.x, Pos.y, Pos.z - 5.0f);		//	カメラ位置
	DirectX::XMFLOAT3 at(0.0f, 0.0f, 1.0f);	//	カメラ方向
	at.x += Pos.x;
	at.y += Pos.y;
	at.z += Pos.z;
	DirectX::XMFLOAT3 up(0.0f, 1.0f, 0.0f);		//	上方位置

	view = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&at), XMLoadFloat3(&up));

	// プロジェクショントランスフォーム
	proj = DirectX::XMMatrixPerspectiveFovLH(3.14159 / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0f);

	//ポリゴントポロジーの指定
	m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	DirectX::XMFLOAT4 light(0.0f, 0.0f, -1.0f, 0.0f);

	m_OF->Render(world, view, proj, light, eye);

	//バックバッファのトランジションをPresentモードにする
	// リソースバリアの設定.
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_RenderTargets[BackBufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pCommandList->ResourceBarrier(1, &barrier);

	//コマンドの書き込み終了
	m_pCommandList->Close();

	//コマンドリストの実行
	ID3D12CommandList* ppCommandLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	//バックバッファをフロントバッファに切り替えてシーンをモニターに表示
	m_pSwapChain->Present(1, 0);

	//GPUの処理が完了するまで待つ
	WaitGPU();
}

//2D
void D3d12::Render_2D() {
	////バックバッファが現在何枚目かを取得
	//UINT backBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	////コマンドリストに書き込む前にはコマンドアロケーターをリセットする
	//m_pCommandAllocator->Reset();
	////コマンドリストをリセットする
	//m_pCommandList->Reset(m_pCommandAllocator, m_pPipelineState);

	////ここからコマンドリストにコマンドを書き込んでいく

	////バックバッファのトランジションをレンダーターゲットモードにする
	//D3D12_RESOURCE_BARRIER barrier = {};
	//barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//barrier.Transition.pResource = m_RenderTargets[backBufferIndex];
	//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	//m_pCommandList->ResourceBarrier(1, &barrier);

	////バックバッファをレンダーターゲットにセット
	//CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTV_Heap->GetCPUDescriptorHandleForHeapStart(), backBufferIndex, m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_DSV_Heap->GetCPUDescriptorHandleForHeapStart();
	//m_pCommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	////ビューポートをセット
	//CD3DX12_VIEWPORT  viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
	//CD3DX12_RECT  scissorRect = CD3DX12_RECT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//m_pCommandList->RSSetViewports(1, &viewport);
	//m_pCommandList->RSSetScissorRects(1, &scissorRect);

	////画面クリア
	//const float clearColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	//m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, NULL);

	////ルートシグネチャをセット
	//m_pCommandList->SetGraphicsRootSignature(m_pRootSignature);

	////ヒープ（アプリにただ1つだけ）をセット
	//ID3D12DescriptorHeap* ppHeaps[] = { m_CBV_SRV_UAV_Heap };
	//m_pCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	////World View Proiection 変換
	//XMMATRIX rotMat, worldMat;
	//XMMATRIX viewMat;
	//XMMATRIX proiMat;
	//// ビュートランスフォーム
	//XMFLOAT3 eyeVec(0.0f, 0.0f, -3.0f);//カメラ 位置
	//XMFLOAT3 dirVec(0.0f, 0.0f, 1.0f);//カメラ　方向
	//XMFLOAT3 upVec(0.0f, 1.0f, 0.0f);//上方位置
	//viewMat = XMMatrixLookToRH(XMLoadFloat3(&eyeVec), XMLoadFloat3(&dirVec), XMLoadFloat3(&upVec));
	//// プロジェクショントランスフォーム
	//proiMat = XMMatrixPerspectiveFovRH(3.14159 / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0f);

	////ポリゴントポロジーの指定
	//m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//for (int i = 0; i < 2; i++) {

	//	//描画
	//	static float r = 0;
	//	r += 0.005;
	//	rotMat = XMMatrixRotationY(r);//単純にyaw回転させる

	//	//コンスタントバッファをルートにセット
	//	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_CBV_SRV_UAV_Heap->GetGPUDescriptorHandleForHeapStart();
	//	handle.ptr += i * m_CBV_SRV_UAV_Increment;
	//	m_pCommandList->SetGraphicsRootDescriptorTable(0, handle);


	//	//コンスタントバッファの内容を更新
	//	CD3DX12_RANGE readRange(0, 0);
	//	readRange = CD3DX12_RANGE(0, 0);
	//	//D3D12_RANGE readRange;
	//	//readRange.Begin = 0;
	//	//readRange.End = 0;

	//	UINT8* pCbvDataBegin;
	//	m_CBV_SRV_UAV[i]->Map(0, &readRange, reinterpret_cast<void**>(&pCbvDataBegin));
	//	Cbuffer cb;
	//	//ワールドトランスフォーム
	//	if (i == 0) {
	//		worldMat = rotMat * XMMatrixTranslation(-0.5f, 0, 0);
	//	}
	//	else {
	//		worldMat = rotMat * XMMatrixTranslation(0.5f, 0, 0);
	//	}
	//	char* ptr = reinterpret_cast<char*>(pCbvDataBegin) + (i * m_CBV_SRV_UAV_Increment);
	//	cb.wvp = XMMatrixTranspose(worldMat * viewMat * proiMat);
	//	memcpy(ptr, &cb, sizeof(Cbuffer));

	//	//テーブルセット
	//	//D3D12_GPU_DESCRIPTOR_HANDLE cbvSrvUavDescHeap = m_CBV_SRV_UAV_Heap->GetGPUDescriptorHandleForHeapStart();
	//	//m_pCommandList->SetGraphicsRootDescriptorTable(0, cbvSrvUavDescHeap);

	//			//バーテックスバッファをセット
	//	m_pCommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
	//	//インデックスバッファをセット
	//	m_pCommandList->IASetIndexBuffer(&m_IndexBufferView);

	//	//描画
	//	m_pCommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
	//}


	////バックバッファのトランジションをPresentモードにする
	//barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//barrier.Transition.pResource = m_RenderTargets[backBufferIndex];
	//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	//m_pCommandList->ResourceBarrier(1, &barrier);

	////コマンドの書き込みはここで終わり、Closeする
	//m_pCommandList->Close();

	////コマンドリストの実行
	//ID3D12CommandList* ppCommandLists[] = { m_pCommandList };
	//m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	////バックバッファをフロントバッファに切り替えてシーンをモニターに表示
	//m_pSwapChain->Present(1, 0);

	////GPUの処理が完了するまで待つ
	//WaitGPU();

}

void D3d12::WaitGPU() {
	//GPUサイドが全て完了したときにGPUサイドから返ってくる値（フェンス値）をセット
	m_pCommandQueue->Signal(m_pFence, m_FenceValue);

	UINT64 i = m_pFence->GetCompletedValue();
	//上でセットしたシグナルがGPUから帰ってくるまでストール（この行で待機）
	do
	{

	} while (m_pFence->GetCompletedValue() < m_FenceValue);

	//フェンス値を更新
	m_FenceValue++;
}