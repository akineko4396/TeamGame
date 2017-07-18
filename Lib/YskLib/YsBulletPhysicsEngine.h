//===============================================================
//! @file YsBulletPhysicsEngine.h
//! @brief �uBullet Physics Engine�v�֌W�̃N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsBulletPhysicsEngine_h__
#define __YsBulletPhysicsEngine_h__

#pragma warning(disable:4316)

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

namespace YskLib{

class YsBulletRigidBody;


// YsBulletWorld			�c �������E


// YsBulletJoint_Point		�c �{�[���W���C���g
// YsBulletJoint_Hinge		�c �q���W�W���C���g
// YsBulletJoint_6Dof		�c 6Dof�W���C���g
// YsBulletJoint_6DofSpring	�c �o�l�t��6Dof�W���C���g


//=====================================================
//! @brief �uBullet Physics Engine�v���[���h�p�N���X
//!
//! @ingroup Physics
//=====================================================
class YsBulletWorld {
public:
	//! @brief �������[���h�擾
	btDynamicsWorld* GetWorld(){return m_dynamicsWorld;}

	//! @brief ������
	void Init();

	//! @brief ���
	void Release();

	//! @brief �V�~�����[�V�����i�s
	int StepSimulation(btScalar timeStep,int MaxSubSteps=1, btScalar fixedTimeStep=btScalar(1.)/btScalar(60.))
	{
		return m_dynamicsWorld->stepSimulation( timeStep ,MaxSubSteps, fixedTimeStep);
	}

	//! @brief �Փˌ��m�p�R�[���o�b�N�ʒm�p
	//!
	//! ���̓��m���Փ˂���Ƃ��̊֐������s�����B				\n
	//! ���̃N���X���p�����A���̊֐����I�[�o�[���C�h���Ďg���B	\n
	//!
	//! @param[in]	cp		�c �Փ˂̏�񂪓����Ă�ۂ�
	//! @param[in]	body0	�c �Փ˂������̂��̂P
	//! @param[in]	body1	�c �Փ˂������̂��̂Q
	virtual bool ContactProcessedCallback(btManifoldPoint& cp, YsBulletRigidBody* body0, YsBulletRigidBody* body1){ return true; }

	//! @brief ����������Ă�H
	bool IsInit(){return (m_dynamicsWorld != nullptr);}

	// 
	YsBulletWorld() {
		m_collisionConfiguration = nullptr;
		m_dispatcher = nullptr;
		m_broadphase = nullptr;
		m_solver = nullptr;
		m_dynamicsWorld = nullptr;
	}
	~YsBulletWorld() {
		Release();
	}


private:
	btDefaultCollisionConfiguration*	m_collisionConfiguration;
	btCollisionDispatcher*				m_dispatcher;
	btBroadphaseInterface*				m_broadphase;
	btConstraintSolver*					m_solver;
	btDynamicsWorld*					m_dynamicsWorld;

protected:
	// �Փˌ��m�p�ÓI�R�[���o�b�N�֐�
	static bool s_ContactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1);

private:
	// �R�s�[�֎~�p
	YsBulletWorld(const YsBulletWorld& src){}
	void operator=(const YsBulletWorld& src){}
};

//==========================================================================
//! @brief Bullet�I�u�W�F�N�g��{�N���X
//!
//! @ingroup Physics
//==========================================================================
class YsBulletObj_Base
{
public:
	YsBulletWorld* GetWorld(){return m_World;}

	virtual void Release() = 0;

	// ���[�U�|�C���^
	void SetUserPointer(void* p){m_lpUserPointer = p;}
	void* GetUserPointer(){return m_lpUserPointer;}

	// 
	YsBulletObj_Base() :
		m_World(nullptr),
		m_lpUserPointer(nullptr) {
	}

	// 
	virtual ~YsBulletObj_Base() {
		m_World = nullptr;
	}

protected:

	YsBulletWorld*	m_World;		// �o�^���Ă��郏�[���h
	void*			m_lpUserPointer;

};

}

#include "YsBullet_Shape.h"
#include "YsBullet_RigidBody.h"
#include "YsBullet_Joint.h"

namespace YskLib {



}

#endif
