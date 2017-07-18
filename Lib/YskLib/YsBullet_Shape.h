//===============================================================
//! @file YsBullet_RigidBody.h
//! @brief �uBullet Physics Engine�v�́u�`��v�֌W�̃N���X
//!
//! �K���ɂ܂Ƃ߂ď����Ă܂��B
//!
//! @author ���c
//===============================================================
#ifndef __YsBullet_Shape_h__
#define __YsBullet_Shape_h__

#pragma warning(disable:4316)

namespace YskLib
{
	// YsBulletShape_Base		�c �`��̊�{�N���X
	// YsBulletShape_Box		�c �����̌`��
	// YsBulletShape_Sphere		�c ���`��
	// YsBulletShape_Capsule	�c �J�v�Z���`��
	// YsBulletShape_Mesh		�c ���b�V���`��
	// YsBulletShape_Compound	�c �����`��

	//==========================================================================
	//! @brief �`���{�N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Base : public YsBulletObj_Base {
	public:
		//! @brief �`����擾
		virtual SPtr<btCollisionShape>	GetShape() { return nullptr; }

		//! @brief �X�P�[���l���擾
		void GetLocalScaling(YsVec3& scale) {
			btCollisionShape* shape = GetShape().get();
			btVector3 v = shape->getLocalScaling();
			scale.Set(v.x(), v.y(), v.z());
		}

		//! @brief �X�P�[���ݒ�
		void SetLocalScaling(const YsVec3& localScale){
			GetShape()->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

	private:

	};

	//==========================================================================
	//! @brief BOX�`��N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Box : public YsBulletShape_Base {
	public:
		//! @brief �`����擾
		virtual SPtr<btCollisionShape>		GetShape() override { return m_Shape; }

		//!@ brief BOX�`��擾
		SPtr<btBoxShape>					GetShape_Box(){ return m_Shape; }

		//! @brief �`����쐬
		//! @param[in]	halfSize	�c �傫��(�n�[�t�T�C�Y)
		//! @param[in]	localScale	�c �����ɋL������`��̃X�P�[�� GetMatrix()���ɉe�����܂�
		void Create(const YsVec3& halfSize, const YsVec3& localScale = YsVec3::One){
			// �`��쐬
			btVector3 shapeHarfSize(halfSize.x, halfSize.y, halfSize.z);
			m_Shape = SPtr<btBoxShape>(new btBoxShape(shapeHarfSize));	// ��Bullet���C�u������make_shared�g���Ǝ��s���鎞���������̂ŁA�ʏ�쐬
			m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

		//! @brief ���
		virtual void Release() override{
			m_Shape = nullptr;
		}

		// 
		YsBulletShape_Box(){
		}
		// 
		~YsBulletShape_Box(){
			Release();
		}

	protected:
		SPtr<btBoxShape>		m_Shape;		//!< �`��

	private:
		// �R�s�[�֎~�p
		YsBulletShape_Box(const YsBulletShape_Box& src){}
		void operator=(const YsBulletShape_Box& src){}

	};

	//==========================================================================
	//! @brief �J�v�Z���`��N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Capsule : public YsBulletShape_Base {
	public:
		//! @brief �`����擾
		virtual SPtr<btCollisionShape>	GetShape() override { return m_Shape; }

		//!@ brief �J�v�Z���`��擾
		SPtr<btCapsuleShape>			GetShape_Capsule(){ return m_Shape; }

		//! @brief �`����쐬
		//! @param[in]	halfSize	�c �傫��(�n�[�t�T�C�Y)
		//! @param[in]	localScale	�c �����ɋL������`��̃X�P�[�� GetMatrix()���ɉe�����܂�
		void Create(float radius, float height, const YsVec3& localScale = YsVec3::One){
			// �`��쐬
			m_Shape = SPtr<btCapsuleShape>(new btCapsuleShape(radius, height));	// ��Bullet���C�u������make_shared�g���Ǝ��s���鎞���������̂ŁA�ʏ�쐬
			m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

		//! @brief ���
		virtual void Release() override{
			m_Shape = nullptr;
		}

		// 
		YsBulletShape_Capsule(){
		}
		// 
		~YsBulletShape_Capsule(){
			Release();
		}

	protected:
		SPtr<btCapsuleShape>		m_Shape;		//!< �`��

	private:
		// �R�s�[�֎~�p
		YsBulletShape_Capsule(const YsBulletShape_Capsule& src){}
		void operator=(const YsBulletShape_Capsule& src){}

	};

	//==========================================================================
	//! @brief ���`��N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Sphere : public YsBulletShape_Base {
	public:
		//! @brief �`����擾
		virtual SPtr<btCollisionShape>	GetShape() override { return m_Shape; }

		//!@ brief �J�v�Z���`��擾
		SPtr<btSphereShape>				GetShape_Sphere(){ return m_Shape; }

		//! @brief �`����쐬
		//! @param[in]	halfSize	�c �傫��(�n�[�t�T�C�Y)
		//! @param[in]	localScale	�c �����ɋL������`��̃X�P�[�� GetMatrix()���ɉe�����܂�
		void Create(float radius, const YsVec3& localScale = YsVec3::One){
			// �`��쐬
			m_Shape = SPtr<btSphereShape>(new btSphereShape(radius));	// ��Bullet���C�u������make_shared�g���Ǝ��s���鎞���������̂ŁA�ʏ�쐬
			m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

		//! @brief ���
		virtual void Release() override{
			m_Shape = nullptr;
		}

		// 
		YsBulletShape_Sphere(){
		}
		// 
		~YsBulletShape_Sphere(){
			Release();
		}

	protected:
		SPtr<btSphereShape>		m_Shape;		//!< �`��

	private:
		// �R�s�[�֎~�p
		YsBulletShape_Sphere(const YsBulletShape_Sphere& src){}
		void operator=(const YsBulletShape_Sphere& src){}

	};

	//==========================================================================
	//! @brief �����`��N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Compound : public YsBulletShape_Base {
	public:
		//! @brief �`����擾
		virtual SPtr<btCollisionShape>	GetShape() override { return m_Shape; }

		//!@ brief �J�v�Z���`��擾
		SPtr<btCompoundShape>			GetShape_Compound(){ return m_Shape; }

		//! @brief �`����쐬
		//! @param[in]	localScale	�c �����ɋL������`��̃X�P�[�� GetMatrix()���ɉe�����܂�
		void Create(const YsVec3& localScale = YsVec3::One){
			// �`��쐬
			m_Shape = SPtr<btCompoundShape>(new btCompoundShape());	// ��Bullet���C�u������make_shared�g���Ǝ��s���鎞���������̂ŁA�ʏ�쐬
			m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

		//! @brief Compound�`��̎q�`��Ƃ��ēo�^
		void AddChildShape(SPtr<YsBulletShape_Base> shape, const YsMatrix& mat);

		//! @brief �o�^����Ă�q�`�󐔂��擾
		int GetChildShapeCnt(){ return m_Shape->getNumChildShapes(); }

		//! @brief ���
		virtual void Release() override;

		// 
		YsBulletShape_Compound(){
		}
		// 
		~YsBulletShape_Compound(){
			Release();
		}

	protected:
		SPtr<btCompoundShape>					m_Shape;		//!< �`��

		std::vector< SPtr<YsBulletShape_Base> >	m_ShapeList;	//!< �q�`�󃊃X�g(�`����폜����ꍇ�́Am_Shape������̍폜���邱��)

	private:
		// �R�s�[�֎~�p
		YsBulletShape_Compound(const YsBulletShape_Compound& src){}
		void operator=(const YsBulletShape_Compound& src){}

	};

	//==========================================================================
	//! @brief ���b�V���`��N���X
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Mesh : public YsBulletShape_Base {
	public:
		//! @brief �`����擾
		virtual SPtr<btCollisionShape>	GetShape() override { return m_Shape; }

		//!@ brief �J�v�Z���`��擾
		SPtr<btBvhTriangleMeshShape>			GetShape_Compound(){ return m_Shape; }

		//! @brief �`����쐬
		//! @param[in]	pMesh	�c �傫��(�n�[�t�T�C�Y)
		//! @param[in]	localScale	�c �����ɋL������`��̃X�P�[�� GetMatrix()���ɉe�����܂�
		void CreateFromMesh(const YsMesh& pMesh, const YsVec3& localScale = YsVec3::One);


		//! @brief ���
		virtual void Release() override{
			m_Shape = nullptr;
		}

		// 
		YsBulletShape_Mesh(){
		}
		// 
		~YsBulletShape_Mesh(){
			Release();
		}

	protected:
		btVector3*							m_Verts;				//!< ���_�z��
		int*								m_Idxs;					//!< �C���f�b�N�X�z��
		btTriangleIndexVertexArray*			m_indexVertexArrays;	//!< 
		//	btGImpactMeshShape*				m_Shape;				//!< �`��
		SPtr<btBvhTriangleMeshShape>		m_Shape;				//!< �`��


	private:
		// �R�s�[�֎~�p
		YsBulletShape_Mesh(const YsBulletShape_Mesh& src){}
		void operator=(const YsBulletShape_Mesh& src){}

	};
}


#endif
