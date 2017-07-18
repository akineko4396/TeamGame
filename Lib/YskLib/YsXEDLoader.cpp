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
	// �p�X�A�t�@�C�������o
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

#pragma region �ǂݍ��݊֐�
	char strHead[256];
	// �ėp
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
	// �T�C�Y(int)�{������
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
				fread(&len, 1, 1, fp);		// ������
				// �J�n����
				BYTE st = 0;
				fread(&st, 1, 1, fp);
				// �w�b�_�m�F�@���s
				if(st == 0x8C){
					break;
				}
				// �s���ȃw�b�_�B�����߂��A�w�b�_�܂Ői�߂�
				else{
					fseek(fp, -1, SEEK_CUR);
				}
			}
			// ������
			fread(strHead, len, 1, fp);
			strHead[len] = '\0';
			// �I�[����
			BYTE term[2];
			fread(term, 2, 1, fp);
			if(term[0] == 0x60 && term[1] == 0x99){	// �I�[�m�F
				break;
			}
			// �I�[�����ł͂Ȃ��B�ŏ�(�������擾��)�܂Ŋ����߂�
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
	// �@
	while(1){
		// ���ړǂݍ���
		RHead();
		// �I��
		if( strcmp(strHead, "End") == 0){
			break;
		}
		//===========================================
		// ���b�V���f�[�^
		//===========================================
		else if( strcmp(strHead, "XEDMeshData") == 0){
			// �����b�V����(int)
			int meshNum;
			RDint(&meshNum);
			outModelTbl.resize(meshNum);

			// ���b�V�����[�v
			for(int mi = 0; mi<meshNum; mi++) {
				// YsSingleModel�쐬
				outModelTbl[mi] = std::make_shared<YsSingleModel>();
				SPtr<YsSingleModel> pModel = outModelTbl[mi];

				// ���̒���YsMesh�쐬
				pModel->SetMesh(std::make_shared<YsMesh>());
				bool bSkinMesh = false;

				// ���_�`��
				YsVertexTypeData vtd = YsVertex_Pos_UV_TBN_Skin::GetVertexTypeData();
				YsVertexTypeData vtdST = YsVertex_Pos_UV_TBN::GetVertexTypeData();
				// ���_�f�[�^
				int numVers = 0;	// ���_��
				std::vector<YsVertex_Pos_UV_TBN_Skin> verTbl;
				// �ʃf�[�^
				/*
				struct XEDFace {
					UINT p1;		// ���_�P�ւ̃C���f�b�N�X
					UINT p2;		// ���_�Q�ւ̃C���f�b�N�X
					UINT p3;		// ���_�R�ւ̃C���f�b�N�X
				};
				*/
				int numFaces = 0;	// �ʐ�
				std::vector<YsMesh::Face> face;		// �ʔz��

				// Subset�f�[�^
				std::vector<YsMeshSubset> subset;

				// �����b�V����(int+������)
				std::string meshName;
				RDstr(meshName);
				pModel->SetName(meshName);

				while(1) {
					// ���ړǂݍ���
					RHead();
					// �I��
					if(strcmp(strHead, "EndMesh") == 0) {

						// �X�L�����b�V���`�����X�^�e�B�b�N���b�V���`����
						YsVertexTypeData* pVTD;
						if(bSkinMesh){
							pVTD = &vtd;
						}
						else{
							pVTD = &vtdST;
						}

						// �ǉ����쐬
						for(int i = 0; i<numFaces; i++) {

							UINT f1 = face[i].idx[0];
							UINT f2 = face[i].idx[1];
							UINT f3 = face[i].idx[2];

							// ���_�̐ڐ��A�]�@���Z�o
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
							// ���_�R�s�[
							verTblST = std::make_unique<YsVertex_Pos_UV_TBN[]>(verTbl.size());

							for(UINT vti = 0; vti<verTbl.size();vti++){
								verTblST[vti] = verTbl[vti];
							}

							// ���b�V���쐬
							if (pModel->GetMesh()->Create(&verTblST[0], verTbl.size(), &face[0], face.size(), subset) == false) {
								fclose(fp);
//								Release();
								return false;
							}
						}
						else {

							// ���b�V���쐬
							if (pModel->GetMesh()->Create(&verTbl[0], verTbl.size(), &face[0], face.size(), subset) == false) {
								fclose(fp);
//								Release();
								return false;
							}
						}



						break;
					}
					// ���_
					else if(strcmp(strHead, "Vertex") == 0) {
						// �����_��(int)
						RDint(&numVers);
						// ���_�f�[�^�p
						verTbl.resize(numVers);

						// 
						while(1) {
							// ���ړǂݍ���
							RHead();
							// �I��
							if(strcmp(strHead, "EndVertex") == 0) {
								break;
							}
							// ���W
							else if(strcmp(strHead, "Pos") == 0) {
								for(int vi = 0; vi<numVers; vi++) {
									// ��3D�x�N�g��(float*3)
									RDfloat3(&verTbl[vi].Pos);
								}
							}
							// �@��
							else if(strcmp(strHead, "Normal") == 0) {
								for(int vi = 0; vi<numVers; vi++) {
									// ��3D�x�N�g��(float*3)
									RDfloat3(&verTbl[vi].Normal);
								}
							}
							// UV
							else if(strcmp(strHead, "UV") == 0) {
								for(int vi = 0; vi<numVers; vi++) {
									// ��2D�x�N�g��(float*2)
									RDfloat2(&verTbl[vi].UV);
								}
							}
							// �{�[���E�F�C�g�n
							else if(strcmp(strHead, "BoneWeight") == 0) {
								for(int vi = 0; vi<numVers; vi++) {
									// ������
									for(int wi = 0; wi<4; wi++) {
										verTbl[vi].BlendIndices[wi] = 0xFFFF;
										verTbl[vi].BlendWeight[wi] = 0;
									}

									// ���e����(int)
									int wCnt;
									RDint(&wCnt);
									for(int wi = 0; wi<wCnt; wi++) {
										// ���{�[��Index(unsigned short)
										RDword(&verTbl[vi].BlendIndices[wi]);
										// ���d��(float)
										RDfloat(&verTbl[vi].BlendWeight[wi]);
									}

									// �X�L�����b�V�����H
									if(verTbl[vi].BlendIndices[0] != 0xFFFF){
										bSkinMesh = true;

										// �d�ݐ��K��
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
					// ��
					else if(strcmp(strHead, "Face") == 0) {

						// ���ʐ�(int)
						RDint(&numFaces);
						face.resize(numFaces);

						// 
						while(1) {
							// ���ړǂݍ���
							RHead();
							// �I��
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
					// �}�e���A��
					else if(strcmp(strHead, "Material") == 0) {
						// ���}�e���A����(int)
						int numMate;
						RDint(&numMate);
						// �}�e���A���f�[�^
						std::vector<YsMaterial> material(numMate);
						// �T�u�Z�b�g�f�[�^
						subset.resize(numMate);	// XED�̃T�u�Z�b�g�̓}�e���A�����Ɠ����ɂ��Ă�

						for(int mai = 0; mai<numMate; mai++) {
							// ���}�e���A����(int+������)
							//������Ȃ�����ǂݍ��ނ����Ŏ̂Ă�
							std::string mateName;
							RDstr(mateName);

							// 
							while(1) {
								// ���ړǂݍ���
								RHead();
								// �I��
								if(strcmp(strHead, "EndMateData") == 0){
									// �ݒ�
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
									// ���Ƃ肠����CSV�Ƃ��ĉ�͂��A�ꕔ�̍\����ݒ�Ƃ��Ďg�p
									YsCsv csv;
									csv.LoadFromText(material[mai].ExtScript);
									// �c���[�v
									for(UINT ci = 0;ci < csv.m_Tbl.size();ci++){
										const std::string strType = csv.m_Tbl[ci][0];
										// �����}�b�v���̋���
										if(strType == "ParallaxVal"){
											material[mai].ParallaxVal = (float)std::stof(csv.m_Tbl[ci][1]);
										}
									}
									*/
								}
								else if(strcmp(strHead, "Texture") == 0) {
									RDstr(material[mai].TextureFileName);

									// �e�N�X�`���ǂݍ���
									std::string strFile = filePath + material[mai].TextureFileName;
									material[mai].TexSet.LoadTextureSet(strFile);
								}
							}

						}

					}
				}

			}

			// �X�L�����b�V���ƃX�^�e�B�b�N���b�V���̎Q�Ƃ𕪂����e�[�u���쐬
//			CreateRefMeshTbl();

		}
		//===========================================
		// �{�[���v���p�e�B
		//===========================================
		else if( strcmp(strHead, "BoneProperty") == 0){
			// ���{�[����
			int boneNum;
			RDint( &boneNum );

			// �{�[�������\�z�Ȃ�\�z����
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
				// ���{�[����
				std::string boneName;
				RDstr( boneName );

				SPtr<YsGameModel::BoneNode> boneNode = outBoneTree[bone];
				boneNode->BoneName = boneName;	// �{�[����
				boneNode->OffsetID = bone;		// OffsetID

				// 
				while(1){
					// ���ړǂݍ���
					RHead();
					// �I��
					if( strcmp(strHead, "End") == 0){
						// �f�[�^�쐬
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
					// IK�f�[�^
					else if(strcmp(strHead, "IKData") == 0) {
						while(1){
							// ���ړǂݍ���
							RHead();
							// �I��
							if(strcmp(strHead, "EndIKData") == 0){
								break;
							}
							// �^�[�Q�b�g�{�[���ԍ�
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
								RDint(&cnt);	// Link��

								boneNode->IK.LinkList.resize(cnt);

								for(int lki = 0; lki<cnt; lki++){
									while(1){
										// ���ړǂݍ���
										RHead();
										// �I��
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
					// �t�^�f�[�^
					else if(strcmp(strHead, "ProvideData") == 0){
						while(1){
							// ���ړǂݍ���
							RHead();
							// �I��
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
					// �E�F�C�gBOX�f�[�^
					else if(strcmp(strHead, "WeightBoxData") == 0){

						while(1){
							// ���ړǂݍ���
							RHead();
							// �I��
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
								// �����b�V����
								int meshCnt = 0;
								RDint(&meshCnt);
								for(int imi = 0; imi<meshCnt; imi++){
									// �����b�V��No
									int meshNo;
									RDint(&meshNo);

									// ���������_���X�g
									int ignoreCnt;
									RDint(&ignoreCnt);
									for(int ivi = 0; ivi<ignoreCnt; ivi++){
										// �����_No
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
		// �A�j���f�[�^
		//===========================================
		else if( strcmp(strHead, "AnimeName") == 0){
			// �A�j���ǉ�
			SPtr<YsAnimationSet> anime = std::make_shared<YsAnimationSet>();
			outAnimeTbl.push_back( anime );

			// ���A�j����
			RDstr(anime->m_AnimeName);
			// ���A�j����
			int animeLen;
			RDint( &animeLen );
			anime->m_TicksPerSecond = 60;
			anime->m_AnimeLen = animeLen;
			anime->CreateFrameAnimeList(outBoneTree.size() );

			// �A�j��������
			// �擪��"//"�Ŏn�܂閼�O�́A�A�j���[�L�[�͓ǂݍ��܂Ȃ�
			bool bEnable = true;
			if(anime->m_AnimeName.substr(0, 2) == "//"){
				bEnable = false;
			}

			bool bBlendAnimeMode = false;
			float fBlendAnimeWeight = 1.0f;

			// 
			while(1){
				// ���ړǂݍ���
				RHead();
				// �I��
				if( strcmp(strHead, "End") == 0){
					// �����A�j���͏���
					if(bEnable == false){
						outAnimeTbl.back()->Release();
//						delete GetAnimeList().back();
						outAnimeTbl.pop_back();
					}

					// �������[�h�A�j���Ȃ�A��������
					if(bBlendAnimeMode){
						for(UINT bi = 0; bi < anime->m_FrameAnimeList.size(); bi++){
							YsKeyframeAnime* srcAnime = anime->m_FrameAnimeList[bi];
							YsKeyframeAnime* destAnime = outAnimeTbl.back()->m_FrameAnimeList[bi];

							if(fBlendAnimeWeight > 0){
								// ��]�L�[���P�ł�����΁A�Ƃ肠���������ւ��Ł@���̂��ɏd�݂��l��������
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
				// �����A�j�����[�h
				else if(strcmp(strHead, "BlendAnimeMode") == 0) {
					int nn;
					RDint(&nn);
					bBlendAnimeMode = (nn == 1 ? true : false);

					// �������[�h�̃A�j���̓��X�g���珜�O
					if(bEnable && bBlendAnimeMode){
						outAnimeTbl.pop_back();
					}
				}
				// �����A�j���̏d��(���ݖ��g�p)
				else if(strcmp(strHead, "BlendAnimeWeight") == 0) {
					RDfloat(&fBlendAnimeWeight);
				}
				// �X�N���v�g�L�[
				else if( strcmp(strHead, "ScriptKey") == 0){
					// �L�[��
					int KeyNum;
					RDint( &KeyNum );
					// �f�[�^
					for(int i=0;i<KeyNum;i++){
						// �L�[�ǉ�
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
				// �{�[���f�[�^
				else if( strcmp(strHead, "BoneData") == 0){
					std::string sBoneName = "";
					int boneIdx = -1;
					// �A
					while(1){
						RHead();
						if( strcmp(strHead, "End") == 0){
							break;
						}
						// �{�[����
						else if( strcmp(strHead, "BoneName") == 0){
							RDstr( sBoneName );

							// �{�[������
							boneIdx = -1;
							for (UINT bi = 0; bi < outBoneTree.size(); bi++) {
								if (sBoneName == outBoneTree[bi]->BoneName) {
									boneIdx = bi;
									break;
								}
							}
//							boneIdx = SearchBoneIndex( sBoneName );
						}
						// ��]�L�[
						else if( strcmp(strHead, "RotateKey") == 0){
							if(boneIdx == -1){
								sErrMsg = "�G���[";
								break;
							}
							// �L�[��
							int keyNum;
							RDint( &keyNum );
							for(int h=0;h<keyNum;h++){
								// �L�[�ǉ�
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
									else if (strcmp(strHead, "PowVal") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValBefore") == 0) {// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValAfter") == 0) {// �����g�p
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

							// �Ȑ��n�̕�Ԃ͏������d���̂ŁA�Ȑ��ɂȂ�悤�Ȑ��`��ԂƂ��ďĂ�����
							if(bakeCurve){
								YsAnimeKey_Quaternion::BakeCurve(anime->m_FrameAnimeList[boneIdx]->m_Rotate, 0);
							}

							/*
							// ���ʓ񎟕�ԗp�̐���_�����炩���ߎZ�o���Ă���
							YsQuat qA, qB, qC;
							auto refRotateList = anime->m_FrameAnimeList[boneIdx]->m_Rotate;
							for(int h = 0; h<keyNum; h++) {
								// ���ʓ񎟕�ԃt���O
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
						// �g��L�[
						else if( strcmp(strHead, "ScaleKey") == 0){
							if(boneIdx == -1){
								sErrMsg = "�G���[";
								break;
							}
							// �L�[��
							int keyNum;
							RDint( &keyNum );
							for(int h=0;h<keyNum;h++){
								// �L�[�ǉ�
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
									else if (strcmp(strHead, "PowVal") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValBefore") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValAfter") == 0) {	// �����g�p
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

							// �Ȑ��n�̕�Ԃ͏������d���̂ŁA�Ȑ��ɂȂ�悤�Ȑ��`��ԂƂ��ďĂ�����
							if(bakeCurve){
								YsAnimeKey_Vector3::BakeCurve(anime->m_FrameAnimeList[boneIdx]->m_Scale, 0);
							}
						}
						// ���W�L�[
						else if( strcmp(strHead, "PosKey") == 0){
							if(boneIdx == -1){
								sErrMsg = "�G���[";
								break;
							}
							// �L�[��
							int keyNum;
							RDint( &keyNum );
							for(int h=0;h<keyNum;h++){
								// �L�[�ǉ�
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
									else if (strcmp(strHead, "PowVal") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValBefore") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if(strcmp(strHead, "SplineValAfter") == 0) {	// �����g�p
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

							// �Ȑ��n�̕�Ԃ͏������d���̂ŁA�Ȑ��ɂȂ�悤�Ȑ��`��ԂƂ��ďĂ�����
							if(bakeCurve){
								YsAnimeKey_Vector3::BakeCurve(anime->m_FrameAnimeList[boneIdx]->m_Pos, 0);
							}
						}
					}
				}
			}
		}
		//===============================================
		// �������Z�f�[�^
		//===============================================
		else if(strcmp(strHead, "Physics") == 0){

			YsXEDRigidBody rb;
			std::vector<YsXEDRigidBody> rbTbl;

			while(1){
				// ���ړǂݍ���
				RHead();
				// �I��
				if(strcmp(strHead, "EndPhysics") == 0){
					outRigidBodyTbl.push_back(rbTbl);
					break;
				}
				// �X�P�[���l
				else if(strcmp(strHead, "SizeRatio") == 0){
					float SizeRatio;
					RDfloat(&SizeRatio);
				}
				// �d�͒l
				else if(strcmp(strHead, "Gravity") == 0){
					float gravity;
					RDfloat(&gravity);
				}
				//==============================
				// ����
				//==============================
				else if(strcmp(strHead, "RigidBody") == 0){
					// ���̐�
					int rbNum;
					RDint(&rbNum);

					for(int rn = 0; rn<rbNum; rn++){


						while(1){
							// ���ړǂݍ���	
							RHead();
							// �I��
							if(strcmp(strHead, "EndRigid") == 0){
								rbTbl.push_back(rb);
								break;
							}
							// ���̖�
							else if(strcmp(strHead, "Name") == 0){
								RDstr(rb.rigidBodyName);
							}
							// �֘A�{�[���ԍ�
							else if(strcmp(strHead, "BoneIdx") == 0){
								RDint(&rb.boneIndex);
							}
							// �O���[�v�ԍ�
							else if(strcmp(strHead, "GroupNo") == 0){
								RDbyte(&rb.rigidBodyGroup);
							}
							// ����O���[�v�}�X�N
							else if(strcmp(strHead, "UnCollisionGroup") == 0){
								RDword(&rb.unCollisionGroupFlag);
							}
							// �`��^�C�v
							else if(strcmp(strHead, "Shape") == 0){
								RDbyte(&rb.shape);
							}
							// �T�C�Y
							else if(strcmp(strHead, "Size") == 0){
								RDfloat3(&rb.size);
							}
							// ���W
							else if(strcmp(strHead, "Pos") == 0){
								RDfloat3(&rb.position);
							}
							// ��]
							else if(strcmp(strHead, "Rota") == 0){
								RDfloat3(&rb.rotation);
							}
							// ����
							else if(strcmp(strHead, "Mass") == 0){
								RDfloat(&rb.mass);
							}
							// �ړ�����
							else if(strcmp(strHead, "MoveAtte") == 0){
								RDfloat(&rb.moveAttenuation);
							}
							// ��]����
							else if(strcmp(strHead, "RotaAtte") == 0){
								RDfloat(&rb.rotationAttenuation);
							}
							// ������
							else if(strcmp(strHead, "Repulsion") == 0){
								RDfloat(&rb.repulsion);
							}
							// ���C��
							else if(strcmp(strHead, "Fric") == 0){
								RDfloat(&rb.friction);
							}
							// �v�Z�^�C�v
							else if(strcmp(strHead, "CalcType") == 0){
								RDbyte(&rb.physicsCalcType);
							}
						}
					}
				}
				//==============================
				// �W���C���g
				//==============================
				else if(strcmp(strHead, "Joint") == 0){
					// �W���C���g��
					int jtNum;
					RDint(&jtNum);

					for(int jn = 0; jn<jtNum; jn++){


						while(1){
							// ���ړǂݍ���
							RHead();
							// �I��
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

		// ���̑�
//		else{
//			RBack();
//		}
	}

	fclose(fp);

	return true;
}
