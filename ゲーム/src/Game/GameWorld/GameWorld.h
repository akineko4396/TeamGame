#pragma once

class Stage;
class BaseGameObj;

//==============================================
// �_���[�W�ʒm�ڍ׃f�[�^
//  �U������ȂǂŎg���p�����[�^
//==============================================
struct AttackParam {

	YsVec3					m_vBlow;			// �Ԃ���΂���(�x�N�g��)

};

//==============================================
// �_���[�W�ʒm�̕ԐM�f�[�^
//  �U�����q�b�g���������肩��̕ԐM���Ƃ��Ďg��
//==============================================
struct DamageReply {
};

//==============================================
// �Q�[���I�u�W�F�N�g��{�N���X
//  �Q�[�����ɓo�ꂷ�镨�̂̊�{�ƂȂ�N���X
//==============================================

class BaseGameObj : public std::enable_shared_from_this<BaseGameObj>{
public:

	YsMatrix	m_Mat;			// �L�����̍s��
	YsMatrix	m_ChangedMat;	// 1�t���[���̈ړ��ŕω������Ԃ�̍s��

	//�X�e�[�W���擾(�Q�[���V�[���ȊO�ł�NULL�����Ԃ�Ȃ�)
	Stage* GetStage();

	//======================================
	// �_���[�W�ʒm�֐�
	//  attacker : �U����
	//  atk : �_���[�W�ʒm�ڍ׃p�����[�^
	//  rep : ���ʕԐM�f�[�^
	//======================================
	virtual bool OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep){ return true; }

};

#include "CollisionEngine/CollisionEngine.h"
#include "Game/GameCamera.h"
#include "GameKey.h"
#include "Input\CInput.h"


// Character
#include "Character\CharacterBase.h"
#include "Character\Player\Player.h"
#include "Character\Box\Box.h"
#include "Character\Map\Map.h"
#include "Character\Spring\Spring.h"
#include "Character\Water\Water.h"
#include "Character\Stairs\Stairs.h"


// Effect
#include "Effect\Effect.h"
#include "Effect\Plode\Plode.h"


// Item
#include "Item\Weapon.h"
#include "Item\Hammer\Hammer.h"


// Scene�֌W
#include "Game\GameWorld\Stage\StageBase.h"
#include "Game\Scene\GameScene\GameScene.h"

#include "GameLib\GameLib.h"
#include "Data\FilePath.h"







