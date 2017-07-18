#include "YskLib.h"

using namespace YskLib;

WPtr<YsPolygon>	YsBillBoard::s_wpPolySquare;

void YsBillBoard::SetVertex(float x, float y, float w, float h)
{
	//[1]      [3]
	// *--------*
	// |＼　　　|
	// |　＼　　|h
	// |　　＼　|
	// |　　　＼|
	// *--------*
	//(x,y) w  [2]
	//[0]

	// 頂点座標
	m_V[0].Pos.x = x;
	m_V[0].Pos.y = y;
	m_V[0].Pos.z = 0;

	m_V[1].Pos.x = x;
	m_V[1].Pos.y = y + h;
	m_V[1].Pos.z = 0;

	m_V[2].Pos.x = x + w;
	m_V[2].Pos.y = y;
	m_V[2].Pos.z = 0;

	m_V[3].Pos.x = x + w;
	m_V[3].Pos.y = y + h;
	m_V[3].Pos.z = 0;

	DirectX::SimpleMath::Vector3;
}

void YsBillBoard::Draw()
{
	// 作業用ポリゴンバッファ設定
	if(m_pPoly == nullptr){
		// ポリゴンデータが存在しないなら作成する
		if(s_wpPolySquare.expired()) {
			m_pPoly = std::make_shared<YsPolygon>();
			m_pPoly->Create(nullptr, 4, m_V[0].GetVertexTypeData());

			s_wpPolySquare = m_pPoly;
		}
		// すでに作成済ならそれを使用
		else{
			m_pPoly = s_wpPolySquare.lock();
		}
	}

	YsVec2 uvMin, uvMax;
	m_Anime.GetUV(&uvMin, &uvMax);
	// UV変更
	m_V[0].UV.x = uvMin.x;
	m_V[0].UV.y = uvMax.y;
	m_V[1].UV.x = uvMin.x;
	m_V[1].UV.y = uvMin.y;
	m_V[2].UV.x = uvMax.x;
	m_V[2].UV.y = uvMax.y;
	m_V[3].UV.x = uvMax.x;
	m_V[3].UV.y = uvMin.y;

	// 頂点データを頂点バッファに書き込む
	m_pPoly->WriteVertexData(m_V, 4);

	// 描画に必要なデータをセット
	m_pPoly->SetDrawData();

	// 描画
	m_pPoly->Draw(4);
}

