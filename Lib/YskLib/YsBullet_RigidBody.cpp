#include "YskLib.h"

using namespace YskLib;

void YsBulletRigidBody::Create(YsBulletWorld& world, SPtr<YsBulletShape_Base> shape, const YsMatrix& startMat, float mass, short group, short mask)
{
	Release();

	m_World = &world;
	m_Shape = shape;

	// 剛体の行列は拡大成分を入れてはいけないので、拡大無しの行列と拡大率を分離する
	//  拡大無し行列 → 剛体に使用
	//  拡大率　　　 → 形状に使用
	YsMatrix mat = (YsMatrix&)startMat;
	YsVec3 scale = mat.GetScale();
	mat.NormalizeScale();

	// 行列
	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&mat._11);

	// 形状に行列の拡大率を適用する
	shape->SetLocalScaling(scale);

	// 質量が0以外なら、動的物体なので形状から慣性テンソルを算出。0なら静的物体。
	if(mass != 0.0f) {
		shape->GetShape()->calculateLocalInertia(mass, m_Inertia);
	}
	else{
		m_Inertia.setValue(0, 0, 0);
	}
	m_Mass = mass;

	// 剛体作成
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape->GetShape().get(), m_Inertia);
	rbInfo.m_additionalDamping = true;
	m_Body = SPtr<btRigidBody>(new btRigidBody(rbInfo));
	m_Body->setUserPointer(this);		// ユーザーポインタに自分のアドレスを記憶させておく

	// 質量0なら、キネマティックにしとこ
	if(mass == 0){
		SetKinematic(true);
	}

	// ワールドに剛体追加
	m_World->GetWorld()->addRigidBody(m_Body.get(), group, mask);
}

void YsBulletRigidBody::Release()
{
	// 剛体削除
	if(m_Body) {
		// モーションステート削除
		if(m_Body->getMotionState())delete m_Body->getMotionState();
		// 
		while(m_Body->getNumConstraintRefs() > 0) {
			m_Body->removeConstraintRef(m_Body->getConstraintRef(0));
		}

		// ワールドから解除
		if(m_World && m_World->GetWorld()) {
			m_World->GetWorld()->removeCollisionObject(m_Body.get());
		}
		// 解放
		m_Body = nullptr;

		m_Inertia.setValue(0, 0, 0);
		m_Mass = 0;
		m_Shape = nullptr;
	}

}
