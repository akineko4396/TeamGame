
#pragma once




#include "Game\GameWorld\Item\Weapon.h"




class BasePlayerAS;

class Player : public CharacterBase{
public:
	Player(){ m_ID = CharaID::PLAYER; }
	~Player(){}
	
	// 初期化 など
	void Init(YsMatrix &_m, SPtr<YsGameModel>& _gm);

	//++++++++++++++++++++++++++++++++++++
	// オーバーライド
	//++++++++++++++++++++++++++++++++++++

	virtual void Update()override;

	virtual void Draw()override;

	// カメラの更新処理
	//void CamUpdate(){ m_Cam.Update(); }
	
	// 視点カメラ
	virtual void SetCamera()override;

	// ダメージ
	bool OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep);


	//++++++++++++++++++++++++++++++++++++
	// ユーザー関数
	//++++++++++++++++++++++++++++++++++++

	// 地面レイ判定 など
	void Ground();

	// 行動状態関係

	int		m_SkyFlag = 0;			// 0:地上 1:空中
	YsVec3	m_vMove;				// 移動力

//private:

	//カメラを遅らせる
	static const int MemoNum = 3;
	YsVec3 m_MemoPos[MemoNum];

	GameCamera		m_Cam;
	SPtr<YsGameModel>	m_pSm;
	SPtr<HitObj_Sphere> m_hitBump;
	SPtr<HitObj_Ray>	m_hitGround;// 地面ぶつかり判定レイ
	SPtr<BasePlayerAS> m_ActionState;

	// 装備アイテム
	WPtr<BaseWeapon>m_Hammer;
	void SetWeapon(SPtr<BaseWeapon> weapon)
	{
		m_Hammer = weapon;
	}


	//プレイヤーに追従させるのに必要なベクトル
	static YsVec3 vTar;
	static YsVec3 GetTar(){ return vTar; }

	//吹っ飛び時に足し込む値が入る
	YsVec3 v;
};

//======================================
//Playerアクション基本クラス
//======================================
class BasePlayerAS{
public:
	virtual ~BasePlayerAS(){}
	//毎回実行される
	virtual void Update(Player & Player){}
};

//======================================
//「待機」状態
//======================================
class PlayerAS_Wait :public BasePlayerAS
{
public:
	~PlayerAS_Wait(){}
	//毎回実行される
	virtual void Update(Player & Player)override;
};
//======================================
//「走る」状態
//======================================
class PlayerAS_Run :public BasePlayerAS
{
public:
	~PlayerAS_Run(){}
	//毎回実行される
	virtual void Update(Player& Player) override;
};
//======================================
//「攻撃」状態
//======================================
class PlayerAS_Attack :public BasePlayerAS
{
public:
	//毎回実行される
	virtual void Update(Player& Player) override;
};

//======================================
//「ジャンプ攻撃」状態
//======================================
class PlayerAS_JumpAttack :public BasePlayerAS
{
public:
	//毎回実行される
	virtual void Update(Player& Player) override;
};

//======================================
//「ジャンプ(上昇)」状態
//======================================
class PlayerAS_JumpUp :public BasePlayerAS
{
public:
	//毎回実行される
	virtual void Update(Player& Player) override;
};

//======================================
//「ジャンプ(降下)」状態
//======================================
class PlayerAS_JumpDown :public BasePlayerAS
{
public:
	//毎回実行される
	virtual void Update(Player& Player) override;
};

//======================================
//「のけ反り」状態
//======================================
class PlayerAS_KnockBack :public BasePlayerAS
{
public:
	//毎回実行される
	virtual void Update(Player& Player) override;
};

//=====================================================
// 汎用行動
// アニメーションが終了したら「立ち」に戻るだけ
//=====================================================
class PlayerAS_Generic : public BasePlayerAS
{
public:
	// 毎回実行される
	virtual void Update(Player& Player) override;
};


