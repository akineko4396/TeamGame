#include "YskLib.h"

using namespace YskLib;

// 作業用ポリゴンデータ
WPtr<YsPolygon>	YsLaser::s_wpPoly;

void YsLaser::Draw(const YsVec3& vCamPos)
{
	if(m_List.size() <= 1)return;

	// 作業用ポリゴンデータ
	if(m_pPoly == nullptr){
		// ポリゴンデータが存在しないなら作成する
		if(s_wpPoly.expired()) {
			m_pPoly = std::make_shared<YsPolygon>();
			m_pPoly->Create(nullptr, 4, YsVertex_Pos_UV_Color::GetVertexTypeData());

			s_wpPoly = m_pPoly;
		}
		// すでに作成済ならそれを使用
		else{
			m_pPoly = s_wpPoly.lock();
		}
	}

	// サイズが小さい時は拡張
	if(m_pPoly->GetVertexNum() < (int)m_List.size() * 2){
		m_pPoly->Create(nullptr, (int)m_List.size() * 2, YsVertex_Pos_UV_Color::GetVertexTypeData());
	}

	// 頂点バッファのアドレス取得。データ書き込み。
	D3D11_MAPPED_SUBRESOURCE pData;
	if(m_pPoly->Map(&pData) == false)return;
	YsVertex_Pos_UV_Color* pVerTbl = (YsVertex_Pos_UV_Color*)pData.pData;

	YsVec3 Dir,vCrs,vNormal,vCam;
	const float fUV = 1.0f/(m_List.size()-1);

	float w = m_W * 0.5f;
	int Idx = 0;
	int i = 0;
	std::list<YsVec3>::iterator it = m_List.begin();
	while(it != m_List.end()){


		std::list<YsVec3>::iterator itNext = it;
		++itNext;

		YsVec3& vP = (*it);
		if(itNext != m_List.end()){
			// 次の点へのベクトル
			Dir = (*itNext) - vP;
		}

		vCam = vCamPos - vP;
		vCam.Normalize();		// 正規化
		YsVec3::Cross(vCrs, Dir, vCam);	// 外積
		vCrs.Normalize();		// 正規化

		vCrs.x *= w;
		vCrs.y *= w;
		vCrs.z *= w;

		pVerTbl[Idx].Pos.x = vP.x - vCrs.x;
		pVerTbl[Idx].Pos.y = vP.y - vCrs.y;
		pVerTbl[Idx].Pos.z = vP.z - vCrs.z;
		pVerTbl[Idx + 1].Pos.x = vP.x + vCrs.x;
		pVerTbl[Idx + 1].Pos.y = vP.y + vCrs.y;
		pVerTbl[Idx + 1].Pos.z = vP.z + vCrs.z;
		

		pVerTbl[Idx].UV.x = 0;
		pVerTbl[Idx].UV.y = fUV*i;

		pVerTbl[Idx + 1].UV.x = 1;
		pVerTbl[Idx + 1].UV.y = fUV*i;

		((YsVec4&)pVerTbl[Idx].Color).Set(1, 1, 1, 1);
		((YsVec4&)pVerTbl[Idx + 1].Color).Set(1, 1, 1, 1);

		i++;
		Idx += 2;

		++it;
	}

	m_pPoly->Unmap();

	// 描画に必要なデータをセット
	m_pPoly->SetDrawData();

	// 描画
	m_pPoly->Draw(i * 2);

}

void YsLaser::DrawStrip(int detailNum)
{
	if(m_List.size() <= 4)return;


	// 作業用ポリゴンデータ
	if(m_pPoly == nullptr){
		// ポリゴンデータが存在しないなら作成する
		if(s_wpPoly.expired()) {
			m_pPoly = std::make_shared<YsPolygon>();
			m_pPoly->Create(nullptr, 4, YsVertex_Pos_UV_Color::GetVertexTypeData());

			s_wpPoly = m_pPoly;
		}
		// すでに作成済ならそれを使用
		else{
			m_pPoly = s_wpPoly.lock();
		}
	}

	// サイズが小さい時は拡張
	if(m_pPoly->GetVertexNum() < (int)m_List.size()*(detailNum + 1) * 2){
		m_pPoly->Create(nullptr, (int)m_List.size()*(detailNum + 1) * 2, YsVertex_Pos_UV_Color::GetVertexTypeData());
	}

	// 頂点バッファのアドレス取得。データ書き込み。
	D3D11_MAPPED_SUBRESOURCE pData;
	if(m_pPoly->Map(&pData) == false)return;
	YsVertex_Pos_UV_Color* pVerTbl = (YsVertex_Pos_UV_Color*)pData.pData;

	const float fUV = 1.0f / (m_List.size() / 2 - 1) / (detailNum + 1);

	std::list<YsVec3>::iterator itPrev[2],itNext[2],itNext2[2];
	int Idx = 0;
	int i = 0;
	std::list<YsVec3>::iterator it = m_List.begin();
	while(it != m_List.end()){

		if(detailNum > 0){
			if(i < 1){
				itPrev[0] = it;
			}
			else{
				itPrev[0] = it;
				--itPrev[0];
				--itPrev[0];
			}

			if(i >= (int)m_List.size()/2-1){
				itNext[0] = it;
				itNext2[0] = it;
			}
			else if(i >= (int)m_List.size()/2-2){
				itNext[0] = it;
				++itNext[0];
				++itNext[0];
				itNext2[0] = itNext[0];
			}
			else{
				itNext[0] = it;
				++itNext[0];
				++itNext[0];
				itNext2[0] = itNext[0];
				++itNext2[0];
				++itNext2[0];
			}

			itPrev[1] = itPrev[0];
			itNext[1] = itNext[0];
			itNext2[1] = itNext2[0];
			++itPrev[1];
			++itNext[1];
			++itNext2[1];
		}

		// 1
		YsVec3& vP = (*it);
		pVerTbl[Idx].Pos.x = vP.x;
		pVerTbl[Idx].Pos.y = vP.y;
		pVerTbl[Idx].Pos.z = vP.z;

		pVerTbl[Idx].UV.x = 0;
		pVerTbl[Idx].UV.y = fUV*(Idx / 2);

		((YsVec4&)pVerTbl[Idx].Color).Set(1, 1, 1, 1);

		Idx++;
		++it;

		// 2
		YsVec3& vP2 = (*it);
		pVerTbl[Idx].Pos.x = vP2.x;
		pVerTbl[Idx].Pos.y = vP2.y;
		pVerTbl[Idx].Pos.z = vP2.z;

		pVerTbl[Idx].UV.x = 1;
		pVerTbl[Idx].UV.y = fUV*(Idx / 2);

		((YsVec4&)pVerTbl[Idx].Color).Set(1, 1, 1, 1);

		Idx++;
		++it;

		// 補間
		for(int n = 0; n<detailNum; n++){
			float f = (1.0f / (detailNum + 1))*(n + 1);

			// 1
			YsVec3 vTmp;
			YsVec3::CatmullRom(vTmp, *itPrev[0], vP, *itNext[0], *itNext2[0], f);

			pVerTbl[Idx].Pos.x	=   vTmp.x;
			pVerTbl[Idx].Pos.y = vTmp.y;
			pVerTbl[Idx].Pos.z = vTmp.z;

			pVerTbl[Idx].UV.x = 0;
			pVerTbl[Idx].UV.y = fUV*(Idx/2);

			((YsVec4&)pVerTbl[Idx].Color).Set(1, 1, 1, 1);

			Idx++;

			// 2
			YsVec3 vTmp2;
			YsVec3::CatmullRom(vTmp2, *itPrev[1], vP2, *itNext[1], *itNext2[1], f);

			pVerTbl[Idx].Pos.x	=   vTmp2.x;
			pVerTbl[Idx].Pos.y = vTmp2.y;
			pVerTbl[Idx].Pos.z = vTmp2.z;

			pVerTbl[Idx].UV.x = 1;
			pVerTbl[Idx].UV.y = fUV*(Idx/2);

			((YsVec4&)pVerTbl[Idx].Color).Set(1, 1, 1, 1);

			Idx++;
		}

		i++;
	}

	m_pPoly->Unmap();

	// 描画に必要なデータをセット
	m_pPoly->SetDrawData();

	// 描画
	m_pPoly->Draw((m_List.size() - 2)*(detailNum + 1));

	/*
	// 頂点シェーダ設定
	lpD3DDev->SetFVF(YSK_BBVERTEX::GetFVF());

	YsDx.PushRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	DWORD tmp[2];
	YsDx.GetDev()->GetSamplerState(0,D3DSAMP_ADDRESSU,&tmp[0]);
	YsDx.GetDev()->GetSamplerState(0,D3DSAMP_ADDRESSV,&tmp[1]);
	YsDx.GetDev()->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	YsDx.GetDev()->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);

	lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (m_List.size()-2)*(DetailNum+1), &workV[0],sizeof(YSK_BBVERTEX));

	YsDx.GetDev()->SetSamplerState(0,D3DSAMP_ADDRESSU,tmp[0]);
	YsDx.GetDev()->SetSamplerState(0,D3DSAMP_ADDRESSV,tmp[1]);

	YsDx.PopRenderState(D3DRS_CULLMODE);
	*/
}
