#include "YskLib.h"

using namespace YskLib;

extern ContactProcessedCallback gContactProcessedCallback;

//==========================================================================
// Bullet Physics Engine ワールド用クラス
//==========================================================================

void YsBulletWorld::Init()
{
	// bullet初期化
	m_collisionConfiguration = new btDefaultCollisionConfiguration();		// デフォルトのコリジョンコンフィグ作成
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);	// デフォルトのコリジョンディスパッチャ作成

	// ブロードフェーズ衝突判定
	m_broadphase = new btDbvtBroadphase();
	/*
	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);
	m_broadphase = new btAxisSweep3 (worldAabbMin, worldAabbMax);
	*/

	// デフォルトの拘束計算（数値解析）ソルバを作成する
	m_solver = new btSequentialImpulseConstraintSolver;

	// 物理ワールドの作成
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver,m_collisionConfiguration);
	// 重力の設定
	m_dynamicsWorld->setGravity(btVector3(0,-9.8f,0));

	// 衝突検知用コールバック関数設定
	gContactProcessedCallback = YsBulletWorld::s_ContactProcessedCallback;

	btGImpactCollisionAlgorithm::registerAlgorithm(m_dispatcher);
}

bool YsBulletWorld::s_ContactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1)
{
	btRigidBody* pBody0 = (btRigidBody*)body0;
	btRigidBody* pBody1 = (btRigidBody*)body1;

	YsBulletRigidBody* pRigidBase0 = (YsBulletRigidBody*)pBody0->getUserPointer();
	YsBulletRigidBody* pRigidBase1 = (YsBulletRigidBody*)pBody1->getUserPointer();

	// Bullet Worldのアドレスを取得
	YsBulletWorld* world = nullptr;
	if(pRigidBase0)world = pRigidBase0->GetWorld();
	else if(pRigidBase1)world = pRigidBase1->GetWorld();

	// 仮想関数呼び出しで通知する
	if(world){
		return world->ContactProcessedCallback(cp, pRigidBase0, pRigidBase1);
	}

	return true;
}

// 解放
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
		// 剛体
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

	YsSafe_Delete(m_dynamicsWorld);			// ワールド削除
	YsSafe_Delete(m_solver);
	YsSafe_Delete(m_broadphase);
	YsSafe_Delete(m_dispatcher);
	YsSafe_Delete(m_collisionConfiguration);
}
