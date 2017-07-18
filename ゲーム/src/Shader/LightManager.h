#ifndef __LightManager_h__
#define __LightManager_h__
//================================================
//
// ���낢��ȃ��C�g�̃f�[�^
//
// �EDirLight		�c ���s�����f�[�^
// �EPointLight	�c �|�C���g���C�g�f�[�^
// �ESpotLight		�c �X�|�b�g���C�g�f�[�^
// �ELightManager	�c ��L�̃f�[�^���Ǘ�����A���C�g�Ǘ��N���X
//
//================================================


//================================================
// ���s�����f�[�^(�f�B���N�V���i�����C�g)
//================================================
struct DirLight {
	YsVec4		Diffuse = YsVec4(1, 1, 1, 1);	// ���C�g�F
	YsVec3		Direction = YsVec3(0, -1, 0);	// ����

	//=========================================================
	// ���s�����Ƃ��ăf�[�^�ݒ�
	// direction	�c ���C�g�̕���
	// diffuse		�c �F
	// specular	�c �X�y�L�����F
	//=========================================================
	void SetData(const YsVec3& direction, const YsVec4& diffuse) {
		Diffuse = diffuse;
		Direction = direction;
	}

};

//================================================
// �_�����f�[�^(�|�C���g���C�g)
//================================================
struct PointLight {
	YsVec4			Diffuse = YsVec4(1, 1, 1, 1);	// ��{�F
	YsVec3			Position;						// ���W

	float			Radius = 1;	// ���a

	//=========================================================
	// �_�����Ƃ��ăf�[�^�ݒ�
	// pos			�c ���W
	// diffuse		�c �F
	// radius		�c ���a
	//=========================================================
	void SetData(const YsVec3& pos, const YsVec4& diffuse, float radius) {
		Position = pos;
		Diffuse = diffuse;
		Radius = radius;
	}

};

//================================================
// �X�|�b�g���C�g�f�[�^
//================================================
struct SpotLight {

	YsVec4			Diffuse = YsVec4(1, 1, 1, 1);		// ��{�F
	YsVec3			Position;							// ���W
	YsVec3			Direction = YsVec3(0, 0, 1);		// ����

	float			Range = 10;		// �Ǝˋ���

	float			MinAngle = 15;	// �p�x
	float			MaxAngle = 20;	// �p�x

	//=========================================================
	// �X�|�b�g�����Ƃ��ăf�[�^�ݒ�
	//=========================================================
	void SetData(const YsVec3& pos, const YsVec3& direction, const YsVec4& diffuse, float range, float minAngle, float maxAngle) {
		Position = pos;
		Diffuse = diffuse;
		Direction = direction;
		Range = range;
		MinAngle = minAngle;
		MaxAngle = maxAngle;
	}
};

//================================================
//
// ���C�g�Ǘ�
//
//   ���s�����A�_�����A�X�|�b�g�������Ǘ�����N���X
//
//   �V�F�[�_�̒萔�o�b�t�@��b5,b6,b7���g�p���܂�
//
//
//================================================

/*
[�g������]

<�O���[�o���Ȃǂŗp��>
SPtr<CPointLight> PL;

<�|�C���g���C�g�쐬>
PL = ShMgr.m_LightMgr.AddPointLight(YsVec3(0, 0, 0), YsVec4(2, 0.2f, 0.2f, 0), 10);

<���C�g�폜>
PL = nullptr;

�����s������CDirLight�^�A�X�|�b�g���C�g��CSpotLight�^���g��

*/

class LightManager {
public:

	//================================================
	// ������
	//================================================
	void Init();

	//================================================
	// ���
	//  �e���C�g�̊Ǘ����X�g�͉������܂���
	//  �萔�o�b�t�@�Ȃǂ��������܂�
	//================================================
	void Release();

	//
	enum {
		MAX_DIRLIGHT = 3,		// �ő啽�s������
		MAX_POINTLIGHT = 100,	// �ő�|�C���g���C�g��
		MAX_SPOTLIGHT = 20,		// �ő�X�|�b�g���C�g��
	};

	//-------------------------------------------
	// ����
	//-------------------------------------------
	YsVec3				m_AmbientLight = YsVec3(0.3f, 0.3f, 0.3f);

	//-------------------------------------------
	// ���s����
	//-------------------------------------------
	std::list<WPtr<DirLight>>		m_DirLightList;	// ���s�����Ǘ����X�g

	// ���s�����ǉ�
	// �߂�l�F�ǉ��������s������shared_ptr
	//  �_������ǉ�����ꍇ�͂��̊֐����ĂԁB�����ł�WPtr�ŕێ����Ă�̂ŁA�N���ێ����Ȃ��Ȃ������_�Ō����͏��ł���d�g��
	//  ���Ŏ��̉���������UpdateBeforeDraw()�ōs���Ă܂��B
	SPtr<DirLight> AddDirLight(){
		SPtr<DirLight> add(new DirLight());		// �������m��
		m_DirLightList.push_back(add);				// ���C�g���X�g�֒ǉ�
		return add;
	}

	//-------------------------------------------
	// �_����
	//-------------------------------------------
	std::list<WPtr<PointLight>>	m_PointLightList;	// �|�C���g���C�g�Ǘ����X�g

	// �|�C���g���C�g�ǉ�
	// �߂�l�F�ǉ������|�C���g���C�g��shared_ptr
	//  �_������ǉ�����ꍇ�͂��̊֐����ĂԁB�����ł�WPtr�ŕێ����Ă�̂ŁA�N���ێ����Ȃ��Ȃ������_�Ō����͏��ł���d�g��
	//  ���Ŏ��̉���������UpdateBeforeDraw()�ōs���Ă܂��B
	SPtr<PointLight> AddPointLight(){
		SPtr<PointLight> add(new PointLight());		// �������m��
		m_PointLightList.push_back(add);				// �|�C���g���C�g���X�g�֒ǉ�
		return add;
	}

	//-------------------------------------------
	// �X�|�b�g����
	//-------------------------------------------
	std::list<WPtr<SpotLight>>		m_SpotLightList;		// �X�|�b�g���C�g�Ǘ����X�g

	// �X�|�b�g���C�g�ǉ�
	// �߂�l�F�ǉ������X�|�b�g���C�g��shared_ptr
	//  �X�|�b�g������ǉ�����ꍇ�͂��̊֐����ĂԁB�����ł�WPtr�ŕێ����Ă�̂ŁA�N���ێ����Ȃ��Ȃ������_�Ō����͏��ł���d�g��
	//  ���Ŏ��̉���������UpdateBeforeDraw()�ōs���Ă܂��B
	SPtr<SpotLight> AddSpotLight(){
		SPtr<SpotLight> add(new SpotLight());				// �������m��
		m_SpotLightList.push_back(add);						// �X�|�b�g���C�g���X�g�֒ǉ�
		return add;
	}

	//-------------------------------------------
	// �X�V����
	//-------------------------------------------
	void Update();


	//===================================================
	// SampleShader�p�@���C�g�p�萔�o�b�t�@�f�[�^
	//===================================================
	struct cbSampleShaderLight {
		// �e�����̐�
		int			DL_Cnt = 0;	// ���s����
		int			PL_Cnt = 0;	// �|�C���g���C�g��
		int			SL_Cnt = 0;	// �X�|�b�g���C�g��
		int tmp[1];	// ���p�b�L���O�K���̂��߁A�S�~�����ݍ���

		//-------------------------------
		// ���s����
		//-------------------------------

		// ���C�g�̃f�[�^
		struct DLData {
			YsVec4		Color;	// �F
			YsVec3		Dir;	// ����
			float		tmp;	// 
		};
		DLData DL[MAX_DIRLIGHT];	// ���s���f�[�^�z��(�ő�3��)

		//-------------------------------
		// �|�C���g���C�g
		//-------------------------------
		// ���C�g�̃f�[�^
		struct PLData {
			YsVec4		Color;		// �F
			YsVec3		Pos;		// ���W
			float		Radius = 1;	// ���a
		};
		PLData PL[MAX_POINTLIGHT];	// �|�C���g���C�g�f�[�^�z��(�ő�100��)

		//-------------------------------
		// �X�|�b�g���C�g
		//-------------------------------
		// ���C�g�̃f�[�^
		struct SLData {
			YsVec4		Color;	// �F
			YsVec3		Pos;	// ���W
			float		Range;	// �Ǝ˔͈�

			YsVec3		Dir;	// ����
			float		MinAng;	// min�`max�ɂ����āA���X�Ɍ��̋��������������
			float		MaxAng;	// 
			float tmp[3];
		};
		SLData SL[MAX_SPOTLIGHT];	// �X�|�b�g���C�g�f�[�^�z��(�ő�100��)



	};
	YsConstantBuffer<cbSampleShaderLight>				m_cb12_SampleLight;			// SampleShader�p���C�g�萔�o�b�t�@(�ʏ�͂P�t���[���P�ʂł̍X�V)

	//===================================================
	// MyShader�p�@���C�g�p�萔�o�b�t�@�f�[�^
	//===================================================




};

#endif
