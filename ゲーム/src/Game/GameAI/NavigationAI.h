#pragma once

//�i�r�Q�[�V����AI
//�E�L�����N�^�[AI�̂��߂̌o�H�T����ʒu��͂��s��

class NavigationAI
{
public:

	//������
	void Init();

	/*Navigate------------------------------------------------
	�E�o�H�T��������

	�E����
	_MyPoa				:	���g�̍��W
	_TargetPos			:	�ڕW�̍��W

	�E�߂�l�Ȃ�
	----------------------------------------------------------*/
	void Navigate(YsVec3 _MyPos, YsVec3 _TargetPos);

	/*CreateWayPoint------------------------------------------------
	�E�E�F�C�|�C���g���쐬����
	�E�}�b�v��p

	�E����
	_sm			:	�Ώۂ̃��b�V��
	_pos			:	�Ώۂ̍��W

	�E�߂�l�Ȃ�
	-------------------------------------------------------------------*/
	void CreateWayPoints(SPtr<YsSingleModel>& _sm, YsVec3& _pos);

	/*UopdateWayPoint----------------------------------------------
	�E�E�F�C�|�C���g�̍X�V���s��
	�E�V�����I�u�W�F�N�g��z�u�����Ƃ��ɂ��̏�����ʂ�

	�E����
	_sm			:	�Ώۂ̃��b�V��
	_pos			:	�Ώۂ̍��W
	-------------------------------------------------------------------*/
	void UpdateWayPoint(SPtr<YsSingleModel>& _sm, YsVec3& _pos, int _num=0);

	/*DebugWayDraw-----------------------------------------
	�E�f�o�b�O�p��WayPoint��`�悷��
	-----------------------------------------------------------*/
	void DebugWayDraw();

private:

	//���̕ϐ�
	SPtr<YsGameModel> m_gmWay;
	YsModelObject	m_moWay;
	YsMatrix m_mWay;
	SPtr<YsGameModel> m_gmMap;
	YsModelObject	m_moMap;
	YsMatrix	m_mMap;

	//�E�F�C�|�C���g���X�g
	std::vector<YsVec3> m_WayPointList;

	//------------------------------------------------------------------
	// �V���O���g��
private:
	NavigationAI(){}
public:
	static NavigationAI& GetInstance()
	{
		static NavigationAI instance;
		return instance;
	}
	//------------------------------------------------------------------
};

#define NAI NavigationAI::GetInstance()