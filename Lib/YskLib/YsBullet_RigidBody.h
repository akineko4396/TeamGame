//===============================================================
//! @file YsBullet_RigidBody.h
//! @brief 「Bullet Physics Engine」の「剛体」関係のクラス
//!
//! 適当にまとめて書いてます。
//!
//! @author 鎌田
//===============================================================
#ifndef __YsBullet_RigidBody_h__
#define __YsBullet_RigidBody_h__

#pragma warning(disable:4316)

namespace YskLib
{
	//==========================================================================
	//! @brief Bullet剛体基本クラス
	//! 実際に物理演算で動く物体。Shapeクラスを引っ付けると、その形状で動作する。
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletRigidBody : public YsBulletObj_Base {
	public:


		//========================================================================
		// 作成・解放
		//========================================================================

		//! @brief 形状と剛体を作成、ワールドへ追加
		//! @param[in]	world		… 剛体を登録する物理ワールド
		//! @param[in]	shape		… 登録する形状
		//! @param[in]	startMat	… 初期行列(拡大は含めれない)
		//! @param[in]	mass		… 質量
		//! @param[in]	group		… 判定グループフィルタ(所属しているグループにのみ衝突を行う)
		//! @param[in]	mask		… 判定マスクフィルタ(指定groupの剛体と衝突するか？のフィルタ 0xFFFFで全ての剛体と衝突する)
		void Create(YsBulletWorld& world, SPtr<YsBulletShape_Base> shape, const YsMatrix& startMat, float mass, short group = 1, short mask = (short)0xFFFF);

		//! @brief 解放
		void Release();


		//========================================================================
		// 取得・設定
		// ※一部だけです
		//========================================================================

		//! @brief 剛体取得
		SPtr<btRigidBody>		GetBody(){ return m_Body; }

		//! @brief キネマティック設定
		//! @param[in]	kinematic	… キネマティックにするか？
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

		//! @brief 静的剛体か？(動かないもの)
		bool IsStatic(){
			if(m_Body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) {
				return true;
			}
			return false;
		}

		//! @brief 質量
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

		//! @brief 質量取得
		float GetMass(){ return m_Mass; }

		//! @brief 慣性テンソル更新
		void UpdateInertia()
		{
			if(m_Shape == nullptr)return;

			if(m_Body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) {
				m_Body->setMassProps(0, btVector3(0, 0, 0));
			}
			else{
				// 慣性テンソル更新
				if(m_Mass != 0.0f) {
					m_Shape->GetShape()->calculateLocalInertia(m_Mass, m_Inertia);
				}
				m_Body->setMassProps(m_Mass, m_Inertia);
			}
			m_Body->updateInertiaTensor();
		}
		//! @brief 反発力設定
		void SetRestitution(float r)
		{
			m_Body->setRestitution(r);
		}
		//! @brief 摩擦力設定
		void SetFriction(float f)
		{
			m_Body->setFriction(f);
		}
		//! @brief 移動減衰設定
		void SetLinearDamping(float d)
		{
			m_Body->setDamping(d, m_Body->getAngularDamping());
		}
		//! @brief 回転減衰設定
		void SetAngularDamping(float d)
		{
			m_Body->setDamping(m_Body->getLinearDamping(), d);
		}
		//! @brief 移動力設定
		void SetLinearVelocity(const YsVec3& v)
		{
			m_Body->setLinearVelocity(btVector3(v.x, v.y, v.z));
		}
		//! @brief 回転力設定
		void SetAngularVelocity(const YsVec3& v)
		{
			m_Body->setAngularVelocity(btVector3(v.x, v.y, v.z));
		}

		//! @brief かかる力をクリアする
		void ClearForces()
		{
			m_Body->clearForces();
		}
		//! @brief 回転力を与える
		void ApplyTorque(const YsVec3& t)
		{
			m_Body->applyTorque(btVector3(t.x, t.y, t.z));
		}
		//! @brief 力を与える(継続的な力。シェット噴射のような挙動)
		void ApplyForce(const YsVec3& force)
		{
			m_Body->applyCentralForce(btVector3(force.x, force.y, force.z));
		}
		//! @brief 指定位置に力を与える(継続的な力。シェット噴射のような挙動)
		void ApplyForce(const YsVec3& force, const YsVec3& rel_pos)
		{
			m_Body->applyForce(btVector3(force.x, force.y, force.z), btVector3(rel_pos.x, rel_pos.y, rel_pos.z));
		}
		//! @brief 力を与える(瞬間的な力。バットで打った時のような挙動)
		void ApplyImpulse(const YsVec3& impulse)
		{
			m_Body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
		}
		//! @brief 指定座標に力を与える(瞬間的な力。バットで打った時のような挙動)
		void ApplyImpulse(const YsVec3& impulse, const YsVec3& rel_pos)
		{
			m_Body->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(rel_pos.x, rel_pos.y, rel_pos.z));
		}
		//! @brief スリープしなくする
		void SetNonSleep(){
			m_Body->setSleepingThresholds(0, 0);
			//		m_Body->setActivationState(DISABLE_DEACTIVATION);
		}

		//! @brief 剛体へ直接行列をセットする
		//! @param[in]	mat		… 設定したい行列
		void SetMatrix(const YsMatrix& mat){

			// スケールは形状の方へセット
			if(m_Shape){
				m_Shape->SetLocalScaling(mat.GetScale());
			}

			// 剛体の行列はスケールを含めない
			YsMatrix m(mat);
			m.NormalizeScale();

			// Bullet用の行列へ変換
			btTransform t;
			t.setFromOpenGLMatrix(&m._11);
			// セット
			m_Body->getMotionState()->setWorldTransform(t);
			m_Body->setCenterOfMassTransform(t);
		}
		//! @brief 剛体から行列を取得する
		//! 形状を持ってる場合は、その大きさも行列に含む
		//! @param[out]	mOut	… 取得した行列を入れるための変数
		void GetMatrix(YsMatrix& mOut){
			// 行列取得
			btTransform t = m_Body->getWorldTransform();
			t.getOpenGLMatrix(&mOut._11);
			if(m_Shape){
				YsVec3 scale;
				m_Shape->GetLocalScaling(scale);
				mOut.Scale_Local(scale);
			}
		}

		//! @brief ワールドから剛体を解除する
		bool RemoveFromWorld(){
			if(m_Body->isInWorld()){
				if(m_World->GetWorld()){
					m_World->GetWorld()->removeCollisionObject(m_Body.get());
				}
				return true;
			}
			return false;
		}
		//! @brief ワールドへこの剛体を追加する
		bool AddToWorld(short group = 1, short mask = -1){
			// ワールドに剛体追加
			if(!m_Body->isInWorld()){
				m_World->GetWorld()->addRigidBody(m_Body.get(), group, mask);
				return true;
			}
			return false;
		}

		//========================================================================
		// モーションステート
		//========================================================================

		//! @brief モーションステート作成
		template<class T>
		T* CreateMotionState(){
			if(m_Body->getMotionState())delete m_Body->getMotionState();
			T* ms = new T();
			m_Body->setMotionState(ms);
			return ms;
		}
		//! @brief モーションステートをセット
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
		btVector3					m_Inertia;	//!< 慣性テンソル
		SPtr<btRigidBody>			m_Body;		//!< 剛体
		float						m_Mass;		//!< 質量

		SPtr<YsBulletShape_Base>	m_Shape;	//!< 設定されてる形状

	};

}


#endif
