//===============================================================
//! @file YsBullet_RigidBody.h
//! @brief 「Bullet Physics Engine」の「ジョイント」関係のクラス
//!
//! 適当にまとめて書いてます。
//!
//! @author 鎌田
//===============================================================
#ifndef __YsBullet_Joint_h__
#define __YsBullet_Joint_h__

#pragma warning(disable:4316)

namespace YskLib
{

	//==========================================================================
	//! @brief ジョイント基本クラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_Base : public YsBulletObj_Base {
	protected:
		YsBulletJoint_Base(){}
	};

	//==========================================================================
	//! @brief ボールジョイントクラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_Point : public YsBulletJoint_Base {
	public:
		//! @brief ボールジョイント取得
		SPtr<btPoint2PointConstraint> GetJoint(){ return m_Joint; }

		//! @brief 解放
		virtual void Release() override
		{
			if(m_Joint){
				if(m_World)m_World->GetWorld()->removeConstraint(m_Joint.get());
				m_Joint = nullptr;
			}
			m_World = nullptr;
		}

		//! @brief ジョイント作成
		//! @param[in]	world		… ジョイントを追加する物理ワールド
		//! @param[in]	body1		… ジョイントに接続するジョイント１
		//! @param[in]	body2		… ジョイントに接続するジョイント２
		//! @param[in]	pivotA		… body1への相対位置
		//! @param[in]	pivotB		… body2への相対位置
		void Create(YsBulletWorld& world, btRigidBody* body1, btRigidBody* body2, const YsVec3& pivotA, const YsVec3& pivotB)
		{
			Release();

			m_World = &world;

			btVector3 vA(pivotA.x, pivotA.y, pivotA.z);
			btVector3 vB(pivotB.x, pivotB.y, pivotB.z);
			m_Joint = SPtr<btPoint2PointConstraint>(new btPoint2PointConstraint(*body1, *body2, vA, vB));
			m_World->GetWorld()->addConstraint(m_Joint.get(), true);
		}

		//! @brief ジョイント作成を作成し、ワールドへ追加
		//! @param[in]	world		… ジョイントを追加する物理ワールド
		//! @param[in]	body1		… ジョイントに接続するジョイント１
		//! @param[in]	pivotA		… body1への相対位置
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
		// コピー禁止用
		YsBulletJoint_Point(const YsBulletJoint_Point& src){}
		void operator=(const YsBulletJoint_Point& src){}
	};

	//==========================================================================
	//! @brief ヒンジジョイントクラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_Hinge : public YsBulletJoint_Base {
	public:
		//! @brief ヒンジジョイント取得
		SPtr<btHingeConstraint> GetJoint(){ return m_Joint; }

		//! @brief 解放
		virtual void Release() override
		{
			if(m_Joint){
				if(m_World)m_World->GetWorld()->removeConstraint(m_Joint.get());
				m_Joint = nullptr;
			}
			m_World = nullptr;
		}

		//! @brief ジョイント作成を作成し、ワールドへ追加
		void Create(YsBulletWorld& world, btRigidBody* body1, btRigidBody* body2, const btVector3& pivotA, const btVector3& pivotB, const btVector3& axisA, const btVector3& axisB)
		{
			m_World = &world;

			m_Joint = SPtr<btHingeConstraint>(new btHingeConstraint(*body1, *body2, pivotA, pivotB, axisA, axisB));
			m_World->GetWorld()->addConstraint(m_Joint.get(), true);
			m_Joint->setUserConstraintPtr(this);
		}

		//! @brief ジョイント作成を作成し、ワールドへ追加
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
		// コピー禁止用
		YsBulletJoint_Hinge(const YsBulletJoint_Hinge& src){}
		void operator=(const YsBulletJoint_Hinge& src){}
	};


	//==========================================================================
	//! @brief 6Dofジョイントクラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_6Dof : public YsBulletJoint_Base {
	public:
		//! @brief 6Dofジョイント取得
		SPtr<btGeneric6DofConstraint> GetJoint(){ return m_Joint; }

		//! @brief 解放
		virtual void Release() override
		{
			if(m_Joint){
				if(m_World)m_World->GetWorld()->removeConstraint(m_Joint.get());
				m_Joint = nullptr;
			}
			m_World = nullptr;
		}

		//! @brief ジョイント作成を作成し、ワールドへ追加
		//! @param[in]	world		… ジョイントを追加する物理ワールド
		//! @param[in]	body1		… ジョイントに接続するジョイント１
		//! @param[in]	body2		… ジョイントに接続するジョイント２
		//! @param[in]	mA			… body1への相対位置
		//! @param[in]	mB			… body2への相対位置
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

		// 制限

		//! @brief 移動制限設定 下限
		void SetLinearLowerLimit(float limitX, float limitY, float limitZ){
			m_Joint->setLinearLowerLimit(btVector3(limitX, limitY, limitZ));
		}
		//! @brief 移動制限設定 上限
		void SetLinearUpperLimit(float limitX, float limitY, float limitZ){
			m_Joint->setLinearUpperLimit(btVector3(limitX, limitY, limitZ));
		}
		//! @brief 回転制限設定 下限(度)
		void SetAngularLowerLimit(float anglimitX, float anglimitY, float anglimitZ){
			m_Joint->setAngularLowerLimit(btVector3(YsToRadian(anglimitX), YsToRadian(anglimitY), YsToRadian(anglimitZ)));
		}
		//! @brief 回転制限設定 上限(度)
		void SetAngularUpperLimit(float anglimitX, float anglimitY, float anglimitZ){
			m_Joint->setAngularUpperLimit(btVector3(YsToRadian(anglimitX), YsToRadian(anglimitY), YsToRadian(anglimitZ)));
		}
		//! @brief Index版 各制限設定
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
		// コピー禁止用
		YsBulletJoint_6Dof(const YsBulletJoint_6Dof& src){}
		void operator=(const YsBulletJoint_6Dof& src){}
	};

	//==========================================================================
	//! @brief 6Dofバネジョイントクラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletJoint_6DofSpring : public YsBulletJoint_Base {
	public:
		//! @brief 6Dofバネジョイント取得
		SPtr<btGeneric6DofSpringConstraint> GetJoint(){ return m_Joint; }

		//! @brief 解放
		virtual void Release() override
		{
			if(m_Joint){
				if(m_World)m_World->GetWorld()->removeConstraint(m_Joint.get());
				m_Joint = nullptr;
			}
			m_World = nullptr;
		}

		//! @brief ジョイント作成を作成し、ワールドへ追加
		//! @param[in]	world		… ジョイントを追加する物理ワールド
		//! @param[in]	body1		… ジョイントに接続するジョイント１
		//! @param[in]	body2		… ジョイントに接続するジョイント２
		//! @param[in]	mA			… body1への相対位置
		//! @param[in]	mB			… body2への相対位置
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

		//! @brief バネを有効にする
		//! Index:0～2が平行移動x,y,z 3～5は回転移動x,y,z
		void EnableSpring(int Index, bool enable)
		{
			m_Joint->enableSpring(Index, enable);
		}
		//! @brief バネの柔らかさ
		void SetSpring_Stiffness(int Index, float f){
			m_Joint->setStiffness(Index, f);
		}
		//! @brief 減衰率
		void SetDamping(int Index, float f){
			m_Joint->setDamping(Index, f); //減衰率
		}

		// 制限

		//! @brief 移動制限設定 下限
		void SetLinearLowerLimit(float limitX, float limitY, float limitZ){
			m_Joint->setLinearLowerLimit(btVector3(limitX, limitY, limitZ));
		}
		//! @brief 移動制限設定 上限
		void SetLinearUpperLimit(float limitX, float limitY, float limitZ){
			m_Joint->setLinearUpperLimit(btVector3(limitX, limitY, limitZ));
		}
		//! @brief 回転制限設定 下限(度)
		void SetAngularLowerLimit(float anglimitX, float anglimitY, float anglimitZ){
			m_Joint->setAngularLowerLimit(btVector3(YsToRadian(anglimitX), YsToRadian(anglimitY), YsToRadian(anglimitZ)));
		}
		//! @brief 回転制限設定 上限(度)
		void SetAngularUpperLimit(float anglimitX, float anglimitY, float anglimitZ){
			m_Joint->setAngularUpperLimit(btVector3(YsToRadian(anglimitX), YsToRadian(anglimitY), YsToRadian(anglimitZ)));
		}
		//! @brief Index版 各制限設定
		void SetLimit(int Index, float limitLo, float limitHi){
			m_Joint->setLimit(Index, limitLo, limitHi);
		}

		//! @brief これを呼んだときのjointの位置関係がバネの復元基準の位置になる
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
		// コピー禁止用
		YsBulletJoint_6DofSpring(const YsBulletJoint_6DofSpring& src){}
		void operator=(const YsBulletJoint_6DofSpring& src){}
	};


}

#endif
