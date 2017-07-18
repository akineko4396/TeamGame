#pragma once

class Stage;
class BaseGameObj;

//==============================================
// ダメージ通知詳細データ
//  攻撃判定などで使うパラメータ
//==============================================
struct AttackParam {

	YsVec3					m_vBlow;			// ぶっ飛ばし力(ベクトル)

};

//==============================================
// ダメージ通知の返信データ
//  攻撃をヒットさせた相手からの返信情報として使う
//==============================================
struct DamageReply {
};

//==============================================
// ゲームオブジェクト基本クラス
//  ゲーム中に登場する物体の基本となるクラス
//==============================================

class BaseGameObj : public std::enable_shared_from_this<BaseGameObj>{
public:

	YsMatrix	m_Mat;			// キャラの行列
	YsMatrix	m_ChangedMat;	// 1フレームの移動で変化したぶんの行列

	//ステージを取得(ゲームシーン以外ではNULLしか返らない)
	Stage* GetStage();

	//======================================
	// ダメージ通知関数
	//  attacker : 攻撃者
	//  atk : ダメージ通知詳細パラメータ
	//  rep : 結果返信データ
	//======================================
	virtual bool OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep){ return true; }

};

#include "CollisionEngine/CollisionEngine.h"
#include "Game/GameCamera.h"
#include "GameKey.h"
#include "Input\CInput.h"


// Character
#include "Character\CharacterBase.h"
#include "Character\Player\Player.h"
#include "Character\Box\Box.h"
#include "Character\Map\Map.h"
#include "Character\Spring\Spring.h"
#include "Character\Water\Water.h"
#include "Character\Stairs\Stairs.h"


// Effect
#include "Effect\Effect.h"
#include "Effect\Plode\Plode.h"


// Item
#include "Item\Weapon.h"
#include "Item\Hammer\Hammer.h"


// Scene関係
#include "Game\GameWorld\Stage\StageBase.h"
#include "Game\Scene\GameScene\GameScene.h"

#include "GameLib\GameLib.h"
#include "Data\FilePath.h"







