#include "D3d12_Texture.h"
#include "DDSTextureLoader.h"
D3d12_Texture::D3d12_Texture()
	:m_cbvSrvStride(NULL)
	, m_pStagingTexture(nullptr)
	, m_pTexture(nullptr)
	, m_srvHeap(nullptr)
{
	for (int i = 0; i < TEXTURE_MAX; i++) {
		m_Texture[i].HandleCPU = D3D12_CPU_DESCRIPTOR_HANDLE();
		m_Texture[i].HandleGPU = D3D12_GPU_DESCRIPTOR_HANDLE();
		m_Texture[i].pResource = nullptr;
	}
}

D3d12_Texture::~D3d12_Texture() {
	SafeRelease(m_pTexture);
	SafeRelease(m_pStagingTexture);
	for (int i = 0; i < TEXTURE_MAX; i++) {
		SafeRelease(m_Texture[i].pResource);
	}
}

bool D3d12_Texture::Texture(ID3D12Device* pDevice, ID3D12CommandAllocator* pCommandAllocator,
	ID3D12CommandQueue* pQueue, ID3D12GraphicsCommandList* pCmdList, ID3D12Fence* pFence,
	UINT64 FenceValue, ID3D12PipelineState* pPSO, const wchar_t& FileName)
{
	//UPLOAD用のテクスチャ
	ID3D12Resource* TextureUploadHeap;

	//COM初期化
	auto result = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	std::unique_ptr<uint8_t[]> TexData;
	D3D12_SUBRESOURCE_DATA SubresouceData;

	//テクスチャの読み込み　データを取得
	auto hr = DirectX::LoadWICTextureFromFile(pDevice, &FileName, &m_pTexture, TexData, SubresouceData);
	if (FAILED(hr)) {
		printf("テクスチャ読み込み		:ERROR\n");
	}
	else {
		printf("テクスチャ読み込み		:OK\n");
	}

	D3D12_RESOURCE_DESC TextureDesc = m_pTexture->GetDesc();

	//サイズ取得
	const UINT UploadSize = GetRequiredIntermediateSize(m_pTexture, 0, 1);

	//GPUアップロードバッファを作成
	D3D12_HEAP_PROPERTIES uploadHeap = {};
	uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
	uploadHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	uploadHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	uploadHeap.CreationNodeMask = 1;
	uploadHeap.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Alignment = 0;
	resDesc.Width = UploadSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	hr = pDevice->CreateCommittedResource(
		&uploadHeap,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&TextureUploadHeap));


	if (FAILED(hr)) {
		printf("テクスチャアップロード		:ERROR\n");
	}
	else {
		printf("テクスチャアップロード		:OK\n");
	}

	UpdateSubresources(pCmdList, m_pTexture, TextureUploadHeap, 0, 0, 1, &SubresouceData);


	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_pTexture;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	pCmdList->ResourceBarrier(1, &barrier);



	//このテクスチャーのビュー（SRV)を作る
	D3D12_SHADER_RESOURCE_VIEW_DESC srvdesc = {};
	srvdesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvdesc.Format = TextureDesc.Format;
	srvdesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvdesc.Texture2D.MipLevels = TextureDesc.MipLevels;



	pDevice->CreateShaderResourceView(m_pTexture, &srvdesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());


	//テクスチャアップロード用のCOM削除
	pCmdList->DiscardResource(TextureUploadHeap, NULL);

	//WIC使用終了
	CoUninitialize();

	pCmdList->Close();

	ID3D12CommandList* ppCommandLists[] = { pCmdList };
	pQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);



	return true;
}