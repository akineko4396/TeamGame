//===============================================================
//! @file YsDataStorage.h
//! @brief データ管理関係
//!
//! @author 鎌田
//===============================================================
#ifndef __YsDataManager_h__
#define __YsDataManager_h__

namespace YskLib
{

//================================================================================
//! @brief 汎用データ保管クラス
//!
//! (Singleton無しのFlyWeightパターンのような感じ)	\n
//!  weak_ptr管理:作成はshared_ptrで返す。所有者がいなくなった時点でデータは自動解放される(weak_ptrなので)。		\n
//!	             map内にノードは残るが、AddData()やGetData()したときに、解放済みのデータがあればmapから解除する。	\n
//!				 またはOptimize()でも可能。																			\n
//!
//!  ・マルチスレッド対応(LockTypeに排他制御クラスの型を指定する)
//!
//! @ingroup Etc
//================================================================================
template<typename T, typename LockType>
class YsDataWeakStorage{
private:
	class LockGuard;

public:
	//==========================================================================
	//! @brief データ追加
	//!
	//! 指定キー名のデータが存在しなければ指定型Xを新規追加し返す	\n
	//! 既に存在する場合はそれを返す								\n
	//! ※生成する型指定Ver											\n
	//!
	//! @param[in]	Name		… キー名
	//! @param[in]	pbCreate	… 生成した場合はtrue　既に存在する場合はfalseが入る
	//! @return 存在する：既存のデータ　存在しない：新規作成したデータ
	//==========================================================================
	template<typename X>
	SPtr<X> AddData_Type(const std::string& Name, bool* pbCreate = nullptr);
	
	//==========================================================================
	//! @brief データ追加
	//!
	//! 指定キー名のデータが存在しなければ型Tを新規追加し返す	\n
	//! 既に存在する場合はそれを返す							\n
	//!
	//! @param[in]	Name		… キー名
	//! @param[in]	pbCreate	… 生成した場合はtrue　既に存在する場合はfalseが入る
	//! @return 存在する：既存のデータ　存在しない：新規作成したデータ
	//==========================================================================
	SPtr<T> AddData(const std::string& Name, bool* pbCreate = nullptr)	
	{
		return AddData_Type<T>(Name, pbCreate);
	}

	//=======================================================
	//! @brief 指定名のデータ取得
	//! @return 存在する：データ　存在しない：nullptr
	//=======================================================
	SPtr<T> GetData(const std::string& Name);

	//=======================================================
	//! @brief 指定キーのデータを管理マップから削除
	//! @param[in]	Name		… キー名
	//=======================================================
	void DelData(const std::string& Name);

	//=======================================================
	//! @brief 解放
	//! 全てのデータを消す
	//=======================================================
	void Release();

	//=======================================================
	//! @brief 最適化
	//!	破壊済みのノードを消す
	//=======================================================
	void Optimize();

	//=======================================================
	//! @brief ノード数取得
	//=======================================================
	UINT GetNodeNum(){
		// 排他制御
		YsLockGuard<LockType> lock(m_Lock);

		return m_DataMap.size();
	}

	//=======================================================
	//! @brief 全リソースマップを取得
	//! ※マップ自体は排他制御でコピーして返しているが、内部データはマルチスレッドアクセス時には注意
	//=======================================================
	std::unordered_map< std::string, const WPtr<T> > GetDataMap() {
		// 排他制御
		YsLockGuard<LockType> lock(m_Lock);

		return (std::unordered_map< std::string, const WPtr<T> >&)m_DataMap;
	}


public:

	// 
	YsDataWeakStorage() : m_Cnt(0){
	}
	// 
	~YsDataWeakStorage(){
		Release();
	}

private:
	std::unordered_map< std::string, WPtr<T> >	m_DataMap;		//!< 登録map
	int											m_Cnt;			//!< 定期的な最適化用カウンタ

	// マルチスレッド用排他制御オブジェクト
	//  MathのYsLocking.hのクラスを指定する
	LockType									m_Lock;


private:
	// コピー禁止用
	YsDataWeakStorage(const YsDataWeakStorage<T, LockType>& src) {}
	void operator=(const YsDataWeakStorage<T, LockType>& src) {}
};

#include "YsDataStorage.inl"


}
#endif