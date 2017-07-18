//===============================================================
//! @file YsKeyframeAnimation.h
//! @brief �L�[�t���[���A�j���[�V�����n�̃N���X�Q
//!
//! @author ���c
//===============================================================
#ifndef __YsKeyFrameAnimatoin_h__
#define __YsKeyFrameAnimatoin_h__

namespace YskLib {

//=====================================================
//! @brief �L�[�t���[���A�j���p�N�H�[�^�j�I���f�[�^
//!
//! @ingroup Animation
//=====================================================
struct YsAnimeKey_Quaternion {
	float Time;			//!< �L�[�̈ʒu(����)
	YsQuat Value;		//!< �L�[�̃N�H�[�^�j�I���l

	// ��XED��XMD�ǂݍ��݂ł́A�������x�D��̂��߂�1��2�̕�Ԃ͋Ȑ��ɂȂ�悤��0�Ƃ��ďĂ����̂ŁA��{��0���g��
	char CalcType;		//!< ��Ԍv�Z���@ 0:���ʐ��`��� 1:���ʂQ����� 2:�x�W�F�Ȑ����

//	YsQuat qABC[3];		//!< CalcType == 1�p�̐���_A,B,C(���O�v�Z)

	// �x�W�F�p
	char BezierA[2];
	char BezierB[2];

	// 
	YsAnimeKey_Quaternion() : Time(0), CalcType(0){
		// �Ƃ肠����MMD�Ɠ��������̏����l
		BezierA[0] = 20;
		BezierA[1] = 20;
		BezierB[0] = 107;
		BezierB[1] = 107;
	}

	//! @brief �w�莞�Ԃ̉�]�f�[�^���Ԍv�Z�ŋ��߂�(static�֐�)
	//! @param[in] rotateList		�c �L�[�t���[���z��
	//! @param[in] time				�c ��Ԍv�Z�ŋ��߂������Ԃ��w��
	//! @param[out]	outQ			�c ��Ԍ��ʂ̉�](�N�H�[�^�j�I��)������
	//! @param[inout] nowKeyIndex	�c �L�[�t���[���z��̂ǂ̈ʒu����L�[�̌������s�����H����Ɍ�����͂���Index������Bnullptr�w��ōŏ����猟��(����������)
	static bool InterpolationFromTime(const std::vector<YsAnimeKey_Quaternion*>& keyList, double time, YsQuat &outQ, UINT* nowKeyIndex = nullptr);

	//! @brief ��]�L�[���X�g(keyList)���́A�Ȑ��n�̕�ԃL�[(�X�v���C����x�W�F)���A�ʏ�L�[�Ƃ��ďĂ����݂��s��(static�֐�)
	//! @param[inout] keyList		�c �ҏW����L�[�t���[���z��
	//! @param[in]	  bakeSpace		�c �Ă����݊Ԋu  0�őS�Ė��߂� 1��Time��1�ÊԊu�������Ă����� 2�`���l
	static void BakeCurve(std::vector<YsAnimeKey_Quaternion*>& keyList, int bakeSpace);


};
//=====================================================
//! @brief �L�[�t���[���A�j���p�x�N�g���f�[�^
//!
//! @ingroup Animation
//=====================================================
struct YsAnimeKey_Vector3 {
	float Time;			//!< �L�[�̈ʒu
	YsVec3 Value;		//!< �L�[�̃x�N�g���l

	// ��XED��XMD�ǂݍ��݂ł́A�������x�D��̂��߂�1��2�̕�Ԃ͋Ȑ��ɂȂ�悤��0�Ƃ��ďĂ����̂ŁA��{��0���g��
	char CalcType;		//!< ��Ԍv�Z���@ 0:���`��� 1:Catmull-Rom�X�v���C�����  2:�x�W�F�Ȑ����

	// �x�W�F�p
	char BezierX_A[2];
	char BezierX_B[2];
	char BezierY_A[2];
	char BezierY_B[2];
	char BezierZ_A[2];
	char BezierZ_B[2];

	// 
	YsAnimeKey_Vector3() : Time(0), Value(0, 0, 0), CalcType(0) {
		BezierX_A[0] = BezierX_A[1] = BezierY_A[0] = BezierY_A[1] = BezierZ_A[0] = BezierZ_A[1] = 20;
		BezierX_B[0] = BezierX_B[1] = BezierY_B[0] = BezierY_B[1] = BezierZ_B[0] = BezierZ_B[1] = 107;
	}

	//! @brief �w�莞�Ԃ̉�]�f�[�^���Ԍv�Z�ŋ��߂�(static�֐�)
	//! @param[in] rotateList		�c �L�[�t���[���z��
	//! @param[in] time				�c ��Ԍv�Z�ŋ��߂������Ԃ��w��
	//! @param[out]	outQ			�c ��Ԍ��ʂ̉�](�N�H�[�^�j�I��)������
	//! @param[inout] nowKeyIndex	�c �L�[�t���[���z��̂ǂ̈ʒu����L�[�̌������s�����H����Ɍ�����͂���Index������Bnullptr�w��ōŏ����猟��(����������)
	static bool InterpolationFromTime(const std::vector<YsAnimeKey_Vector3*>& keyList, double time, YsVec3 &outV, UINT* nowKeyIndex = nullptr);

	//! @brief ���W�L�[���X�g(m_Rotate)���́A�Ȑ��n�̕�ԃL�[(�X�v���C����x�W�F)���A�ʏ�L�[�Ƃ��ďĂ����݂��s��
	//! @param[inout] keyList		�c �ҏW����L�[�t���[���z��
	//! @param[in]	bakeSpace	�c �Ă����݊Ԋu  0�őS�Ė��߂� 1��Time��1�ÊԊu�������Ă����� 2�`���l
	static void BakeCurve(std::vector<YsAnimeKey_Vector3*>& keyList, int bakeSpace);

};
//=====================================================
//! @brief �L�[�t���[���A�j���p�X�N���v�g�f�[�^
//!
//! @ingroup Animation
//=====================================================
struct YsAnimeKey_Script {
	float Time;			//!< �L�[�̈ʒu
	std::string Value;	//!< �L�[�̃X�N���v�g������

	YsAnimeKey_Script() : Time(0) {
	}
};

//==========================================================
//! @brief �����̉�]�E�g��E���W���������A�j���[�V�����f�[�^
//!
//! @ingroup Animation
//==========================================================
class YsKeyframeAnime {
public:
	std::vector<YsAnimeKey_Quaternion*>		m_Rotate;	//!< ��]�L�[���X�g
	std::vector<YsAnimeKey_Vector3*>		m_Scale;	//!< �g��L�[���X�g
	std::vector<YsAnimeKey_Vector3*>		m_Pos;		//!< ���W�L�[���X�g

	//! @brief ���
	void Release();

	//
	YsKeyframeAnime(){
	}
	//
	~YsKeyframeAnime() {
		Release();
	}
};

//==========================================================
//!
//! @brief �P��ނ̃A�j���[�V�����̃f�[�^
//!
//! �����̍s��(�{�[���Ȃ�)���A�j���[�V�����\	\n
//!
//! @ingroup Animation
//==========================================================
class YsAnimationSet {
public:
	std::string						m_AnimeName;			//!< �A�j����
	double							m_TicksPerSecond;		//!< �A�j���̎��s���x
	double							m_AnimeLen;				//!< �A�j���̒���
	std::vector<YsKeyframeAnime*>	m_FrameAnimeList;		//!< �S�t���[���Ԃ�̃L�[�t���[���f�|�^
	std::vector<YsAnimeKey_Script*>	m_Script;				//!< �X�N���v�g�L�[

	//! @brief ���
	void Release();

	//! @brief m_FrameAnimeList���w��size�̌����m��
	void CreateFrameAnimeList(UINT size);

	//! @brief �A�j���[�V�����t�@�C��(XMD)��ǂݍ���
	//!  XMD�t�@�C����KAMADA SkinMesh Editor����o�͂ł���`���ł�
	//! @param[in] filename				�c XMD�t�@�C���p�X���w��
	//! @param[in] registerAnimeName	�c �ǂݍ��񂾃A�j���������̖��O�ɕύX����@nullptr�Ȃ�ύX���Ȃ�
	//! @param[in] bakeCurve			�c �Ȑ���Ԍn�̃L�[�́A�S�Đ��`��ԂƂ��ĕϊ��E�ǉ�����(�������y���Ȃ�܂�)
	bool LoadXMD(const std::string& filename, const char* registerAnimeName = nullptr, bool bakeCurve = true);

	// 
	YsAnimationSet() : m_TicksPerSecond(60), m_AnimeLen(0){
	}
	// 
	~YsAnimationSet() {
		Release();
	}


};


//=====================================================================
//!
//! @brief �A�j���[�V�����g���b�N���N���X
//!
//! �A�j���[�V�����̐i�s���L�����邽�߂̕�
//!
//! @ingroup Animation
//=====================================================================
class YsAnimatorTrack {
public:
	//=====================================================================
	// �g���b�N�f�[�^
	//=====================================================================
	SPtr<YsAnimationSet>	m_pSkinAnime;	//!< �A�j���f�[�^�ւ̃A�h���X
	int						m_AnimeNo;		//!< ���ݑI�𒆂̃A�j���ԍ�
	float					m_Weight;		//!< �d��	
	double					m_Speed;		//!< ���x
	double					m_AnimePos;		//!< �A�j���ʒu	
	bool					m_Loop;			//!< ���[�v�t���O
	bool					m_Enable;		//!< �L���t���O
	bool					m_DeleteTrack;	//!< �폜�t���O true�Ńg���b�N�폜


	//=====================================================================
	// �g���b�N�C�x���g�m�[�h�p�N���X
	//=====================================================================
	//! @brief �g���b�N�C�x���g��{�N���X
	class Event_Base {
	public:
		//! @brief �C�x���g�̎��
		enum SKINTRACKEVENT {
			STE_ENABLE,			//!< �L�������C�x���g
			STE_POSITION,		//!< �A�j���ʒu�C�x���g
			STE_SPEED,			//!< �A�j�����x�C�x���g
			STE_WEIGHT,			//!< �d�݃C�x���g
			STE_DELETETRACK,	//!< �g���b�N�폜�C�x���g
		};

		UINT Type;			//!< �C�x���g�̎��
		double fStartTime;	//!< �J�n����
	};

	//! @brief  Enable�C�x���g
	class Event_Enable : public Event_Base {
	public:
		bool bl;			// Enable�C�x���g�p
	};
	
	//! @brief  Delete�C�x���g
	class Event_DeleteTrack : public Event_Base {
	public:
	};
	
	//! @brief  Potision�C�x���g
	class Event_Position : public Event_Base {
	public:
		double Pos;
	};
	
	//! @brief  Speed�C�x���g
	class Event_Speed : public Event_Base {
	public:
		double fDuration;	// �ω�����
		double NewSpeed;
		double OldSpeed;

		char startflag;
		Event_Speed() :startflag(0) {}
	};
	
	//! @brief  �E�F�C�g�C�x���g
	class Event_Weight : public Event_Base {
	public:
		double fDuration;	// �ω�����
		float NewWeight;
		float OldWeight;

		char startflag;
		Event_Weight() :startflag(0) {}
	};

	//=====================================================================
	//! @brief ������
	//=====================================================================
	void Init() {
		m_pSkinAnime = nullptr;
		m_AnimeNo = 0;
		m_Weight = 0;
		m_Speed = 0;
		m_AnimePos = 0;
		m_Enable = false;
		m_Loop = false;
		m_DeleteTrack = false;
	}

	//--------------------------------------------------------------------------
	//! @brief Event�i�s
	//--------------------------------------------------------------------------
	void UpdateEvent(double val);

	//=====================================================================
	// �C�x���g�n�֐�
	//=====================================================================
	void EventTrackEnable(bool NewEnable, double StartTime) {
		UPtr<Event_Enable> lpAdd = UPtr<Event_Enable>(new Event_Enable());

		lpAdd->Type = Event_Base::STE_ENABLE;
		lpAdd->bl = NewEnable;
		lpAdd->fStartTime = StartTime;
		EventList.push_back(std::move(lpAdd));
	}
	void EventTrackDelete(double StartTime) {
		UPtr<Event_DeleteTrack> lpAdd = UPtr<Event_DeleteTrack>(new Event_DeleteTrack());

		lpAdd->Type = Event_Base::STE_DELETETRACK;
		lpAdd->fStartTime = StartTime;
		EventList.push_back(std::move(lpAdd));
	}
	void EventTrackPosition(double NewPos, double StartTime) {
		UPtr<Event_Position> lpAdd = UPtr<Event_Position>(new Event_Position());

		lpAdd->Type = Event_Base::STE_POSITION;
		lpAdd->Pos = NewPos;
		lpAdd->fStartTime = StartTime;
		EventList.push_back(std::move(lpAdd));
	}
	void EventTrackSpeed(double NewSpeed, double StartTime, double Duration) {
		UPtr<Event_Speed> lpAdd = UPtr<Event_Speed>(new Event_Speed());

		lpAdd->Type = Event_Base::STE_SPEED;
		lpAdd->NewSpeed = NewSpeed;
		lpAdd->OldSpeed = 0;
		lpAdd->fStartTime = StartTime;
		lpAdd->fDuration = Duration;
		EventList.push_back(std::move(lpAdd));
	}
	void EventTrackWeight(float NewWeight, double StartTime, double Duration) {
		UPtr<Event_Weight> lpAdd = UPtr<Event_Weight>(new Event_Weight());

		lpAdd->Type = Event_Base::STE_WEIGHT;
		lpAdd->NewWeight = NewWeight;
		lpAdd->OldWeight = 0;
		lpAdd->fStartTime = StartTime;
		lpAdd->fDuration = Duration;
		EventList.push_back(std::move(lpAdd));
	}

	// �C�x���g�����X�g����S�ď���
	void UnkeyTrackEvents() {
		std::list<UPtr<Event_Base>>::iterator it = EventList.begin();
		while(it != EventList.end()) {
			it = EventList.erase(it);
		}
	}

	// 
	YsAnimatorTrack() {
		Init();
	}
	~YsAnimatorTrack(){
		UnkeyTrackEvents();
	}

private:

	std::list<UPtr<Event_Base>> EventList;	//!< �C�x���g���X�g

};

//=====================================================================
//!
//! @brief �A�j���[�V�����Ǘ��N���X
//! 
//!  �A�j���[�V�����̍Đ��S�ʂ�S���A�o�^���Ă���s��ɔ��f����							\n
//!  �Q�[�����f���I�u�W�F�N�g(YsModelObject)���A����ŃA�j���[�V�����\				\n
//!  m_Track�ɉғ����Ă���A�j���[�V�����̏�񂪓���									\n
//!  ��{��m_Track[0]�����݂̃A�j���ƂȂ�B�A�j����������[1]�ȍ~���쐬�����			\n
//!  (ChangeAnimeSmooth�֐��Ȃ�)														\n
//!
//! @ingroup Animation
//=====================================================================
class YsAnimator {
public:
	//=====================================================================
	//
	// �����ݒ�n
	//
	//=====================================================================

	//-----------------------------------------------------------
	//! @brief ������
	//-----------------------------------------------------------
	void Init();

	//-----------------------------------------------------------
	//! @brief ���������AsrcAnimator�̓��e�Ɠ����悤�ɂ���
	//!  �A�j�����X�g�ƍs��Q�ƃe�[�u���𓯂��ɍ\�z����B�g���b�N���̓R�s�[����Ȃ��B
	//-----------------------------------------------------------
	void Init(YsAnimator& srcAnimator);

	//-----------------------------------------------------------
	//! @brief ���������A�s��Q�ƃe�[�u����refMat��o�^����
	//! ����ɍs���ǉ��������ꍇ��AddRefMatrix�֐��ŁB
	//-----------------------------------------------------------
	void Init(YsMatrix* refMat);


	//=====================================================================
	//
	// �A�j���[�V�������ʂ�K�p����A�s��Q�ƃe�[�u���ݒ�
	//
	//=====================================================================

	//-----------------------------------------------------------
	//! @brief �A�j���[�V�������ʂ�K�p����s��e�[�u�����폜����
	//-----------------------------------------------------------
	void ClearRefMatrix() {
		m_RefMatrixTbl.clear();
		m_RefMatrixTbl.shrink_to_fit();
	}

	//-----------------------------------------------------------
	//! @brief �A�j���[�V�������ʂ�K�p����s��̎Q�Ƃ��P�ǉ��o�^����
	//! @param[inout] mat				�c �A�j���[�V�����i�s���ɍX�V�����s��̃A�h���X
	//! @param[in] isBlendTrack_Rota	�c �����g���b�N�ғ����A��]�������u�����h���邩�H
	//! @param[in] isBlendTrack_Scale	�c �����g���b�N�ғ����A�g�听�����u�����h���邩�H
	//! @param[in] isBlendTrack_Pos		�c �����g���b�N�ғ����A���W�������u�����h���邩�H
	//-----------------------------------------------------------
	void AddRefMatrix(YsMatrix* mat,
		bool isBlendTrack_Rota = true,
		bool isBlendTrack_Scale = true,
		bool isBlendTrack_Pos = true
	) {
		RefMatrixData data;
		data.refTransMat = mat;
		data.isBlendTrack_Rota = isBlendTrack_Rota;
		data.isBlendTrack_Scale = isBlendTrack_Scale;
		data.isBlendTrack_Pos = isBlendTrack_Pos;
		m_RefMatrixTbl.push_back(data);
	}

	//-----------------------------------------------------------
	//! @brief �A�j���[�V�������ʂ�K�p����s��̎Q�Ƃ�ύX����
	//! @param[in] idx					�c �o�^Index���w��
	//! @param[inout] mat				�c �A�j���[�V�����i�s���ɍX�V�����s��̃A�h���X
	//! @param[in] isBlendTrack_Rota	�c �����g���b�N�ғ����A��]�������u�����h���邩�H
	//! @param[in] isBlendTrack_Scale	�c �����g���b�N�ғ����A�g�听�����u�����h���邩�H
	//! @param[in] isBlendTrack_Pos		�c �����g���b�N�ғ����A���W�������u�����h���邩�H
	//-----------------------------------------------------------
	void ChangeRefMatrix(int idx, YsMatrix* mat,
		bool isBlendTrack_Rota = true,
		bool isBlendTrack_Scale = true,
		bool isBlendTrack_Pos = true
	) {
		m_RefMatrixTbl[idx].refTransMat = mat;
		m_RefMatrixTbl[idx].isBlendTrack_Rota = isBlendTrack_Rota;
		m_RefMatrixTbl[idx].isBlendTrack_Scale = isBlendTrack_Scale;
		m_RefMatrixTbl[idx].isBlendTrack_Pos = isBlendTrack_Pos;
	}

	//=====================================================================
	//
	// �A�j���[�V�����o�^
	//
	//=====================================================================

	//-----------------------------------------------------------
	//! @brief �A�j���[�V�����f�[�^�P��ǉ��o�^����
	//! @param[in] allowSameName	�c true:�������Œǉ����� �@false:�����̃A�j����������ꍇ�A�����ɏ㏑�������
	//-----------------------------------------------------------
	void AddAnimation(SPtr<YsAnimationSet> anime, bool allowSameName = false);

	//-----------------------------------------------------------
	//! @brief �A�j���[�V�����f�[�^���t�@�C������ǂݍ��݁A�ǉ��o�^����
	//! @param[in] XmdFileName			�c XMD�`���̃A�j���[�V�����t�@�C��(KAMADA SkinMesh Editor�ŏo�͉\)
	//! @param[in] registerAnimeName	�c �ǂݍ��񂾃A�j���������̖��O�ɕύX����@nullptr�Ȃ�ύX���Ȃ�
	//! @param[in] allowSameName		�c true:�������Œǉ����� �@false:�����̃A�j����������ꍇ�A�����ɏ㏑�������
	//! @return �ǂݍ��݁E�ǉ����ꂽ�A�j���[�V�����f�[�^��shared_ptr
	//-----------------------------------------------------------
	SPtr<YsAnimationSet> AddAnimation(const std::string& XmdFileName, const char* registerAnimeName = nullptr, bool allowSameName = false);

	void CopyAnimationList(const std::vector<SPtr<YsAnimationSet>>& animeTbl) {
		m_AnimeList = animeTbl;
	}

	//=====================================================================
	//
	// �A�j���[�V�����i�s�E�v�Z
	//
	//=====================================================================

	//-----------------------------------------------------------
	//! @brief �g���b�N���擾
	//! ���ݍĐ����̃g���b�N��[0]
	//-----------------------------------------------------------
	std::deque<SPtr<YsAnimatorTrack>>& GetTrack() { return m_Track; }

	//-----------------------------------------------------------
	//! @brief �w��g���b�N�̃A�j����No�擾
	//! @param[in]	trackNo	�c �擾�������g���b�N�ԍ�
	//-----------------------------------------------------------
	int		GetAnimeNo(int trackNo = 0) { return m_Track[trackNo]->m_AnimeNo; }

	//-----------------------------------------------------------
	//! @brief �w��g���b�N�̃A�j�����x��No�擾
	//! @param[in]	trackNo	�c �擾�������g���b�N�ԍ�
	//-----------------------------------------------------------
	double	GetAnimeSpeed(int trackNo = 0){ return m_Track[trackNo]->m_Speed; }

	//-----------------------------------------------------------
	//! @brief �w��g���b�N�̃A�j���ʒu���擾
	//! @param[in]	trackNo	�c �擾�������g���b�N�ԍ�
	//-----------------------------------------------------------
	double	GetAnimePos(int trackNo = 0) { return m_Track[trackNo]->m_AnimePos; }

	//-----------------------------------------------------------
	//! @brief �ő�A�j�����擾
	//-----------------------------------------------------------
	int		GetMaxAnimeNum();

	//-----------------------------------------------------------
	//! @brief �A�j���[�V�������Ō�܂ōs����������
	//! @param[in]	trackNo	�c �擾�������g���b�N�ԍ�
	//! @return �Ō�܂ł�����:true
	//-----------------------------------------------------------
	bool IsAnimationEnd(UINT TrackNo = 0) {
		if(TrackNo >= m_Track.size())return true;
		if(m_Track[TrackNo]->m_Enable == false)return true;
		if(m_Track[TrackNo]->m_pSkinAnime == nullptr)return true;

		if(m_Track[TrackNo]->m_AnimePos >= m_Track[TrackNo]->m_pSkinAnime->m_AnimeLen)return true;
		return false;
	}

	//-----------------------------------------------------------
	//! @brief Val�����A�j���[�V������i�s�����A�S�{�[����TransMat���X�V����
	//! @param[in]	  Val			�c �i�s���x
	//! @param[inout] mulDifference0�c 0�Ԗڂɓo�^����Ă���Q�ƍs��̕ω��������A���̍s��ɍ�������
	//-----------------------------------------------------------
	void Animation(double Val, YsMatrix* mulDifference0 = nullptr);

	//-----------------------------------------------------------
	//! @brief Val�����A�j���[�V������i�s�����A�S�{�[����TransMat���X�V����
	//!
	//! �����݂̃A�j���ʒu����Val�̊ԂɃX�N���v�g�L�[�����݂���Ƃ��A���̃X�N���v�g�L�[�̌���	\n
	//! �@Animation(Val)�����s���ArunProc(scr)�����s����B										\n
	//!
	//! @param[in]	Val					�c �i�s���x
	//! @param[in]	onScriptExecProc	�c ���s����X�N���v�g�f�[�^�������Ƃ��ɌĂяo���֐�\n
	//!										�����ɂ͂��̃X�N���v�g�f�[�^(AnimeKey_Script�^)������
	//! @param[inout] mulDifference0	�c 0�Ԗڂɓo�^����Ă���Q�ƍs��̕ω��������A���̍s��ɍ�������
	//-----------------------------------------------------------
	void AnimationAndScript(double Val, std::function<void(YsAnimeKey_Script*)> onScriptExecProc, YsMatrix* mulDifference0 = nullptr);

	//-----------------------------------------------------------
	//! @brief �A�j���[�V�����ύX
	//!
	//! �����ڎw�肵���g���b�N�Ƀf�[�^���㏑������										\n
	//!   �w�肵���g���b�N�����݂��Ȃ������ꍇ�⑶�݂��Ȃ�AnimeNo�̏ꍇ��false���Ԃ�	\n
	//!
	//! @param[in] AnimeNo		�c �A�j���ԍ�
	//! @param[in] loop			�c ���[�v�Đ�������H
	//! @param[in] SetTrackNo	�c �����Z�b�g����g���b�N�̔ԍ�
	//! @param[in] bEnableTrack	�c ���̃g���b�N��L���ɂ���H
	//! @param[in] Speed		�c �A�j�����x
	//! @param[in] Weight		�c �d��
	//! @param[in] AnimePos		�c �A�j���ʒu
	//! @return ����ɕύX�ł�����true
	//-----------------------------------------------------------
	bool ChangeAnime(UINT AnimeNo, bool loop, UINT SetTrackNo = 0, bool bEnableTrack = true, double Speed = 1.0, float Weight = 1.0f, double AnimePos = 0);

	//-----------------------------------------------------------
	//! @brief ChangeAnime�֐��́A�A�j�����w��o�[�W����
	//-----------------------------------------------------------
	bool ChangeAnime_FromName(const std::string& AnimeName, bool loop, UINT SetTrackNo = 0, bool bEnableTrack = true, double Speed = 1.0, float Weight = 1.0f, double AnimePos = 0);

	//-----------------------------------------------------------
	//! @brief ���炩�ɃA�j����ύX
	//!
	//! ���V�����g���b�N���쐬�E�ݒ肵�A�O��̃g���b�N�͍폜����				\n
	//! ��Duration�ɂ��A���炩�Ƀg���b�N���m�̃u�����h���s�����Ƃ��ł���̂ŁA\n
	//!   �Q�[���̃A�j���؂�ւ��Ŏg�p����Ɨǂ�								\n
	//!
	//! @param[in] AnimeNo		�c �A�j���ԍ�
	//! @param[in] StartTime	�c �ω����J�n���鎞��
	//! @param[in] Duration		�c �ω��ɂ����鎞�� 0���Ƒ����ɕύX
	//! @param[in] loop			�c ���[�v�Đ�������H
	//! @param[in] Speed		�c �A�j�����x
	//! @param[in] AnimePos		�c �A�j���ʒu
	//! @return ����ɕύX�ł�����true
	//-----------------------------------------------------------
	bool ChangeAnimeSmooth(UINT AnimeNo, float StartTime, float Duration, bool loop, double Speed = 1.0, double AnimePos = 0);

	//-----------------------------------------------------------
	//! @brief ChangeAnimeSmooth�֐��́A�A�j�����w��o�[�W����
	//-----------------------------------------------------------
	bool ChangeAnimeSmooth_FromName(const std::string& AnimeName, float StartTime, float Duration, bool loop, double Speed = 1.0, double AnimePos = 0);

	//
	bool AddAnimeSmooth(UINT AnimeNo, float StartTime, float Duration, bool loop, double Speed, double AnimePos, float startWeight, float endWeight);
	//
	bool AddAnimeSmooth_FromName(const std::string& AnimeName, float StartTime, float Duration, bool loop, double Speed, double AnimePos, float startWeight, float endWeight);

	//-----------------------------------------------------------
	//! @brief �S�g���b�N�̃C�x���g��S�ď���
	//-----------------------------------------------------------
	void UnkeyAllTrackEvents() {
		for(auto& var : m_Track){
			var->UnkeyTrackEvents();
		}
	}

	//-----------------------------------------------------------
	//! @brief [0]�������A�S�g���b�N���폜
	//-----------------------------------------------------------
	void ResetTrack(){
		m_Track.clear();
		m_Track.push_back(std::make_shared<YsAnimatorTrack>());
		m_Track[0]->Init();
	}

	//-----------------------------------------------------------
	//! @brief �A�j���[�V����������Index���擾�B�Ȃ��ꍇ��-1
	//! @param[in]	AnimeName	�c �A�j����
	//! @return �����F�A�j���ԍ��@�A�j�������F-1
	//-----------------------------------------------------------
	int SearchAnimation(const std::string& AnimeName) {
		for (UINT i = 0; i<m_AnimeList.size(); i++) {
			if (m_AnimeList[i]->m_AnimeName == AnimeName) {
				return i;
			}
		}
		return -1;
	}

	//=====================================================================
	//
	// �x�[�X�E�F�C�g
	//
	//=====================================================================

	//-----------------------------------------------------------
	//! @brief �A�j���[�^�̏d�݂��擾
	//-----------------------------------------------------------
	float GetBaseWeight() { return m_BaseWeight; }

	//-----------------------------------------------------------
	//! @brief �A�j���[�^�̍����̏d�݂��Z�b�g
	//! @param[in] weight	�c �Z�b�g����d��(0�`1)
	//! @param[in] speed	�c �d�݂��A���ݒn����weight�֏��X�ɕω������鑬�x�@1�ő����ɃZ�b�g
	//-----------------------------------------------------------
	void SetBaseWeight(float weight, float speed = 1.0f) {
		m_BaseWeightAnime = weight;
		m_BaseWeightAnimeSpeed = speed;
		if(speed >= 1.0f) {
			m_BaseWeight = weight;
		}
	}



	//=====================================================================
	// 
	YsAnimator();

	// 
	~YsAnimator() {
	}

private:

	//! @brief �g���b�N�̏�񂩂�S�{�[����TransMat�X�V
	void CalcAnimation();

	//-----------------------------------------------------------
	// �A�j���[�^���̂̍����̏d��
	//-----------------------------------------------------------
	float			m_BaseWeight;					//!< ���ݑS�̂̃x�[�X�u�����h�E�F�C�g
	float			m_BaseWeightAnime;				//!< ���炩�ω����Ɏg�p�B�ω���̃x�[�X�u�����h�E�F�C�g
	float			m_BaseWeightAnimeSpeed;			//!< ���炩�ω����Ɏg�p�B�ω����x

	//-----------------------------------------------------------
	//! @brief �A�j���[�V�������X�g
	//-----------------------------------------------------------
	std::vector<SPtr<YsAnimationSet>>	m_AnimeList;

	//-----------------------------------------------------------
	//! @brief �A�j���[�V�����g���b�N���
	//!
	//!  ���Œ�ł��P�͑��݂���	\n
	//!  ��ChangeAnimeSmooth�n�̊֐��ŐV����Track���ǉ������B����Track������ꍇ�́A�����A�j�������������B\n
	//-----------------------------------------------------------
	std::deque<SPtr<YsAnimatorTrack>>	m_Track;

	//-----------------------------------------------------------
	// �s��Q�ƍ\���f�[�^
	//  �P�̃{�[���P�ʂł�����
	//-----------------------------------------------------------
	struct RefMatrixData{

		// �Q�ƍs��@�����ŎQ�Ƃ���Ă���s��ցA�A�j���[�V�������ʂ��R�s�[�����
		YsMatrix*	refTransMat = nullptr;
		
		// �A�j���[�V�����Đ� �œK���p�f�[�^
		UINT		nowKeyIndex_Rota = 0;
		UINT		nowKeyIndex_Scale = 0;
		UINT		nowKeyIndex_Pos = 0;

		bool		isBlendTrack_Rota = true;	// �����g���b�N�ғ����A��]���u�����h���邩�H
		bool		isBlendTrack_Scale = true;	// �����g���b�N�ғ����A�g����u�����h���邩�H
		bool		isBlendTrack_Pos = true;	// �����g���b�N�ғ����A���W���u�����h���邩�H

	};

	std::vector<RefMatrixData>	m_RefMatrixTbl;

};


}

#endif
