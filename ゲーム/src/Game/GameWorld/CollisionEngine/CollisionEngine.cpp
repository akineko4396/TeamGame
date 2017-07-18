

#include "main.h"

#include "Game\GameWorld\GameWorld.h"

#include "CollisionEngine.h"



BOOL CheckViewFrustum(SPtr<CharacterBase> _MyData, SPtr<YsMesh> _Mesh)
{

	//自身からカメラまでの距離格納用
	YsVec3 vTargetVec = _MyData->GetPos();

	//距離＝自身の座標ーカメラの座標
	vTargetVec = vTargetVec - VF.GetCamPos();

	for (int i = 0; i < 4; i++)
	{
		//内積を求める
		float Len = 0;

		//内積を返す
		Len = D3DXVec3Dot(&vTargetVec, &VF.GetNormal(i));

		//内積で求めた距離 > 自身のバウンディングスフィアの半径
		if (Len > _Mesh->GetBSRadius()* _MyData->GetScale().x)
		{
			//描画しない
			return false;
		}
	}

	//描画する
	return true;
}

void ViewFrustum::Create(YsMatrix _Proj, YsMatrix _View, YsMatrix _Cam)
{

	//射影行列とビュー行列を合成
	YsMatrix mVP = _View*_Proj;

	//カメラのワールド座標取得
	YsMatrix mInvV = _View;

	//逆行列作成
	mInvV.CreateInverse();

	//カメラ座標取得
	m_CamPos = _Cam.GetPos();

	//逆行列にする
	mVP.CreateInverse();

	//視錘台頂点名
	enum VERTEX
	{
		LeftTop = 0,
		RightTop = 1,
		LeftBottom = 2,
		RightBottom = 3,
		MAX,
	};

	//視錘台頂点格納用
	YsVec3 vFrustumVertex[VERTEX::MAX] = {
		YsVec3(-1, 1, 1),	//LeftTop
		YsVec3(1, 1, 1),	//RightTop
		YsVec3(-1, -1, 1),//LeftBottom
		YsVec3(1, -1, 1)	//RightBottom
	};

	//頂点までの距離格納用
	YsVec3 VertexLength[VERTEX::MAX] = {
		YsVec3(0, 0, 0),	//LeftTopまでの距離
		YsVec3(0, 0, 0),	//RigthTopまでの距離
		YsVec3(0, 0, 0),	//LeftBottomまでの距離
		YsVec3(0, 0, 0)	//RightBottomまでの距離
	};

	//頂点数分ループ
	for (int i = 0; i < VERTEX::MAX; i++)
	{

		//合成した行列と視錘台の頂点を合成するための射影行列
		YsMatrix mPVertex;

		//頂点行列化用
		YsMatrix mTransVertex;

		//2D頂点を行列化
		D3DXMatrixTranslation(&mTransVertex, vFrustumVertex[i].x, vFrustumVertex[i].y, vFrustumVertex[i].z);

		//3D空間として保管する
		D3DXVec3TransformCoord(&vFrustumVertex[i], &vFrustumVertex[i], &mVP);

		//カメラとの距離を格納
		VertexLength[i] = vFrustumVertex[i] - m_CamPos;
	}

	//それぞれの頂点同士を結びそれぞれの面に対しての外積を求める

	enum NVERTEX
	{
		NLeft = 0,
		NTop = 1,
		NRight = 2,
		NBottom = 3,
		NMAX
	};

	YsVec3 Cross[NVERTEX::NMAX];

	//NLeft
	D3DXVec3Cross(&m_vN[NVERTEX::NLeft], &vFrustumVertex[VERTEX::LeftBottom], &VertexLength[VERTEX::LeftTop]);

	//NTop
	D3DXVec3Cross(&m_vN[NVERTEX::NTop], &vFrustumVertex[VERTEX::LeftTop], &VertexLength[VERTEX::RightTop]);

	//NRight
	D3DXVec3Cross(&m_vN[NVERTEX::NRight], &vFrustumVertex[VERTEX::RightTop], &VertexLength[VERTEX::RightBottom]);

	//NButtom
	D3DXVec3Cross(&m_vN[NVERTEX::NBottom], &vFrustumVertex[VERTEX::RightBottom], &VertexLength[VERTEX::LeftBottom]);

	//正規化
	for (int i = 0; i < NVERTEX::NMAX; i++)
	{
		D3DXVec3Normalize(&m_vN[i], &m_vN[i]);
	}
}













float BaseHitObj::CalcMassRatio(float mass)
{
	// 自分の質量が0なら、自分は不動
	if (m_Mass == 0)return 0.0f;
	// 相手の質量が0なら、自分は動く
	if (mass == 0)return 1.0f;

	// 両方質量あり
	return mass * (1.0f / (m_Mass + mass));
}

// 球 vs その他
bool HitObj_Sphere::HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2)
{

	// vs 球
	if (obj->m_Shape == HitShapes::SPHERE)	// 型判定
	{
		HitObj_Sphere* sph = static_cast<HitObj_Sphere*>(obj);	// 高速ダウンキャスト

		// 球判定
		YsVec3 vDir = sph->m_vPos - m_vPos;
		float dist = vDir.Length();

		// 当たってないなら終了
		if (dist >= m_Rad + sph->m_Rad)return false;

		// 当たってるなら結果を登録
		vDir.Normalize();
		float f = (m_Rad + sph->m_Rad) - dist;	// めり込んでいる距離を求める
		YsVec3 vPush = (-vDir * f);				// 押し戻すためのベクトルを求める

		// 自分に結果を記憶する
		if (resData1){
			// 結果データ追加
			resData1->m_youHitObj = obj;
			resData1->m_vHitPos = m_vPos + vDir*(dist*0.5f);
			resData1->m_vPush = vPush;
		}

		// 相手に結果を記憶する
		if (resData2){
			// 結果データ追加
			resData2->m_youHitObj = this;
			resData2->m_vHitPos = sph->m_vPos + -vDir*(dist*0.5f);
			resData2->m_vPush = -vPush;
		}

		return true;
	}
	// vs Ray
	else if (obj->m_Shape == HitShapes::RAY){
		HitObj_Ray* ray = static_cast<HitObj_Ray*>(obj);	// 高速ダウンキャスト

		YsVec3 vQPush;		// 球を押しているベクトル
		float dist;				// レイが当たった距離

		// 球 vs レイ判定
		if (YsCollision::SphereToRay(m_vPos, m_Rad, ray->m_vPos1, ray->m_vDir, &dist, &vQPush) == false){
			// 当たってないなら終了
			return false;
		}

		// 長さチェック
		if (dist > ray->m_RayLen)return false;

		// 当たってるなら、結果を登録

		// 自分に結果を記憶する
		if (resData1){
			// 結果データ追加
			resData1->m_youHitObj = obj;
			resData1->m_vPush = vQPush;
			resData1->m_vHitPos = ray->m_vPos1 + ray->m_vDir*dist;
		}
		// 相手に結果を記憶する
		if (resData2){
			// 結果データ追加
			resData2->m_youHitObj = this;
			resData2->m_RayDist = dist;
			resData2->m_FaceIdx = -1;
			resData2->m_vHitPos = ray->m_vPos1 + ray->m_vDir*dist;
		}

		return true;

	}
	// vs メッシュ
	else if (obj->m_Shape == HitShapes::MESH){
		HitObj_Mesh* mesh = static_cast<HitObj_Mesh*>(obj);

		//==================================================
		// 球 vs メッシュ判定
		//==================================================
		YsVec3 vOut;
		std::vector<YsCollision::MTS_HitData>	hitData;	// ヒット結果のデータ配列
		if (YsCollision::MeshToSphere(*mesh->m_pMesh, &mesh->m_Mat, m_vPos, m_Rad, &vOut, 0, -1, &hitData) == false){
			// 当たってないなら終了
			return false;
		}

		// 自分に結果を記憶する
		if (resData1){
			// 結果データ追加
			resData1->m_youHitObj = obj;
			resData1->m_vHitPos = hitData[0].vHitPos;
			resData1->m_vPush = vOut;
		}
		// 相手に結果を記憶する
		if (resData2){
			// 結果データ追加
			resData2->m_youHitObj = this;
			resData2->m_vHitPos = hitData[0].vHitPos;
			resData2->m_vPush = -vOut;
		}

		return true;

	}

	return false;
}

void HitObj_Sphere::DebugDraw(float alpha)
{
	YsVec4 mulCol(1, 1, 1, alpha);

	// 球ライン描画
	YsMatrix m;
	m.SetPos(m_vPos);
	ShMgr.m_Samp.DrawSphereLine(m_Rad, &(m_Debug_Color * mulCol), &m);

	// 当たったものへのライン描画
	for (auto& res : m_HitResTbl){
		ShMgr.m_Samp.DrawLine(m_vPos, res.m_vHitPos, &YsVec4(1, 1, 0, alpha));
	}

}


// レイ vs その他
bool HitObj_Ray::HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2)
{
	// vs 球
	if (obj->m_Shape == HitShapes::SPHERE){
		// 球側の関数を呼ぶ
		return obj->HitTest(this, resData2, resData1);
	}
	// vs Ray
	else if (obj->m_Shape == HitShapes::RAY){
		// レイ vs レイは判定しない
		return false;
	}
	// vs メッシュ
	else if (obj->m_Shape == HitShapes::MESH){
		HitObj_Mesh* mesh = static_cast<HitObj_Mesh*>(obj);

		//==================================================
		// レイ vs メッシュ判定
		//==================================================
		float dist;
		UINT faceIdx;
		if (YsCollision::RayToMesh(*mesh->m_pMesh, &mesh->m_Mat, m_vPos1, m_vPos2, &dist, &faceIdx) == false)
		{
			// 当たってないなら終了
			return false;
		}

		// 長さ判定
		if (dist > m_RayLen)return false;

		// 自分に結果を記憶する
		if (resData1){
			// 結果データ追加
			resData1->m_youHitObj = obj;
			resData1->m_vHitPos = m_vPos1 + m_vDir*dist;
			resData1->m_RayDist = dist;
			resData1->m_FaceIdx = faceIdx;
		}
		// 相手に結果を記憶する
		if (resData2){
			// 結果データ追加
			resData2->m_youHitObj = this;
			resData2->m_vHitPos = m_vPos1 + m_vDir*dist;
		}

		return true;

	}
	return false;
}

// デバッグ描画
void HitObj_Ray::DebugDraw(float alpha)
{
	YsVec4 mulCol(1, 1, 1, alpha);

	// ライン描画
	ShMgr.m_Samp.DrawLine(m_vPos1, m_vPos2, &(m_Debug_Color*mulCol));

	// 当たった場所に円を描画
	for (auto& res : m_HitResTbl){
		YsMatrix m;
		m.SetPos(m_vPos1 + m_vDir*res.m_RayDist);
		m.SetLookAt(m_vDir, YsVec3(0, 1, 0));
		ShMgr.m_Samp.DrawCircleLine(0.1f, &YsVec4(1, 1, 0, alpha), &m);
	}
}

// メッシュ vs その他
bool HitObj_Mesh::HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2)
{

	// vs 球
	if (obj->m_Shape == HitShapes::SPHERE){
		// 球側の関数を呼ぶ
		return obj->HitTest(this, resData2, resData1);
	}
	// vs Ray
	else if (obj->m_Shape == HitShapes::RAY){
		// レイ側の関数を呼ぶ
		return obj->HitTest(this, resData2, resData1);
	}
	// vs メッシュ
	else if (obj->m_Shape == HitShapes::MESH){
		// メッシュ同士の判定はしない
		return false;
	}
	return false;

}

void HitObj_Mesh::DebugDraw(float alpha)
{
	YsVec4 mulCol(1, 1, 1, alpha);

	// ワイヤーフレームでメッシュを描画
	YsRasterizeState save;
	save.SetAll_GetState();

	YsRasterizeState rs;
	rs.SetAll_Standard();
	rs.Set_FillMode_Wireframe();
	rs.SetState();

	ShMgr.m_Samp.SetLightEnable(false);
	ShMgr.m_Samp.DrawMeshOnly(*m_pMesh, &(m_Debug_Color*mulCol), &m_Mat);
	ShMgr.m_Samp.SetLightEnable(true);

	save.SetState();
}


//======================================================
//
// CollisionEngine
//
//======================================================

void CollisionEngine::Test(BaseHitObj* atkObj)
{
	// atkObj vs 全DefObj

	// 判定結果データをクリアする
	atkObj->m_HitResTbl.clear();

	HitResData res;
	BaseHitObj* defObj;

	//-----------------------------------------------
	// 全DefListと判定実行
	//-----------------------------------------------
	for (UINT di = 0; di<m_DefList.size(); di++){
		defObj = m_DefList[di].get();

		// 汎用事前チェック関数
		if (atkObj->m_BroadPhaseProc){
			if (atkObj->m_BroadPhaseProc(defObj) == false)continue;
		}

		// 自分自身は除外
		if (atkObj == defObj)continue;

		// 判定して良い形状？
		if (!(atkObj->m_ShapeFilter & defObj->m_Shape)){
			continue;
		}

		// 同じ持ち主の物は除外
		// lock()は遅いから使わない。　試しに3000x3000個判定したとき、[lock():300ms] vs [expired() + _Get():7ms]とダンチ
		if (WPtr_Get(atkObj->m_wpTask) == WPtr_Get(defObj->m_wpTask)){
			continue;
		}
		// lock()バージョンは遅いから、ここでは使わないように
		//		if(atkObj->m_wpTask.lock() == defObj->m_wpTask.lock())continue;

		// 自分[攻グループ] vs 相手[防グループ]
		if (!(atkObj->m_AtkFilter & defObj->m_DefFilter))continue;

		// 上記のチェックをすべてクリアしたもの同士だけ、判定実行
		if (atkObj->HitTest(defObj, &res, nullptr)){
			// 結果を登録
			atkObj->m_HitResTbl.push_back(res);
		}
	}

	//-----------------------------------------------
	// ヒット状態フラグ操作
	//-----------------------------------------------

	// 初回HITフラグとHIT終了フラグをOFF
	atkObj->m_HitState &= ~HitStates::ENTER;	// OFF
	atkObj->m_HitState &= ~HitStates::EXIT;		// OFF


	// 初回HITならHITフラグ系をON
	if (atkObj->m_HitResTbl.size() > 0){
		if (!(atkObj->m_HitState & HitStates::STAY)){	// 前回はHIT中ではない
			atkObj->m_HitState |= HitStates::ENTER;		// 初回HIT ON
			atkObj->m_HitState |= HitStates::STAY;		// HIT中 ON
		}
	}
	// 何もHITしていないとき
	else{
		// HIT中ならHIT中フラグOFFして、HIT終了フラグON
		if (atkObj->m_HitState & HitStates::STAY){
			atkObj->m_HitState |= HitStates::EXIT;	// ON
			atkObj->m_HitState &= ~HitStates::STAY;	// OFF
		}
	}
}

void CollisionEngine::Run()
{

	/*
	//=================================
	// マルチスレッドで並列判定実行
	//=================================
	std::atomic<int> nowIdx = 0;	// アトミック変数
	auto testThread = [this, &nowIdx]() {
	while(1){
	int idx = nowIdx.fetch_add(1);		// nowIdxを加算し、加算「前」の値を返す
	if(idx >= m_AtkList.size())break;

	// atkObj vs 全m_DefList
	Test(m_AtkList[idx].get());
	}
	};

	// CPU数 取得
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	std::vector<std::future<void>> fut(sys.dwNumberOfProcessors);

	// スレッド起動
	for(DWORD i = 0;i<sys.dwNumberOfProcessors;i++){
	fut[i] = std::async(std::launch::async | std::launch::deferred, testThread);
	}
	// スレッド終了待機
	for(DWORD i = 0; i<sys.dwNumberOfProcessors; i++){
	fut[i].wait();
	}
	*/

	//=================================
	// シングルスレッドでの判定
	//=================================
	// あたり判定実行
	// 全ての攻タイプが防タイプに対して判定を行う
	for (UINT ai = 0; ai<m_AtkList.size(); ai++){	// 判定する側

		// atkObj vs 全m_DefList
		Test(m_AtkList[ai].get());

	}

	// ヒットしたものは、通知関数を実行する
	for (UINT ai = 0; ai<m_AtkList.size(); ai++){
		BaseHitObj* atkObj = m_AtkList[ai].get();

		// １つでもヒットしていれば通知関数を実行する
		if (atkObj->m_HitResTbl.size() > 0){

			// ヒットした最初の一回目のみ
			if (atkObj->m_HitState & HitStates::ENTER && atkObj->m_OnHitEnter){
				atkObj->m_OnHitEnter(m_AtkList[ai]);
			}
			// ヒットしてる間ずっと
			if (atkObj->m_HitState & HitStates::STAY && atkObj->m_OnHitStay){
				atkObj->m_OnHitStay(m_AtkList[ai]);
			}
		}

		// ヒットしなくなった最初の一回目のみ
		if (atkObj->m_HitState & HitStates::EXIT && atkObj->m_OnHitExit){
			atkObj->m_OnHitExit(m_AtkList[ai]);
		}
	}
}

void CollisionEngine::DebugDraw(float alpha)
{
	// 全Objectをデバッグ描画

	for (UINT ai = 0; ai<m_AtkList.size(); ai++){
		m_AtkList[ai]->DebugDraw(alpha);
	}
	for (UINT di = 0; di<m_DefList.size(); di++){
		m_DefList[di]->DebugDraw(alpha);
	}
}