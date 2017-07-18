//===============================================================
//! @file YsGameModel.h
//! @brief ゲーム3Dモデルクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsGameModel_h__
#define __YsGameModel_h__

#include "YsKeyframeAnimation.h"

namespace YskLib{

//=======================================================================
// 剛体データ
//=======================================================================
struct YsXEDRigidBody {
	std::string rigidBodyName;				// 剛体名
	int boneIndex;							// 関連ボーンIndex - 関連なしの場合は-1
	BYTE rigidBodyGroup;					// グループ
	unsigned short unCollisionGroupFlag;	// 非衝突グループフラグ
	BYTE shape;								// 形状 - 0:球 1:箱 2:カプセル
	YsVec3 size;							// サイズ(x,y,z)
	YsVec3 position;						// 位置(x,y,z)
	YsVec3 rotation;						// 回転(x,y,z) -> ラジアン角
	float mass;								// 質量
	float moveAttenuation;					// 移動減衰
	float rotationAttenuation;				// 回転減衰
	float repulsion;						// 反発力
	float friction;							// 摩擦力
	BYTE physicsCalcType;					// 剛体の物理演算 - 0:ボーン追従(static) 1:物理演算(dynamic) 2:物理演算 + Bone位置合わせ

	YsXEDRigidBody() :
		rigidBodyGroup(0),
		unCollisionGroupFlag(0),
		shape(0),
		size(0.1f, 0.1f, 0.1f),
		position(0, 0, 0),
		rotation(0, 0, 0),
		mass(1), moveAttenuation(0.5f), rotationAttenuation(0.5f), repulsion(0), friction(0.5f), physicsCalcType(0)
	{
	}

	// rotation,positionから行列を返す
	YsMatrix GetMatrix(){
		YsMatrix m;
		m.CreateRotateZ(rotation.z);
		m.RotateX(rotation.x);
		m.RotateY(rotation.y);
		m.Move(position);
		return m;
	}
};

//=================================================================================
//! @brief ゲームに適した多機能モデルクラス
//! 
//! 複数のモデルデータ・ボーン階層構造・アニメーションなどを保持するクラス	\n	
//! スキンメッシュ・スタティックメッシュ両方扱えます						\n
//!	[主な機能]																\n
//!   ・複数のメッシュデータ(複数のYsSingleModel)							\n
//!   ・ボーンデータ														\n
//!   　※スタティックメッシュでも最低１つはボーンができる(Rootボーン)		\n
//!       ただしこのボーンは動かしても全く頂点には影響しない				\n
//!   ・アニメーションデータ												\n
//!																			\n
//!  YsModelObjectクラスにセットして使用する。								\n
//!
//! @ingroup Graphics_Model_Important
//=================================================================================
class YsGameModel : public YsIResource{
public:
	// YsGameModel用ボーンノードクラス
	class BoneNode;

public:
	//=================================================================================
	// 情報取得
	//=================================================================================

	//! @brief ボーン参照用配列取得
	std::vector<SPtr<BoneNode>>&			GetBoneTree() { return m_BoneTree; }
	
	//! @brief ルートボーン取得
	SPtr<BoneNode>							GetRoot() { return (m_BoneTree.size() == 0 ? nullptr : m_BoneTree[0]); }
	
	//! @brief メッシュ配列取得(全て)
	std::vector<SPtr<YsSingleModel>>&		GetModelTbl() {return m_ModelTbl;}
	
	//! @brief メッシュ配列取得(スキンメッシュのみ)
	std::vector<SPtr<YsSingleModel>>&		GetModelTbl_Skin(){ return m_RefModelTbl_SkinOnly; }
	
	//! @brief メッシュ配列取得(スタティックメッシュのみ)
	std::vector<SPtr<YsSingleModel>>&		GetModelTbl_Static(){ return m_RefModelTbl_StaticOnly; }
	
	//! @brief アニメーションリストを取得
	std::vector<SPtr<YsAnimationSet>>&		GetAnimeList() { return m_AnimeList; }

	//! @brief 剛体リストを取得
	std::vector< std::vector<YsXEDRigidBody> >&	GetRigidBodyTbl(){ return m_RigidBodyTbl; }
	
	//! @brief ファイルまでのパス取得
	const std::string&						GetFilePath() const { return m_FilePath; }

	//! @brief ファイル名取得
	const std::string&						GetFileName() const { return m_FileName; }

	//! @brief パスを含めたファイル名取得
	std::string								GetFullFileName() const { return m_FilePath + m_FileName; }

	//------------------------------------------------------
	//! @brief メッシュ読み込み(スタティックメッシュ、スキンメッシュ両対応)
	//!	対応形式：X,Xed
	//! @param[in]	fileName	… ファイル名
	//! @param[in]  bakeCurve	… 曲線補間系のアニメキーは、全て線形補間として変換・追加する(XED用 処理が軽くなります)
	//------------------------------------------------------
	bool LoadMesh(const std::string& fileName, bool bakeCurve = true);

private:
	//------------------------------------------------------
	//! @brief Xファイル読み込み ※DirectX SDKのD3DX9を使用して読み込みしてます
	//!
	//! "//"から始まるアニメ名のアニメは無視されます
	//!
	//------------------------------------------------------
	bool LoadXFile(const std::string& fileName);

	//------------------------------------------------------
	//! @brief XED(独自形式)読み込み
	//!
	//! XEDファイルはKAMADA SkinMesh Editorから出力できるモデル形式です		\n
	//! "//"から始まるアニメ名のアニメは無視されます	\n
	//!
	//! @param[in]  bakeCurve	… 曲線補間系のアニメキーは、全て線形補間として変換・追加する(処理が軽くなります)
	//------------------------------------------------------
	bool LoadXEDFile(const std::string& fileName, bool bakeCurve = true);

public:
	//------------------------------------------------------
	//! @brief 解放
	//------------------------------------------------------
	void Release();

	//=================================================================================
	// その他
	//=================================================================================

	//------------------------------------------------------
	//! @brief ボーン名検索 見つからない場合はnullptr
	//! @param[in]	Name	… ボーン名
	//! @return ボーン発見:ボーンデータのアドレス  ボーン無し:nullptr
	//------------------------------------------------------
	SPtr<BoneNode> SearchBone(const std::string& Name){
		for(UINT i = 0; i<m_BoneTree.size(); i++) {
			// 文字列判定
			if(m_BoneTree[i]->BoneName == Name) {
				return m_BoneTree[i];
			}
		}
		return nullptr;
	}

	//------------------------------------------------------
	//! @brief ボーン名からインデックス番号検索 見つからない場合は-1
	//! @param[in]	Name	… ボーン名
	//! @return ボーン発見:ボーン番号  ボーン無し:-1
	//------------------------------------------------------
	int SearchBoneIndex(const std::string& Name) const{
		for(int i=0;i<(int)m_BoneTree.size();i++){
			// 文字列判定
			if(m_BoneTree[i]->BoneName == Name) {
				return i;
			}
		}
		return -1;
	}

	//------------------------------------------------------
	//! @brief アニメーションが存在するか？
	//------------------------------------------------------
	bool IsAnimationExist(){
		return !m_AnimeList.empty();
	}

	//------------------------------------------------------
	//! @brief アニメーション名からIndexを取得。ない場合は-1
	//! @param[in]	AnimeName	… アニメ名
	//! @return 発見：アニメ番号　アニメ無し：-1
	//------------------------------------------------------
	int SearchAnimation(const std::string& AnimeName){
		for(UINT i=0;i<m_AnimeList.size();i++){
			if(m_AnimeList[i]->m_AnimeName == AnimeName){
				return i;
			}
		}
		return -1;
	}

	//------------------------------------------------------
	//! @brief アニメーションを全削除
	//------------------------------------------------------
	void DeleteAnimation(){
		m_AnimeList.clear();
		m_AnimeList.shrink_to_fit();
	}

	//------------------------------------------------------
	//! @brief リソース情報
	//------------------------------------------------------
	virtual std::string GetTypeName() const override {
		std::string str = "YsGameModel : ";
		str += GetFullFileName();
		return str;
	}

	// 
	YsGameModel();
	// 
	~YsGameModel();


private:
	std::string						m_FilePath;						//!< ファイルまでのパス

	std::string						m_FileName;						//!< ファイル名


	//---------------------------------------------------------------------------------
	// メッシュテーブル
	//---------------------------------------------------------------------------------
	std::vector<SPtr<YsSingleModel>>	m_ModelTbl;						//!< 全てのメッシュへの参照が入っている配列
	
	std::vector<SPtr<YsSingleModel>>	m_RefModelTbl_SkinOnly;			//!< m_ModelTblの中で、スキンメッシュのみのメッシュへの参照が入っている配列　CreateRefMeshTbl関数で作成される
	std::vector<SPtr<YsSingleModel>>	m_RefModelTbl_StaticOnly;		//!< m_ModelTblの中で、スタティックメッシュのみのメッシュへの参照が入っている配列　CreateRefMeshTbl関数で作成される
		void CreateRefMeshTbl();		//!< m_ModelTblの内容から、スキンメッシュとスタティックメッシュの参照を分けたテーブル作成

	//---------------------------------------------------------------------------------
	//! @brief ボーン参照用配列(ボーン配列)
	//! [0]がRoot ツリー構造であるが、1次元配列としてもアクセス可能。
	//---------------------------------------------------------------------------------
	std::vector<SPtr<BoneNode>>			m_BoneTree;

	//---------------------------------------------------------------------------------
	// アニメーションリスト
	//---------------------------------------------------------------------------------
	std::vector<SPtr<YsAnimationSet>>	m_AnimeList;					//!< アニメーションリスト

	//---------------------------------------------------------------------------------
	// 剛体リスト
	//---------------------------------------------------------------------------------
	std::vector< std::vector<YsXEDRigidBody> >		m_RigidBodyTbl;	// 二次元配列です KAMADA SkinMesh Editorでは0～9の10個登録できるので、m_RigidBodyTbl[0～9][剛体Index]となる

private:
	// コピー禁止用
	YsGameModel(const YsGameModel& src) {}
	void operator=(const YsGameModel& src) {}

public:
	//=================================================================================
	//! @brief YsGameModel用ボーンノード
	//! @ingroup Graphics_Model
	//=================================================================================
	class BoneNode {
	public:
		//=========================
		// ボーンデータ
		//=========================
		std::string					BoneName;		//!< ボーン名
		int							Level;			//!< 階層 Rootが0
		YsMatrix					OffsetMat;		//!< オフセット行列
		DWORD						OffsetID;		//!< 行列テーブルのインデックス番号

		YsMatrix					DefTransMat;	//!< デフォルト姿勢での変換行列　特別なことがない限り使用しない
		YsMatrix					DefWorldMat;	//!< デフォルト姿勢でのワールド行列　特別なことがない限り使用しない

		WPtr<BoneNode>				pMother;		//!< 親ボーンのアドレス
		std::vector<SPtr<BoneNode>>	Child;			//!< 子ボーン配列

		//=========================
		// XED拡張データ
		//=========================
#pragma region XED拡張データ
		//! @brief ボーン表示レイヤー(Xed用)
		int						BoneLayers;

		//! @brief ボーンフラグ(Xed用)
		int						BoneFlag;

		//! @brief ボーンフラグ用定数(Xed用)
		enum {
			BF_ROTATE = 0x00000002,		//!< 回転可
			BF_MOVE = 0x00000004,		//!< 移動可
			BF_DISPBONE = 0x00000008,	//!< 表示
			BF_OPERATE = 0x00000010,	//!< 操作可
			BF_IK = 0x00000020,			//!< IK有効

			BF_PROVIDEROTA = 0x00000100,	//!< 回転付与
			BF_PROVIDEMOVE = 0x00000200,	//!< 移動付与

			BF_DISPBONELINE = 0x00010000,	//!< 親からのボーンライン表示ON
		};

		//! @brief ボーンフラグ：回転有効？(Xed用)
		bool BF_IsRotate(){ return (BoneFlag & BF_ROTATE ? true : false); }
		//! @brief ボーンフラグ：移動有効？(Xed用)
		bool BF_IsMove(){ return (BoneFlag & BF_MOVE ? true : false); }
		//! @brief ボーンフラグ：ボーン表示有効？(Xed用)
		bool BF_IsDispBone(){ return (BoneFlag & BF_DISPBONE ? true : false); }
		//! @brief ボーンフラグ：IK有効？(Xed用)
		bool BF_IsIK(){ return (BoneFlag & BF_IK ? true : false); }
		//! @brief ボーンフラグ：操作可能？(Xed用)
		bool BF_IsOperate(){ return (BoneFlag & BF_OPERATE ? true : false); }
		//! @brief ボーンフラグ：親からのボーンライン表示有効？(Xed用)
		bool BF_IsDispBoneLine(){ return (BoneFlag & BF_DISPBONELINE ? true : false); }
		//! @brief ボーンフラグ：回転付与有効？(Xed用)
		bool BF_IsProvideRotate(){ return (BoneFlag & BF_PROVIDEROTA ? true : false); }
		//! @brief ボーンフラグ：移動付与有効？(Xed用)
		bool BF_IsProvideMove(){ return (BoneFlag & BF_PROVIDEMOVE ? true : false); }

		// IKデータ(Xed用)
		struct IKData {
			int		boneIdx;			// ターゲットボーンIndex
			int		LoopCnt;			// 計算ループカウント
			float	ikLimitedAng;		// 単位角
			BYTE	flags;				// 未使用

			struct LinkData {
				int			boneIdx;		// リンクボーンIndex
				bool		bLimitAng;		// 角度制限有効？
				YsVec3		minLimitAng;	// 角度制限：最低角度
				YsVec3		maxLimitAng;	// 角度制限：最大角度

				LinkData() : boneIdx(-1), bLimitAng(false), minLimitAng(0, 0, 0), maxLimitAng(0, 0, 0) {}
			};

			std::vector<LinkData>	LinkList;

			IKData() : boneIdx(-1), LoopCnt(0), ikLimitedAng(0), flags(0) {}
		};
		IKData					IK;	//!< IKデータ(Xed用)

#pragma endregion

		// 
		BoneNode() : OffsetID(0), Level(-1), BoneLayers(0), BoneFlag(0)
		{
		}
	};
};


}

#include "YsXFileLoader.h"
#include "YsXEDLoader.h"


#endif
