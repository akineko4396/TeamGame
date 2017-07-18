

#pragma once

// 識別用ID
enum CharaID{

	UNKOWN = 0,
	PLAYER,
	ENEMY,
	GROUND,
	BOX,
	LBOX,
	SPRING,
	WATER,
	STAIRS

};


// メッシュを持つキャラの基本クラス
//
//
class CharacterBase : public BaseGameObj{
public:
	virtual ~CharacterBase(){}
	CharacterBase(){}

	virtual void Update();
	virtual void Draw();
	virtual void SetCamera(){}

	// リスト管理

	static void AllUpdate();
	static void AllDraw();
	static void PushObject(SPtr<CharacterBase> _obj){ m_List.push_back(_obj); }
	static std::vector<SPtr<CharacterBase>> GetList(){ return m_List; }
	static void FullClear(){ m_List.clear(); }
	static void CheckKillFlg();

	// ゲッター

	int GetID(){ return m_ID; }
	int GetKillFlg(){ return m_KillFlg; }
	YsModelObject & GetModelObject(){ return m_Mo; }
	YsAnimator & GetAnime(){ return m_Anim; }
	YsMatrix GetMatrix(){ return m_Mat; }
	YsVec3 GetAngle(){ return m_Angle; }

	YsGameModel & GetGameModel(){ return *m_Mo.GetGameModel(); }
	int GetKeys() { return m_Keys; }
	
	// Templateクラスで使用
	YsVec3 GetPos(){ return m_Mat.GetPos(); }
	YsVec3 GetScale(){ return m_Mat.GetScale(); }

	// セッター

	// 入力コントローラ(コンポーネント)をセット
	inline void SetInput(SPtr<GameInputComponent> p) {
		m_Controller = p;
	}

	void SetKillFlg(){ m_KillFlg = true; }

	//	指定されたクラスをインスタンス化してリストに格納する
	template <class T>
	inline static SPtr<T> CreateCharaTask()
	{
		SPtr<T> add(new T);
		PushObject(add);
		return add;
	}

protected:

	static std::vector<SPtr<CharacterBase>> m_List;

	int m_ID;
	bool m_KillFlg=false;

	YsModelObject	m_Mo;
	YsAnimator		m_Anim;
	YsVec3 m_Angle;


	// 現在押下されてるキー このキー情報によりキャラを動かす(PlayerもAIも)
	// 各ビット(0～31)が各キーの状態
	int m_Keys = 0;

	// 入力コントローラ
	SPtr<GameInputComponent> m_Controller;
};

