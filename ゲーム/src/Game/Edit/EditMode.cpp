
#include "main.h"

#include "Game\GameWorld\GameWorld.h"

#include "EditMode.h"


EditMode* EditMode::s_pInst = nullptr;

std::vector<SPtr<EditObject>> EditMode::m_eList;

EditMode::E_ID EditMode::eidlist[NUM];

void EditMode::Release(){
	m_eList.clear();
	m_Cam = nullptr;
	m_wpNow.reset();
	delete m_PickUp;
	Edit = false;
}

void EditMode::CheckKill()
{
	std::vector<SPtr<EditObject>>::iterator it = m_eList.begin();
	while (it != m_eList.end())
	{

		if ((*it)->GetKillFlg() == true)
		{
			*it = nullptr;
			it = m_eList.erase(it);
		}
		else{
			it++;
		}
	}
}

//Edit
bool EditMode::Edit = false;

void EditMode::EditInit(){
	Edit = true;

	m_Cam = std::make_shared<GameCamera>();
	m_Cam->Init(20, 180, 0);
	m_PickUp = new PickUp();
	m_wpNow.reset();

}
void EditMode::EditUpdate()
{
	if (INPUT.KeyCheck_Enter(VK_LEFT))
	{

		//m_Cnt--;
		//if (m_Cnt < 0){ m_Cnt = MAX - 1; }
		//m_SelectFlg = ID_List[m_Cnt];



		m_Cnt--;
		if (m_Cnt < 0){ m_Cnt = NUM - 1; }
		m_SelectFlg = EditMode::eidlist[m_Cnt].id;



		m_wpNow.reset();
	}
	if (INPUT.KeyCheck_Enter(VK_RIGHT))
	{

		//m_Cnt++;
		//if (m_Cnt == MAX){ m_Cnt = 0; }
		//m_SelectFlg = ID_List[m_Cnt];


		m_Cnt++;
		if (m_Cnt == NUM){ m_Cnt = 0; }
		m_SelectFlg = EditMode::eidlist[m_Cnt].id;


		m_wpNow.reset();
	}


	//+++++++++++++++++++++++
	// マウス操作
	//+++++++++++++++++++++++

	POINT p;
	GetCursorPos(&p);
	const LONG window_x = 1280;
	const LONG window_y = 720;
	// ウインドウの枠内
	if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y)
	{

		// ブロックの大きさ
		const float block_size = 1.0f;

		// 値を受け取るための変数
		YsVec3 HitPos;
		SPtr<CharacterBase>HitObj;
		bool HeadFlg = false;

		// レイ判定関数
		if (MouceTo3D(*m_Cam, HitPos, HitObj, HeadFlg))
		{
			// ブロックの上にカーソル

			// 配置物判定
			for each(auto made in m_eList)
			{
				if (made->m_wpObj.expired() == false){
					if (made->m_wpObj.lock() == HitObj){
						//作った物の上には置かない
						HeadFlg = true;
					}
				}
			}

			// 頭上チェック　&　作った物でない
			if (HeadFlg == false){	//頭上になにもない

				HitPos.y += block_size;

				float x = cell(HitPos.x, block_size);
				float y = cell(HitPos.y, block_size);
				float z = cell(HitPos.z, block_size);

				YsMatrix m;
				m.CreateMove(x, y, z);

				if (EditObject::NowSetting == false){


					//専用リスト
					SPtr<EditObject>obj(new EditObject(m_SelectFlg, m_Cam,m_Cnt));
					obj->Init(m);
					Push(obj);
					//現在の編集キャラ
					m_wpNow = obj;

				}

				//現在の編集キャラにビルボードの位置を知らせる
				if (m_wpNow.expired() == false){
					m_wpNow.lock()->m_PickUp->SetPos(m);
				}
				else
				{
					//ビルボード
					m_PickUp->SetPos(m);
				}
			}
			else{
				//現在の編集キャラに消えてもらう
				if (m_wpNow.expired() == false){
					if (m_wpNow.lock()->m_wpObj.expired() == true){
						m_wpNow.lock()->SetKill();
						m_wpNow.reset();
						//m_PickUp->SetPos(m);
					}
				}
			}
		}
		else
		{

			//現在の編集キャラに消えてもらう
			if (m_wpNow.expired() == false){
				if (m_wpNow.lock()->m_wpObj.expired() == true){
					m_wpNow.lock()->SetKill();
					m_wpNow.reset();
					//m_PickUp->SetPos(m);
				}
			}

		}

	}


	if (INPUT.MouseCheck_Enter(CInput::MOUSEBUTTON::MOUSE_R))
	{
		POINT p;
		GetCursorPos(&p);
		const LONG window_x = 1280;
		const LONG window_y = 720;

		if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y){

			// 作ったオブジェクト
			// ブロックの上にカーソル

			YsVec3 HitPos;
			SPtr<CharacterBase> HitObj;
			bool HeadFlg = false;
			if (MouceTo3D(*m_Cam, HitPos, HitObj, HeadFlg)){

				//作ったオブジェクトなら消せる
				for each(auto made in m_eList){
					if (made->m_wpObj.expired() == false){
						if (made->m_wpObj.lock() == HitObj){
							HitObj->SetKillFlg();
							made->SetKill();
						}
					}
				}
			}

		}
	}



	// 設置物更新
	for each(auto obj in m_eList){
		obj->Update();
	}
	// クリア処理
	CheckKill();
}
void EditMode::EditDraw()
{
	// 設置物描画
	for each(auto obj in m_eList)
	{
		obj->Draw();
	}

	// 何も選択してないときのビルボード
	if (m_wpNow.expired() == true){
		m_PickUp->Draw();
	}
}

//開発者向けEdit

void EditMode::MapInit(){
	Edit = false;

	m_Cam = std::make_shared<GameCamera>();
	m_Cam->Init(20, 180, 0);
	m_PickUp = new PickUp();
}
void EditMode::MapUpdate()
{
	if (INPUT.KeyCheck_Enter(VK_LEFT))
	{

		//m_Cnt--;
		//if (m_Cnt < 0){ m_Cnt = MAX - 1; }
		//m_SelectFlg = ID_List[m_Cnt];


		m_Cnt--;
		if (m_Cnt < 0){ m_Cnt = NUM - 1; }
		m_SelectFlg = EditMode::eidlist[m_Cnt].id;

		m_wpNow.reset();
	}
	if (INPUT.KeyCheck_Enter(VK_RIGHT))
	{

		//m_Cnt++;
		//if (m_Cnt == MAX){ m_Cnt = 0; }
		//m_SelectFlg = ID_List[m_Cnt];


		m_Cnt++;
		if (m_Cnt == NUM){ m_Cnt = 0; }
		m_SelectFlg = EditMode::eidlist[m_Cnt].id;

		m_wpNow.reset();
	}

	//+++++++++++++++++++++++
	// マウス操作
	//+++++++++++++++++++++++

	POINT p;
	GetCursorPos(&p);
	const LONG window_x = 1280;
	const LONG window_y = 720;
	// ウインドウの枠内
	if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y)
	{

		// ブロックの大きさ
		const float block_size = 1.0f;

		// 値を受け取るための変数
		YsVec3 HitPos;
		SPtr<CharacterBase>HitObj;
		bool HeadFlg = false;

		// レイ判定関数
		if (MouceTo3D(*m_Cam, HitPos, HitObj, HeadFlg))
		{

			// ブロックの上にカーソル

			if (HeadFlg == false){	//頭上になにもない

				HitPos.y += block_size;
				float x = cell(HitPos.x, block_size);
				float y = cell(HitPos.y, block_size);
				float z = cell(HitPos.z, block_size);

				YsMatrix m;
				m.CreateMove(x, y, z);

				if (EditObject::NowSetting == false){


					//専用リスト
					SPtr<EditObject>obj(new EditObject(m_SelectFlg, m_Cam,m_Cnt));
					obj->Init(m);
					Push(obj);
					//現在の編集キャラ
					m_wpNow = obj;


				}

				//現在の編集キャラにビルボードの位置を知らせる
				if (m_wpNow.expired() == false){
					m_wpNow.lock()->m_PickUp->SetPos(m);
				}
				else{
					//ビルボード
					m_PickUp->SetPos(m);
				}
			}

		}
		else
		{

			// なにも無い所にカーソル

			D3DXVECTOR3 MousePoint = Calc2(*m_Cam);
			float x = cell(MousePoint.x, block_size);
			float z = cell(MousePoint.z, block_size);

			// XZ平面,Yは0の高さ
			YsMatrix m;
			m.CreateMove(x, 0, z);


			if (EditObject::NowSetting == false){

				//専用リスト
				SPtr<EditObject>obj(new EditObject(m_SelectFlg, m_Cam,m_Cnt));
				obj->Init(m);
				Push(obj);
				//現在の編集キャラ
				m_wpNow = obj;

			}
			//現在の編集キャラにビルボードの位置を知らせる
			if (m_wpNow.expired() == false){
				m_wpNow.lock()->m_PickUp->SetPos(m);
			}
			else{
				//ビルボード
				m_PickUp->SetPos(m);
			}
		}

	}


	if (INPUT.MouseCheck_Enter(CInput::MOUSEBUTTON::MOUSE_R))
	{
		POINT p;
		GetCursorPos(&p);
		const LONG window_x = 1280;
		const LONG window_y = 720;

		if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y){

			// ブロックの上にカーソル

			YsVec3 HitPos;
			SPtr<CharacterBase> HitObj;
			bool HeadFlg = false;
			if (MouceTo3D(*m_Cam, HitPos, HitObj, HeadFlg)){
				if (HitObj){
					// 頭上判定なし
					HitObj->SetKillFlg();
				}
			}

		}
	}



	// 設置物更新
	for each(auto obj in m_eList){
		obj->Update();
	}

	CheckKill();
}
void EditMode::MapDraw()
{
	// 設置物描画
	for each(auto obj in m_eList)
	{
		obj->Draw();
	}

	// 何も選択してないときのビルボード
	if (m_wpNow.expired() == true){
		m_PickUp->Draw();
	}
}


//大きめの箱
void LBox::Init(YsMatrix &_m)
{
	m_hitBump = std::make_shared<HitObj_Mesh>();
	// 詳細設定
	m_hitBump->m_Mass = 0;

	m_pSm = APP.m_ResStg.LoadMesh("data/original/largebox.x");
	m_Mo.SetModel(m_pSm);

	m_Mat = _m;
}
void LBox::Update()
{
	auto& models = m_Mo.GetGameModel()->GetModelTbl_Static();
	YsSingleModel* mesh = models[0].get();

	m_hitBump->Set(YsToSPtr(this), mesh->GetMesh(), m_Mat);
	GetStage()->m_ColEng.AddDef(m_hitBump); // 判定される側


	m_Mo.CalcBoneMatrix(&m_Mat);
}
void LBox::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}



// スクリーン座標をワールド座標に変換
// 1: D3DXVECTOR3 * pout
// 2: スクリーンX座標
// 3: スクリーンY座標
// 4: 射影空間でのZ値（0～1）
D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy,
	float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj) {
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return pout;
}

// XZ平面とスクリーン座標の交点算出関数
// (地面 = Y = 0 の高さのみ)
D3DXVECTOR3* CalcScreenToXZ(D3DXVECTOR3* pout, float Sx, float Sy,
	int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	if (ray.y <= 0) {
		// 床交点
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray)*ray;
	}
	else {
		*pout = farpos;
	}

	return pout;
}


// スクリーン座標をワールド座標に変換 ver2
// 1: GameCamera
D3DXVECTOR3 Calc2(GameCamera _Camera){

	POINT p;
	YsVec3 Pt;
	GetCursorPos(&p);
	Pt.x = (float)p.x;
	Pt.y = (float)p.y;

	//クライアントの大きさ取得可能
	RECT Lect;
	GetWindowRect(APP.m_hWnd, &Lect);
	GetClientRect(APP.m_hWnd, &Lect);


	Pt.x = Pt.x * ((float)1280 / (float)1280);
	Pt.y = Pt.y * ((float)(720 - 30) / (float)720);//ウインドウの枠の分





	D3DXVECTOR3 GroundPoint;
	CalcScreenToXZ(&GroundPoint, Pt.x, Pt.y, 1280, 720, &_Camera.mView, &_Camera.mProj);

	return GroundPoint;
}

// マップ座標(0,0,0)を中央にセル状に。中心点を返す。
// 1: XorYorZの座標
// 2: セル一つのサイズ
float cell(float _f, float _size)
{
	float f = _f;
	if (f >= 0){
		f = _f + _size / 2.0f;
	}
	else{
		f = _f - _size / 2.0f;
	}
	int i = (int)(f / _size);
	float ret = (float)i*_size;
	return ret;
}

// レイ判定とオブジェクトの位置などを取得
// 戻り値: カーソルの指す位置にメッシュがある?
bool MouceTo3D(GameCamera _Camera, YsVec3 &_Out, SPtr<CharacterBase> &_HitObj, bool &_HeadFlg)
{
	// ローカル変数

	std::vector<float> Dist_List;			//ヒットした距離
	std::vector<SPtr<CharacterBase>> Hit_List;	//ヒットしたオブジェクト
	POINT pt;								//マウス座標
	bool Hit = false;						//メッシュとの判定

	//+++++++++++++++++++

	GetCursorPos(&pt);						//カーソル位置の絶対座標を取得
	ScreenToClient(APP.m_hWnd, &pt);		//カーソル絶対座標から、そのウィンドウ内の座標に補正
	pt.x = pt.x * ((LONG)1280 / (LONG)1280);//現在のサイズで補正（枠サイズ等は非考慮）
	pt.y = pt.y * ((LONG)720 / (LONG)720);


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	//　メッシュループ
	//
	for each(auto obj in CharacterBase::GetList())
	{
		D3DXMATRIX mWorldView;
		D3DXMATRIX m;
		D3DXVECTOR3 vRayDir;
		D3DXVECTOR3 vStartPoint;
		D3DXVECTOR3 v;

		//マウス座標をスクリーンの中心が原点になるように補正（正規化）
		v.x = (((2.0f*(float)pt.x) / (float)1280) - 1.0f) / _Camera.mProj._11;
		v.y = -(((2.0f*(float)pt.y) / (float)720) - 1.0f) / _Camera.mProj._22;
		v.z = 1.0f;

		mWorldView = obj->GetMatrix() * _Camera.mView;
		D3DXMatrixInverse(&m, NULL, &mWorldView);

		// vRayDirはレイを飛ばす方向ベクトル
		vRayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
		vRayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
		vRayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;

		// vStartPointはスクリーン上の座標を3Dへ変換したもの
		vStartPoint.x = m._41;
		vStartPoint.y = m._42;
		vStartPoint.z = m._43;

		//レイ交差判定
		BOOL bHit;
		float fDist;
		for (UINT i = 0; i < obj->GetModelObject().GetGameModel()->GetModelTbl().size(); i++){
			auto & models = obj->GetModelObject().GetGameModel()->GetModelTbl();
			YsSingleModel* mesh = models[i].get();
			bHit = YsCollision::RayToMesh(*mesh->GetMesh().get(), NULL, vStartPoint, (vStartPoint + vRayDir * 100), &fDist, NULL);

			////当たった位置?
			//YsVec3 Point;
			//if (bHit){
			//	Point = vRayDir*fDist + vStartPoint;
			//}

			if (bHit)			// メッシュを見つけた。
			{
				//ヒットフラグ
				Hit = true;
				//判定リストに登録
				Dist_List.push_back(fDist);
				Hit_List.push_back(obj);
			}
		}
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	// 一番近い物の表示位置を取る
	// ...形状は今後...
	//

	float min;
	int No, MinNo;
	No = MinNo = 0;
	//まずリストの最初の距離で初期化
	for each(float dis in Dist_List){
		min = dis;
		break;
	}
	//一番近い距離を判定
	for each(float dis in Dist_List){
		if (min > dis){
			min = dis;
			MinNo = No;
		}
		No++;
	}

	//一番近い距離をもつオブジェクトまで、判定物リストをたどる
	//	それを判定したオブジェクトにする
	No = 0;
	for each(auto hit_obj in Hit_List)
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//
		if (MinNo == No)	//リストをたどれた。
		{
			_Out = hit_obj->GetMatrix().GetPos();	//参照の引数に値渡し
			_HitObj = hit_obj;						//参照の引数に値渡し

			//頭上チェック
			bool buf = false;
			_HeadFlg = false;
			for each(auto head in CharacterBase::GetList()){
				if (_HitObj == head)continue;
				for (UINT i = 0; i < head->GetModelObject().GetGameModel()->GetModelTbl().size(); i++){
					auto & models = head->GetModelObject().GetGameModel()->GetModelTbl();
					YsSingleModel* mesh = models[i].get();
					const float max_h = 1.5f;			//頭上：+1まで
					buf = YsCollision::RayToMesh(*mesh->GetMesh().get(), &head->GetMatrix(),
						YsVec3(_Out.x, _Out.y + 0.5f, _Out.z), YsVec3(_Out.x, _Out.y + max_h, _Out.z), NULL, NULL);
					if (buf){ _HeadFlg = true; break; }	//頭上になにかある。
				}
			}
		}
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
		No++;	//リストのカウントアップ
	}

	return Hit;
}



// Edit専用のオブジェクト
bool EditObject::NowSetting = false;

void EditObject::Init(YsMatrix &_m)
{
	m_Mat = _m;

	//if (m_ID == CharaID::BOX){
	//	m_spMo = YsDx.GetResStg()->LoadMesh("data/original/stand_box.x");
	//	SPtr<EditVec2> center(new EditVec2(0, 0));
	//	center->Center = true;
	//	m_Shape.push_back(center);
	//}
	//else if (m_ID == CharaID::LBOX){
	//	m_spMo = YsDx.GetResStg()->LoadMesh("data/original/largebox.x");

	//	SPtr<EditVec2> center(new EditVec2(0, 0));
	//	center->Center = true;
	//	m_Shape.push_back(center);
	//	SPtr<EditVec2> top(new EditVec2(0, 1.0f));
	//	m_Shape.push_back(top);
	//	SPtr<EditVec2> bot(new EditVec2(0, -1.0f));
	//	m_Shape.push_back(bot);
	//	SPtr<EditVec2> left(new EditVec2(-1.0f, 0));
	//	m_Shape.push_back(left);
	//	SPtr<EditVec2> right(new EditVec2(1.0f, 0));
	//	m_Shape.push_back(right);
	//}else
	//if (m_ID == CharaID::SPRING){
	//	m_spMo = YsDx.GetResStg()->LoadMesh("data/original/Spring/Spring.xed");
	//	SPtr<EditVec2> center(new EditVec2(0, 0));
	//	center->Center = true;
	//	m_Shape.push_back(center);
	//}
	//else{
	//	m_spMo = YsDx.GetResStg()->LoadMesh("data/original/stand_box.x.x");
	//	SPtr<EditVec2> center(new EditVec2(0, 0));
	//	center->Center = true;
	//	m_Shape.push_back(center);
	//}


	m_spMo = YsDx.GetResStg()->LoadMesh(EditMode::eidlist[m_Num].path);
	if (EditMode::eidlist[m_Num].shpe == EditMode::E_ID::BOX1){
		SPtr<EditVec2> center(new EditVec2(0, 0));
		center->Center = true;
		m_Shape.push_back(center);
	}
	else if (EditMode::eidlist[m_Num].shpe == EditMode::E_ID::BOX5){
		SPtr<EditVec2> center(new EditVec2(0, 0));
		center->Center = true;
		m_Shape.push_back(center);
		SPtr<EditVec2> top(new EditVec2(0, 1.0f));
		m_Shape.push_back(top);
		SPtr<EditVec2> bot(new EditVec2(0, -1.0f));
		m_Shape.push_back(bot);
		SPtr<EditVec2> left(new EditVec2(-1.0f, 0));
		m_Shape.push_back(left);
		SPtr<EditVec2> right(new EditVec2(1.0f, 0));
		m_Shape.push_back(right);
	}





	m_Mo.SetModel(m_spMo);

	m_State = std::make_shared<Edit_Set>(m_Mat, m_wpNowCam);


	m_wpObj.reset();
}
void EditObject::Update()
{
	m_State->Update(*this);
	m_Mat = m_State->r_Mat;
	m_Mo.CalcBoneMatrix(&m_Mat);


}
void EditObject::Draw()
{
	//ステートで描画
	m_State->Draw(*this);
}

void Edit_Set::Update(EditObject & E_Obj)
{
	if (INPUT.KeyCheck_Enter(VK_LEFT) || INPUT.KeyCheck_Enter(VK_RIGHT)){
		E_Obj.m_KillFlg = true;
		return;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++

	//設置ステートで実行

	GameCamera *m_Cam = m_wpCamera.lock().get();

	m_onBlock = false;
	m_onY = false;

	POINT p;
	GetCursorPos(&p);
	const LONG window_x = 1280;
	const LONG window_y = 720;
	if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y){


		//１ブロックのサイズ
		const float block_size = 1.0f;


		// 判定したオブジェクトを受け取る変数

		YsVec3 HitPos;
		SPtr<CharacterBase>HitObj;
		bool HeadFlg = false;


		for each(auto shape in E_Obj.m_Shape){

			//++++++++++++++++++++++++++++++++
			//ダミーカメラ作成
			//++++++++++++++++++++++++++++++++
			GameCamera dumy = *m_Cam;	//実体をコピー
			dumy.m_BaseMat.Move_Local(shape->x, 0, shape->z);
			dumy.SetCamera();


			if (MouceTo3D(dumy, HitPos, HitObj, HeadFlg)){

				// ブロックの上にカーソル

				if (EditMode::Edit){
					// 配置物判定
					for each(auto made in EditMode::m_eList)
					{
						if (made->m_wpObj.expired() == false){
							if (made->m_wpObj.lock() == HitObj){
								//作った物の上には置かない
								HeadFlg = true;
							}
						}
					}

				}


				if (HeadFlg == false){	//頭上になにもない

					HitPos.y += block_size;
					float x = cell(HitPos.x, block_size);
					float y = cell(HitPos.y, block_size);
					float z = cell(HitPos.z, block_size);

					// 最終的な配置位置
					YsMatrix m;
					m.CreateMove(x, y, z);


					//設置可能
					m_onBlock = true;
					//中央
					if (shape->Center){
						m_Set = m;
					}
				}
				else
				{
					//設置不可
					m_onBlock = false;
					m_onY = false;

					// EditではOFF　MapEditではON
					if (EditMode::Edit)return;	//リターン

					//break;
				}
			}

			else{

				D3DXVECTOR3 MousePoint = Calc2(dumy);
				float x = cell(MousePoint.x, block_size);
				float z = cell(MousePoint.z, block_size);

				// 最終的な配置位置
				YsMatrix m;
				m.CreateMove(x, 0, z);


				//設置可能
				m_onY = true;
				//中央
				if (shape->Center){
					m_Set = m;
				}


				// なにも無い所にカーソル
				// EditではOFF　MapEditではON
				if (EditMode::Edit)return;	//リターン
			}
		}
	}

	//
	if (m_onBlock == true && m_onY == true)return;
	if (m_onBlock == false && m_onY == false)return;

	if (INPUT.MouseCheck_Enter(CInput::MOUSEBUTTON::MOUSE_L)){
		//位置を決定・回転を操作へ
		YsMatrix m = m_Set;
		E_Obj.m_State = nullptr;
		E_Obj.m_State = std::make_shared<Edit_Rot>(m);
	}
}

void Edit_Set::Draw(EditObject & E_Obj)
{

	if (m_onBlock == true && m_onY == true)return;
	if (m_onBlock == false && m_onY == false)return;
	//if (m_onY && EditMode::Edit)return;

	//半透明描画とか

	//ShMgr.m_Samp.DrawModel(E_Obj.m_Mo, &E_Obj.m_Mat);

	for each(auto s in E_Obj.m_Shape){
		YsMatrix copy = E_Obj.m_PickUp->m_Buf;
		copy.Move_Local(s->x, 0, s->z);
		copy.Move_Local(0, 0.01f, 0);
		copy.RotateX_Local(90.0f);

		E_Obj.m_PickUp->Draw(copy);
	}
}

void Edit_Rot::Update(EditObject & E_Obj)
{
	if (INPUT.KeyCheck_Enter(VK_LEFT) || INPUT.KeyCheck_Enter(VK_RIGHT)){
		E_Obj.m_KillFlg = true;
		return;
	}


	//セット終わり
	OneFrame++;
	if (INPUT.MouseCheck_Enter(CInput::MOUSEBUTTON::MOUSE_L) && m_PauseFlg == 0 && OneFrame >= 1)
	{
		//(回転終了時に) 生成する

		//箱
		if (E_Obj.m_ID == CharaID::BOX){
			SPtr<Box>box(new Box);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}
		//十字
		else if (E_Obj.m_ID == CharaID::LBOX)
		{
			SPtr<LBox>box(new LBox);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}
		//スプリング
		else if (E_Obj.m_ID == CharaID::SPRING){
			SPtr<Spring>box(new Spring);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}

		//ここに生成を追加していく


		else if (E_Obj.m_ID == CharaID::STAIRS){
			SPtr<Stairs>box(new Stairs);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}










		else{
			SPtr<Box>box(new Box);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}

		YsMatrix m = r_Mat;//保存
		E_Obj.m_State = nullptr;
		E_Obj.m_State = std::make_shared<Edit_Put>(m);
		return;
	}


	//+++++++++++++++++++++++++++++
	//行列を受け取って操作する
	//+++++++++++++++++++++++++++++

	r_Mat = E_Obj.m_Mat;

	//回転などをやる
	if (INPUT.GetMouseWheel() > 0 && m_PauseFlg == 0){
		m_PauseFlg = UP;
	}
	if (INPUT.GetMouseWheel() < 0 && m_PauseFlg == 0){
		m_PauseFlg = DOWN;
	}
	switch (m_PauseFlg){
	case UP:
		r_Mat.RotateY_Local(4.5f);
		m_PauseCnt++;
		break;
	case DOWN:
		r_Mat.RotateY_Local(-4.5f);
		m_PauseCnt++;
		break;
	default:
		break;
	}
	if (m_PauseCnt == 20){
		m_PauseCnt = 0;
		m_PauseFlg = 0;
	}
}

void Edit_Rot::Draw(EditObject & E_Obj)
{
	ShMgr.m_Samp.DrawModel(E_Obj.m_Mo, &E_Obj.m_Mat);

	for each(auto s in E_Obj.m_Shape){
		YsMatrix copy = E_Obj.m_PickUp->m_Buf;
		copy.Move_Local(s->x, 0, s->z);
		copy.Move_Local(0, 0.01f, 0);
		copy.RotateX_Local(90.0f);

		E_Obj.m_PickUp->Draw(copy);
	}
}

void Edit_Put::Update(EditObject & E_Obj)
{
	r_Mat = E_Obj.m_Mat;

	//ワールド側のオブジェクトが消失
	if (E_Obj.m_wpObj.expired() == true){
		//自分(Edit側の)のキルフラグ
		E_Obj.m_KillFlg = true;
	}
}

void Edit_Put::Draw(EditObject & E_Obj)
{
	//指定時間エフェクトを出す

	//その後上空に目印とかを描画する
}