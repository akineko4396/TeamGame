#include "YskLib.h"

#include "YsXEDLoader.h"

using namespace YskLib;

bool YskLib::YsLoadXEDFile(
	const std::string& fileName,
	std::vector<SPtr<YsSingleModel>>& outModelTbl,
	std::vector<SPtr<YsGameModel::BoneNode>>& outBoneTree,
	std::vector<SPtr<YsAnimationSet>>& outAnimeTbl,
	std::vector< std::vector<YsXEDRigidBody> >&	outRigidBodyTbl,
	bool bakeCurve
)
{
	// パス、ファイル名抽出
	char szDir  [_MAX_PATH];
	char szFname[_MAX_FNAME]; 
	char szExt  [_MAX_EXT]; 
	_splitpath( fileName.c_str(), nullptr, szDir, szFname, szExt);
	
//	m_FileName = szFname;
//	m_FileName += szExt;
	std::string filePath = szDir;

	// 
	FILE* fp = fopen(fileName.c_str(), "rb");
	if( !fp )return FALSE;

#pragma region 読み込み関数
	char strHead[256];
	// 汎用
	auto RD = [fp](void* p, int size){fread(p, size, 1, fp);};
	auto RDchar = [fp](char* c) {fread(c, 1, 1, fp); };
	auto RDbyte = [fp](BYTE* c) {fread(c, 1, 1, fp); };
	auto RDword = [fp](WORD* w) {fread(w, 2, 1, fp); };
	auto RDint = [fp](int* n) {fread(n, 4, 1, fp); };
	auto RDuint = [fp](UINT* n) {fread(n, 4, 1, fp); };
	auto RDfloat = [fp](float* f) {fread(f, 4, 1, fp); };
	auto RDfloat2 = [fp](YsVec2* v) {fread(v, 8, 1, fp); };
	auto RDfloat3 = [fp](YsVec3* v) {fread(v, 12, 1, fp); };
	auto RDfloat4 = [fp](YsVec4* v) {fread(v, 16, 1, fp); };
	auto RDfloat4x4 = [fp](YsMatrix* m) {fread(m, 64, 1, fp); };
	auto RDdouble = [fp](double* f) {fread(f, 8, 1, fp); };
	// サイズ(int)＋文字列
	auto RDstr = [fp](std::string& str){
		int n;
		fread(&n, 4, 1, fp);	// size
		str.resize(n);
		if(n > 0){
			fread(&str[0], n, 1, fp);
		}
	};
	auto RHead = [fp, &strHead]() mutable{
		while(1){
			BYTE len;
			while(1){
				fread(&len, 1, 1, fp);		// 文字数
				// 開始文字
				BYTE st = 0;
				fread(&st, 1, 1, fp);
				// ヘッダ確認　続行
				if(st == 0x8C){
					break;
				}
				// 不正なヘッダ。巻き戻し、ヘッダまで進める
				else{
					fseek(fp, -1, SEEK_CUR);
				}
			}
			// 文字列
			fread(strHead, len, 1, fp);
			strHead[len] = '\0';
			// 終端文字
			BYTE term[2];
			fread(term, 2, 1, fp);
			if(term[0] == 0x60 && term[1] == 0x99){	// 終端確認
				break;
			}
			// 終端文字ではない。最初(文字数取得後)まで巻き戻す
			else{
				fseek(fp, -(1+len+2), SEEK_CUR);
			}
		}
	};
#pragma endregion


	outRigidBodyTbl.clear();

	//===========================================
	// Main Header
	//===========================================
	char head[4];
	RD(head ,3);
	head[3] = '\0';

	if( strcmp(head, "XED") != 0){
		fclose(fp);
		return false;
	}

	std::string sErrMsg;
	// ①
	while(1){
		// 項目読み込み
		RHead();
		// 終了
		if( strcmp(strHead, "End") == 0){
			break;
		}
		//===========================================
		// メッシュデータ
		//===========================================
		else if( strcmp(strHead, "XEDMeshData") == 0){
			// ★メッシュ数(int)
			int meshNum;
			RDint(&meshNum);
			outModelTbl.resize(meshNum);

			// メッシュループ
			for(int mi = 0; mi<meshNum; mi++) {
				// YsSingleModel作成
				outModelTbl[mi] = std::make_shared<YsSingleModel>();
				SPtr<YsSingleModel> pModel = outModelTbl[mi];

				// その中のYsMesh作成
				pModel->SetMesh(std::make_shared<YsMesh>());
				bool bSkinMesh = false;

				// 頂点形式
				YsVertexTypeData vtd = YsVertex_Pos_UV_TBN_Skin::GetVertexTypeData();
				YsVertexTypeData vtdST = YsVertex_Pos_UV_TBN::GetVertexTypeData();
				// 頂点データ
				int numVers = 0;	// 頂点数
				std::vector<YsVertex_Pos_UV_TBN_Skin> verTbl;
				// 面データ
				/*
				struct XEDFace {
					UINT p1;		// 頂点１へのインデックス
					UINT p2;		// 頂点２へのインデックス
					UINT p3;		// 頂点３へのインデックス
				};
				*/
				int numFaces = 0;	// 面数
				std::vector<YsMesh::Face> face;		// 面配列

				// Subsetデータ
				std::vector<YsMeshSubset> subset;

				// ★メッシュ名(int+文字列)
				std::string meshName;
				RDstr(meshName);
				pModel->SetName(meshName);

				while(1) {
					// 項目読み込み
					RHead();
					// 終了
					if(strcmp(strHead, "EndMesh") == 0) {

						// スキンメッシュ形式かスタティックメッシュ形式か
						YsVertexTypeData* pVTD;
						if(bSkinMesh){
							pVTD = &vtd;
						}
						else{
							pVTD = &vtdST;
						}

						// 追加情報作成
						for(int i = 0; i<numFaces; i++) {

							UINT f1 = face[i].idx[0];
							UINT f2 = face[i].idx[1];
							UINT f3 = face[i].idx[2];

							// 頂点の接線、従法線算出
							YsVec3 tangent, binormal;
							YsVec3 vN = (verTbl[f1].Normal + verTbl[f2].Normal + verTbl[f3].Normal) / 3;
							YsCalcTangentAndBinormal(&verTbl[f1].Pos, &verTbl[f1].UV,
								&verTbl[f2].Pos, &verTbl[f2].UV,
								&verTbl[f3].Pos, &verTbl[f3].UV,
								&vN, &tangent, &binormal);
							vN.Normalize();

							// 
							verTbl[f1].Tangent = tangent;
							verTbl[f2].Tangent = tangent;
							verTbl[f3].Tangent = tangent;
							verTbl[f1].Binormal = binormal;
							verTbl[f2].Binormal = binormal;
							verTbl[f3].Binormal = binormal;

						}


						// 
						if (bSkinMesh == false) {
							UPtr<YsVertex_Pos_UV_TBN[]> verTblST;
							// 頂点コピー
							verTblST = std::make_unique<YsVertex_Pos_UV_TBN[]>(verTbl.size());

							for(UINT vti = 0; vti<verTbl.size();vti++){
								verTblST[vti] = verTbl[vti];
							}

							// メッシュ作成
							if (pModel->GetMesh()->Create(&verTblST[0], verTbl.size(), &face[0], face.size(), subset) == false) {
								fclose(fp);
//								Release();
								return false;
							}
						}
						else {

							// メッシュ作成
							if (pModel->GetMesh()->Create(&verTbl[0], verTbl.size(), &face[0], face.size(), subset) == false) {
								fclose(fp);
//								Release();
								return false;
							}
						}



						break;
					}
					// 頂点
					else if(strcmp(strHead, "Vertex") == 0) {
						// ★頂点数(int)
						RDint(&numVers);
						// 頂点データ用
						verTbl.resize(numVers);

						// 
						while(1) {
							// 項目読み込み
							RHead();
							// 終了
							if(strcmp(strHead, "EndVertex") == 0) {
								break;
							}
							// 座標
							else if(strcmp(strHead, "Pos") == 0) {
								for(int vi = 0; vi<numVers; vi++) {
									// ★3Dベクトル(float*3)
									RDfloat3(&verTbl[vi].Pos);
								}
							}
							// 法線
							else if(strcmp(strHead, "Normal") == 0) {
								for(int vi = 0; vi<numVers; vi++) {
									// ★3Dベクトル(float*3)
									RDfloat3(&verTbl[vi].Normal);
								}
							}
							// UV
							else if(strcmp(strHead, "UV") == 0) {
								for(int vi = 0; vi<numVers; vi++) {
									// ★2Dベクトル(float*2)
									RDfloat2(&verTbl[vi].UV);
								}
							}
							// ボーンウェイト系
							else if(strcmp(strHead, "BoneWeight") == 0) {
								for(int vi = 0; vi<numVers; vi++) {
									// 初期化
									for(int wi = 0; wi<4; wi++) {
										verTbl[vi].BlendIndices[wi] = 0xFFFF;
										verTbl[vi].BlendWeight[wi] = 0;
									}

									// ★影響数(int)
									int wCnt;
									RDint(&wCnt);
									for(int wi = 0; wi<wCnt; wi++) {
										// ★ボーンIndex(unsigned short)
										RDword(&verTbl[vi].BlendIndices[wi]);
										// ★重み(float)
										RDfloat(&verTbl[vi].BlendWeight[wi]);
									}

									// スキンメッシュか？
									if(verTbl[vi].BlendIndices[0] != 0xFFFF){
										bSkinMesh = true;

										// 重み正規化
										float total = 0;
										for(int wi = 0; wi<wCnt; wi++) {
											total += verTbl[vi].BlendWeight[wi];
										}
										if(total > 0){
											for(int wi = 0; wi<wCnt; wi++) {
												verTbl[vi].BlendWeight[wi] /= total;
											}
										}
										else{
											verTbl[vi].BlendWeight[0] = 1.0f;
										}
									}

								}
							}
						}
					}
					// 面
					else if(strcmp(strHead, "Face") == 0) {

						// ★面数(int)
						RDint(&numFaces);
						face.resize(numFaces);

						// 
						while(1) {
							// 項目読み込み
							RHead();
							// 終了
							if(strcmp(strHead, "EndFace") == 0) {
								break;
							}
							else if(strcmp(strHead, "Index") == 0) {
								for(int fi = 0; fi<numFaces; fi++) {
									RDuint(&face[fi].idx[0]);
									RDuint(&face[fi].idx[1]);
									RDuint(&face[fi].idx[2]);
								}
							}
						}

					}
					// マテリアル
					else if(strcmp(strHead, "Material") == 0) {
						// ★マテリアル数(int)
						int numMate;
						RDint(&numMate);
						// マテリアルデータ
						std::vector<YsMaterial> material(numMate);
						// サブセットデータ
						subset.resize(numMate);	// XEDのサブセットはマテリアル数と同じにしてる

						for(int mai = 0; mai<numMate; mai++) {
							// ★マテリアル名(int+文字列)
							//※いらないから読み込むだけで捨てる
							std::string mateName;
							RDstr(mateName);

							// 
							while(1) {
								// 項目読み込み
								RHead();
								// 終了
								if(strcmp(strHead, "EndMateData") == 0){
									// 設定
									pModel->SetMaterials(material);
									break;
								}
								else if(strcmp(strHead, "FaceVertexData") == 0){
									int VerSt;
									int VerNum;
									int FaceSt;
									int FaceNum;
									RDint(&VerSt);
									RDint(&VerNum);
									RDint(&FaceSt);
									RDint(&FaceNum);

									subset[mai].AttribId = (DWORD)mai;
									subset[mai].FaceStart = (DWORD)FaceSt;
									subset[mai].FaceCount = (DWORD)FaceNum;
								} 
								else if(strcmp(strHead, "Diffuse") == 0) {
									RDfloat4((YsVec4*)&material[mai].Diffuse);
								} 
								else if(strcmp(strHead, "Specular") == 0) {
									RDfloat3((YsVec3*)&material[mai].Specular);
									material[mai].Specular.w = 1;
								} 
								else if(strcmp(strHead, "SpecularPower") == 0) {
									RDfloat(&material[mai].Power);
								} 
								else if(strcmp(strHead, "Emissive") == 0) {
									RDfloat3((YsVec3*)&material[mai].Emissive);
									material[mai].Emissive.w = 0;
								} 
								else if(strcmp(strHead, "Ambient") == 0) {
									RDfloat3(&material[mai].Ambient);
								} 
								else if(strcmp(strHead, "DrawFlag") == 0) {
									RDchar(&material[mai].DrawFlag);
								}
								else if(strcmp(strHead, "UV_Tiling") == 0) {
									RDfloat2(&material[mai].UV_Tiling);
								}
								else if(strcmp(strHead, "UV_Offset") == 0) {
									RDfloat2(&material[mai].UV_Offset);
								}
								else if(strcmp(strHead, "ExtScript") == 0) {
									RDstr(material[mai].ExtScript);

									/*
									// ※とりあえずCSVとして解析し、一部の構文を設定として使用
									YsCsv csv;
									csv.LoadFromText(material[mai].ExtScript);
									// 縦ループ
									for(UINT ci = 0;ci < csv.m_Tbl.size();ci++){
										const std::string strType = csv.m_Tbl[ci][0];
										// 視差マップ時の強さ
										if(strType == "ParallaxVal"){
											material[mai].ParallaxVal = (float)std::stof(csv.m_Tbl[ci][1]);
										}
									}
									*/
								}
								else if(strcmp(strHead, "Texture") == 0) {
									RDstr(material[mai].TextureFileName);

									// テクスチャ読み込み
									std::string strFile = filePath + material[mai].TextureFileName;
									material[mai].TexSet.LoadTextureSet(strFile);
								}
							}

						}

					}
				}

			}

			// スキンメッシュとスタティックメッシュの参照を分けたテーブル作成
//			CreateRefMeshTbl();

		}
		//===========================================
		// ボーンプロパティ
		//===========================================
		else if( strcmp(strHead, "BoneProperty") == 0){
			// ★ボーン数
			int boneNum;
			RDint( &boneNum );

			// ボーンが未構築なら構築する
			bool bBuildBone = false;
			if(outBoneTree.size() == 0){
				bBuildBone = true;
			}

			if(bBuildBone){
				outBoneTree.resize(boneNum);
				for(int bone = 0; bone<boneNum; bone++) {
					outBoneTree[bone] = std::make_shared<YsGameModel::BoneNode>();
				}
			}

			for(int bone=0;bone<boneNum;bone++){
				// ★ボーン名
				std::string boneName;
				RDstr( boneName );

				SPtr<YsGameModel::BoneNode> boneNode = outBoneTree[bone];
				boneNode->BoneName = boneName;	// ボーン名
				boneNode->OffsetID = bone;		// OffsetID

				// 
				while(1){
					// 項目読み込み
					RHead();
					// 終了
					if( strcmp(strHead, "End") == 0){
						// データ作成
						if(bBuildBone){
							if(boneNode->pMother.expired() == false){
								boneNode->pMother.lock()->Child.push_back(boneNode);
								boneNode->DefWorldMat = boneNode->DefTransMat * boneNode->pMother.lock()->DefWorldMat;
							}
						}

						break;
					}
					else if(strcmp(strHead, "Level") == 0){
						RDint(&boneNode->Level);
					}
					else if(strcmp(strHead, "ParentBoneIdx") == 0) {
						int parentBoneIdx;
						RDint(&parentBoneIdx);
						if(parentBoneIdx >= 0){
							boneNode->pMother = outBoneTree[parentBoneIdx];
						}
					}
					else if(strcmp(strHead, "TransMat") == 0) {
						RDfloat4x4(&boneNode->DefTransMat);
					}
					else if(strcmp(strHead, "OffsetMat") == 0) {
						RDfloat4x4(&boneNode->OffsetMat);
					}					
					else if( strcmp(strHead, "EulerMode") == 0){
						char c;
						RDchar( &c );
					}
					else if( strcmp(strHead, "BoneLayers") == 0){
						RDint(&boneNode->BoneLayers);

					}
					else if( strcmp(strHead, "BoneFlag") == 0){
						RDint(&boneNode->BoneFlag);
					}
					else if( strcmp(strHead, "SymmetryBone") == 0){
						std::string str;
						RDstr( str );

					}
					else if( strcmp(strHead, "LimitAng") == 0){
						float f;
						RDfloat(&f);
						RDfloat(&f);
						RDfloat(&f);
						RDfloat(&f);
						RDfloat(&f);
						RDfloat(&f);
					}
					// IKデータ
					else if(strcmp(strHead, "IKData") == 0) {
						while(1){
							// 項目読み込み
							RHead();
							// 終了
							if(strcmp(strHead, "EndIKData") == 0){
								break;
							}
							// ターゲットボーン番号
							else if(strcmp(strHead, "TargetBone") == 0){
								// TargetBone
								RDint(&boneNode->IK.boneIdx);
							}
							else if(strcmp(strHead, "LoopCnt") == 0){
								// LoopCnt
								RDint(&boneNode->IK.LoopCnt);
							}
							else if(strcmp(strHead, "LimitAng") == 0){
								// LimitAng
								RDfloat(&boneNode->IK.ikLimitedAng);
							}
							else if(strcmp(strHead, "Flags") == 0) {
								RDbyte(&boneNode->IK.flags);
							}
							else if(strcmp(strHead, "LinkData") == 0) {
								int cnt;
								RDint(&cnt);	// Link数

								boneNode->IK.LinkList.resize(cnt);

								for(int lki = 0; lki<cnt; lki++){
									while(1){
										// 項目読み込み
										RHead();
										// 終了
										if(strcmp(strHead, "EndLinkData") == 0){
											break;
										}
										else if(strcmp(strHead, "LinkBone") == 0){
											RDint(&boneNode->IK.LinkList[lki].boneIdx);
										}
										else if(strcmp(strHead, "LimitAngEnable") == 0){
											char LimitAngEnable;
											RDchar(&LimitAngEnable);

											boneNode->IK.LinkList[lki].bLimitAng = LimitAngEnable ? true : false;
										}
										else if(strcmp(strHead, "MinLimAng") == 0){
											YsVec3 minLimitAng;
											RDfloat3(&boneNode->IK.LinkList[lki].minLimitAng);
										}
										else if(strcmp(strHead, "MaxLimAng") == 0){
											YsVec3 maxLimitAng;
											RDfloat3(&boneNode->IK.LinkList[lki].maxLimitAng);
										}
									}// LinkData while
								}// LinkList for
							}
						}// IKData while
					}
					// 付与データ
					else if(strcmp(strHead, "ProvideData") == 0){
						while(1){
							// 項目読み込み
							RHead();
							// 終了
							if(strcmp(strHead, "EndProvideData") == 0){
								break;
							}
							else if(strcmp(strHead, "Parent") == 0){
								int providedParentBoneIdx;
								RDint(&providedParentBoneIdx);
							}
							else if(strcmp(strHead, "Ratio") == 0){
								float providedRatio;
								RDfloat(&providedRatio);
							}
						}// while
					}
					else if(strcmp(strHead, "ExtDrawPos") == 0){
						YsVec3 extDrawPos;
						RDfloat3(&extDrawPos);
					}
					else if(strcmp(strHead, "ExtDrawFileName") == 0){
						std::string extDrawFileName;
						RDstr(extDrawFileName);
					}
					// ウェイトBOXデータ
					else if(strcmp(strHead, "WeightBoxData") == 0){

						while(1){
							// 項目読み込み
							RHead();
							// 終了
							if(strcmp(strHead, "EndWeightBoxData") == 0){

								break;
							}
							else if(strcmp(strHead, "Matrix") == 0){
								YsMatrix mat;
								RD(&mat, 4 * 4 * 4);
							}
							else if(strcmp(strHead, "InflMeshNo") == 0){
								BYTE no;
								RDbyte(&no);

							}
							else if(strcmp(strHead, "IngoreVerts") == 0){
								// ★メッシュ数
								int meshCnt = 0;
								RDint(&meshCnt);
								for(int imi = 0; imi<meshCnt; imi++){
									// ★メッシュNo
									int meshNo;
									RDint(&meshNo);

									// ★無視頂点リスト
									int ignoreCnt;
									RDint(&ignoreCnt);
									for(int ivi = 0; ivi<ignoreCnt; ivi++){
										// ★頂点No
										int verNo;
										RDint(&verNo);

									}
								}
							}
							else if(strcmp(strHead, "Atte") == 0){
								int Attenuation;
								RDint(&Attenuation);
							}
						}// while
					}
					else{
						int a = 0;
					}
				}
			}
		}
		//===========================================
		// アニメデータ
		//===========================================
		else if( strcmp(strHead, "AnimeName") == 0){
			// アニメ追加
			SPtr<YsAnimationSet> anime = std::make_shared<YsAnimationSet>();
			outAnimeTbl.push_back( anime );

			// ★アニメ名
			RDstr(anime->m_AnimeName);
			// ★アニメ長
			int animeLen;
			RDint( &animeLen );
			anime->m_TicksPerSecond = 60;
			anime->m_AnimeLen = animeLen;
			anime->CreateFrameAnimeList(outBoneTree.size() );

			// アニメ名判定
			// 先頭が"//"で始まる名前は、アニメーキーは読み込まない
			bool bEnable = true;
			if(anime->m_AnimeName.substr(0, 2) == "//"){
				bEnable = false;
			}

			bool bBlendAnimeMode = false;
			float fBlendAnimeWeight = 1.0f;

			// 
			while(1){
				// 項目読み込み
				RHead();
				// 終了
				if( strcmp(strHead, "End") == 0){
					// 無効アニメは消す
					if(bEnable == false){
						outAnimeTbl.back()->Release();
//						delete GetAnimeList().back();
						outAnimeTbl.pop_back();
					}

					// 合成モードアニメなら、合成する
					if(bBlendAnimeMode){
						for(UINT bi = 0; bi < anime->m_FrameAnimeList.size(); bi++){
							YsKeyframeAnime* srcAnime = anime->m_FrameAnimeList[bi];
							YsKeyframeAnime* destAnime = outAnimeTbl.back()->m_FrameAnimeList[bi];

							if(fBlendAnimeWeight > 0){
								// 回転キーが１つでもあれば、とりあえず差し替えで　※のちに重みも考慮したい
								if(srcAnime->m_Rotate.size() > 0){
									destAnime->m_Rotate = srcAnime->m_Rotate;
								}
								if(srcAnime->m_Scale.size() > 0){
									destAnime->m_Scale = srcAnime->m_Scale;
								}
								if(srcAnime->m_Pos.size() > 0){
									destAnime->m_Pos = srcAnime->m_Pos;
								}
							}
						}
					}


					break;
				}
				// 合成アニメモード
				else if(strcmp(strHead, "BlendAnimeMode") == 0) {
					int nn;
					RDint(&nn);
					bBlendAnimeMode = (nn == 1 ? true : false);

					// 合成モードのアニメはリストから除外
					if(bEnable && bBlendAnimeMode){
						outAnimeTbl.pop_back();
					}
				}
				// 合成アニメの重み(現在未使用)
				else if(strcmp(strHead, "BlendAnimeWeight") == 0) {
					RDfloat(&fBlendAnimeWeight);
				}
				// スクリプトキー
				else if( strcmp(strHead, "ScriptKey") == 0){
					// キー数
					int KeyNum;
					RDint( &KeyNum );
					// データ
					for(int i=0;i<KeyNum;i++){
						// キー追加
						YsAnimeKey_Script* lpScr = new YsAnimeKey_Script();
						anime->m_Script.push_back(lpScr);

						while(1){
							RHead();
							if( strcmp(strHead, "End") == 0){
								break;
							}
							else if( strcmp(strHead, "Time") == 0){
								int n;
								RDint( &n );
								lpScr->Time = (float)n;
							}
							else if( strcmp(strHead, "ScriptText") == 0){
								RDstr( lpScr->Value );
							}
						}
					}
				}
				// TicksPerSecound
				else if( strcmp(strHead, "TicksPerSecond") == 0){
					RDdouble(&anime->m_TicksPerSecond);
				}
				// ボーンデータ
				else if( strcmp(strHead, "BoneData") == 0){
					std::string sBoneName = "";
					int boneIdx = -1;
					// ②
					while(1){
						RHead();
						if( strcmp(strHead, "End") == 0){
							break;
						}
						// ボーン名
						else if( strcmp(strHead, "BoneName") == 0){
							RDstr( sBoneName );

							// ボーン検索
							boneIdx = -1;
							for (UINT bi = 0; bi < outBoneTree.size(); bi++) {
								if (sBoneName == outBoneTree[bi]->BoneName) {
									boneIdx = bi;
									break;
								}
							}
//							boneIdx = SearchBoneIndex( sBoneName );
						}
						// 回転キー
						else if( strcmp(strHead, "RotateKey") == 0){
							if(boneIdx == -1){
								sErrMsg = "エラー";
								break;
							}
							// キー数
							int keyNum;
							RDint( &keyNum );
							for(int h=0;h<keyNum;h++){
								// キー追加
								YsAnimeKey_Quaternion* key = new YsAnimeKey_Quaternion();
								anime->m_FrameAnimeList[boneIdx]->m_Rotate.push_back(key);
								while(1){
									RHead();
									if( strcmp(strHead, "End") == 0){
										break;
									}
									else if( strcmp(strHead, "Time") == 0){
										int n;
										RDint( &n );
										key->Time = (float)n;
									}
									else if( strcmp(strHead, "Rota") == 0){
										RD( &key->Value, 4*4 );
									}
									else if( strcmp(strHead, "CalcType") == 0){
										RDchar( &key->CalcType );
									}
									else if (strcmp(strHead, "PowVal") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValBefore") == 0) {// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValAfter") == 0) {// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "Bezier") == 0) {
										RDchar(&key->BezierA[0]);
										RDchar(&key->BezierA[1]);
										RDchar(&key->BezierB[0]);
										RDchar(&key->BezierB[1]);
									}
								}
							}

							// 曲線系の補間は処理が重いので、曲線になるような線形補間として焼き込む
							if(bakeCurve){
								YsAnimeKey_Quaternion::BakeCurve(anime->m_FrameAnimeList[boneIdx]->m_Rotate, 0);
							}

							/*
							// 球面二次補間用の制御点をあらかじめ算出しておく
							YsQuat qA, qB, qC;
							auto refRotateList = anime->m_FrameAnimeList[boneIdx]->m_Rotate;
							for(int h = 0; h<keyNum; h++) {
								// 球面二次補間フラグ
								if(refRotateList[h]->CalcType == 1) {
									int idx[4];
									idx[0] = h - 1;
									idx[1] = h;
									idx[2] = h + 1;
									idx[3] = h + 2;
									if(idx[0] < 0)idx[0] = 0;
									if(idx[2] >= (int)refRotateList.size())idx[2] = h;
									if(idx[3] >= (int)refRotateList.size())idx[3] = h;

									YsQuat::SquadSetup2(
										refRotateList[h]->qABC[0],
										refRotateList[h]->qABC[1],
										refRotateList[h]->qABC[2],
										refRotateList[idx[0]]->Value,
										refRotateList[idx[1]]->Value,
										refRotateList[idx[2]]->Value,
										refRotateList[idx[3]]->Value,
										refRotateList[idx[1]]->SplineValBefore,
										refRotateList[idx[1]]->SplineValAfter
										);
								}
							}
							*/
						}
						// 拡大キー
						else if( strcmp(strHead, "ScaleKey") == 0){
							if(boneIdx == -1){
								sErrMsg = "エラー";
								break;
							}
							// キー数
							int keyNum;
							RDint( &keyNum );
							for(int h=0;h<keyNum;h++){
								// キー追加
								YsAnimeKey_Vector3* key = new YsAnimeKey_Vector3();
								anime->m_FrameAnimeList[boneIdx]->m_Scale.push_back(key);
								while(1){
									RHead();
									if( strcmp(strHead, "End") == 0){
										break;
									}
									else if( strcmp(strHead, "Time") == 0){
										int n;
										RDint( &n );
										key->Time = (float)n;
									}
									else if( strcmp(strHead, "Scale") == 0){
										RD( &key->Value, 4*3 );
									}
									else if( strcmp(strHead, "CalcType") == 0){
										RDchar( &key->CalcType );
									}
									else if (strcmp(strHead, "PowVal") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValBefore") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValAfter") == 0) {	// ※未使用
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
								YsAnimeKey_Vector3::BakeCurve(anime->m_FrameAnimeList[boneIdx]->m_Scale, 0);
							}
						}
						// 座標キー
						else if( strcmp(strHead, "PosKey") == 0){
							if(boneIdx == -1){
								sErrMsg = "エラー";
								break;
							}
							// キー数
							int keyNum;
							RDint( &keyNum );
							for(int h=0;h<keyNum;h++){
								// キー追加
								YsAnimeKey_Vector3* key = new YsAnimeKey_Vector3();
								anime->m_FrameAnimeList[boneIdx]->m_Pos.push_back(key);
								while(1){
									RHead();
									if( strcmp(strHead, "End") == 0){
										break;
									}
									else if( strcmp(strHead, "Time") == 0){
										int n;
										RDint( &n );
										key->Time = (float)n;
									}
									else if( strcmp(strHead, "Pos") == 0){
										RD( &key->Value, 4*3 );
									}
									else if( strcmp(strHead, "CalcType") == 0){
										RDchar( &key->CalcType );
									}
									else if (strcmp(strHead, "PowVal") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValBefore") == 0) {	// ※未使用
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValAfter") == 0) {	// ※未使用
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
								YsAnimeKey_Vector3::BakeCurve(anime->m_FrameAnimeList[boneIdx]->m_Pos, 0);
							}
						}
					}
				}
			}
		}
		//===============================================
		// 物理演算データ
		//===============================================
		else if(strcmp(strHead, "Physics") == 0){

			YsXEDRigidBody rb;
			std::vector<YsXEDRigidBody> rbTbl;

			while(1){
				// 項目読み込み
				RHead();
				// 終了
				if(strcmp(strHead, "EndPhysics") == 0){
					outRigidBodyTbl.push_back(rbTbl);
					break;
				}
				// スケール値
				else if(strcmp(strHead, "SizeRatio") == 0){
					float SizeRatio;
					RDfloat(&SizeRatio);
				}
				// 重力値
				else if(strcmp(strHead, "Gravity") == 0){
					float gravity;
					RDfloat(&gravity);
				}
				//==============================
				// 剛体
				//==============================
				else if(strcmp(strHead, "RigidBody") == 0){
					// 剛体数
					int rbNum;
					RDint(&rbNum);

					for(int rn = 0; rn<rbNum; rn++){


						while(1){
							// 項目読み込み	
							RHead();
							// 終了
							if(strcmp(strHead, "EndRigid") == 0){
								rbTbl.push_back(rb);
								break;
							}
							// 剛体名
							else if(strcmp(strHead, "Name") == 0){
								RDstr(rb.rigidBodyName);
							}
							// 関連ボーン番号
							else if(strcmp(strHead, "BoneIdx") == 0){
								RDint(&rb.boneIndex);
							}
							// グループ番号
							else if(strcmp(strHead, "GroupNo") == 0){
								RDbyte(&rb.rigidBodyGroup);
							}
							// 判定グループマスク
							else if(strcmp(strHead, "UnCollisionGroup") == 0){
								RDword(&rb.unCollisionGroupFlag);
							}
							// 形状タイプ
							else if(strcmp(strHead, "Shape") == 0){
								RDbyte(&rb.shape);
							}
							// サイズ
							else if(strcmp(strHead, "Size") == 0){
								RDfloat3(&rb.size);
							}
							// 座標
							else if(strcmp(strHead, "Pos") == 0){
								RDfloat3(&rb.position);
							}
							// 回転
							else if(strcmp(strHead, "Rota") == 0){
								RDfloat3(&rb.rotation);
							}
							// 質量
							else if(strcmp(strHead, "Mass") == 0){
								RDfloat(&rb.mass);
							}
							// 移動減衰
							else if(strcmp(strHead, "MoveAtte") == 0){
								RDfloat(&rb.moveAttenuation);
							}
							// 回転減衰
							else if(strcmp(strHead, "RotaAtte") == 0){
								RDfloat(&rb.rotationAttenuation);
							}
							// 反発力
							else if(strcmp(strHead, "Repulsion") == 0){
								RDfloat(&rb.repulsion);
							}
							// 摩擦力
							else if(strcmp(strHead, "Fric") == 0){
								RDfloat(&rb.friction);
							}
							// 計算タイプ
							else if(strcmp(strHead, "CalcType") == 0){
								RDbyte(&rb.physicsCalcType);
							}
						}
					}
				}
				//==============================
				// ジョイント
				//==============================
				else if(strcmp(strHead, "Joint") == 0){
					// ジョイント数
					int jtNum;
					RDint(&jtNum);

					for(int jn = 0; jn<jtNum; jn++){


						while(1){
							// 項目読み込み
							RHead();
							// 終了
							if(strcmp(strHead, "EndJoint") == 0){
								break;
							}
							else if(strcmp(strHead, "Name") == 0){
								std::string jointName;
								RDstr(jointName);
							}
							else if(strcmp(strHead, "Type") == 0){
								char jointType;
								RDchar(&jointType);
							}
							else if(strcmp(strHead, "CFM") == 0){
								float CFM;
								RDfloat(&CFM);
							}
							else if(strcmp(strHead, "ERP") == 0){
								float ERP;
								RDfloat(&ERP);
							}
							else if(strcmp(strHead, "RigidAIdx") == 0){
								int rigidBodyAIndex;
								RDint(&rigidBodyAIndex);
							}
							else if(strcmp(strHead, "RigidBIdx") == 0){
								int rigidBodyBIndex;
								RDint(&rigidBodyBIndex);
							}
							else if(strcmp(strHead, "Pos") == 0){
								YsVec3 position;
								RDfloat3(&position);
							}
							else if(strcmp(strHead, "Rota") == 0){
								YsVec3 rotation;
								RDfloat3(&rotation);
							}
							else if(strcmp(strHead, "MoveLimMin") == 0){
								YsVec3 moveLimitationMin;
								RDfloat3(&moveLimitationMin);
							}
							else if(strcmp(strHead, "MoveLimMax") == 0){
								YsVec3 moveLimitationMax;
								RDfloat3(&moveLimitationMax);
							}
							else if(strcmp(strHead, "RotaLimMin") == 0){
								YsVec3 rotationLimitationMin;
								RDfloat3(&rotationLimitationMin);
							}
							else if(strcmp(strHead, "RotaLimMax") == 0){
								YsVec3 rotationLimitationMax;
								RDfloat3(&rotationLimitationMax);
							}
							else if(strcmp(strHead, "SpringMoveVal") == 0){
								YsVec3 springMoveCoefficient;
								RDfloat3(&springMoveCoefficient);
							}
							else if(strcmp(strHead, "SpringRotaVal") == 0){
								YsVec3 springRotationCoefficient;
								RDfloat3(&springRotationCoefficient);
							}
							else if(strcmp(strHead, "SpringMoveDamp") == 0){
								YsVec3 springMoveDamping;
								RDfloat3(&springMoveDamping);
							}
							else if(strcmp(strHead, "SpringRotaDamp") == 0){
								YsVec3 springRotationDamping;
								RDfloat3(&springRotationDamping);
							}
						}
					}
				}
			}
		}

		// その他
//		else{
//			RBack();
//		}
	}

	fclose(fp);

	return true;
}
