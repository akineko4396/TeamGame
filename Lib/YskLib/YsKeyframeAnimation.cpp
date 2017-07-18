#include "YskLib.h"

using namespace YskLib;


// �x�W�F�Ȑ��̎w��X�̈ʒu����t�����߁A������g��Y�����߂�
static float CalcBezierY_FromX(float x, float x1, float y1, float x2, float y2) {

	// �}eps �͈͓̔��Ɏ��܂����甲����
	static const float eps = 0.001f;

	float down = 0.0f;
	float up = 1.0f;

	// �񕪒T���Ŏw��l�ɋ߂Â��܂Ōv�Z
	float t;
	while(true) {
		t = (down + up) / 2;
		// �v�Z
		float px = 3 * (1 - t)*(1 - t)*t*x1 + 3 * (1 - t)*t*t*x2 + t*t*t;

		// �w��l�͈̔͊O�Ȃ玟
		if(px < x - eps) {
			down = t;
		}
		// �w��l�͈̔͊O�Ȃ玟
		else if(px > x + eps) {
			up = t;
		}
		// �͈͓�
		else {
			break;
		}
	}

	return 3 * (1 - t)*(1 - t)*t*y1 + 3 * (1 - t)*t*t*y2 + t*t*t;
}



bool YsAnimeKey_Quaternion::InterpolationFromTime(const std::vector<YsAnimeKey_Quaternion*>& keyList, double time, YsQuat &outQ, UINT* nowKeyIndex)
{
	if(keyList.size() == 1) {		// �L�[���P�̂Ƃ�
		// ���̓��e���g�p
		outQ = keyList.front()->Value;
		return true;
	}
	else if(keyList.size() >= 2) {			// �L�[������

		// �ŏ��̃L�[����O�Ȃ�A�擪��Ԃ�
		if(time < keyList[0]->Time) {
			outQ = keyList.front()->Value;
			return true;
		}
		// �Ō�̃L�[���߂��Ă�Ƃ�
		if(time >= keyList.back()->Time) {
			// �Ō�̃f�[�^��Ԃ�
			outQ = keyList.back()->Value;
			return true;
		}

		// �ʒu������
		UINT i = 0;
		if(nowKeyIndex == nullptr){
			for(i = 0; i<keyList.size(); i++) {
				// ��������̃L�[�𔭌�
				if(time < keyList[i]->Time) {
					break;
				}
			}
		}
		else{

			// �O���Index
			UINT idx = *nowKeyIndex;
			if(idx >= keyList.size())idx = keyList.size() - 1;

			i = idx;
			int searchEndIndex = (int)idx - 1;
			if(searchEndIndex < 0)searchEndIndex = keyList.size() - 2;

			while(1) {
				if((int)i >= (int)keyList.size() - 1)i = 0;		// �Ō��1�O�܂ōs���΁A�ŏ��ɖ߂�
				if(i == searchEndIndex)break;

				// �͈͓��H
				if(time >= keyList[i]->Time && time < keyList[i + 1]->Time) {
					break;
				}

				i++;
			}

			*nowKeyIndex = (unsigned int)i;
			i++;
		}


		// �v�Z
		if(i < keyList.size()){
			// �䗦�Z�o
			float sa = (keyList[i]->Time - keyList[i - 1]->Time);
			double f = time - keyList[i - 1]->Time;

			if(f == 0.0){
				outQ = keyList[i - 1]->Value;
			}
			else{

				// ���ʐ��`���
				if(keyList[i - 1]->CalcType == 0) {
					YsQuat::Slerp(outQ,
						keyList[i - 1]->Value,
						keyList[i]->Value,
						(float)(f * (1.0f / sa)));
				}
				// ���ʂQ�����(����Ȃ�ɏd������A�������y���������ꍇ�͎��O�ɏĂ����ݐ��`��Ԃɂ���̂��ǂ�(BaseCurve�֐�))
				else if(keyList[i - 1]->CalcType == 1) {
					float fT = (float)(f * (1.0f / sa));

					int idx[4];
					idx[0] = i - 2;
					idx[1] = i - 1;
					idx[2] = i;
					idx[3] = i + 1;
					if (idx[0] < 0)idx[0] = 0;
					if (idx[3] >= (int)keyList.size())idx[3] = i;

					// ����|�C���g�Z�o(�����������l����Ȃ�A����͎��O�v�Z�����Ă�ق����ǂ�)
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
				// �x�W�F�Ȑ����(����Ȃ�ɏd������A�������y���������ꍇ�͎��O�ɏĂ����ݐ��`��Ԃɂ���̂��ǂ�(BaseCurve�֐�))
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
	// ��x�R�s�[
	auto copyRefRotateList = keyList;
	int keyNum = (int)keyList.size();
	for (int h = 0; h < keyNum; h++) {
		// ��ԃ^�C�v���Ȑ��n�̏ꍇ
		if (copyRefRotateList[h]->CalcType != 0) {


			// �Ō�̃L�[�łȂ���΁A�Ă�����
			if (h < (int)copyRefRotateList.size() - 1) {
				// ����(�����Q���Ah��h+1�Ԗڂ̃L�[)
				int cnt = (int)copyRefRotateList[h + 1]->Time - (int)copyRefRotateList[h]->Time + 1;

				// ���ʓ񎟕�Ԃł́A���炩���ߎ��ӂ̃L�[���琧��|�C���g�����߂�Ă���
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

					// ���ʓ񎟕�Ԃ̐���|�C���g���Z�o
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
				// �ŏ�(h)�ƌ㕔(h+1)�L�[�ȊO
				for (int bki = 1; bki<cnt - 1; bki++) {
					if (bakeInterval <= 0) {
						bakeInterval = bakeSpace;

						int time = (int)copyRefRotateList[h]->Time + bki;
						float sa = (copyRefRotateList[h + 1]->Time - copyRefRotateList[h]->Time);
						double f = time - copyRefRotateList[h]->Time;
						float fT = (float)(f * (1.0f / sa));

						YsQuat outQ;
						// ��Ԍv�Z
						UINT idx = h;
						YsAnimeKey_Quaternion::InterpolationFromTime(copyRefRotateList, time, outQ, &idx);

						// ���ʂ��L�[�Ƃ��Ēǉ�
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

			// ���̃L�[�͋Ȑ�����߂Đ��`�ɖ߂�
			copyRefRotateList[h]->CalcType = 0;

		}

	}

	// �\�[�g
	std::sort(keyList.begin(), keyList.end(),
		[](const YsAnimeKey_Quaternion* v1, const YsAnimeKey_Quaternion* v2) {
			return v1->Time < v2->Time;
		}
	);
}

bool YsAnimeKey_Vector3::InterpolationFromTime(const std::vector<YsAnimeKey_Vector3*>& keyList, double time, YsVec3 &outV, UINT* nowKeyIndex)
{
	if(keyList.size() == 1) {		// �L�[���P�̂Ƃ�
		// ���̓��e���g�p
		outV.Set( keyList.front()->Value );
		return true;
	}
	else if(keyList.size() >= 2) {			// �L�[������
		// �ŏ��̃L�[����O�Ȃ�A�擪��Ԃ�
		if (time < keyList[0]->Time) {
			outV.Set(keyList.front()->Value);
			return true;
		}
		// �Ō�̃L�[���߂��Ă�Ƃ�
		if(time >= keyList.back()->Time) {
			// �Ō�̃f�[�^��Ԃ�
			outV.Set(keyList.back()->Value);
			return true;
		}



		// �ʒu������
		UINT i = 0;
		if(nowKeyIndex == nullptr){
			for(i = 0; i<keyList.size(); i++) {
				// ��������̃L�[�𔭌�
				if(time < keyList[i]->Time) {
					break;
				}
			}
		}
		else{

			// �O���Index
			UINT idx = *nowKeyIndex;
			if(idx >= keyList.size())idx = keyList.size() - 1;

			i = idx;
			int searchEndIndex = (int)idx - 1;
			if(searchEndIndex < 0)searchEndIndex = keyList.size() - 2;

			while(1) {
				if((int)i >= (int)keyList.size() - 1)i = 0;		// �Ō��1�O�܂ōs���΁A�ŏ��ɖ߂�
				if(i == searchEndIndex)break;

				// �͈͓��H
				if(time >= keyList[i]->Time && time < keyList[i + 1]->Time) {
					break;
				}

				i++;
			}

			*nowKeyIndex = (unsigned int)i;
			i++;
		}

		if(i < keyList.size()){
			// �䗦�Z�o
			float sa = (keyList[i]->Time - keyList[i - 1]->Time);
			double f = time - keyList[i - 1]->Time;
			if(f == 0){
				outV.Set( keyList[i - 1]->Value );
			}
			else{
				// ���`���
				if(keyList[i - 1]->CalcType == 0) {
					YsVec3::Lerp(outV,
						keyList[i - 1]->Value,
						keyList[i]->Value,
						(float)(f * (1.0f / sa)));
				}
				// Catmull-Rom�X�v���C�����(����Ȃ�ɏd������A�������y���������ꍇ�͎��O�ɏĂ����ݐ��`��Ԃɂ���̂��ǂ�(BaseCurve�֐�))
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
				// �x�W�F�Ȑ����(����Ȃ�ɏd������A�������y���������ꍇ�͎��O�ɏĂ����ݐ��`��Ԃɂ���̂��ǂ�(BaseCurve�֐�))
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
	// ��x�R�s�[
	auto copyRefPosList = keyList;
	int keyNum = (int)keyList.size();
	for(int h = 0; h < keyNum; h++) {
		// ��ԃ^�C�v���Ȑ��n�̏ꍇ
		if(copyRefPosList[h]->CalcType != 0) {


			// �Ō�̃L�[�łȂ���΁A�Ă�����
			if(h < (int)copyRefPosList.size() - 1) {
				// ����(�����Q���Ah��h+1�Ԗڂ̃L�[)
				int cnt = (int)copyRefPosList[h + 1]->Time - (int)copyRefPosList[h]->Time + 1;

				int bakeInterval = bakeSpace;
				// �ŏ�(h)�ƌ㕔(h+1)�L�[�ȊO
				for(int bki = 1; bki<cnt - 1; bki++) {
					if(bakeInterval <= 0) {
						bakeInterval = bakeSpace;

						int time = (int)copyRefPosList[h]->Time + bki;
						float sa = (copyRefPosList[h + 1]->Time - copyRefPosList[h]->Time);
						double f = time - copyRefPosList[h]->Time;
						float fT = (float)(f * (1.0f / sa));

						YsVec3 outV;
						// ��Ԍv�Z
						UINT idx = h;
						YsAnimeKey_Vector3::InterpolationFromTime(copyRefPosList, time, outV, &idx);

						// ���ʂ��L�[�Ƃ��Ēǉ�
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
			// ���̃L�[�͋Ȑ�����߂Đ��`�ɂ���
			copyRefPosList[h]->CalcType = 0;

		}

	}

	// �\�[�g
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
	// �ėp
#pragma region �֗��֐�
	auto RD = [fp](void* p, int size) {fread(p, size, 1, fp); };
	auto RDchar = [fp](char* c) {fread(c, 1, 1, fp); };
	auto RDbyte = [fp](BYTE* c) {fread(c, 1, 1, fp); };
	auto RDword = [fp](WORD* w) {fread(w, 2, 1, fp); };
	auto RDint = [fp](int* n) {fread(n, 4, 1, fp); };
	auto RDfloat = [fp](float* f) {fread(f, 4, 1, fp); };
	auto RDfloat3 = [fp](YsVec3* v) {fread(v, 12, 1, fp); };
	auto RDdouble = [fp](double* d) {fread(d, 8, 1, fp); };
	// �T�C�Y(int)�{������
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
				fread(&len, 1, 1, fp);		// ������
											// �J�n����
				BYTE st = 0;
				fread(&st, 1, 1, fp);
				// �w�b�_�m�F�@���s
				if (st == 0x8C) {
					break;
				}
				// �s���ȃw�b�_�B�����߂��A�w�b�_�܂Ői�߂�
				else {
					fseek(fp, -1, SEEK_CUR);
				}
			}
			// ������
			fread(strHead, len, 1, fp);
			strHead[len] = '\0';
			// �I�[����
			BYTE term[2];
			fread(term, 2, 1, fp);
			if (term[0] == 0x60 && term[1] == 0x99) {	// �I�[�m�F
				break;
			}
			// �I�[�����ł͂Ȃ��B�ŏ�(�������擾��)�܂Ŋ����߂�
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
		// ���ړǂݍ���
		RHead();
		// �I��
		if (strcmp(strHead, "EndXMD") == 0) {
			break;
		}
		//===========================================
		// �A�j���f�[�^
		//===========================================
		else if (strcmp(strHead, "AnimeName") == 0) {
#pragma region �A�j��
			// �A�j���ǉ�
			//			SPtr<XEDAnimation> anime(new XEDAnimation());
			//			m_AnimeList.push_back(anime);
			YsAnimationSet* anime = this;

			// ���A�j����
			RDstr(anime->m_AnimeName);
			// �A�j�����w�肪����ꍇ�́A�����ւ�
			if (registerAnimeName) {
				anime->m_AnimeName = registerAnimeName;
			}

			// ���A�j����
			int animeLen;
			RDint(&animeLen);
			anime->m_TicksPerSecond = 60;
			anime->m_AnimeLen = animeLen;

			while (1) {
				// ���ړǂݍ���
				RHead();
				// �I��
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
				// �X�N���v�g�L�[
				else if (strcmp(strHead, "ScriptKey") == 0) {
					// �L�[��
					int KeyNum;
					RDint(&KeyNum);
					// �f�[�^
					for (int i = 0; i<KeyNum; i++) {
						// �L�[�ǉ�
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
				// �{�[���f�[�^
				else if (strcmp(strHead, "BoneData") == 0) {
					//					std::string sBoneName = "";
					//					int boneIdx = -1;

					YsKeyframeAnime* aniSet;
					// �A
					while (1) {
						RHead();
						if (strcmp(strHead, "End") == 0) {
							break;
						}
						// �{�[����
						else if (strcmp(strHead, "BoneName") == 0) {
							// �ǉ�
							aniSet = new YsKeyframeAnime();
							anime->m_FrameAnimeList.push_back(aniSet);

							// 
							std::string boneName;
							RDstr(boneName);
						}
						// ��]�L�[
						else if (strcmp(strHead, "RotateKey") == 0) {
							// �L�[��
							int keyNum;
							RDint(&keyNum);
							for (int h = 0; h<keyNum; h++) {
								// �L�[�ǉ�
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
									else if (strcmp(strHead, "PowVal") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValBefore") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValAfter") == 0) {	// �����g�p
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

							// �Ȑ��n�̕�Ԃ͏������d���̂ŁA�Ȑ��ɂȂ�悤�Ȑ��`��ԂƂ��ďĂ�����
							if(bakeCurve){
								YsAnimeKey_Quaternion::BakeCurve(aniSet->m_Rotate, 0);
							}
						}
						// �g��L�[
						else if (strcmp(strHead, "ScaleKey") == 0) {
							// �L�[��
							int keyNum;
							RDint(&keyNum);
							for (int h = 0; h<keyNum; h++) {
								// �L�[�ǉ�
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
									else if (strcmp(strHead, "PowVal") == 0) {			// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValBefore") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValAfter") == 0) {	// �����g�p
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
								YsAnimeKey_Vector3::BakeCurve(aniSet->m_Scale, 0);
							}
						}
						// ���W�L�[
						else if (strcmp(strHead, "PosKey") == 0) {
							// �L�[��
							int keyNum;
							RDint(&keyNum);
							for (int h = 0; h<keyNum; h++) {
								// �L�[�ǉ�
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
									else if (strcmp(strHead, "PowVal") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValBefore") == 0) {	// �����g�p
										float ff;
										RDfloat(&ff);
									}
									else if (strcmp(strHead, "SplineValAfter") == 0) {	// �����g�p
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

							// �Ȑ��n�̕�Ԃ͏������d���̂ŁA�Ȑ��ɂȂ�悤�Ȑ��`��ԂƂ��ďĂ�����
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
	// �C�x���g����
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

			// ���̃C�x���g������
			it = EventList.erase(it);
			continue;
		}
		// DeleteTrack
		else if ((*it)->Type == Event_Base::STE_DELETETRACK) {
			Event_DeleteTrack *p = static_cast<Event_DeleteTrack*>((*it).get());

			m_DeleteTrack = true;

			// ���̃C�x���g������
			it = EventList.erase(it);
			continue;
		}
		// Position
		else if ((*it)->Type == Event_Base::STE_POSITION) {
			Event_Position *p = static_cast<Event_Position*>((*it).get());

			m_AnimePos = p->Pos;

			// ���̃C�x���g������
			it = EventList.erase(it);
			continue;
		}
		// Speed
		else if ((*it)->Type == Event_Base::STE_SPEED) {
			Event_Speed *p = static_cast<Event_Speed*>((*it).get());

			if (p->startflag == 0) {
				p->OldSpeed = m_Speed;// ���݂̑��x���L��
				p->startflag = 1;
			}

			if (p->fDuration == 0) {	// �����ɕύX

				m_Speed = p->NewSpeed;

				// ���̃C�x���g������
				it = EventList.erase(it);
				continue;
			}
			else {					// ���ԕ��
				double f;
				f = (-p->fStartTime) / p->fDuration;
				if (f >= 1.0f) {
					// �Ō�
					m_Speed = p->NewSpeed;

					// ���̃C�x���g������
					it = EventList.erase(it);
					continue;
				}
				else {
					// ���
					m_Speed = (float)(p->NewSpeed*(f)+p->OldSpeed*(1 - f));
				}
			}
		}
		//Weight
		else if ((*it)->Type == Event_Base::STE_WEIGHT) {
			Event_Weight* p = static_cast<Event_Weight*>((*it).get());

			if (p->startflag == 0) {
				p->OldWeight = m_Weight;// ���݂̏d�݂��L��
				p->startflag = 1;
			}

			if (p->fDuration == 0) {	// �����ɕύX

				m_Weight = p->NewWeight;

				// ���̃C�x���g������
				it = EventList.erase(it);
				continue;
			}
			else {					// ���ԕ��
				double f;
				f = (-p->fStartTime) / p->fDuration;
				if (f >= 1.0f) {
					// �Ō�
					m_Weight = p->NewWeight;

					// ���̃C�x���g������
					it = EventList.erase(it);
					continue;
				}
				else {
					// ���
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
	// �����̃A�j��������Ȃ����ւ�
	if (allowSameName == false) {
		int idx = SearchAnimation(anime->m_AnimeName);
		if (idx != -1) {
			// ����@�����ւ�
			m_AnimeList[idx] = anime;
			return;
		}
	}

	// �V�K�ǉ�
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
	// RootMove�@�\
	//====================================================
	// ���݂̃A�j���ʒu�������Ă���
	double aniPos = m_Track.back()->m_AnimePos;

	// �A�j���O��Root�̃��[�J���s����L��
	YsMatrix mPrev;
	if (mulDifference0 && m_RefMatrixTbl.size() >= 1 && m_RefMatrixTbl[0].refTransMat) {
		mPrev = *m_RefMatrixTbl[0].refTransMat;
		mPrev.CreateInverse();
	}


	//====================================================
	// �d�݃A�j������
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
		// �e�g���b�N�̏���
		//====================================================
		for (UINT i = 0; i<m_Track.size(); i++) {
			track = m_Track[i].get();

			//========================
			// �A�j���i�߂�
			//========================
			if (track->m_Enable) {
				YsAnimationSet* lpSA = track->m_pSkinAnime.get();

				// �i�߂�
				track->m_AnimePos += Val * track->m_Speed * (lpSA->m_TicksPerSecond / 60.0);

				// �Ō㔻����ɂ���
				if (lpSA->m_AnimeLen <= 0) {	// �A�j������0
					track->m_AnimePos = 0;
				}
				else if (track->m_Loop) {	// ���[�v
	//				while(track->m_AnimePos > lpSA->m_Period) {
	//					track->m_AnimePos -= lpSA->m_Period;
	//				}
					if (track->m_AnimePos >= lpSA->m_AnimeLen) {
						track->m_AnimePos = 0;
					}
				}
				else {				// ���[�v�Ȃ�
					if (track->m_AnimePos > lpSA->m_AnimeLen) {
						track->m_AnimePos = lpSA->m_AnimeLen;
					}
				}
			}

			//========================
			// �C�x���g����
			//========================
			track->UpdateEvent(Val);
			if (track->m_DeleteTrack) {
				// �Ō��1�͏����Ȃ�
				if (m_Track.size() >= 2) {
					m_Track.erase(m_Track.begin() + i);
					i--;
				}
			}
		}

	}

	//====================================================
	// �A�j���[�V�����f�[�^���g���A�ϊ��s��X�V
	//====================================================
	CalcAnimation();

	// �A�j�����Root�̃��[�J���s����擾���A�j���O�Ƃ̍��������߂āA���̕�������
	if (mulDifference0 && m_RefMatrixTbl.size() >= 1 && m_RefMatrixTbl[0].refTransMat) {
		// �A�j���ʒu�������߂��Ă���Ƃ��́A�ړ������Ȃ�
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
	// �X�N���v�g�ɂ��A�ׂ����i�s������
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
			// nowTime - nextTime�ԂɃX�N���v�g�����邩�H
			for (auto& sc : lpSA->m_Script) {
				// �����I
				if (sc->Time > nowTime + margin && sc->Time <= nextTime) {
					// �ʒu�Z�o
					double p = (sc->Time - track->m_AnimePos);
					// �O�̃X�N���v�g����̑��Έʒu��o�^
					valTbl.push_back(p - prevPos);
					scrTbl.push_back(sc);	// �X�N���v�g���o�^

					prevPos = p;
				}
			}

			// ���������X�N���v�g�̈ʒu�����ɑS�g�̃|�[�Y�����肵�A�X�N���v�g�����s����
			if (valTbl.size() > 0) {
				for (UINT i = 0; i < valTbl.size(); i++) {

					double v = valTbl[i];
					Val -= v * track->m_Speed;
					double buSpeed = track->m_Speed;
					track->m_Speed = 1.0;
					Animation(v, mulDifference0);
					track->m_Speed = buSpeed;

					// �X�N���v�g���s
					YsAnimeKey_Script* sc = scrTbl[i];
					onScriptExecProc(sc);
				}
				// �c��̂Ԃ��i�s
				if (Val > 0) {
					Animation(Val, mulDifference0);
				}

				return;
			}
		}

		// ���s����X�N���v�g���Ȃ��ꍇ�́A�ʏ�A�j��

	}


	// �ʏ�A�j��
	Animation(Val, mulDifference0);

}

void YsAnimator::CalcAnimation()
{
	UINT i;

	YsAnimationSet* anime;
	YsMatrix m;
	YsQuat qTotal;
	YsVec3 vTotal;

	// �t���[����
	UINT FrameSize = m_RefMatrixTbl.size();

	YsAnimatorTrack* track;

	// �S�t���[������
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
		// �S�g���b�N
		for (UINT tr = 0; tr<m_Track.size(); tr++) {
			track = m_Track[tr].get();
			if (!track->m_Enable)continue;

			anime = track->m_pSkinAnime.get();
			if (i >= anime->m_FrameAnimeList.size())continue;

			// �A�j���ʒu
			fAnimeTime = track->m_AnimePos;
			if (fAnimeTime < 0)fAnimeTime = 0;


			// ��]
			if(YsAnimeKey_Quaternion::InterpolationFromTime(anime->m_FrameAnimeList[i]->m_Rotate, fAnimeTime, qRota, &m_RefMatrixTbl[i].nowKeyIndex_Rota)) {
				// ���� �܂��� �d�݂�1
				if (RotaCnt == 0 || track->m_Weight >= 1.0f) {
					qTotal = qRota;
				}
				// �u�����h
				else {
					if (m_RefMatrixTbl[i].isBlendTrack_Rota) {
						YsQuat::Slerp(qTotal,
							qTotal,
							qRota,
							track->m_Weight);		// ���(0.0�`1.0)
					}
				}
				RotaCnt++;
			}
			// �g��
			if(YsAnimeKey_Vector3::InterpolationFromTime(anime->m_FrameAnimeList[i]->m_Scale, fAnimeTime, vScale, &m_RefMatrixTbl[i].nowKeyIndex_Scale)) {
				// ���� �܂��� �d�݂�1
				if (ScaleCnt == 0 || track->m_Weight >= 1.0f) {
					sTotal = vScale;
				}
				// �u�����h
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
			// ���W
			if(YsAnimeKey_Vector3::InterpolationFromTime(anime->m_FrameAnimeList[i]->m_Pos, fAnimeTime, vPos, &m_RefMatrixTbl[i].nowKeyIndex_Pos)) {
				// ���� �܂��� �d�݂�1
				if (PosCnt == 0 || track->m_Weight >= 1.0f) {
					vTotal = vPos;
				}
				// �u�����h
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

		// �L�[���Ȃ��ꍇ�́A�������Ȃ�
		if (RotaCnt == 0 && ScaleCnt == 0 && PosCnt == 0) continue;

		m.CreateIdentity();

		mOldTransMat = *refMat;	// �v�Z�O�̃��[�J���s����L��

		// �x�[�X�E�F�C�g�ł̃u�����h
		// ��]
		if (RotaCnt > 0) {
			// �x�[�X�E�F�C�g��1�����Ȃ�A�v�Z�O�̍s��ƃu�����h(1�ȏゾ�Ə㏑���ɂȂ�̂ŁA�v�Z���Ȃ��Ă悢)
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
		// �g��
		if (ScaleCnt > 0) {
			// �x�[�X�E�F�C�g��1�����Ȃ�A�v�Z�O�̍s��ƃu�����h(1�ȏゾ�Ə㏑���ɂȂ�̂ŁA�v�Z���Ȃ��Ă悢)
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
		// ���W
		if (PosCnt > 0) {
			// �x�[�X�E�F�C�g��1�����Ȃ�A�v�Z�O�̍s��ƃu�����h(1�ȏゾ�Ə㏑���ɂȂ�̂ŁA�v�Z���Ȃ��Ă悢)
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
	// �A�j������
	int animeNo = SearchAnimation(AnimeName);
	if (animeNo == -1)return false;

	return ChangeAnime(animeNo, loop, SetTrackNo, bEnableTrack, Speed, Weight, AnimePos);
}


// �u�����h�Ŋ��炩�A�j���ύX
bool YsAnimator::ChangeAnimeSmooth(UINT AnimeNo, float StartTime, float Duration, bool loop, double Speed, double AnimePos)
{
	// �V�����g���b�N�쐬
	SPtr<YsAnimatorTrack> newTrack = std::make_shared<YsAnimatorTrack>();
	m_Track.push_front(newTrack);

	// �����ɕύX
	if (Duration == 0) {
		// �V�����A�j����ݒ�
		if (ChangeAnime(AnimeNo, loop, 0, true, Speed, 1, AnimePos) == false)return false;

		// �O�̃A�j���͖����ɂ���B
		if (m_Track.size() >= 2) {
			// �P�O�̃g���b�N
			YsAnimatorTrack* prevTrack = m_Track[1].get();
			prevTrack->m_Enable = false;
			prevTrack->m_DeleteTrack = true;
			prevTrack->m_Weight = 0;
		}
	}
	// ���炩�ɕύX
	else {
		// �V�����A�j�����Z�b�g
		if (ChangeAnime(AnimeNo, loop, 0, true, Speed, 0, AnimePos) == false)return false;
		// �V�����A�j���̏d�݂��グ�Ă����ݒ�
		newTrack->EventTrackWeight(1, StartTime, Duration);

		// �O�̃A�j���̏d�݂����炵�A�����ɂ��Ă����B
		if (m_Track.size() >= 2) {
			// �P�O�̃g���b�N
			YsAnimatorTrack* prevTrack = m_Track[1].get();

			prevTrack->EventTrackEnable(false, StartTime + Duration);	// �����C�x���g
			prevTrack->EventTrackDelete(StartTime + Duration);			// �폜�C�x���g

			prevTrack->EventTrackSpeed(0, StartTime, Duration);			// ���x�C�x���g
			prevTrack->EventTrackWeight(0.0f, StartTime, Duration);		// �d�݃C�x���g
		}

	}
	return true;
}

bool YsAnimator::ChangeAnimeSmooth_FromName(const std::string& AnimeName, float StartTime, float Duration, bool loop, double Speed, double AnimePos)
{
	// �A�j������
	int animeNo = SearchAnimation(AnimeName);
	if (animeNo == -1)return false;

	return ChangeAnimeSmooth(animeNo, StartTime, Duration, loop, Speed, AnimePos);
}

bool YsAnimator::AddAnimeSmooth(UINT AnimeNo, float StartTime, float Duration, bool loop, double Speed, double AnimePos, float startWeight, float endWeight)
{
	// �V�����g���b�N�쐬
	SPtr<YsAnimatorTrack> newTrack = std::make_shared<YsAnimatorTrack>();
	m_Track.push_back(newTrack);	// ���ɒǉ�

	// �V�����A�j�����Z�b�g
	if(ChangeAnime(AnimeNo, loop, (int)m_Track.size() - 1, true, Speed, startWeight, AnimePos) == false)return false;

	// �V�����A�j���̏d�݂�ω������Ă����ݒ�
	newTrack->EventTrackWeight(endWeight, StartTime, Duration);

	// endWeight��0�Ȃ�A0�ɂȂ�����g���b�N�������
	if(endWeight == 0){
		newTrack->EventTrackEnable(false, StartTime + Duration);	// �����C�x���g
		newTrack->EventTrackDelete(StartTime + Duration);			// �폜�C�x���g
	}

	return true;
}

bool YsAnimator::AddAnimeSmooth_FromName(const std::string& AnimeName, float StartTime, float Duration, bool loop, double Speed, double AnimePos, float startWeight, float endWeight)
{
	// �A�j������
	int animeNo = SearchAnimation(AnimeName);
	if(animeNo == -1)return false;

	return AddAnimeSmooth(animeNo, StartTime, Duration, loop, Speed, AnimePos, startWeight, endWeight);
}


