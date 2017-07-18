//===============================================================
//! @file Helper.h
//! @brief その他補助的なもの
//!
//! @author 鎌田
//===============================================================
#ifndef __YsHelper_h__
#define __YsHelper_h__

namespace YskLib
{

class YsVec2;
class YsVec3;

//! @addtogroup Etc
//! @{


//! @brief ラジアン角 → 度 変換
#define YsToRadian(x) ((x)*0.017453f)

//! @brief 度 → ラジアン角 変換
#define YsToDegree(x) ((x)*57.3f)

// 安全な解放系

//! @brief 安全にReleaseするマクロ
template<typename T>
inline void YsSafe_Release(T*& p) {
	if (p) {
		p->Release();
		p = nullptr;
	}
}

//! @brief newされた変数を安全に解放するマクロ
template<typename T>
inline void YsSafe_Delete(T*& p) {
	if (p) {
		delete p;
		p = nullptr;
	}
}

//! @brief newされた配列を安全に解放するマクロ
template<typename T>
inline void YsSafe_Delete_Array(T*& p) {
	if (p) {
		delete[] p;
		p = nullptr;
	}
}

//===============================================
// スマートポインタ 短縮名
//===============================================
#define SPtr				std::shared_ptr
#define WPtr				std::weak_ptr
#define SPtr_dynamic_cast	std::dynamic_pointer_cast
#define SPtr_static_cast	std::static_pointer_cast
#define UPtr				std::unique_ptr
// weak_ptrから生ポインタを取得(lock()より高速)
// ※1フレームに何万回とlock()を行ってしまうとかなり重い
// 　lock()みたいにshared_ptrはいらんが、生ポインタでいい場合はコレ
#define WPtr_Get(wp)	(wp.expired() == false ? wp._Get() : nullptr)

//===============================================
// 生ポインタからshared_ptrへ変換する
// ※ただし、std::enable_shared_from_thisを継承したクラスに限る(それ以外のクラスはコンパイルエラーになる)
// ※当然そのクラスは、make_shared関数やnewで作られたshared_ptrに限る(普通の変数の場合はダメ)
// ※つまりこの生ポインタ(ptr)は、どこかでshared_ptrで保持されている必要がある
// 詳しくはstd::enable_shared_from_thisを調べてください
//  ptr	… 変換元となる生ポインタ
//  戻り値 : 変換後のshared_ptr
//===============================================
template<typename T>
inline SPtr<T> YsToSPtr(T* ptr) {
	return SPtr_static_cast<T>(ptr->shared_from_this());
}

//! @brief RGBA指定(0～255)からABGR形式のUINT作成
#define RGBA(r,g,b,a)	((UINT)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))
//! @brief RGBA指定(0.0～1.0)からABGR形式のUINT作成
#define fRGBA(r,g,b,a)	RGBA((DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f),(DWORD)((a)*255.f))



//=====================================================================
//! @brief ファイル名と拡張子の間に、指定文字列(ext)を入れる
//! @param[in]	FileName	… 元となるファイル名
//! @param[in]	ext			… 挿入したい文字列
//! @return extが挿入された文字列
//=====================================================================
std::string YsConvertExtFileName(const std::string& FileName, const std::string& ext);


//===================================================
//! @brief 法線マップ用のTangent,Binormalを算出する
//===================================================
void YsCalcTangentAndBinormal(
	YsVec3* p0, YsVec2* uv0,
	YsVec3* p1, YsVec2* uv1,
	YsVec3* p2, YsVec2* uv2,
	const YsVec3* vN, YsVec3* outTangent, YsVec3* outBinormal
);

//! @}

}
#endif