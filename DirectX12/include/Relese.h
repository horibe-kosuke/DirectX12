#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//! @brief					メモリ解放
//! @tparam T				COMオブジェクト	 
//! 
//! @param COM				コンポーネントオブジェクトモデル
template<class T>
inline void SafeRelease(T*& COM) {
	if (COM != nullptr) {
		COM->Release();
		COM = nullptr;
	}
}