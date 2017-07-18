//===============================================================
//! @file YsBulletPhysicsEngine.h
//! @brief 「Bullet Physics Engine」関係のクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsBulletPhysicsEngine_h__
#define __YsBulletPhysicsEngine_h__

#pragma warning(disable:4316)

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

namespace YskLib{

class YsBulletRigidBody;


// YsBulletWorld			… 物理世界


// YsBulletJoint_Point		… ボールジョイント
// YsBulletJoint_Hinge		… ヒンジジョイント
// YsBulletJoint_6Dof		… 6Dofジョイント
// YsBulletJoint_6DofSpring	… バネ付き6Dofジョイント


//=====================================================
//! @brief 「Bullet Physics Engine」ワールド用クラス
//!
//! @ingroup Physics
//=====================================================
class YsBulletWorld {
public:
	//! @brief 物理ワールド取得
	btDynamicsWorld* GetWorld(){return m_dynamicsWorld;}

	//! @brief 初期化
	void Init();

	//! @brief 解放
	void Release();

	//! @brief シミュレーション進行
	int StepSimulation(btScalar timeStep,int MaxSubSteps=1, btScalar fixedTimeStep=btScalar(1.)/btScalar(60.))
	{
		return m_dynamicsWorld->stepSimulation( timeStep ,MaxSubSteps, fixedTimeStep);
	}

	//! @brief 衝突検知用コールバック通知用
	//!
	//! 剛体同士が衝突するとこの関数が実行される。				\n
	//! このクラスを継承し、この関数をオーバーライドして使う。	\n
	//!
	//! @param[in]	cp		… 衝突の情報が入ってるぽい
	//! @param[in]	body0	… 衝突した剛体その１
	//! @param[in]	body1	… 衝突した剛体その２
	virtual bool ContactProcessedCallback(btManifoldPoint& cp, YsBulletRigidBody* body0, YsBulletRigidBody* body1){ return true; }

	//! @brief 初期化されてる？
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
	// 衝突検知用静的コールバック関数
	static bool s_ContactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1);

private:
	// コピー禁止用
	YsBulletWorld(const YsBulletWorld& src){}
	void operator=(const YsBulletWorld& src){}
};

//==========================================================================
//! @brief Bulletオブジェクト基本クラス
//!
//! @ingroup Physics
//==========================================================================
class YsBulletObj_Base
{
public:
	YsBulletWorld* GetWorld(){return m_World;}

	virtual void Release() = 0;

	// ユーザポインタ
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

	YsBulletWorld*	m_World;		// 登録しているワールド
	void*			m_lpUserPointer;

};

}

#include "YsBullet_Shape.h"
#include "YsBullet_RigidBody.h"
#include "YsBullet_Joint.h"

namespace YskLib {



}

#endif
