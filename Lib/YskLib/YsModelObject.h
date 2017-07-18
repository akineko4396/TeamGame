//===============================================================
//! @file YsModelObject.h
//! @brief メッシュオブジェクト操作クラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsModelObject_h__
#define __YsModelObject_h__

namespace YskLib{

//===========================================================================================
//!
//! @brief モデルオブジェクト操作クラス
//!
//! YsGameModelクラスを参照し、その情報を元にボーンを動作させるクラス				\n
//! [主な機能]																		\n
//! ・渡されたYsGameModelと同じボーン構造を作成										\n
//! ・アニメーションを行いたい場合は、YsAnimatorクラスを別途使用すればよい			\n
//! ・全ボーンのワールド行列をローカル行列を元に計算								\n
//!
//! @ingroup Graphics_Model_Important
//===========================================================================================
class YsModelObject{
public:
	class BoneNode;

	//=================================================================================
	// 情報取得
	//=================================================================================

	//! @brief 登録されているゲームモデルクラス取得
	SPtr<YsGameModel>					GetGameModel() { return m_pGameModel; }

	//! @brief ルートボーン取得　ボーンがない場合はnullptr
	SPtr<BoneNode>						GetRootBone() { return m_BoneTree.size() == 0 ? nullptr : m_BoneTree[0]; }

	//! @brief ボーンテーブル取得
	std::vector<SPtr<BoneNode>>&		GetBoneTree() { return m_BoneTree; }

	//! @brief アニメーションが存在するか？
	bool								IsAnimationExist() { return m_pGameModel->IsAnimationExist(); }

	//=================================================================================
	// 設定・解放
	//=================================================================================

	//------------------------------------------------------------------------
	//! @brief モデルを設定する
	//!
	//! YsGameModelを元にデータを構築する
	//!
	//! @param[in] pGameMesh	… モデルデータであるYsGameModel
	//! @param[in] animatorNum	… アニメ管理クラスをいくつ作成するか？
	//------------------------------------------------------------------------
	bool SetModel(SPtr<YsGameModel> pGameMesh, int animatorNum = 5);

	//------------------------------------------------------------------------
	//! @brief メッシュを読み込む
	//!
	//!  実際の読み込みはYsGameModelクラスで行っている		\n
	//!  この関数はYsGameModelを作成してメッシュを読込、SetMesh()読んでるだけ
	//!
	//! @param[in]	filename	… ファイル名
	//------------------------------------------------------------------------
	bool LoadMesh(const std::string& filename);

	//------------------------------------------------------------------------
	//! @brief 解放
	//------------------------------------------------------------------------
	void Release();

	//------------------------------------------------------------------------
	//! @brief このモデル用にアニメータを初期化する
	//! ※Rootボーンに関しては、複数トラックブレンドが無効になります
	//! @param[in]	animator	… 初期化するアニメータ
	//------------------------------------------------------------------------
	void InitAnimator(YsAnimator& animator);

	//=================================================================================
	// ボーン行列演算
	//=================================================================================

	//------------------------------------------------------------------------
	//! @brief m_Matをベースに全てのボーンのWorldMatを更新する(計算にはTransMatを使用)。
	//! 内部でrecCalcBoneMatrix関数を呼んでいるだけ
	//! @param[in]	baseMat	… 処理の基になる行列。この行列を先頭に全ボーンを計算していく。
	//------------------------------------------------------------------------
	void CalcBoneMatrix(const YsMatrix* baseMat);

	//! @brief m_Matをベースに全てのボーンのWorldMatを更新する(計算にはTransMatを使用)。
	void CalcBoneMatrix_SkipRoot(const YsMatrix* baseMat);

		//! @brief parentMatをベースに指定ボーン以下のWorldMatを更新する(計算にはTransMatを使用)。
		void recCalcBoneMatrix(BoneNode& node, const YsMatrix& parentMat);

	//------------------------------------------------------------------------
	//! @brief 全ボーンのTransMatをデフォルトに戻す
	//------------------------------------------------------------------------
	void ResetDefaultTransMat();

	//=================================================================================
	//=================================================================================
	
	//------------------------------------------------------------------------
	//! @brief ボーンテーブルから指定した名前のボーンの情報を取得 名前が存在しない場合はnullptrが返る
	//! @param[in]	BoneName	… 検索したいボーン名
	//! @return 存在する場合:ボーンデータのアドレス　存在しない場合:nullptr
	//------------------------------------------------------------------------
	SPtr<BoneNode> SearchBone(const std::string& BoneName) {
		for(UINT i = 0; i<m_BoneTree.size(); i++) {
			if(m_BoneTree[i]->pSrcBoneNode->BoneName == BoneName) {
				return m_BoneTree[i];
			}
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	//! @brief ボーンテーブルから指定した名前のボーンの番号を取得 名前が存在しない場合は-1が返る
	//! @param[in]	BoneName	… 検索したいボーン名
	//! @return 存在する場合:ボーン番号　存在しない場合:-1
	//------------------------------------------------------------------------
	int SearchBoneIndex(const std::string& BoneName){
		for(UINT i = 0; i<m_BoneTree.size(); i++) {
			if(m_BoneTree[i]->pSrcBoneNode->BoneName == BoneName) {
				return i;
			}
		}
		return -1;
	}



	// 
	YsModelObject() {
	}
	~YsModelObject() {
		Release();
	}

private:
	//---------------------------------------------------------------------------------
	//! @brief 元となるYsGameModelのポインタ
	//---------------------------------------------------------------------------------
	SPtr<YsGameModel>						m_pGameModel = nullptr;

	//---------------------------------------------------------------------------------
	//! @brief ボーンツリー＆ボーン配列
	//! [0]がRoot ツリー構造であるが、1次元配列としてもアクセス可能。
	//---------------------------------------------------------------------------------
	std::vector<SPtr<BoneNode>>					m_BoneTree;

public:
	//=====================================================================
	//! @brief YsModelObject用ボーンノード
	//! @ingroup Graphics_Model_Important
	//=====================================================================
	class BoneNode{
	public:
		SPtr<YsGameModel::BoneNode>	pSrcBoneNode;	//!< 元データ側のボーンへの参照

		WPtr<BoneNode>				Mother;			//!< 親ボーンへのアドレス
		std::vector<SPtr<BoneNode>>	Child;			//!< 子ボーン配列

		int							Level;			//!< 階層 Rootが0
		YsMatrix					TransMat;		//!< ローカル行列(Animation関数で更新される)
		YsMatrix					WorldMat;		//!< ワールド行列(CalcBoneMatrix関数で更新される)

		//-----------------------------------------------------------
		//! @brief 描画時に必要なOffsetWorld行列を算出する
		//! @param[out] resultOffsetWorldMat	… 計算結果が入る
		//-----------------------------------------------------------
		void CalcOffsetWorldMat(YsMatrix& resultOffsetWorldMat) {
			// Offset行列 * WorldMat
			YsMatrix::Multiply(resultOffsetWorldMat, pSrcBoneNode->OffsetMat, WorldMat);
		}

		//-----------------------------------------------------------
		//! @brief ボーンIndex取得
		//! @return ボーンIndex  失敗時は-1
		//-----------------------------------------------------------
		int GetBoneIndex() {
			if(pSrcBoneNode == nullptr)return -1;
			return pSrcBoneNode->OffsetID;
		}

		// 
		BoneNode() : Level(0), pSrcBoneNode(0){
		}
	};

private:
	// コピー禁止用
	YsModelObject(const YsModelObject& src) {}
	void operator=(const YsModelObject& src) {}
};

}
#endif
