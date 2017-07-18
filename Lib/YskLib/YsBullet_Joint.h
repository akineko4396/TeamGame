//===============================================================
//! @file YsBullet_RigidBody.h
//! @brief �uBullet Physics Engine�v�́u�W���C���g�v�֌W�̃N���X
//!
//! �K���ɂ܂Ƃ߂ď����Ă܂��B
//!
//! @author ���c
//===============================================================
#ifndef __YsBullet_Joint_h__
#define __YsBullet_Joint_h__

#pragma warning(disable:4316)

namespace YskLib
{

	//==========================================================================
	//! @brief �W���C���g��{�N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_Base : public YsBulletObj_Base {
	protected:
		YsBulletJoint_Base(){}
	};

	//==========================================================================
	//! @brief �{�[���W���C���g�N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_Point : public YsBulletJoint_Base {
	public:
		//! @brief �{�[���W���C���g�擾
		SPtr<btPoint2PointConstraint> GetJoint(){ return m_Joint; }

		//! @brief ���
		virtual void Release() override
		{
			if(m_Joint){
				if(m_World)m_World->GetWorld()->removeConstraint(m_Joint.get());
				m_Joint = nullptr;
			}
			m_World = nullptr;
		}

		//! @brief �W���C���g�쐬
		//! @param[in]	world		�c �W���C���g��ǉ����镨�����[���h
		//! @param[in]	body1		�c �W���C���g�ɐڑ�����W���C���g�P
		//! @param[in]	body2		�c �W���C���g�ɐڑ�����W���C���g�Q
		//! @param[in]	pivotA		�c body1�ւ̑��Έʒu
		//! @param[in]	pivotB		�c body2�ւ̑��Έʒu
		void Create(YsBulletWorld& world, btRigidBody* body1, btRigidBody* body2, const YsVec3& pivotA, const YsVec3& pivotB)
		{
			Release();

			m_World = &world;

			btVector3 vA(pivotA.x, pivotA.y, pivotA.z);
			btVector3 vB(pivotB.x, pivotB.y, pivotB.z);
			m_Joint = SPtr<btPoint2PointConstraint>(new btPoint2PointConstraint(*body1, *body2, vA, vB));
			m_World->GetWorld()->addConstraint(m_Joint.get(), true);
		}

		//! @brief �W���C���g�쐬���쐬���A���[���h�֒ǉ�
		//! @param[in]	world		�c �W���C���g��ǉ����镨�����[���h
		//! @param[in]	body1		�c �W���C���g�ɐڑ�����W���C���g�P
		//! @param[in]	pivotA		�c body1�ւ̑��Έʒu
		void Create(YsBulletWorld& world, btRigidBody* body1, const btVector3& pivotA)
		{
			m_World = &world;

			m_Joint = SPtr<btPoint2PointConstraint>(new btPoint2PointConstraint(*body1, pivotA));
			m_World->GetWorld()->addConstraint(m_Joint.get(), true);
			m_Joint->setUserConstraintPtr(this);
		}

		// 
		YsBulletJoint_Point(){
		}

		~YsBulletJoint_Point() {
			Release();
		}


	protected:
		SPtr<btPoint2PointConstraint>		m_Joint;

	private:
		// �R�s�[�֎~�p
		YsBulletJoint_Point(const YsBulletJoint_Point& src){}
		void operator=(const YsBulletJoint_Point& src){}
	};

	//==========================================================================
	//! @brief �q���W�W���C���g�N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_Hinge : public YsBulletJoint_Base {
	public:
		//! @brief �q���W�W���C���g�擾
		SPtr<btHingeConstraint> GetJoint(){ return m_Joint; }

		//! @brief ���
		virtual void Release() override
		{
			if(m_Joint){
				if(m_World)m_World->GetWorld()->removeConstraint(m_Joint.get());
				m_Joint = nullptr;
			}
			m_World = nullptr;
		}

		//! @brief �W���C���g�쐬���쐬���A���[���h�֒ǉ�
		void Create(YsBulletWorld& world, btRigidBody* body1, btRigidBody* body2, const btVector3& pivotA, const btVector3& pivotB, const btVector3& axisA, const btVector3& axisB)
		{
			m_World = &world;

			m_Joint = SPtr<btHingeConstraint>(new btHingeConstraint(*body1, *body2, pivotA, pivotB, axisA, axisB));
			m_World->GetWorld()->addConstraint(m_Joint.get(), true);
			m_Joint->setUserConstraintPtr(this);
		}

		//! @brief �W���C���g�쐬���쐬���A���[���h�֒ǉ�
		void Create(YsBulletWorld& world, btRigidBody* body1, const btVector3& pivotA, const btVector3& axisA)
		{
			m_World = &world;

			m_Joint = SPtr<btHingeConstraint>(new btHingeConstraint(*body1, pivotA, axisA));
			m_World->GetWorld()->addConstraint(m_Joint.get(), true);
		}

		// 
		YsBulletJoint_Hinge(){
		}

		~YsBulletJoint_Hinge() {
			Release();
		}

	protected:
		SPtr<btHingeConstraint>		m_Joint;

	private:
		// �R�s�[�֎~�p
		YsBulletJoint_Hinge(const YsBulletJoint_Hinge& src){}
		void operator=(const YsBulletJoint_Hinge& src){}
	};


	//==========================================================================
	//! @brief 6Dof�W���C���g�N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_6Dof : public YsBulletJoint_Base {
	public:
		//! @brief 6Dof�W���C���g�擾
		SPtr<btGeneric6DofConstraint> GetJoint(){ return m_Joint; }

		//! @brief ���
		virtual void Release() override
		{
			if(m_Joint){
				if(m_World)m_World->GetWorld()->removeConstraint(m_Joint.get());
				m_Joint = nullptr;
			}
			m_World = nullptr;
		}

		//! @brief �W���C���g�쐬���쐬���A���[���h�֒ǉ�
		//! @param[in]	world		�c �W���C���g��ǉ����镨�����[���h
		//! @param[in]	body1		�c �W���C���g�ɐڑ�����W���C���g�P
		//! @param[in]	body2		�c �W���C���g�ɐڑ�����W���C���g�Q
		//! @param[in]	mA			�c body1�ւ̑��Έʒu
		//! @param[in]	mB			�c body2�ւ̑��Έʒu
		void Create(YsBulletWorld& world, btRigidBody* body1, btRigidBody* body2, const YsMatrix& mA, const YsMatrix& mB)
		{
			Release();

			m_World = &world;

			btTransform tA;
			tA.setFromOpenGLMatrix(&mA._11);
			btTransform tB;
			tB.setFromOpenGLMatrix(&mB._11);
			m_Joint = SPtr<btGeneric6DofConstraint>(new btGeneric6DofConstraint(*body1, *body2, tA, tB, true));
			m_World->GetWorld()->addConstraint(m_Joint.get(), true);
			m_Joint->setUserConstraintPtr(this);

		}

		// ����

		//! @brief �ړ������ݒ� ����
		void SetLinearLowerLimit(float limitX, float limitY, float limitZ){
			m_Joint->setLinearLowerLimit(btVector3(limitX, limitY, limitZ));
		}
		//! @brief �ړ������ݒ� ���
		void SetLinearUpperLimit(float limitX, float limitY, float limitZ){
			m_Joint->setLinearUpperLimit(btVector3(limitX, limitY, limitZ));
		}
		//! @brief ��]�����ݒ� ����(�x)
		void SetAngularLowerLimit(float anglimitX, float anglimitY, float anglimitZ){
			m_Joint->setAngularLowerLimit(btVector3(YsToRadian(anglimitX), YsToRadian(anglimitY), YsToRadian(anglimitZ)));
		}
		//! @brief ��]�����ݒ� ���(�x)
		void SetAngularUpperLimit(float anglimitX, float anglimitY, float anglimitZ){
			m_Joint->setAngularUpperLimit(btVector3(YsToRadian(anglimitX), YsToRadian(anglimitY), YsToRadian(anglimitZ)));
		}
		//! @brief Index�� �e�����ݒ�
		void SetLimit(int Index, float limitLo, float limitHi){
			m_Joint->setLimit(Index, limitLo, limitHi);
		}

		// 
		YsBulletJoint_6Dof(){
		}

		~YsBulletJoint_6Dof() {
			Release();
		}

	protected:
		SPtr<btGeneric6DofConstraint>		m_Joint;

	private:
		// �R�s�[�֎~�p
		YsBulletJoint_6Dof(const YsBulletJoint_6Dof& src){}
		void operator=(const YsBulletJoint_6Dof& src){}
	};

	//==========================================================================
	//! @brief 6Dof�o�l�W���C���g�N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_6DofSpring : public YsBulletJoint_Base {
	public:
		//! @brief 6Dof�o�l�W���C���g�擾
		SPtr<btGeneric6DofSpringConstraint> GetJoint(){ return m_Joint; }

		//! @brief ���
		virtual void Release() override
		{
			if(m_Joint){
				if(m_World)m_World->GetWorld()->removeConstraint(m_Joint.get());
				m_Joint = nullptr;
			}
			m_World = nullptr;
		}

		//! @brief �W���C���g�쐬���쐬���A���[���h�֒ǉ�
		//! @param[in]	world		�c �W���C���g��ǉ����镨�����[���h
		//! @param[in]	body1		�c �W���C���g�ɐڑ�����W���C���g�P
		//! @param[in]	body2		�c �W���C���g�ɐڑ�����W���C���g�Q
		//! @param[in]	mA			�c body1�ւ̑��Έʒu
		//! @param[in]	mB			�c body2�ւ̑��Έʒu
		void Create(YsBulletWorld& world, btRigidBody* body1, btRigidBody* body2, const YsMatrix& mA, const YsMatrix& mB)
		{
			Release();

			m_World = &world;

			btTransform tA;
			tA.setFromOpenGLMatrix(&mA._11);
			btTransform tB;
			tB.setFromOpenGLMatrix(&mB._11);
			m_Joint = SPtr<btGeneric6DofSpringConstraint>(new btGeneric6DofSpringConstraint(*body1, *body2, tA, tB, true));
			m_World->GetWorld()->addConstraint(m_Joint.get(), true);
			m_Joint->setUserConstraintPtr(this);

			m_Joint->setEquilibriumPoint();
		}

		//! @brief �o�l��L���ɂ���
		//! Index:0�`2�����s�ړ�x,y,z 3�`5�͉�]�ړ�x,y,z
		void EnableSpring(int Index, bool enable)
		{
			m_Joint->enableSpring(Index, enable);
		}
		//! @brief �o�l�̏_�炩��
		void SetSpring_Stiffness(int Index, float f){
			m_Joint->setStiffness(Index, f);
		}
		//! @brief ������
		void SetDamping(int Index, float f){
			m_Joint->setDamping(Index, f); //������
		}

		// ����

		//! @brief �ړ������ݒ� ����
		void SetLinearLowerLimit(float limitX, float limitY, float limitZ){
			m_Joint->setLinearLowerLimit(btVector3(limitX, limitY, limitZ));
		}
		//! @brief �ړ������ݒ� ���
		void SetLinearUpperLimit(float limitX, float limitY, float limitZ){
			m_Joint->setLinearUpperLimit(btVector3(limitX, limitY, limitZ));
		}
		//! @brief ��]�����ݒ� ����(�x)
		void SetAngularLowerLimit(float anglimitX, float anglimitY, float anglimitZ){
			m_Joint->setAngularLowerLimit(btVector3(YsToRadian(anglimitX), YsToRadian(anglimitY), YsToRadian(anglimitZ)));
		}
		//! @brief ��]�����ݒ� ���(�x)
		void SetAngularUpperLimit(float anglimitX, float anglimitY, float anglimitZ){
			m_Joint->setAngularUpperLimit(btVector3(YsToRadian(anglimitX), YsToRadian(anglimitY), YsToRadian(anglimitZ)));
		}
		//! @brief Index�� �e�����ݒ�
		void SetLimit(int Index, float limitLo, float limitHi){
			m_Joint->setLimit(Index, limitLo, limitHi);
		}

		//! @brief ������Ă񂾂Ƃ���joint�̈ʒu�֌W���o�l�̕�����̈ʒu�ɂȂ�
		void SetEquilibriumPoint(){
			m_Joint->setEquilibriumPoint();
		}

		// 
		YsBulletJoint_6DofSpring() :
			m_Joint(nullptr) {
//			m_Type = JO_6DOFSPRING;
		}

		~YsBulletJoint_6DofSpring() {
			Release();
		}

	protected:
		SPtr<btGeneric6DofSpringConstraint>		m_Joint;

	private:
		// �R�s�[�֎~�p
		YsBulletJoint_6DofSpring(const YsBulletJoint_6DofSpring& src){}
		void operator=(const YsBulletJoint_6DofSpring& src){}
	};


}

#endif
