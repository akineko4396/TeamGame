//===============================================================
//! @file YsBullet_RigidBody.h
//! @brief �uBullet Physics Engine�v�́u���́v�֌W�̃N���X
//!
//! �K���ɂ܂Ƃ߂ď����Ă܂��B
//!
//! @author ���c
//===============================================================
#ifndef __YsBullet_RigidBody_h__
#define __YsBullet_RigidBody_h__

#pragma warning(disable:4316)

namespace YskLib
{
	//==========================================================================
	//! @brief Bullet���̊�{�N���X
	//! ���ۂɕ������Z�œ������́BShape�N���X�������t����ƁA���̌`��œ��삷��B
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletRigidBody : public YsBulletObj_Base {
	public:


		//========================================================================
		// �쐬�E���
		//========================================================================

		//! @brief �`��ƍ��̂��쐬�A���[���h�֒ǉ�
		//! @param[in]	world		�c ���̂�o�^���镨�����[���h
		//! @param[in]	shape		�c �o�^����`��
		//! @param[in]	startMat	�c �����s��(�g��͊܂߂�Ȃ�)
		//! @param[in]	mass		�c ����
		//! @param[in]	group		�c ����O���[�v�t�B���^(�������Ă���O���[�v�ɂ̂ݏՓ˂��s��)
		//! @param[in]	mask		�c ����}�X�N�t�B���^(�w��group�̍��̂ƏՓ˂��邩�H�̃t�B���^ 0xFFFF�őS�Ă̍��̂ƏՓ˂���)
		void Create(YsBulletWorld& world, SPtr<YsBulletShape_Base> shape, const YsMatrix& startMat, float mass, short group = 1, short mask = (short)0xFFFF);

		//! @brief ���
		void Release();


		//========================================================================
		// �擾�E�ݒ�
		// ���ꕔ�����ł�
		//========================================================================

		//! @brief ���̎擾
		SPtr<btRigidBody>		GetBody(){ return m_Body; }

		//! @brief �L�l�}�e�B�b�N�ݒ�
		//! @param[in]	kinematic	�c �L�l�}�e�B�b�N�ɂ��邩�H
		void SetKinematic(bool kinematic)
		{
			if(kinematic) {
				m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
				m_Body->setActivationState(DISABLE_DEACTIVATION);
				m_Body->setMassProps(0, btVector3(0, 0, 0));
				m_Mass = 0;
				m_Body->updateInertiaTensor();
			}
			else{
				m_Body->setCollisionFlags(m_Body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
				m_Body->setActivationState(ACTIVE_TAG);
				m_Body->setMassProps(m_Mass, m_Inertia);
				m_Body->updateInertiaTensor();
			}
		}

		//! @brief �ÓI���̂��H(�����Ȃ�����)
		bool IsStatic(){
			if(m_Body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) {
				return true;
			}
			return false;
		}

		//! @brief ����
		void SetMass(float mass){
			m_Mass = mass;
			if(m_Body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) {
				m_Body->setMassProps(0, btVector3(0, 0, 0));
			}
			else{
				m_Body->setMassProps(m_Mass, m_Inertia);
			}
			m_Body->updateInertiaTensor();
		}

		//! @brief ���ʎ擾
		float GetMass(){ return m_Mass; }

		//! @brief �����e���\���X�V
		void UpdateInertia()
		{
			if(m_Shape == nullptr)return;

			if(m_Body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) {
				m_Body->setMassProps(0, btVector3(0, 0, 0));
			}
			else{
				// �����e���\���X�V
				if(m_Mass != 0.0f) {
					m_Shape->GetShape()->calculateLocalInertia(m_Mass, m_Inertia);
				}
				m_Body->setMassProps(m_Mass, m_Inertia);
			}
			m_Body->updateInertiaTensor();
		}
		//! @brief �����͐ݒ�
		void SetRestitution(float r)
		{
			m_Body->setRestitution(r);
		}
		//! @brief ���C�͐ݒ�
		void SetFriction(float f)
		{
			m_Body->setFriction(f);
		}
		//! @brief �ړ������ݒ�
		void SetLinearDamping(float d)
		{
			m_Body->setDamping(d, m_Body->getAngularDamping());
		}
		//! @brief ��]�����ݒ�
		void SetAngularDamping(float d)
		{
			m_Body->setDamping(m_Body->getLinearDamping(), d);
		}
		//! @brief �ړ��͐ݒ�
		void SetLinearVelocity(const YsVec3& v)
		{
			m_Body->setLinearVelocity(btVector3(v.x, v.y, v.z));
		}
		//! @brief ��]�͐ݒ�
		void SetAngularVelocity(const YsVec3& v)
		{
			m_Body->setAngularVelocity(btVector3(v.x, v.y, v.z));
		}

		//! @brief ������͂��N���A����
		void ClearForces()
		{
			m_Body->clearForces();
		}
		//! @brief ��]�͂�^����
		void ApplyTorque(const YsVec3& t)
		{
			m_Body->applyTorque(btVector3(t.x, t.y, t.z));
		}
		//! @brief �͂�^����(�p���I�ȗ́B�V�F�b�g���˂̂悤�ȋ���)
		void ApplyForce(const YsVec3& force)
		{
			m_Body->applyCentralForce(btVector3(force.x, force.y, force.z));
		}
		//! @brief �w��ʒu�ɗ͂�^����(�p���I�ȗ́B�V�F�b�g���˂̂悤�ȋ���)
		void ApplyForce(const YsVec3& force, const YsVec3& rel_pos)
		{
			m_Body->applyForce(btVector3(force.x, force.y, force.z), btVector3(rel_pos.x, rel_pos.y, rel_pos.z));
		}
		//! @brief �͂�^����(�u�ԓI�ȗ́B�o�b�g�őł������̂悤�ȋ���)
		void ApplyImpulse(const YsVec3& impulse)
		{
			m_Body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
		}
		//! @brief �w����W�ɗ͂�^����(�u�ԓI�ȗ́B�o�b�g�őł������̂悤�ȋ���)
		void ApplyImpulse(const YsVec3& impulse, const YsVec3& rel_pos)
		{
			m_Body->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(rel_pos.x, rel_pos.y, rel_pos.z));
		}
		//! @brief �X���[�v���Ȃ�����
		void SetNonSleep(){
			m_Body->setSleepingThresholds(0, 0);
			//		m_Body->setActivationState(DISABLE_DEACTIVATION);
		}

		//! @brief ���̂֒��ڍs����Z�b�g����
		//! @param[in]	mat		�c �ݒ肵�����s��
		void SetMatrix(const YsMatrix& mat){

			// �X�P�[���͌`��̕��փZ�b�g
			if(m_Shape){
				m_Shape->SetLocalScaling(mat.GetScale());
			}

			// ���̂̍s��̓X�P�[�����܂߂Ȃ�
			YsMatrix m(mat);
			m.NormalizeScale();

			// Bullet�p�̍s��֕ϊ�
			btTransform t;
			t.setFromOpenGLMatrix(&m._11);
			// �Z�b�g
			m_Body->getMotionState()->setWorldTransform(t);
			m_Body->setCenterOfMassTransform(t);
		}
		//! @brief ���̂���s����擾����
		//! �`��������Ă�ꍇ�́A���̑傫�����s��Ɋ܂�
		//! @param[out]	mOut	�c �擾�����s������邽�߂̕ϐ�
		void GetMatrix(YsMatrix& mOut){
			// �s��擾
			btTransform t = m_Body->getWorldTransform();
			t.getOpenGLMatrix(&mOut._11);
			if(m_Shape){
				YsVec3 scale;
				m_Shape->GetLocalScaling(scale);
				mOut.Scale_Local(scale);
			}
		}

		//! @brief ���[���h���獄�̂���������
		bool RemoveFromWorld(){
			if(m_Body->isInWorld()){
				if(m_World->GetWorld()){
					m_World->GetWorld()->removeCollisionObject(m_Body.get());
				}
				return true;
			}
			return false;
		}
		//! @brief ���[���h�ւ��̍��̂�ǉ�����
		bool AddToWorld(short group = 1, short mask = -1){
			// ���[���h�ɍ��̒ǉ�
			if(!m_Body->isInWorld()){
				m_World->GetWorld()->addRigidBody(m_Body.get(), group, mask);
				return true;
			}
			return false;
		}

		//========================================================================
		// ���[�V�����X�e�[�g
		//========================================================================

		//! @brief ���[�V�����X�e�[�g�쐬
		template<class T>
		T* CreateMotionState(){
			if(m_Body->getMotionState())delete m_Body->getMotionState();
			T* ms = new T();
			m_Body->setMotionState(ms);
			return ms;
		}
		//! @brief ���[�V�����X�e�[�g���Z�b�g
		void SetMotionState(btMotionState* ms){
			if(m_Body->getMotionState())delete m_Body->getMotionState();
			m_Body->setMotionState(ms);
		}


		// 
		YsBulletRigidBody() : m_Body(nullptr), m_Inertia(0, 0, 0), m_Mass(0)
		{
		}
		// 
		~YsBulletRigidBody(){
			Release();
		}

	protected:
		btVector3					m_Inertia;	//!< �����e���\��
		SPtr<btRigidBody>			m_Body;		//!< ����
		float						m_Mass;		//!< ����

		SPtr<YsBulletShape_Base>	m_Shape;	//!< �ݒ肳��Ă�`��

	};

}


#endif
