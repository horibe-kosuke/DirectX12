#pragma once
#include "PCH.h"
#define MAX_BONES 255

//struct FbxCbuffer {
//	DirectX::XMMATRIX w;//ワールド行列
//	DirectX::XMMATRIX wvp;//ワールドから射影までの変更行列
//	DirectX::XMFLOAT4 lightDir;//ライトベクター
//	DirectX::XMFLOAT4 eye;//ライトカラー
//	DirectX::XMFLOAT4 ambient;//環境光
//	DirectX::XMFLOAT4 diffuse;//拡散反射光
//	DirectX::XMFLOAT4 specular;//鏡面反射光
//
//	FbxCbuffer() {
//		ZeroMemory(this, sizeof(FbxCbuffer));
//	}
//};
//
//struct SkinCbuffer {
//	DirectX::XMMATRIX w;//ワールド行列
//	DirectX::XMMATRIX wvp;//ワールドから射影までの変換行列
//	DirectX::XMFLOAT4 lightDir;//ライト方向
//	DirectX::XMFLOAT4 eye;//視点位置
//	DirectX::XMFLOAT4 ambient;//環境光
//	DirectX::XMFLOAT4 diffuse;//拡散反射光
//	DirectX::XMFLOAT4 specular;//鏡面反射光
//
//	DirectX::XMMATRIX bone[MAX_BONES];//ボーン
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
	DirectX::XMMATRIX wvp;//ワールドから射影までの変換行列
};


//struct  Cbuffer
//{
//		DirectX::XMFLOAT4X4 W;//ワールド行列
//		DirectX::XMFLOAT4X4 WVP;//ワールドから射影までの変更行列
//		DirectX::XMFLOAT4 LightVector;//ライトベクター
//		DirectX::XMFLOAT4 LightColor;//ライトカラー
//		DirectX::XMFLOAT4 ambient;//環境光
//		DirectX::XMFLOAT4 diffuse;//拡散反射光
//		DirectX::XMFLOAT4 specular;//鏡面反射光
//	Cbuffer() {
//		ZeroMemory(this, sizeof(Cbuffer));
//	}
//};