#include "YskLib.h"

using namespace YskLib;

WPtr<YsPolygon>	YsBillBoard::s_wpPolySquare;

void YsBillBoard::SetVertex(float x, float y, float w, float h)
{
	//[1]      [3]
	// *--------*
	// |�_�@�@�@|
	// |�@�_�@�@|h
	// |�@�@�_�@|
	// |�@�@�@�_|
	// *--------*
	//(x,y) w  [2]
	//[0]

	// ���_���W
	m_V[0].Pos.x = x;
	m_V[0].Pos.y = y;
	m_V[0].Pos.z = 0;

	m_V[1].Pos.x = x;
	m_V[1].Pos.y = y + h;
	m_V[1].Pos.z = 0;

	m_V[2].Pos.x = x + w;
	m_V[2].Pos.y = y;
	m_V[2].Pos.z = 0;

	m_V[3].Pos.x = x + w;
	m_V[3].Pos.y = y + h;
	m_V[3].Pos.z = 0;

	DirectX::SimpleMath::Vector3;
}

void YsBillBoard::Draw()
{
	// ��Ɨp�|���S���o�b�t�@�ݒ�
	if(m_pPoly == nullptr){
		// �|���S���f�[�^�����݂��Ȃ��Ȃ�쐬����
		if(s_wpPolySquare.expired()) {
			m_pPoly = std::make_shared<YsPolygon>();
			m_pPoly->Create(nullptr, 4, m_V[0].GetVertexTypeData());

			s_wpPolySquare = m_pPoly;
		}
		// ���łɍ쐬�ςȂ炻����g�p
		else{
			m_pPoly = s_wpPolySquare.lock();
		}
	}

	YsVec2 uvMin, uvMax;
	m_Anime.GetUV(&uvMin, &uvMax);
	// UV�ύX
	m_V[0].UV.x = uvMin.x;
	m_V[0].UV.y = uvMax.y;
	m_V[1].UV.x = uvMin.x;
	m_V[1].UV.y = uvMin.y;
	m_V[2].UV.x = uvMax.x;
	m_V[2].UV.y = uvMax.y;
	m_V[3].UV.x = uvMax.x;
	m_V[3].UV.y = uvMin.y;

	// ���_�f�[�^�𒸓_�o�b�t�@�ɏ�������
	m_pPoly->WriteVertexData(m_V, 4);

	// �`��ɕK�v�ȃf�[�^���Z�b�g
	m_pPoly->SetDrawData();

	// �`��
	m_pPoly->Draw(4);
}

