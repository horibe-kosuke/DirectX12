#include "D3d12_Shader.h"

const wchar_t* hlsl_arrary[][2] = {
	{L"shader/MaterialVS.hlsl",L"shader/MaterialPS.hlsl"},
};


D3d12_Shader::D3d12_Shader()
{

}

D3d12_Shader::~D3d12_Shader()
{
}

bool D3d12_Shader::ConstantBuffer(ID3D12Device*& pDevice, ID3D12DescriptorHeap*& CBV_SRV_UAV_Heap,
	ID3D12Resource* (&CBV_SRV_UAV)[ConstantBufferCount], UINT& CB_Size, UINT& CBV_SRV_UAV_Increment)
{

	//�萔�o�b�t�@�p�f�B�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC CBVdesc = {};
	CBVdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	CBVdesc.NumDescriptors = ConstantBufferCount;
	CBVdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CBVdesc.NodeMask = 0;

	auto hr = pDevice->CreateDescriptorHeap(
		&CBVdesc,
		IID_PPV_ARGS(&CBV_SRV_UAV_Heap)
	);

	if (FAILED(hr)) {
		printf("�f�B�X�N���v�^�q�[�v(�萔�o�b�t�@)	:ERROR\n");
		return false;
	}
	else {
		printf("�f�B�X�N���v�^�q�[�v(�萔�o�b�t�@)	:OK\n");
	}

	//�萔�o�b�t�@����

	//�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	// ���\�[�X�̐ݒ�.
	CB_Size = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT * ((sizeof(Cbuffer) / D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT) + 1);
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width =static_cast<UINT64>((UINT64)(CB_Size) * ConstantBufferCount);
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;


	for (DWORD i = 0; i < ConstantBufferCount; i++) {
		// ���\�[�X����.
		hr = pDevice->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&CBV_SRV_UAV[i])
		);

		if (FAILED(hr))
		{
			printf("�萔�o�b�t�@				:ERROR\n");
			return false;
		}
		else {
			printf("�萔�o�b�t�@				:OK\n");
		}


		D3D12_CONSTANT_BUFFER_VIEW_DESC CBVdesc = {};
		CBVdesc.BufferLocation = CBV_SRV_UAV[i]->GetGPUVirtualAddress() + i * CB_Size;
		CBVdesc.SizeInBytes = CB_Size;
		D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle = CBV_SRV_UAV_Heap->GetCPUDescriptorHandleForHeapStart();
		CPUHandle.ptr += static_cast<UINT>(i * CBV_SRV_UAV_Increment);

		// �萔�o�b�t�@�r���[�𐶐�.
		pDevice->CreateConstantBufferView(&CBVdesc, CPUHandle);
	}



	return true;
}

bool D3d12_Shader::DepthBuffer(ID3D12Device*& pDevice, ID3D12Resource*& DepthBuffer, ID3D12DescriptorHeap*& DSV_Heap) {
	//�[�x�o�b�t�@�q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES DspHeapProp = {};
	ZeroMemory(&DspHeapProp, sizeof(D3D12_HEAP_PROPERTIES));
	DspHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	DspHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	DspHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	DspHeapProp.CreationNodeMask = NULL;
	DspHeapProp.VisibleNodeMask = NULL;

	//�[�x�o�b�t�@�̐ݒ�
	D3D12_RESOURCE_DESC DspResDesc = {};
	ZeroMemory(&DspResDesc, sizeof(D3D12_RESOURCE_DESC));

	DspResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	DspResDesc.Alignment = 0;
	DspResDesc.Width = WINDOW_WIDTH;
	DspResDesc.Height = WINDOW_HEIGHT;
	DspResDesc.DepthOrArraySize = 1;
	DspResDesc.MipLevels = 1;
	DspResDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DspResDesc.SampleDesc.Count = 1;
	DspResDesc.SampleDesc.Quality = 0;
	DspResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	DspResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE ClearValue;
	ZeroMemory(&ClearValue, sizeof(D3D12_CLEAR_VALUE));
	ClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	ClearValue.DepthStencil.Depth = 1.0f;
	ClearValue.DepthStencil.Stencil = 0;

	//���\�[�X����
	auto hr = pDevice->CreateCommittedResource(
		&DspHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&DspResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&ClearValue,
		IID_PPV_ARGS(&DepthBuffer)
	);
	if (FAILED(hr)) {
		printf("�[�x�o�b�t�@				:ERROR\n");
	}
	else {
		printf("�[�x�o�b�t�@				:OK\n");
	}


	//�[�x�o�b�t�@�r���[�p�̃q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC DsvHeapDesc = {};
	ZeroMemory(&DsvHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	DsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DsvHeapDesc.NumDescriptors = 1;
	DsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DsvHeapDesc.NodeMask = NULL;

	//�[�x�o�b�t�@�p �f�B�X�N���v�^�q�[�v����
	hr = pDevice->CreateDescriptorHeap(&DsvHeapDesc, IID_PPV_ARGS(&DSV_Heap));
	if (FAILED(hr)) {
		printf("�f�B�X�N���v�^�q�[�v(�[�x�o�b�t�@)	:ERROR\n");
	}
	else {
		printf("�f�B�X�N���v�^�q�[�v(�[�x�o�b�t�@)	:OK\n");
	}

	//�[�x�o�b�t�@�r���[�̐ݒ�
	D3D12_DEPTH_STENCIL_VIEW_DESC DsvViewDesc = {};
	ZeroMemory(&DsvViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	DsvViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DsvViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DsvViewDesc.Flags = D3D12_DSV_FLAG_NONE;
	DsvViewDesc.Texture2D.MipSlice = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE DsvHandle = DSV_Heap->GetCPUDescriptorHandleForHeapStart();

	pDevice->CreateDepthStencilView(DepthBuffer, &DsvViewDesc, DsvHandle);

	return true;
}


bool D3d12_Shader::RootSignature(ID3D12Device*& pDevice, ID3D12RootSignature*& pRootSignature) {

	auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	//�X�^�e�B�b�N�T���v���̐ݒ�
	D3D12_STATIC_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	SamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	SamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	SamplerDesc.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	SamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	SamplerDesc.MinLOD = 0.0f;
	SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	SamplerDesc.ShaderRegister = 0;
	SamplerDesc.RegisterSpace = 0;
	SamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//���[�g�p�����[�^�[�̐ݒ�
	D3D12_ROOT_PARAMETER RootParam[2] = {};


	D3D12_DESCRIPTOR_RANGE range[2] = {};
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range[0].NumDescriptors = ConstantBufferCount;
	range[0].BaseShaderRegister = 0;
	range[0].RegisterSpace = 0;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[1].NumDescriptors = 1;
	range[1].BaseShaderRegister = 0;
	range[1].RegisterSpace = 0;
	range[1].OffsetInDescriptorsFromTableStart = 0;

	RootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[0].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[0].DescriptorTable.pDescriptorRanges = &range[0];
	RootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	RootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[1].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[1].DescriptorTable.pDescriptorRanges = &range[1];
	RootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;



	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
	RootSignatureDesc.NumParameters = _countof(RootParam);
	RootSignatureDesc.pParameters = RootParam;
	RootSignatureDesc.NumStaticSamplers = 1;
	RootSignatureDesc.pStaticSamplers = &SamplerDesc;
	RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* pBlob;
	ID3DBlob* pErrorBlob;

	//�V���A���C�Y
	auto hr = D3D12SerializeRootSignature(
		&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&pBlob,
		&pErrorBlob
	);
	if (FAILED(hr)) {
		printf("�V���A���C�Y				:ERROR\n");
		return false;
	}
	else {
		printf("�V���A���C�Y				:OK\n");
	}

	//���[�g�V�O�l�`������
	hr = pDevice->CreateRootSignature(
		0,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		IID_PPV_ARGS(&pRootSignature)
	);
	if (FAILED(hr)) {
		printf("���[�g�V�O�l�`��			:ERROR\n");
		return false;
	}
	else {
		printf("���[�g�V�O�l�`��			:OK\n");
	}

	return true;
}

bool D3d12_Shader::PipelineState(ID3D12Device*& pDevice,
	ID3D12RootSignature*& pRootSignature,
	ID3D12PipelineState*& pPipelineState) {

	//���̓��C�A�E�g�̐ݒ�
	D3D12_INPUT_ELEMENT_DESC elements[] =
	{
		{ "POSITION"   , 0, DXGI_FORMAT_R32G32B32_FLOAT	    , 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL"	   , 0, DXGI_FORMAT_R32G32B32_FLOAT	    , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//{ "COLOR"	   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT		, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//{ "BONE_INDEX" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};


	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
		FALSE, FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL
	};



	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		descBS.RenderTarget[i] = descRTBS;
	}

	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	//���_�V�F�[�_
	/*auto hr = D3DCompileFromFile(L"shader/SkinAnim.hlsl", NULL, NULL, "VS", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pVSBlob, nullptr);*/

	auto hr = D3DCompileFromFile(hlsl_arrary[0][0], NULL, NULL, "main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pVSBlob, &pErrorBlob);
	if (FAILED(hr)) {
		printf("���_�V�F�[�_				:ERROR\n");
	}
	else {
		printf("���_�V�F�[�_				:OK\n");
	}

	//�s�N�Z���V�F�[�_
	/*hr = D3DCompileFromFile(L"shader/SkinAnim.hlsl", NULL, NULL, "PS", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pPSBlob, nullptr);*/

	hr = D3DCompileFromFile(hlsl_arrary[0][1], NULL, NULL, "main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pPSBlob, &pErrorBlob);

	if (FAILED(hr)) {
		printf("�s�N�Z���V�F�[�_			:ERROR\n");
	}
	else {
		printf("�s�N�Z���V�F�[�_			:OK\n");
	}


	//���X�^���C�Y�̐ݒ�
	D3D12_RASTERIZER_DESC descRS;
	descRS.FillMode = D3D12_FILL_MODE_SOLID;
	//descRS.CullMode = D3D12_CULL_MODE_BACK;
	descRS.CullMode = D3D12_CULL_MODE_NONE;
	descRS.FrontCounterClockwise = false;
	descRS.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	descRS.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	descRS.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	descRS.DepthClipEnable = false;
	descRS.MultisampleEnable = FALSE;
	descRS.AntialiasedLineEnable = FALSE;
	descRS.ForcedSampleCount = 0;
	descRS.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
	{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };

	//�p�C�v���C���X�e�[�g�̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.InputLayout = { elements, _countof(elements) };
	desc.pRootSignature = pRootSignature;
	desc.VS = { pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize() };
	desc.PS = { pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize() };
	desc.RasterizerState = descRS;
	desc.BlendState = descBS;
	desc.DepthStencilState.DepthEnable = true;
	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	desc.DepthStencilState.FrontFace = defaultStencilOp;
	desc.DepthStencilState.BackFace = defaultStencilOp;
	desc.DepthStencilState.StencilEnable = false;
	desc.SampleMask = UINT_MAX;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;


	hr = pDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pPipelineState));
	if (FAILED(hr)) {
		printf("�p�C�v���C���X�e�[�g			:ERROR\n");
		return false;
	}
	else {
		printf("�p�C�v���C���X�e�[�g			:OK\n");
	}


	pVSBlob->Release();
	pPSBlob->Release();
	pErrorBlob->Release();

	return true;
}

void D3d12_Shader::ViewPort(D3D12_VIEWPORT& ViewPort, D3D12_RECT& ScissorRect) {
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = static_cast<float>(WINDOW_WIDTH);
	ViewPort.Height = static_cast<float>(WINDOW_HEIGHT);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

	ScissorRect.left = 0;
	ScissorRect.right = WINDOW_WIDTH;
	ScissorRect.top = 0;
	ScissorRect.bottom = WINDOW_HEIGHT;
}


bool D3d12_Shader::VertexBuffer(ID3D12Device*& pDevice, ID3D12Resource*& VertexBuffer, D3D12_VERTEX_BUFFER_VIEW& VertexBufferView) {
	//���_�f�[�^
	Vertex Vertices[] = {
		{ { -0.5f, 0.5f , 0.0f } },
		{ { -0.5f, -0.5f , 0.0f } },
		{ { 0.5f, -0.5f , 0.0f } },

		{ { 0.5f, 0.5f , 0.0f } },
	};

	//�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	// ���\�[�X�̐ݒ�.
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = sizeof(Vertices);
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&VertexBuffer)
	);
	if (FAILED(hr)) {
		printf("���_�o�b�t�@				:ERROR\n");
		return false;
	}
	else {
		printf("���_�o�b�t�@				:OK\n");
	}

	//�}�b�s���O
	void* ptr = nullptr;
	hr = VertexBuffer->Map(0, nullptr, reinterpret_cast<void**> (&ptr));
	if (FAILED(hr)) {
		printf("���_�o�b�t�@(�}�b�s���O)		:ERROR\n");
		return false;
	}
	else {
		printf("���_�o�b�t�@(�}�b�s���O)		:OK\n");
	}

	//���_�f�[�^���}�b�s���O��ɐݒ�
	memcpy(ptr, Vertices, sizeof(Vertices));

	//�}�b�s���O����
	VertexBuffer->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̐ݒ�.
	VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
	VertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(Vertices));
	VertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(Vertex));

	return true;
}


bool D3d12_Shader::IndexBuffer(ID3D12Device*& pDevice, ID3D12Resource*& IndexBuffer, D3D12_INDEX_BUFFER_VIEW& IndexBufferView) {
	uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };

	// �q�[�v�v���p�e�B.
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	// ���\�[�X�̐ݒ�.
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = sizeof(indices);
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	// ���\�[�X�𐶐�.
	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&IndexBuffer)
	);

	if (FAILED(hr))
	{
		printf("�C���f�b�N�X�o�b�t�@			:ERROR\n");
		return false;
	}
	else {
		printf("�C���f�b�N�X�o�b�t�@			:OK\n");
	}


	// �}�b�s���O����
	void* ptr = nullptr;
	hr = IndexBuffer->Map(0, nullptr, &ptr);
	if (FAILED(hr))
	{
		printf("�C���f�b�N�X�o�b�t�@(�}�b�s���O)	:ERROR\n");
		return false;
	}
	else {
		printf("�C���f�b�N�X�o�b�t�@(�}�b�s���O)	:OK\n");
	}

	// �C���f�b�N�X�f�[�^���}�b�s���O��ɐݒ�
	memcpy(ptr, indices, sizeof(indices));

	// �}�b�s���O����
	IndexBuffer->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�.
	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = sizeof(indices);

	return true;
}






