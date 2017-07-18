#include "YskLib.h"

using namespace YskLib;

void YsBulletRigidBody::Create(YsBulletWorld& world, SPtr<YsBulletShape_Base> shape, const YsMatrix& startMat, float mass, short group, short mask)
{
	Release();

	m_World = &world;
	m_Shape = shape;

	// ���̂̍s��͊g�听�������Ă͂����Ȃ��̂ŁA�g�喳���̍s��Ɗg�嗦�𕪗�����
	//  �g�喳���s�� �� ���̂Ɏg�p
	//  �g�嗦�@�@�@ �� �`��Ɏg�p
	YsMatrix mat = (YsMatrix&)startMat;
	YsVec3 scale = mat.GetScale();
	mat.NormalizeScale();

	// �s��
	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&mat._11);

	// �`��ɍs��̊g�嗦��K�p����
	shape->SetLocalScaling(scale);

	// ���ʂ�0�ȊO�Ȃ�A���I���̂Ȃ̂Ō`�󂩂犵���e���\�����Z�o�B0�Ȃ�ÓI���́B
	if(mass != 0.0f) {
		shape->GetShape()->calculateLocalInertia(mass, m_Inertia);
	}
	else{
		m_Inertia.setValue(0, 0, 0);
	}
	m_Mass = mass;

	// ���̍쐬
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape->GetShape().get(), m_Inertia);
	rbInfo.m_additionalDamping = true;
	m_Body = SPtr<btRigidBody>(new btRigidBody(rbInfo));
	m_Body->setUserPointer(this);		// ���[�U�[�|�C���^�Ɏ����̃A�h���X���L�������Ă���

	// ����0�Ȃ�A�L�l�}�e�B�b�N�ɂ��Ƃ�
	if(mass == 0){
		SetKinematic(true);
	}

	// ���[���h�ɍ��̒ǉ�
	m_World->GetWorld()->addRigidBody(m_Body.get(), group, mask);
}

void YsBulletRigidBody::Release()
{
	// ���̍폜
	if(m_Body) {
		// ���[�V�����X�e�[�g�폜
		if(m_Body->getMotionState())delete m_Body->getMotionState();
		// 
		while(m_Body->getNumConstraintRefs() > 0) {
			m_Body->removeConstraintRef(m_Body->getConstraintRef(0));
		}

		// ���[���h�������
		if(m_World && m_World->GetWorld()) {
			m_World->GetWorld()->removeCollisionObject(m_Body.get());
		}
		// ���
		m_Body = nullptr;

		m_Inertia.setValue(0, 0, 0);
		m_Mass = 0;
		m_Shape = nullptr;
	}

}
