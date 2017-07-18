#include "main.h"
#include "Game\GameWorld\GameWorld.h"
#include "Game\GameWorld\Effect\Effect.h"



void EF_Explode::Init(float x, float y, float z, float size)
{
	YsTextureSet		m_tsExplode;	// ����
	//m_tsExplode.LoadTextureSet("data/Texture/Effect/Explode.png");
	m_tsExplode = APP.m_ResStg.LoadTexture("data/Texture/Effect/Explode.png");


	m_BB.SetTex(&m_tsExplode);
	m_BB.SetVertex(-size / 2, -size / 2, size, size);
	m_BB.GetAnime().Init(8, 8);

	m_Mat.CreateMove(x, y, z);

	m_Ang = 0;
	m_AngPow = APP.m_Rnd.GetFloat(-1, 1);

	m_AnimeSpeed = 1;
}



// �����֐�
void EF_Explode::Update()
{
	// �ړ�
	m_Mat.Move(m_vMove);

	// ��]�p�x
	m_Ang += m_AngPow;

	// �A�j���[�V�����i�s
	m_BB.GetAnime().Animation(m_AnimeSpeed, false);

	// 
	if (m_BB.GetAnime().IsEnd()){
		m_KillFlg = true;
	}

	// �|�C���g���C�g������Ȃ�A���W�X�V
	if (m_PL){
		m_PL->Position = m_Mat.GetPos();
	}

}

// �`��֐�
void EF_Explode::Draw()
{
	if (m_BB.GetAnime().GetPos() < 20){
		ShMgr.m_bsAdd.SetState();
	}
	else{
		ShMgr.m_bsAlpha.SetState();
	}

	YsMatrix m = m_Mat;
	// �J�����̌����ɍ��킹��
	m.SetBillBoard(YsCamera::LastCam.mCam);
	// �����Z��]
	m.RotateZ_Local(m_Ang);

	ShMgr.m_Samp.DrawBillBoard(m_BB, &m);
}
