#pragma once

//ナビゲーションAI
//・キャラクターAIのための経路探索や位置解析を行う

class NavigationAI
{
public:

	//初期化
	void Init();

	/*Navigate------------------------------------------------
	・経路探索をする

	・引数
	_MyPoa				:	自身の座標
	_TargetPos			:	目標の座標

	・戻り値なし
	----------------------------------------------------------*/
	void Navigate(YsVec3 _MyPos, YsVec3 _TargetPos);

	/*CreateWayPoint------------------------------------------------
	・ウェイポイントを作成する
	・マップ専用

	・引数
	_sm			:	対象のメッシュ
	_pos			:	対象の座標

	・戻り値なし
	-------------------------------------------------------------------*/
	void CreateWayPoints(SPtr<YsSingleModel>& _sm, YsVec3& _pos);

	/*UopdateWayPoint----------------------------------------------
	・ウェイポイントの更新を行う
	・新しくオブジェクトを配置したときにこの処理を通す

	・引数
	_sm			:	対象のメッシュ
	_pos			:	対象の座標
	-------------------------------------------------------------------*/
	void UpdateWayPoint(SPtr<YsSingleModel>& _sm, YsVec3& _pos, int _num=0);

	/*DebugWayDraw-----------------------------------------
	・デバッグ用にWayPointを描画する
	-----------------------------------------------------------*/
	void DebugWayDraw();

private:

	//仮の変数
	SPtr<YsGameModel> m_gmWay;
	YsModelObject	m_moWay;
	YsMatrix m_mWay;
	SPtr<YsGameModel> m_gmMap;
	YsModelObject	m_moMap;
	YsMatrix	m_mMap;

	//ウェイポイントリスト
	std::vector<YsVec3> m_WayPointList;

	//------------------------------------------------------------------
	// シングルトン
private:
	NavigationAI(){}
public:
	static NavigationAI& GetInstance()
	{
		static NavigationAI instance;
		return instance;
	}
	//------------------------------------------------------------------
};

#define NAI NavigationAI::GetInstance()