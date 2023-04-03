#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//! @brief					���������
//! @tparam T				COM�I�u�W�F�N�g	 
//! 
//! @param COM				�R���|�[�l���g�I�u�W�F�N�g���f��
template<class T>
inline void SafeRelease(T*& COM) {
	if (COM != nullptr) {
		COM->Release();
		COM = nullptr;
	}
}