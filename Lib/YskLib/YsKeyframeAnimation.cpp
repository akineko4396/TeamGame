#include "YskLib.h"

using namespace YskLib;


// ベジェ曲線の指定Xの位置からtを求め、それを使いYを求める
static float CalcBezierY_FromX(float x, float x1, float y1, float x2, float y2) {

	// ±eps の範囲内に収まったら抜ける
	static const float eps = 0.001f;

	float down = 0.0f;
	float up = 1.0f;

	// 二分探索で指定値に近づくまで計算
	float t;
	while(true) {
		t = (down + up) / 2;
		// 計算
		float px = 3 * (1 - t)*(1 - t)*t*x1 + 3 * (1 - t)*t*t*x2 + t*t*t;

		// 指定値の範囲外なら次
		if(px < x - eps) {
			down = t;
		}
		// 指定値の範囲外なら次
		else if(px > x + eps) {
			up = t;
		}
		// 範囲内
		else {
			break;
		}
	}

	return 3 * (1 - t)*(1 - t)*t*y1 + 3 * (1 - t)*t*t*y2 + t*t*t;
}



bool YsAnimeKey_Quaternion::InterpolationFromTime(const std::vector<YsAnimeKey_Quaternion*>& keyList, double time, YsQuat &outQ, UINT* nowKeyIndex)
{
	if(keyList.size() == 1) {		// キーが１つのとき
		// その内容を使用
		outQ = keyList.front()->Value;
		return true;
	}
	else if(keyList.size() >= 2) {			// キーが複数

		// 最初のキーより手前なら、先頭を返す
		if(time < keyList[0]->Time) {
			outQ = keyList.front()->Value;
			return true;
		}
		// 最後のキーを過ぎてるとき
		if(time >= keyList.back()->Time) {
			// 最後のデータを返す
			outQ = keyList.back()->Value;
			return true;
		}

		// 位置を検索
		UINT i = 0;
		if(nowKeyIndex == nullptr){
			for(i = 0; i<keyList.size(); i++) {
				// 自分より上のキーを発見
				if(time < keyList[i]->Time) {
					break;
				}
			}
		}
		else{

			// 前回のIndex
			UINT idx = *nowKeyIndex;
			if(idx >= keyList.size())idx = keyList.size() - 1;

			i = idx;
			int searchEndIndex = (int)idx - 1;
			if(searchEndIndex < 0)searchEndIndex = keyList.size() - 2;

			while(1) {
				if((int)i >= (int)keyList.size() - 1)i = 0;		// 最後の1つ前まで行けば、最初に戻す
				if(i == searchEndIndex)break;

				// 範囲内？
				if(time >= keyList[i]->Time && time < keyList[i + 1]->Time) {
					break;
				}

				i++;
			}

			*nowKeyIndex = (unsigned int)i;
			i++;
		}


		// 計算
		if(i < keyList.size()){
			// 比率算出
			float sa = (keyList[i]->Time - keyList[i - 1]->Time);
			double f = time - keyList[i - 1]->Time;

			if(f == 0.0){
				outQ = keyList[i - 1]->Value;
			}
			else{

				// 球面線形補間
				if(keyList[i - 1]->CalcType == 0) {
					YsQuat::Slerp(outQ,
						keyList[i - 1]->Value,
						keyList[i]->Value,
						(float)(f * (1.0f / sa)));
				}
				// 球面２次補間(それなりに重いから、処理を軽くしたい場合は事前に焼き込み線形補間にするのが良い(BaseCurve関数))
				else if(keyList[i - 1]->CalcType == 1) {
					float fT = (float)(f * (1.0f / sa));

					int idx[4];
					idx[0] = i - 2;
					idx[1] = i - 1;
					idx[2] = i;
					idx[3] = i + 1;
					if (idx[0] < 0)idx[0] = 0;
					if (idx[3] >= (int)keyList.size())idx[3] = i;

					// 制御ポイント算出(処理効率を考えるなら、これは事前計算がしてるほうが良い)
					YsQuat qABC[3];
					YsQuat::SquadSetup(
								qABC[0],
								qABC[1],
								qABC[2],
								keyList[idx[0]]->Value,
								keyList[idx[1]]->Value,
								keyList[idx[2]]->Value,
								keyList[idx[3]]->Value
					);


					YsQuat::Squad(outQ,
								keyList[i - 1]->Value,
								qABC[0],
								qABC[1],
								qABC[2],
								fT);
				}
				// ベジェ曲線補間(それなりに重いから、処理を軽くしたい場合は事前に焼き込み線形補間にするのが良い(BaseCurve関数))
				else if (keyList[i - 1]->CalcType == 2) {
					float fT = (float)(f * (1.0f / sa));

					float by = CalcBezierY_FromX(fT,
						keyList[i - 1]->BezierA[0] / 127.0f,
						keyList[i - 1]->BezierA[1] / 127.0f,
						keyList[i - 1]->BezierB[0] / 127.0f,
						keyList[i - 1]->BezierB[1] / 127.0f);

					YsQuat::Slerp(outQ,
						keyList[i - 1]->Value,
						keyList[i]->Value,
						by);

				}
				
			}

			return true;
		}

		return false;
	}
	return false;
}

void YsAnimeKey_Quaternion::BakeCurve(std::vector<YsAnimeKey_Quaternion*>& keyList, int bakeSpace)
{
	// 一度コピー
	auto copyRefRotateList = keyList;
	int keyNum = (int)keyList.size();
	for (int h = 0; h < keyNum; h++) {
		// 補間タイプが曲線系の場合
		if (copyRefRotateList[h]->CalcType != 0) {


			// 最後のキーでなければ、焼き込む
			if (h < (int)copyRefRotateList.size() - 1) {
				// 総数(うち２つが、hとh+1番目のキー)
				int cnt = (int)copyRefRotateList[h + 1]->Time - (int)copyRefRotateList[h]->Time + 1;

				// 球面二次補間では、あらかじめ周辺のキーから制御ポイントを求めるておく
				/*
				YsQuat qABC[3];
				if (copyRefRotateList[h]->CalcType == 1) {
					int idx[4];
					idx[0] = h - 1;
					idx[1] = h;
					idx[2] = h + 1;
					idx[3] = h + 2;
					if (idx[0] < 0)idx[0] = 0;
					if (idx[2] >= (int)copyRefRotateList.size())idx[2] = (int)copyRefRotateList.size() - 1;
					if (idx[3] >= (int)copyRefRotateList.size())idx[3] = (int)copyRefRotateList.size() - 1;

					// 球面二次補間の制御ポイントを算出
					YsQuat::SquadSetup(
						qABC[0],
						qABC[1],
						qABC[2],
						copyRefRotateList[idx[0]]->Value,
						copyRefRotateList[idx[1]]->Value,
						copyRefRotateList[idx[2]]->Value,
						copyRefRotateList[idx[3]]->Value
					);
				}
				*/

				int bakeInterval = bakeSpace;
				// 最初(h)と後部(h+1)キー以外
				for (int bki = 1; bki<cnt - 1; bki++) {
					if (bakeInterval <= 0) {
						bakeInterval = bakeSpace;

						int time = (int)copyRefRotateList[h]->Time + bki;
						float sa = (copyRefRotateList[h + 1]->Time - copyRefRotateList[h]->Time);
						double f = time - copyRefRotateList[h]->Time;
						float fT = (float)(f * (1.0f / sa));

						YsQuat outQ;
						// 補間計算
						UINT idx = h;
						YsAnimeKey_Quaternion::InterpolationFromTime(copyRefRotateList, time, outQ, &idx);

						// 結果をキーとして追加
						YsAnimeKey_Quaternion* add2 = new YsAnimeKey_Quaternion();
						keyList.push_back(add2);
						add2->Time = (float)time;
						add2->Value = outQ;
						add2->CalcType = 0;
					}
					else {
						bakeInterval--;
					}
				}

			}

			// このキーは曲線をやめて線形に戻す
			copyRefRotateList[h]->CalcType = 0;

		}

	}

	// ソート
	std::sort(keyList.begin(), keyList.end(),
		[](const YsAnimeKey_Quaternion* v1, const YsAnimeKey_Quaternion* v2) {
			return v1->Time < v2->Time;
		}
	);
}

bool YsAnimeKey_Vector3::InterpolationFromTime(const std::vector<YsAnimeKey_Vector3*>& keyList, double time, YsVec3 &outV, UINT* nowKeyIndex)
{
	if(keyList.size() == 1) {		// キーが１つのとき
		// その内容を使用
		outV.Set( keyList.front()->Value );
		return true;
	}
	else if(keyList.size() >= 2) {			// キーが複数
		// 最初のキーより手前なら、先頭を返す
		if (time < keyList[0]->Time) {
			outV.Set(keyList.front()->Value);
			return true;
		}
		// 最後のキーを過ぎてるとき
		if(time >= keyList.back()->Time) {
			// 最後のデータを返す
			outV.Set(keyList.back()->Value);
			return true;
		}



		// 位置を検索
		UINT i = 0;
		if(nowKeyIndex == nullptr){
			for(i = 0; i<keyList.size(); i++) {
				// 自分より上のキーを発見
				if(time < keyList[i]->Time) {
					break;
				}
			}
		}
		else{

			// 前回のIndex
			UINT idx = *nowKeyIndex;
			if(idx >= keyList.size())idx = keyList.size() - 1;

			i = idx;
			int searchEndIndex = (int)idx - 1;
			if(searchEndIndex < 0)searchEndIndex = keyList.size() - 2;

			while(1) {
				if((int)i >= (int)keyList.size() - 1)i = 0;		// 最後の1つ前まで行けば、最初に戻す
				if(i == searchEndIndex)break;

				// 範囲内？
				if(time >= keyList[i]->Time && time < keyList[i + 1]->Time) {
					break;
				}

				i++;
			}

			*nowKeyIndex = (unsigned int)i;
			i++;
		}

		if(i < keyList.size()){
			// 比率算出
			float sa = (keyList[i]->Time - keyList[i - 1]->Time);
			double f = time - keyList[i - 1]->Time;
			if(f == 0){
				outV.Set( keyList[i - 1]->Value );
			}
			else{
				// 線形補間
				if(keyList[i - 1]->CalcType == 0) {
					YsVec3::Lerp(outV,
						keyList[i - 1]->Value,
						keyList[i]->Value,
						(float)(f * (1.0f / sa)));
				}
				// Catmull-Romスプライン補間(それなりに重いから、処理を軽くしたい場合は事前に焼き込み線形補間にするのが良い(BaseCurve関数))
				else if(keyList[i - 1]->CalcType == 1) {
					int idx[4];
					idx[0] = i - 2;
					idx[1] = i - 1;
					idx[2] = i;
					idx[3] = i + 1;
					if(idx[0] < 0)idx[0] = 0;
					if(idx[3] >= (int)keyList.size())idx[3] = i;

					float fT = (float)(f * (1.0f / sa));

					YsVec3::CatmullRom(outV,
						keyList[idx[0]]->Value,
						keyList[idx[1]]->Value,
						keyList[idx[2]]->Value,
						keyList[idx[3]]->Value,
						fT
						);
				}
				// ベジェ曲線補間(それなりに重いから、処理を軽くしたい場合は事前に焼き込み線形補間にするのが良い(BaseCurve関数))
				else if (keyList[i - 1]->CalcType == 2) {
					float fT = (float)(f * (1.0f / sa));

					// X
					float by = CalcBezierY_FromX(fT,
						keyList[i - 1]->BezierX_A[0] / 127.0f,
						keyList[i - 1]->BezierX_A[1] / 127.0f,
						keyList[i - 1]->BezierX_B[0] / 127.0f,
						keyList[i - 1]->BezierX_B[1] / 127.0f);

					outV.x = keyList[i - 1]->Value.x + (keyList[i]->Value.x - keyList[i - 1]->Value.x) * by;

					// Y
					by = CalcBezierY_FromX(fT,
						keyList[i - 1]->BezierY_A[0] / 127.0f,
						keyList[i - 1]->BezierY_A[1] / 127.0f,
						keyList[i - 1]->BezierY_B[0] / 127.0f,
						keyList[i - 1]->BezierY_B[1] / 127.0f);

					outV.y = keyList[i - 1]->Value.y + (keyList[i]->Value.y - keyList[i - 1]->Value.y) * by;

					// Z
					by = CalcBezierY_FromX(fT,
						keyList[i - 1]->BezierZ_A[0] / 127.0f,
						keyList[i - 1]->BezierZ_A[1] / 127.0f,
						keyList[i - 1]->BezierZ_B[0] / 127.0f,
						keyList[i - 1]->BezierZ_B[1] / 127.0f);

					outV.z = keyList[i - 1]->Value.z + (keyList[i]->Value.z - keyList[i - 1]->Value.z) * by;
				}
			}
			return true;
		}

		return false;
	}
	return false;
}

void YsAnimeKey_Vector3::BakeCurve(std::vector<YsAnimeKey_Vector3*>& keyList, int bakeSpace)
{
	// 一度コピー
	auto copyRefPosList = keyList;
	int keyNum = (int)keyList.size();
	for(int h = 0; h < keyNum; h++) {
		// 補間タイプが曲線系の場合
		if(copyRefPosList[h]->CalcType != 0) {


			// 最後のキーでなければ、焼き込む
			if(h < (int)copyRefPosList.size() - 1) {
				// 総数(うち２つが、hとh+1番目のキー)
				int cnt = (int)copyRefPosList[h + 1]->Time - (int)copyRefPosList[h]->Time + 1;

				int bakeInterval = bakeSpace;
				// 最初(h)と後部(h+1)キー以外
				for(int bki = 1; bki<cnt - 1; bki++) {
					if(bakeInterval <= 0) {
						bakeInterval = bakeSpace;

						int time = (int)copyRefPosList[h]->Time + bki;
						float sa = (copyRefPosList[h + 1]->Time - copyRefPosList[h]->Time);
						double f = time - copyRefPosList[h]->Time;
						float fT = (float)(f * (1.0f / sa));

						YsVec3 outV;
						// 補間計算
						UINT idx = h;
						YsAnimeKey_Vector3::InterpolationFromTime(copyRefPosList, time, outV, &idx);

						// 結果をキーとして追加
						YsAnimeKey_Vector3* add2 = new YsAnimeKey_Vector3();
						keyList.push_back(add2);
						add2->Time = (float)time;
						add2->Value = outV;
						add2->CalcType = 0;
					}
					else {
						bakeInterval--;
					}
				}

			}
			// このキーは曲線をやめて線形にする
			copyRefPosList[h]->CalcType = 0;

		}

	}

	// ソート
	std::sort(keyList.begin(), keyList.end(),
		[](const YsAnimeKey_Vector3* v1, const YsAnimeKey_Vector3* v2) {
			return v1->Time < v2->Time;
		}
	);

}


//=============================================================================
//
// YsKeyframeAnime
//
//=============================================================================

void YsKeyframeAnime::Release()
{
	for (UINT i = 0; i<m_Rotate.size(); i++) {
		YsSafe_Delete(m_Rotate[i]);
	}
	m_Rotate.clear();
	for (UINT i = 0; i<m_Scale.size(); i++) {
		YsSafe_Delete(m_Scale[i]);
	}
	m_Scale.clear();
	for (UINT i = 0; i<m_Pos.size(); i++) {
		YsSafe_Delete(m_Pos[i]);
	}
	m_Pos.clear();
}

//=============================================================================
//
// YsAnimationSet
//
//=============================================================================
void YsAnimationSet::Release()
{
	m_AnimeName = "";
	m_TicksPerSecond = 60;
	m_AnimeLen = 0;

	for (UINT i = 0; i<m_FrameAnimeList.size(); i++) {
		YsSafe_Delete(m_FrameAnimeList[i]);
	}
	m_FrameAnimeList.clear();

	for (UINT i = 0; i<m_Script.size(); i++) {
		YsSafe_Delete(m_Script[i]);
	}
	m_Script.clear();
}

void YsAnimationSet::CreateFrameAnimeList(UINT size)
{
	for (UINT i = 0; i<m_FrameAnimeList.size(); i++) {
		YsSafe_Delete(m_FrameAnimeList[i]);
	}
	m_FrameAnimeList.resize(size);

	for (UINT i = 0; i<m_FrameAnimeList.size(); i++) {
		m_FrameAnimeList[i] = new YsKeyframeAnime();
	}
}

bool YsAnimationSet::LoadXMD(const std::string& filename, const char* registerAnimeName, bool bakeCurve)
{
	// 
	FILE* fp = fopen(filename.c_str(), "rb");
	if (!fp)return FALSE;
	char strHead[256];
	// 汎用
#pragma region 便利関数
	auto RD = [fp](void* p, int size) {fread(p, size, 1, fp); };
	auto RDchar = [fp](char* c) {fread(c, 1, 1, fp); };
	auto RDbyte = [fp](BYTE* c) {fread(c, 1, 1, fp); };
	auto RDword = [fp](WORD* w) {fread(w, 2, 1, fp); };
	auto RDint = [fp](int* n) {fread(n, 4, 1, fp); };
	auto RDfloat = [fp](float* f) {fread(f, 4, 1, fp); };
	auto RDfloat3 = [fp](YsVec3* v) {fread(v, 12, 1, fp); };
	auto RDdouble = [fp](double* d) {fread(d, 8, 1, fp); };
	// サイズ(int)＋文字列
	auto RDstr = [fp](std::string& str) {
		int n;
		fread(&n, 4, 1, fp);	// size
		str.resize(n);
		//		str[n] = '\0';
		if (n > 0) {
			fread(&str[0], n, 1, fp);
		}
	};
	BYTE lastReadSize = 0;
	auto RHead = [fp, &strHead, &lastReadSize]() mutable {
		while (1) {
			BYTE len;
			while (1) {
				fread(&len, 1, 1, fp);		// 文字数
											// 開始文字
				BYTE st = 0;
				fread(&st, 1, 1, fp);
				// ヘッダ確認　続行
				if (st == 0x8C) {
					break;
				}
				// 不正なヘッダ。巻き戻し、ヘッダまで進める
				else {
					fseek(fp, -1, SEEK_CUR);
				}
			}
			// 文字列
			fread(strHead, len, 1, fp);
			strHead[len] = '\0';
			// 終端文字
			BYTE term[2];
			fread(term, 2, 1, fp);
			if (term[0] == 0x60 && term[1] == 0x99) {	// 終端確認
				break;
			}
			// 終端文字ではない。最初(文字数取得後)まで巻き戻す
			else {
				fseek(fp, -(1 + len + 2), SEEK_CUR);
			}
		}
	};
#pragma endregion

	//===========================================
	// Main Header
	//===========================================
	char head[4];
	RD(head, 3);
	head[3] = '\0';

	if (strcmp(head, "XMD") != 0) {
		fclose(fp);
		return false;
	}

	while (1) {
		// 項目読み込み
		RHead();
		// 終了
		if (strcmp(strHead, "EndXMD") == 0) {
			break;
		}
		//===========================================
		// アニメデータ
		//===========================================
		else if (strcmp(strHead, "AnimeName") == 0) {
#pragma region アニメ
			// アニメ追加
			//			SPtr<XEDAnimation> anime(new XEDAnimation());
			//			m_AnimeList.push_back(anime);
			YsAnimationSet* anime = this;

			// ★アニメ名
			RDstr(anime->m_AnimeName);
			// アニメ名指定がある場合は、差し替え
			if (registerAnimeName) {
				anime->m_AnimeName = registerAnimeName;
			}

			// ★アニメ長
			int animeLen;
			RDint(&animeLen);
			anime->m_TicksPerSecond = 60;
			anime->m_AnimeLen = animeLen;

			while (1) {
				// 項目読み込み
				RHead();
				// 終了
				if (strcmp(strHead, "End") == 0) {
					break;
				}
				else if (strcmp(strHead, "BlendAnimeMode") == 0) {
					int nn;
					RDint(&nn);
					//					anime->m_bBlendAnimeMode = (nn == 1 ? true : false);
				}
				else if (strcmp(strHead, "BlendAnimeWeight") == 0) {
					//					RDfloat(&anime->m_fBlendAnimeWeight);
				}
				// スクリプトキー
				else if (strcmp(strHead, "ScriptKey") == 0) {
					// キー数
					int KeyNum;
					RDint(&KeyNum);
					// データ
					for (int i = 0; i<KeyNum; i++) {
						// キー追加
						YsAnimeKey_Script* lpScr = new YsAnimeKey_Script();
						anime->m_Script.push_back(lpScr);

						while (1) {
							RHead();
							if (strcmp(strHead, "End") == 0) {
								break;
							}
							else if (strcmp(strHead, "Time") == 0) {
								int n;
								RDint(&n);
								lpScr->Time = (float)n;
							}
							else if (strcmp(strHead, "ScriptText") == 0) {
								RDstr(lpScr->Value);
							}
						}
					}
				}
				// TicksPerSecound
				else if(strcmp(strHead, "TicksPerSecond") == 0){
					RDdouble(&anime->m_TicksPerSecond);
				}
				// ボーンデータ
				else if (strcmp(strHead, "BoneData") == 0) {
					//					std::string sBoneName = "";
					//					int boneIdx = -1;

					YsKeyframeAnime* aniSet;
					// ②
					while (1) {
						RHead();
						if (strcmp(strHead, "End") == 0) {
							break;
						}
						// ボーン名
						else if (strcmp(strHead, "BoneName") == 0) {
							// 追加
							aniSet = new YsKeyframeAnime();
							anime->m_FrameAnimeList.push_back(aniSet);

							// 
							std::string boneName;
							RDstr(boneName);
						}
						// 回転キー
						else if (strcmp(strHead, "RotateKey") == 0) {
							// キー数
							int keyNum;
							RDint(&keyNum);
							for (int h = 0; h<keyNum; h++) {
								// キー追加
								YsAnimeKey_Quaternion* key = new YsAnimeKey_Quaternion();
								aniSet->m_Rotate.push_back(key);
								while (1) {
									RHead();
									if (strcmp(strHead, "End") == 0) {
										break;
									}
									else if (strcmp(strHead, "Time") == 0) {
										int n;
										RDint(&n);
										key->Time = (float)n;
									}
									else if (strcmp(strHead, "Rota") == 0) {
										RD(&key->Value, 4 * 4);
										//										D3DXQuaternionNormalize(&key->Value,&key->Value);
									}
									else if (strcmp(strHead, "CalcType") == 0) {
										RDchar(&key->CalcType);
									}
									else if (strcmp(strHead, "PowVal") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValBefore") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValAfter") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "Bezier") == 0) {
										RDchar(&key->BezierA[0]);
										RDchar(&key->BezierA[1]);
										RDchar(&key->BezierB[0]);
										RDchar(&key->BezierB[1]);
									}

								}
							}

							// 曲線系の補間は処理が重いので、曲線になるような線形補間として焼き込む
							if(bakeCurve){
								YsAnimeKey_Quaternion::BakeCurve(aniSet->m_Rotate, 0);
							}
						}
						// 拡大キー
						else if (strcmp(strHead, "ScaleKey") == 0) {
							// キー数
							int keyNum;
							RDint(&keyNum);
							for (int h = 0; h<keyNum; h++) {
								// キー追加
								YsAnimeKey_Vector3* key = new YsAnimeKey_Vector3();
								aniSet->m_Scale.push_back(key);
								while (1) {
									RHead();
									if (strcmp(strHead, "End") == 0) {
										break;
									}
									else if (strcmp(strHead, "Time") == 0) {
										int n;
										RDint(&n);
										key->Time = (float)n;
									}
									else if (strcmp(strHead, "Scale") == 0) {
										RD(&key->Value, 4 * 3);
									}
									else if (strcmp(strHead, "CalcType") == 0) {
										RDchar(&key->CalcType);
									}
									else if (strcmp(strHead, "PowVal") == 0) {			// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValBefore") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValAfter") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "Bezier") == 0) {
										RDchar(&key->BezierX_A[0]);
										RDchar(&key->BezierX_A[1]);
										RDchar(&key->BezierX_B[0]);
										RDchar(&key->BezierX_B[1]);
										RDchar(&key->BezierY_A[0]);
										RDchar(&key->BezierY_A[1]);
										RDchar(&key->BezierY_B[0]);
										RDchar(&key->BezierY_B[1]);
										RDchar(&key->BezierZ_A[0]);
										RDchar(&key->BezierZ_A[1]);
										RDchar(&key->BezierZ_B[0]);
										RDchar(&key->BezierZ_B[1]);
									}
								}
							}

							// 曲線系の補間は処理が重いので、曲線になるような線形補間として焼き込む
							if(bakeCurve){
								YsAnimeKey_Vector3::BakeCurve(aniSet->m_Scale, 0);
							}
						}
						// 座標キー
						else if (strcmp(strHead, "PosKey") == 0) {
							// キー数
							int keyNum;
							RDint(&keyNum);
							for (int h = 0; h<keyNum; h++) {
								// キー追加
								YsAnimeKey_Vector3* key = new YsAnimeKey_Vector3();
								aniSet->m_Pos.push_back(key);
								while (1) {
									RHead();
									if (strcmp(strHead, "End") == 0) {
										break;
									}
									else if (strcmp(strHead, "Time") == 0) {
										int n;
										RDint(&n);
										key->Time = (float)n;
									}
									else if (strcmp(strHead, "Pos") == 0) {
										RD(&key->Value, 4 * 3);
									}
									else if (strcmp(strHead, "CalcType") == 0) {
										RDchar(&key->CalcType);
									}
									else if (strcmp(strHead, "PowVal") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValBefore") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValAfter") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "Bezier") == 0) {
										RDchar(&key->BezierX_A[0]);
										RDchar(&key->BezierX_A[1]);
										RDchar(&key->BezierX_B[0]);
										RDchar(&key->BezierX_B[1]);
										RDchar(&key->BezierY_A[0]);
										RDchar(&key->BezierY_A[1]);
										RDchar(&key->BezierY_B[0]);
										RDchar(&key->BezierY_B[1]);
										RDchar(&key->BezierZ_A[0]);
										RDchar(&key->BezierZ_A[1]);
										RDchar(&key->BezierZ_B[0]);
										RDchar(&key->BezierZ_B[1]);
									}

								}
							}

							// 曲線系の補間は処理が重いので、曲線になるような線形補間として焼き込む
							if(bakeCurve){
								YsAnimeKey_Vector3::BakeCurve(aniSet->m_Pos, 0);
							}
						}
					}
				}
			}
#pragma endregion
		}
	}

	// 
	fclose(fp);

	return true;
}

//====================================================================================================
//
// YsAnimatorTrack
//
//====================================================================================================

void YsAnimatorTrack::UpdateEvent(double val)
{
	// イベント処理
	auto it = EventList.begin();
	while (it != EventList.end()) {
		(*it)->fStartTime -= val;

		if ((*it)->fStartTime > 0) {
			++it;
			continue;
		}

		// Enable
		if ((*it)->Type == Event_Base::STE_ENABLE) {
			Event_Enable *p = static_cast<Event_Enable*>((*it).get());

			m_Enable = p->bl;

			// このイベントを消す
			it = EventList.erase(it);
			continue;
		}
		// DeleteTrack
		else if ((*it)->Type == Event_Base::STE_DELETETRACK) {
			Event_DeleteTrack *p = static_cast<Event_DeleteTrack*>((*it).get());

			m_DeleteTrack = true;

			// このイベントを消す
			it = EventList.erase(it);
			continue;
		}
		// Position
		else if ((*it)->Type == Event_Base::STE_POSITION) {
			Event_Position *p = static_cast<Event_Position*>((*it).get());

			m_AnimePos = p->Pos;

			// このイベントを消す
			it = EventList.erase(it);
			continue;
		}
		// Speed
		else if ((*it)->Type == Event_Base::STE_SPEED) {
			Event_Speed *p = static_cast<Event_Speed*>((*it).get());

			if (p->startflag == 0) {
				p->OldSpeed = m_Speed;// 現在の速度を記憶
				p->startflag = 1;
			}

			if (p->fDuration == 0) {	// 即座に変更

				m_Speed = p->NewSpeed;

				// このイベントを消す
				it = EventList.erase(it);
				continue;
			}
			else {					// 中間補間
				double f;
				f = (-p->fStartTime) / p->fDuration;
				if (f >= 1.0f) {
					// 最後
					m_Speed = p->NewSpeed;

					// このイベントを消す
					it = EventList.erase(it);
					continue;
				}
				else {
					// 補間
					m_Speed = (float)(p->NewSpeed*(f)+p->OldSpeed*(1 - f));
				}
			}
		}
		//Weight
		else if ((*it)->Type == Event_Base::STE_WEIGHT) {
			Event_Weight* p = static_cast<Event_Weight*>((*it).get());

			if (p->startflag == 0) {
				p->OldWeight = m_Weight;// 現在の重みを記憶
				p->startflag = 1;
			}

			if (p->fDuration == 0) {	// 即座に変更

				m_Weight = p->NewWeight;

				// このイベントを消す
				it = EventList.erase(it);
				continue;
			}
			else {					// 中間補間
				double f;
				f = (-p->fStartTime) / p->fDuration;
				if (f >= 1.0f) {
					// 最後
					m_Weight = p->NewWeight;

					// このイベントを消す
					it = EventList.erase(it);
					continue;
				}
				else {
					// 補間
					m_Weight = (float)(p->NewWeight*(f)+p->OldWeight*(1 - f));
				}
			}
		}

		++it;
	}

}

//====================================================================================================
//
// YsAnimator
//
//====================================================================================================

void YsAnimator::Init()
{
	m_BaseWeight = 1;
	m_BaseWeightAnime = 1;
	m_BaseWeightAnimeSpeed = 0.1f;
	UnkeyAllTrackEvents();

	m_AnimeList.clear();


	m_Track.clear();
	m_Track.push_back(std::make_shared<YsAnimatorTrack>());
	m_Track[0]->Init();

	m_RefMatrixTbl.clear();

}

void YsAnimator::Init(YsAnimator& srcAnimator)
{
	Init();

	m_AnimeList = srcAnimator.m_AnimeList;

	m_RefMatrixTbl.clear();
	m_RefMatrixTbl.resize(srcAnimator.m_RefMatrixTbl.size());
	for (UINT i = 0; i < m_RefMatrixTbl.size(); i++) {
		m_RefMatrixTbl[i].refTransMat = srcAnimator.m_RefMatrixTbl[i].refTransMat;
	}
}


void YsAnimator::Init(YsMatrix* refMat)
{
	Init();

	ClearRefMatrix();
	AddRefMatrix(refMat);
}

void YsAnimator::AddAnimation(SPtr<YsAnimationSet> anime, bool allowSameName) {
	// 同名のアニメがあるなら入れ替え
	if (allowSameName == false) {
		int idx = SearchAnimation(anime->m_AnimeName);
		if (idx != -1) {
			// あり　差し替え
			m_AnimeList[idx] = anime;
			return;
		}
	}

	// 新規追加
	m_AnimeList.push_back(anime);
}

SPtr<YsAnimationSet> YsAnimator::AddAnimation(const std::string& XmdFileName, const char* registerAnimeName, bool allowSameName)
{
	SPtr<YsAnimationSet> ani = std::make_shared<YsAnimationSet>();
	ani->LoadXMD(XmdFileName, registerAnimeName);

	AddAnimation(ani, allowSameName);

	return ani;
}


YsAnimator::YsAnimator()
{
}

int YsAnimator::GetMaxAnimeNum()
{
	return (int)m_AnimeList.size();
}

void YsAnimator::Animation(double Val, YsMatrix* mulDifference0)
{
	YsAnimatorTrack* track;

	//====================================================
	// RootMove機能
	//====================================================
	// 現在のアニメ位置を憶えておく
	double aniPos = m_Track.back()->m_AnimePos;

	// アニメ前のRootのローカル行列を記憶
	YsMatrix mPrev;
	if (mulDifference0 && m_RefMatrixTbl.size() >= 1 && m_RefMatrixTbl[0].refTransMat) {
		mPrev = *m_RefMatrixTbl[0].refTransMat;
		mPrev.CreateInverse();
	}


	//====================================================
	// 重みアニメ処理
	//====================================================
	if(Val > 0){
		if (m_BaseWeight != m_BaseWeightAnime) {
			if (m_BaseWeight > m_BaseWeightAnime) {
				m_BaseWeight -= (float)( m_BaseWeightAnimeSpeed * Val );
				if (m_BaseWeight < m_BaseWeightAnime) {
					m_BaseWeight = m_BaseWeightAnime;
				}
			}
			else if (m_BaseWeight < m_BaseWeightAnime) {
				m_BaseWeight += (float)( m_BaseWeightAnimeSpeed * Val );
				if (m_BaseWeight > m_BaseWeightAnime) {
					m_BaseWeight = m_BaseWeightAnime;
				}
			}
		}

		//====================================================
		// 各トラックの処理
		//====================================================
		for (UINT i = 0; i<m_Track.size(); i++) {
			track = m_Track[i].get();

			//========================
			// アニメ進める
			//========================
			if (track->m_Enable) {
				YsAnimationSet* lpSA = track->m_pSkinAnime.get();

				// 進める
				track->m_AnimePos += Val * track->m_Speed * (lpSA->m_TicksPerSecond / 60.0);

				// 最後判定を先にする
				if (lpSA->m_AnimeLen <= 0) {	// アニメ長が0
					track->m_AnimePos = 0;
				}
				else if (track->m_Loop) {	// ループ
	//				while(track->m_AnimePos > lpSA->m_Period) {
	//					track->m_AnimePos -= lpSA->m_Period;
	//				}
					if (track->m_AnimePos >= lpSA->m_AnimeLen) {
						track->m_AnimePos = 0;
					}
				}
				else {				// ループなし
					if (track->m_AnimePos > lpSA->m_AnimeLen) {
						track->m_AnimePos = lpSA->m_AnimeLen;
					}
				}
			}

			//========================
			// イベント処理
			//========================
			track->UpdateEvent(Val);
			if (track->m_DeleteTrack) {
				// 最後の1個は消さない
				if (m_Track.size() >= 2) {
					m_Track.erase(m_Track.begin() + i);
					i--;
				}
			}
		}

	}

	//====================================================
	// アニメーションデータを使い、変換行列更新
	//====================================================
	CalcAnimation();

	// アニメ後のRootのローカル行列を取得しアニメ前との差分を求めて、その分動かす
	if (mulDifference0 && m_RefMatrixTbl.size() >= 1 && m_RefMatrixTbl[0].refTransMat) {
		// アニメ位置が巻き戻っているときは、移動させない
		if (m_Track[0]->m_AnimePos > aniPos) {
			*mulDifference0 = (*m_RefMatrixTbl[0].refTransMat * mPrev) * (*mulDifference0);
		}
	}

}

void YsAnimator::AnimationAndScript(double Val, std::function<void(YsAnimeKey_Script*)> onScriptExecProc, YsMatrix* mulDifference0)
{
	if(m_Track.size() == 0)return;

	YsAnimatorTrack* track;
	//===========================================
	// スクリプトにより、細かく進行させる
	//===========================================
	if (m_Track[0]->m_Enable)
	{
		track = m_Track[0].get();
		YsAnimationSet* lpSA = track->m_pSkinAnime.get();

		if (lpSA->m_Script.size() > 0) {
			std::vector<double>					valTbl;
			std::vector<YsAnimeKey_Script*>	scrTbl;

			double nowTime = track->m_AnimePos;
			double margin = 0;
			if (nowTime == 0)margin = -0.0000001;
			double nextTime = (track->m_AnimePos + (Val * track->m_Speed*(lpSA->m_TicksPerSecond / 60)));
			double prevPos = 0;
			// nowTime - nextTime間にスクリプトがあるか？
			for (auto& sc : lpSA->m_Script) {
				// 発見！
				if (sc->Time > nowTime + margin && sc->Time <= nextTime) {
					// 位置算出
					double p = (sc->Time - track->m_AnimePos);
					// 前のスクリプトからの相対位置を登録
					valTbl.push_back(p - prevPos);
					scrTbl.push_back(sc);	// スクリプトも登録

					prevPos = p;
				}
			}

			// 発見したスクリプトの位置を元に全身のポーズを決定し、スクリプトも実行する
			if (valTbl.size() > 0) {
				for (UINT i = 0; i < valTbl.size(); i++) {

					double v = valTbl[i];
					Val -= v * track->m_Speed;
					double buSpeed = track->m_Speed;
					track->m_Speed = 1.0;
					Animation(v, mulDifference0);
					track->m_Speed = buSpeed;

					// スクリプト実行
					YsAnimeKey_Script* sc = scrTbl[i];
					onScriptExecProc(sc);
				}
				// 残りのぶんを進行
				if (Val > 0) {
					Animation(Val, mulDifference0);
				}

				return;
			}
		}

		// 実行するスクリプトがない場合は、通常アニメ

	}


	// 通常アニメ
	Animation(Val, mulDifference0);

}

void YsAnimator::CalcAnimation()
{
	UINT i;

	YsAnimationSet* anime;
	YsMatrix m;
	YsQuat qTotal;
	YsVec3 vTotal;

	// フレーム数
	UINT FrameSize = m_RefMatrixTbl.size();

	YsAnimatorTrack* track;

	// 全フレーム処理
	double fAnimeTime;
	YsMatrix mOldTransMat;
	YsQuat qRota;
	YsVec3 vScale;
	YsVec3 sTotal;
	YsVec3 vPos;
	YsQuat qTmp;
	YsVec3 vTmp;

	for (i = 0; i<FrameSize; i++) {

		YsMatrix* refMat = m_RefMatrixTbl[i].refTransMat;
		if (refMat == nullptr)continue;

		int RotaCnt = 0;
		int PosCnt = 0;
		int ScaleCnt = 0;
		// 全トラック
		for (UINT tr = 0; tr<m_Track.size(); tr++) {
			track = m_Track[tr].get();
			if (!track->m_Enable)continue;

			anime = track->m_pSkinAnime.get();
			if (i >= anime->m_FrameAnimeList.size())continue;

			// アニメ位置
			fAnimeTime = track->m_AnimePos;
			if (fAnimeTime < 0)fAnimeTime = 0;


			// 回転
			if(YsAnimeKey_Quaternion::InterpolationFromTime(anime->m_FrameAnimeList[i]->m_Rotate, fAnimeTime, qRota, &m_RefMatrixTbl[i].nowKeyIndex_Rota)) {
				// 初回 または 重みが1
				if (RotaCnt == 0 || track->m_Weight >= 1.0f) {
					qTotal = qRota;
				}
				// ブレンド
				else {
					if (m_RefMatrixTbl[i].isBlendTrack_Rota) {
						YsQuat::Slerp(qTotal,
							qTotal,
							qRota,
							track->m_Weight);		// 補間(0.0～1.0)
					}
				}
				RotaCnt++;
			}
			// 拡大
			if(YsAnimeKey_Vector3::InterpolationFromTime(anime->m_FrameAnimeList[i]->m_Scale, fAnimeTime, vScale, &m_RefMatrixTbl[i].nowKeyIndex_Scale)) {
				// 初回 または 重みが1
				if (ScaleCnt == 0 || track->m_Weight >= 1.0f) {
					sTotal = vScale;
				}
				// ブレンド
				else {
					if (m_RefMatrixTbl[i].isBlendTrack_Scale) {
						YsVec3::Lerp(sTotal,
							sTotal,
							vScale,
							track->m_Weight);
					}
				}
				ScaleCnt++;
			}
			// 座標
			if(YsAnimeKey_Vector3::InterpolationFromTime(anime->m_FrameAnimeList[i]->m_Pos, fAnimeTime, vPos, &m_RefMatrixTbl[i].nowKeyIndex_Pos)) {
				// 初回 または 重みが1
				if (PosCnt == 0 || track->m_Weight >= 1.0f) {
					vTotal = vPos;
				}
				// ブレンド
				else {
					if (m_RefMatrixTbl[i].isBlendTrack_Pos) {
						YsVec3::Lerp(vTotal,
							vTotal,
							vPos,
							track->m_Weight);
					}
				}
				PosCnt++;
			}

		}

		// キーがない場合は、何もしない
		if (RotaCnt == 0 && ScaleCnt == 0 && PosCnt == 0) continue;

		m.CreateIdentity();

		mOldTransMat = *refMat;	// 計算前のローカル行列を記憶

		// ベースウェイトでのブレンド
		// 回転
		if (RotaCnt > 0) {
			// ベースウェイトが1未満なら、計算前の行列とブレンド(1以上だと上書きになるので、計算しなくてよい)
			if (m_BaseWeight < 1) {
				YsMatrix::ToQuaternion(qTmp, mOldTransMat);
				if (m_BaseWeight > 0) {
					YsQuat::Slerp(qTotal, qTmp, qTotal, m_BaseWeight);
				}
				else {
					qTotal = qTmp;
				}
			}

			qTotal.ToMatrix(m);
			refMat->SetRotation(m);
		}
		// 拡大
		if (ScaleCnt > 0) {
			// ベースウェイトが1未満なら、計算前の行列とブレンド(1以上だと上書きになるので、計算しなくてよい)
			if (m_BaseWeight < 1) {
				vTmp.Set(mOldTransMat.GetXScale(), mOldTransMat.GetYScale(), mOldTransMat.GetZScale());

				if (m_BaseWeight > 0) {
					YsVec3::Lerp(sTotal, vTmp, sTotal, m_BaseWeight);
				}
				else {
					sTotal = vTmp;
				}
			}

			refMat->NormalizeScale();
			m.CreateScale(sTotal);
			*refMat = m * (*refMat);
		}
		// 座標
		if (PosCnt > 0) {
			// ベースウェイトが1未満なら、計算前の行列とブレンド(1以上だと上書きになるので、計算しなくてよい)
			if (m_BaseWeight < 1) {
				vTmp.Set(mOldTransMat.GetPos());

				if (m_BaseWeight > 0) {
					YsVec3::Lerp(vTotal, vTmp, vTotal, m_BaseWeight);
				}
				else {
					vTotal = vTmp;
				}
			}
			refMat->SetPos(vTotal);
		}
	}
}

bool YsAnimator::ChangeAnime(UINT AnimeNo, bool loop, UINT SetTrackNo, bool bEnableTrack, double Speed, float Weight, double AnimePos)
{
	if (m_AnimeList.size() <= AnimeNo)return false;

	if (SetTrackNo >= m_Track.size())return false;

	m_Track[SetTrackNo]->m_pSkinAnime = m_AnimeList[AnimeNo];
	m_Track[SetTrackNo]->m_AnimeNo = AnimeNo;
	m_Track[SetTrackNo]->m_AnimePos = AnimePos;
	m_Track[SetTrackNo]->m_Enable = bEnableTrack;
	m_Track[SetTrackNo]->m_Speed = Speed;
	m_Track[SetTrackNo]->m_Weight = Weight;
	m_Track[SetTrackNo]->m_Loop = loop;


	return true;
}
bool YsAnimator::ChangeAnime_FromName(const std::string& AnimeName, bool loop, UINT SetTrackNo, bool bEnableTrack, double Speed, float Weight, double AnimePos)
{
	// アニメ検索
	int animeNo = SearchAnimation(AnimeName);
	if (animeNo == -1)return false;

	return ChangeAnime(animeNo, loop, SetTrackNo, bEnableTrack, Speed, Weight, AnimePos);
}


// ブレンドで滑らかアニメ変更
bool YsAnimator::ChangeAnimeSmooth(UINT AnimeNo, float StartTime, float Duration, bool loop, double Speed, double AnimePos)
{
	// 新しいトラック作成
	SPtr<YsAnimatorTrack> newTrack = std::make_shared<YsAnimatorTrack>();
	m_Track.push_front(newTrack);

	// 即座に変更
	if (Duration == 0) {
		// 新しいアニメを設定
		if (ChangeAnime(AnimeNo, loop, 0, true, Speed, 1, AnimePos) == false)return false;

		// 前のアニメは無効にする。
		if (m_Track.size() >= 2) {
			// １つ前のトラック
			YsAnimatorTrack* prevTrack = m_Track[1].get();
			prevTrack->m_Enable = false;
			prevTrack->m_DeleteTrack = true;
			prevTrack->m_Weight = 0;
		}
	}
	// 滑らかに変更
	else {
		// 新しいアニメをセット
		if (ChangeAnime(AnimeNo, loop, 0, true, Speed, 0, AnimePos) == false)return false;
		// 新しいアニメの重みを上げていく設定
		newTrack->EventTrackWeight(1, StartTime, Duration);

		// 前のアニメの重みを減らし、無効にしていく。
		if (m_Track.size() >= 2) {
			// １つ前のトラック
			YsAnimatorTrack* prevTrack = m_Track[1].get();

			prevTrack->EventTrackEnable(false, StartTime + Duration);	// 無効イベント
			prevTrack->EventTrackDelete(StartTime + Duration);			// 削除イベント

			prevTrack->EventTrackSpeed(0, StartTime, Duration);			// 速度イベント
			prevTrack->EventTrackWeight(0.0f, StartTime, Duration);		// 重みイベント
		}

	}
	return true;
}

bool YsAnimator::ChangeAnimeSmooth_FromName(const std::string& AnimeName, float StartTime, float Duration, bool loop, double Speed, double AnimePos)
{
	// アニメ検索
	int animeNo = SearchAnimation(AnimeName);
	if (animeNo == -1)return false;

	return ChangeAnimeSmooth(animeNo, StartTime, Duration, loop, Speed, AnimePos);
}

bool YsAnimator::AddAnimeSmooth(UINT AnimeNo, float StartTime, float Duration, bool loop, double Speed, double AnimePos, float startWeight, float endWeight)
{
	// 新しいトラック作成
	SPtr<YsAnimatorTrack> newTrack = std::make_shared<YsAnimatorTrack>();
	m_Track.push_back(newTrack);	// 後ろに追加

	// 新しいアニメをセット
	if(ChangeAnime(AnimeNo, loop, (int)m_Track.size() - 1, true, Speed, startWeight, AnimePos) == false)return false;

	// 新しいアニメの重みを変化させていく設定
	newTrack->EventTrackWeight(endWeight, StartTime, Duration);

	// endWeightが0なら、0になったらトラックから消す
	if(endWeight == 0){
		newTrack->EventTrackEnable(false, StartTime + Duration);	// 無効イベント
		newTrack->EventTrackDelete(StartTime + Duration);			// 削除イベント
	}

	return true;
}

bool YsAnimator::AddAnimeSmooth_FromName(const std::string& AnimeName, float StartTime, float Duration, bool loop, double Speed, double AnimePos, float startWeight, float endWeight)
{
	// アニメ検索
	int animeNo = SearchAnimation(AnimeName);
	if(animeNo == -1)return false;

	return AddAnimeSmooth(animeNo, StartTime, Duration, loop, Speed, AnimePos, startWeight, endWeight);
}


