#pragma once
#include "PCH.h"
#define MAX_BONES 255

//struct FbxCbuffer {
//	DirectX::XMMATRIX w;//���[���h�s��
//	DirectX::XMMATRIX wvp;//���[���h����ˉe�܂ł̕ύX�s��
//	DirectX::XMFLOAT4 lightDir;//���C�g�x�N�^�[
//	DirectX::XMFLOAT4 eye;//���C�g�J���[
//	DirectX::XMFLOAT4 ambient;//����
//	DirectX::XMFLOAT4 diffuse;//�g�U���ˌ�
//	DirectX::XMFLOAT4 specular;//���ʔ��ˌ�
//
//	FbxCbuffer() {
//		ZeroMemory(this, sizeof(FbxCbuffer));
//	}
//};
//
//struct SkinCbuffer {
//	DirectX::XMMATRIX w;//���[���h�s��
//	DirectX::XMMATRIX wvp;//���[���h����ˉe�܂ł̕ϊ��s��
//	DirectX::XMFLOAT4 lightDir;//���C�g����
//	DirectX::XMFLOAT4 eye;//���_�ʒu
//	DirectX::XMFLOAT4 ambient;//����
//	DirectX::XMFLOAT4 diffuse;//�g�U���ˌ�
//	DirectX::XMFLOAT4 specular;//���ʔ��ˌ�
//
//	DirectX::XMMATRIX bone[MAX_BONES];//�{�[��
//	SkinCbuffer()
//	{
//		ZeroMemory(this, sizeof(SkinCbuffer));
//		for (int i = 0; i < MAX_BONES; i++)
//		{
//			bone[i] = DirectX::XMMatrixIdentity();
//		}
//	}
//};

struct  Cbuffer {
	DirectX::XMMATRIX wvp;//���[���h����ˉe�܂ł̕ϊ��s��
};


//struct  Cbuffer
//{
//		DirectX::XMFLOAT4X4 W;//���[���h�s��
//		DirectX::XMFLOAT4X4 WVP;//���[���h����ˉe�܂ł̕ύX�s��
//		DirectX::XMFLOAT4 LightVector;//���C�g�x�N�^�[
//		DirectX::XMFLOAT4 LightColor;//���C�g�J���[
//		DirectX::XMFLOAT4 ambient;//����
//		DirectX::XMFLOAT4 diffuse;//�g�U���ˌ�
//		DirectX::XMFLOAT4 specular;//���ʔ��ˌ�
//	Cbuffer() {
//		ZeroMemory(this, sizeof(Cbuffer));
//	}
//};