#include "..\include\OriginalFormat.h"

OriginalFormat::OriginalFormat()
	: m_pDevice(nullptr)
	, m_pCommandAllocator(nullptr)
	, m_pQueue(nullptr)
	, m_pCommandList(nullptr)
	, m_pFence(nullptr)
	, m_FenceValue(0)
	, m_pPSO(nullptr)
{
}

OriginalFormat::~OriginalFormat()
{
	m_VertexBuffer->Release();
	for (int i = 0; i < NumMaterial; i++) {
		SafeRelease(m_IndexBuffer[i]);
	}


	delete[] m_Vertex;
	delete[] m_IndexBuffer;
}

float OriginalFormat::GetValue(const char* str, int offset)
{
	int i = 0;					//�t�@�C��������p
	int j = 0;					//ret������p
	int currentPos = 0;			//���݈ʒu
	char ret[256] = { NULL };	//����
	while (str[i] != '\n') {

		if (str[i] == ',') {
			i++;
			currentPos++;
		}

		if (currentPos == offset) {
			while (str[i] != ',' && str[i] != '\n')
			{
				ret[j] = str[i];
				j++;
				i++;
			}
			return atof(ret);
		}


		i++;
	}


	return 0.0f;
}

bool OriginalFormat::LoadMesh()
{
	FILE* fp;
	fp = fopen("OriginalFormat/Box_ASC.txt", "r");
	//fp = fopen("OriginalFormat/Cone_ASC.txt", "r");
	//fp = fopen("OriginalFormat/Box_Material_ASC.txt", "r");

	char str[256] = { NULL };

	//���_�̑���
	fgets(str, 256, fp);
	NumVertex = atoi(str);

	//���_�A�@���@�ǂݍ���
	m_Vertex = new OriginalVertex[NumVertex];
	for (int i = 0; i < NumVertex; i++) {
		fgets(str, 256, fp);
		m_Vertex[i].Pos.x = GetValue(str, 0);
		m_Vertex[i].Pos.y = GetValue(str, 1);
		m_Vertex[i].Pos.z = GetValue(str, 2);

		m_Vertex[i].Normal.x = GetValue(str, 3);
		m_Vertex[i].Normal.y = GetValue(str, 4);
		m_Vertex[i].Normal.z = GetValue(str, 5);
	}

	{
		//�o�[�e�b�N�X�o�b�t�@�[�쐬
		const UINT vertexbufferSize = sizeof(OriginalVertex) * NumVertex;

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
		desc.Width = vertexbufferSize;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		m_pDevice->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_VertexBuffer));

		//�o�[�e�b�N�X�o�b�t�@�Ƀt�@�C������ǂݍ��񂾒��_�f�[�^���l�ߍ���
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);

		m_VertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
		memcpy(pVertexDataBegin, m_Vertex, vertexbufferSize);
		m_VertexBuffer->Unmap(0, NULL);

		//�o�[�e�b�N�X�o�b�t�@�r���[��������
		m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
		m_VertexBufferView.StrideInBytes = sizeof(OriginalVertex);
		m_VertexBufferView.SizeInBytes = vertexbufferSize;
	}





	//���s
	fgets(str, 256, fp);

	//�}�e���A������
	fgets(str, 256, fp);
	NumMaterial = atoi(str);
	m_Material = new OriginalMaterial[NumMaterial];


	//�C���f�b�N�X
	m_IndexBuffer = new ID3D12Resource * [NumMaterial];
	m_IndexBufferView = new D3D12_INDEX_BUFFER_VIEW[NumMaterial];

	//�}�e���A���ǂݍ���
	for (int i = 0; i < NumMaterial; i++) {
		//Ambient
		fgets(str, 256, fp);
		m_Material[i].Ambient.x = GetValue(str, 0);
		m_Material[i].Ambient.y = GetValue(str, 1);
		m_Material[i].Ambient.z = GetValue(str, 2);

		//Diffuse
		fgets(str, 256, fp);
		m_Material[i].Diffuse.x = GetValue(str, 0);
		m_Material[i].Diffuse.y = GetValue(str, 1);
		m_Material[i].Diffuse.z = GetValue(str, 2);

		//Specular
		fgets(str, 256, fp);
		m_Material[i].Specular.x = GetValue(str, 0);
		m_Material[i].Specular.y = GetValue(str, 1);
		m_Material[i].Specular.z = GetValue(str, 2);

		//���s
		fgets(str, 256, fp);

		//�C���f�b�N�X����
		fgets(str, 256, fp);
		NumIndices[i] = atoi(str);

		m_Indices = new int[NumIndices[i]];

		//�C���f�b�N�X�ǂݍ���
		for (int j = 0; j <NumIndices[i]; j += 3) {
			fgets(str, 256, fp);
			m_Indices[j] = GetValue(str, 0);
			m_Indices[j + 1] = GetValue(str, 1);
			m_Indices[j + 2] = GetValue(str, 2);
		}

		{
			//�C���f�b�N�X�o�b�t�@�[���쐬
			UINT indexbufferSize = sizeof(int) * NumIndices[i];

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
			desc.Width = indexbufferSize;
			desc.Height = 1;
			desc.DepthOrArraySize = 1;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			desc.Flags = D3D12_RESOURCE_FLAG_NONE;

			m_pDevice->CreateCommittedResource(
				&prop,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_IndexBuffer[i]));

			//�C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�f�[�^���l�ߍ���
			UINT8* pIndexDataBegin;
			CD3DX12_RANGE readRange2(0, 0);
			m_IndexBuffer[i]->Map(0, &readRange2, reinterpret_cast<void**>(&pIndexDataBegin));
			memcpy(pIndexDataBegin, m_Indices, indexbufferSize);
			m_IndexBuffer[i]->Unmap(0, NULL);

			//�C���f�b�N�X�o�b�t�@�r���[��������		
			m_IndexBufferView[i].BufferLocation = m_IndexBuffer[i]->GetGPUVirtualAddress();
			m_IndexBufferView[i].Format = DXGI_FORMAT_R32_UINT;
			m_IndexBufferView[i].SizeInBytes = indexbufferSize;

		}

		//���s
		fgets(str, 256, fp);
	}


	//�A�j���[�V�����t���[������
	fgets(str, 256, fp);
	int Num = atoi(str);
	NumAnim = new int[Num];

	for (int i = 0; i < Num; i++) {

	}




	fclose(fp);



	return true;
}


bool OriginalFormat::Init(ID3D12Device* pDevice, ID3D12CommandAllocator* pCommandAllocator, ID3D12CommandQueue* pQueue, ID3D12GraphicsCommandList* pCmdList, ID3D12Fence* pFence, UINT64 FenceValue, ID3D12PipelineState* pPSO)
{
	m_pDevice = pDevice;
	m_pCommandAllocator = pCommandAllocator;
	m_pQueue = pQueue;
	m_pCommandList = pCmdList;
	m_pFence = pFence;
	m_FenceValue = FenceValue;
	m_pPSO = pPSO;

	if (LoadMesh() == false) {
		printf("StaticMesh		:ERROR\n");
		return false;
	}


	//�R���X�^���g�o�b�t�@
	{
		//�R���X�^���g�o�b�t�@�쐬
		UINT alignment = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1;
		m_CBSize = (sizeof(OriginalCbuffer) + alignment) & ~alignment;
		auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(m_CBSize * NumMaterial);
		m_pDevice->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&buffer,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBuffer));

		//�R���X�^���g�o�b�t�@�r���[�ƃe�N�X�`���[�r���[�̃q�[�v�쐬
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = 1 * NumMaterial;//SRV(texture)�̕�������̂Ł@(1+1)
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_CBVHeap));

		//�R���X�^���g�o�b�t�@�r���[�쐬
		UINT cbvSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		UINT srvSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_CPU_DESCRIPTOR_HANDLE cHandle = m_CBVHeap->GetCPUDescriptorHandleForHeapStart();
		for (int i = 0; i < NumMaterial; i++)
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = m_ConstantBuffer->GetGPUVirtualAddress() + i * m_CBSize;
			cbvDesc.SizeInBytes = m_CBSize;
			m_pDevice->CreateConstantBufferView(&cbvDesc, cHandle);


			cHandle.ptr += cbvSize;
		}
	}

	return true;
}

void OriginalFormat::Render(DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj, DirectX::XMFLOAT4& light, DirectX::XMFLOAT3& eye)
{

	static float r = 0.0f;
	static float Yr = 0.0f;
	r += 0.01f;
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationY(r);//��]������
	//DirectX::XMMATRIX Mat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90));
	world *= rotMat;
	//�o�[�e�b�N�X�o�b�t�@���Z�b�g
	m_pCommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);

	//�R���X�^���g�o�b�t�@�̓��e���X�V
	CD3DX12_RANGE readRange(0, 0);
	readRange = CD3DX12_RANGE(0, 0);
	UINT8* pCbvDataBegin;
	m_ConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pCbvDataBegin));
	OriginalCbuffer cb;
	char* ptr = reinterpret_cast<char*>(pCbvDataBegin);

	cb.wvp = DirectX::XMMatrixTranspose(world * view * proj);//���[���h�A�J�����A�ˉe�s���n��
	cb.w = DirectX::XMMatrixTranspose(world);//���[���h�s���n��
	cb.lightDir = light;//���C�g������n��
	//cb.eye = DirectX::XMFLOAT4(eye.x, eye.y, eye.z, 0);//���_��n��


	//�q�[�v�i�A�v���ɂ���1�����j���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { m_CBVHeap };
	m_pCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��i�T�u���b�V���j
	for (int i = 0; i < NumMaterial; i++)
	{
		//diffuse ambient specular
		cb.diffuse = m_Material[i].Diffuse;
		//cb.ambient = m_Material[i].Ambient;
		//cb.specular =m_Material[i].Specular;

		//texture

		//���̃}�e���A�����i�[����R���X�^���g�o�b�t�@�̈ʒu�܂Ń|�C���^�[��i�߂�	

		//CBV��SRV�i�̃q�[�v�̐擪�A�h���X�j���e�[�u���Ƃ����`�ň����o��0�ɃZ�b�g
		D3D12_GPU_DESCRIPTOR_HANDLE handle = m_CBVHeap->GetGPUDescriptorHandleForHeapStart();
		handle.ptr += i * m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_pCommandList->SetGraphicsRootDescriptorTable(0, handle);

		//�R���X�^���g�o�b�t�@���̓����ʒu��i�߂�
		ptr = reinterpret_cast<char*>(pCbvDataBegin) + m_CBSize * i;
		memcpy(ptr, &cb, sizeof(OriginalCbuffer));

		//�C���f�b�N�X�o�b�t�@���Z�b�g
		m_pCommandList->IASetIndexBuffer(&m_IndexBufferView[i]);

		//�`��
		m_pCommandList->DrawIndexedInstanced(NumIndices[i], 1, 0, 0, 0);
	}
}
