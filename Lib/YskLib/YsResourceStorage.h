//===============================================================
//! @file YsResourceStorage.h
//! @brief リソース管理関係
//!
//! @author 鎌田
//===============================================================
#ifndef __YsResourceStorage_h__
#define __YsResourceStorage_h__

#include <map>

namespace YskLib{

class YsTexture;
class YsGameModel;
class YsSoundData;

//===================================================================================
//! @brief リソース倉庫
//!
//!	YsDataWeakStorage<YsIResource>を継承(FlyWeightパターン的な感じ)						\n
//!		※これはYsIResourceクラスを継承したクラスインスタンスを作成・保管できるクラス。	\n
//!		　内部ではmapで登録しているので、重複読み込みが防げる。							\n
//!		※戻り値のデータはsharedポインタとして返るので、sharedポインタの特性上、		\n
//!		　解放処理は書かなくてよい。持ち主がいなくなった時点で自動で解放される。		\n
//!
//! @ingroup Etc
//===================================================================================
class YsResourceStorage : public YsDataWeakStorage<YsIResource, YsLock>{
public:
	//====================================================================================================
	//! @brief テスクチャを読み込みそれを返す。既に登録済みなら読み込みは行わず、それを返す。
	//! 詳細な読み込みの設定は、変数m_LoadTextureInfoを使用してるので、この変数を変更すると読み込みの細かな設定を変更できます
	//! @param[in]	filename	… ファイル名
	//! @return テクスチャ
	//====================================================================================================
	SPtr<YsTexture> LoadTexture(const std::string& filename);
	D3DX11_IMAGE_LOAD_INFO	m_LoadTextureInfo;		// LoadTexture関数で画像読み込み時の詳細設定　読み込みの細かな設定はこいつを変更すればOK

	//====================================================================================================
	//! @brief メッシュを読み込みそれを返す。既に登録済みなら読み込みは行わず、それを返す。
	//!	XファイルとXEDファイルに対応(スキンメッシュ対応)
	//! @param[in]	filename	… ファイル名
	//! @param[in]	bakeCurve	… 曲線補間系のアニメキーは、全て線形補間として変換・追加する(XED用 処理が軽くなります)
	//! @return YsGameModelモデル
	//====================================================================================================
	SPtr<YsGameModel> LoadMesh(const std::string& filename, bool bakeCurve = true);

	//====================================================================================================
	//! @brief 音声データのを読み込みそれを返す。既に登録済みなら読み込みは行わず、それを返す。
	//! @param[in]	b3D		… 3Dサウンドとして読み込む？(モノラル形式の音声ファイルでないと3Dサウンドにはできません)
	//! @return サウンドデータ
	//====================================================================================================
	SPtr<YsSoundData> LoadSound(const std::string& filename, bool b3D);




	//====================================================================================================
	// 全リソース名を取得
	//====================================================================================================
	void DEBUG_GetNowResources(std::vector<std::string>& out){
		auto dataMap = GetDataMap();
		auto it = dataMap.begin();
		while(it != dataMap.end()){
			if((*it).second.expired() == false){
				out.push_back( (*it).second.lock()->GetTypeName() );
			}

			++it;
		}
	}

	// 
	YsResourceStorage();
};

}

#endif
