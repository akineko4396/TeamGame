#ifndef __Scene_h__
#define __Scene_h__

//============================================================
// �V�[����{�N���X
//  �������p�����ĐV�����V�[�������
//============================================================
class BaseScene{
public:
	BaseScene(){
	}
	virtual ~BaseScene(){};

	virtual int Update() = 0;	// �V�[���X�V����
	virtual void Draw() = 0;	// �V�[���`�揈��
};

#endif
