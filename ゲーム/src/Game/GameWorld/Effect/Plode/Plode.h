
#pragma once

// �����p�N��

class EF_Explode : public BaseEffectObj {
public:
	// �ړ��͐ݒ�
	void SetvMove(const YsVec3& v){
		m_vMove.Set(v);
	}
	// �A�j�����x�ݒ�
	void SetAnimeSpeed(float speed){
		m_AnimeSpeed = speed;
	}

	void Init(float x, float y, float z, float size);
	// �����֐�
	virtual void Update() override;
	// �`��֐�
	virtual void Draw() override;

	SPtr<PointLight>	m_PL;

private:

	YsBillBoard	m_BB;				// �{���S���I�u�W�F�N�g
	YsVec3		m_vMove;			// �ړ���
	float		m_Ang;				// ��]�p�x
	float		m_AngPow;			// ��]��
	float		m_AnimeSpeed;		// �A�j�����x
};