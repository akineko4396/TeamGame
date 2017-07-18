
#pragma once

// 爆発パクり

class EF_Explode : public BaseEffectObj {
public:
	// 移動力設定
	void SetvMove(const YsVec3& v){
		m_vMove.Set(v);
	}
	// アニメ速度設定
	void SetAnimeSpeed(float speed){
		m_AnimeSpeed = speed;
	}

	void Init(float x, float y, float z, float size);
	// 処理関数
	virtual void Update() override;
	// 描画関数
	virtual void Draw() override;

	SPtr<PointLight>	m_PL;

private:

	YsBillBoard	m_BB;				// 板ボリゴンオブジェクト
	YsVec3		m_vMove;			// 移動力
	float		m_Ang;				// 回転角度
	float		m_AngPow;			// 回転力
	float		m_AnimeSpeed;		// アニメ速度
};