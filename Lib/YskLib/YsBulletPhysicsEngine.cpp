#include "YskLib.h"

using namespace YskLib;

extern ContactProcessedCallback gContactProcessedCallback;

//==========================================================================
// Bullet Physics Engine ���[���h�p�N���X
//==========================================================================

void YsBulletWorld::Init()
{
	// bullet������
	m_collisionConfiguration = new btDefaultCollisionConfiguration();		// �f�t�H���g�̃R���W�����R���t�B�O�쐬
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);	// �f�t�H���g�̃R���W�����f�B�X�p�b�`���쐬

	// �u���[�h�t�F�[�Y�Փ˔���
	m_broadphase = new btDbvtBroadphase();
	/*
	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);
	m_broadphase = new btAxisSweep3 (worldAabbMin, worldAabbMax);
	*/

	// �f�t�H���g�̍S���v�Z�i���l��́j�\���o���쐬����
	m_solver = new btSequentialImpulseConstraintSolver;

	// �������[���h�̍쐬
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver,m_collisionConfiguration);
	// �d�͂̐ݒ�
	m_dynamicsWorld->setGravity(btVector3(0,-9.8f,0));

	// �Փˌ��m�p�R�[���o�b�N�֐��ݒ�
	gContactProcessedCallback = YsBulletWorld::s_ContactProcessedCallback;

	btGImpactCollisionAlgorithm::registerAlgorithm(m_dispatcher);
}

bool YsBulletWorld::s_ContactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1)
{
	btRigidBody* pBody0 = (btRigidBody*)body0;
	btRigidBody* pBody1 = (btRigidBody*)body1;

	YsBulletRigidBody* pRigidBase0 = (YsBulletRigidBody*)pBody0->getUserPointer();
	YsBulletRigidBody* pRigidBase1 = (YsBulletRigidBody*)pBody1->getUserPointer();

	// Bullet World�̃A�h���X���擾
	YsBulletWorld* world = nullptr;
	if(pRigidBase0)world = pRigidBase0->GetWorld();
	else if(pRigidBase1)world = pRigidBase1->GetWorld();

	// ���z�֐��Ăяo���Œʒm����
	if(world){
		return world->ContactProcessedCallback(cp, pRigidBase0, pRigidBase1);
	}

	return true;
}

// ���
void YsBulletWorld::Release()
{
	if(m_dynamicsWorld){
		while(m_dynamicsWorld->getNumConstraints() > 0){
			btTypedConstraint* obj = m_dynamicsWorld->getConstraint(0);
			YsBulletObj_Base* p = (YsBulletObj_Base*)obj->getUserConstraintPtr();
			if(p){
				p->Release();
			}
		}
		// ����
		for (int i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--){
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if(body){
				YsBulletObj_Base* p = (YsBulletObj_Base*)body->getUserPointer();
				if(p){
					p->Release();
				}
			}
		}
	}

	YsSafe_Delete(m_dynamicsWorld);			// ���[���h�폜
	YsSafe_Delete(m_solver);
	YsSafe_Delete(m_broadphase);
	YsSafe_Delete(m_dispatcher);
	YsSafe_Delete(m_collisionConfiguration);
}
