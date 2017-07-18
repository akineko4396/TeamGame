#include "YskLib.h"

using namespace YskLib;

// �t�@�C�����Ɗg���q�̊ԂɁA�w�蕶�����}������������𐶐�
std::string YskLib::YsConvertExtFileName(const std::string& FileName, const std::string& ext)
{
	std::string extName = FileName;
	int pos = extName.find_last_of(".");
	if(pos == -1)return "";
	std::string tmp = ".";
	tmp += ext;
	extName.insert(pos, tmp);
	return extName;
}

//=====================================================================
// �@���}�b�v�p��Tangent,Binormal���Z�o����B
//=====================================================================
void YskLib::YsCalcTangentAndBinormal(
	YsVec3* p0, YsVec2* uv0,
	YsVec3* p1, YsVec2* uv1,
	YsVec3* p2, YsVec2* uv2,
	const YsVec3* vN, YsVec3* outTangent, YsVec3* outBinormal
)
{
	// 5������3�������_��
	YsVec3 CP0[3] = {
		YsVec3(p0->x, uv0->x, uv0->y),
		YsVec3(p0->y, uv0->x, uv0->y),
		YsVec3(p0->z, uv0->x, uv0->y),
	};
	YsVec3 CP1[3] = {
		YsVec3(p1->x, uv1->x, uv1->y),
		YsVec3(p1->y, uv1->x, uv1->y),
		YsVec3(p1->z, uv1->x, uv1->y),
	};
	YsVec3 CP2[3] = {
		YsVec3(p2->x, uv2->x, uv2->y),
		YsVec3(p2->y, uv2->x, uv2->y),
		YsVec3(p2->z, uv2->x, uv2->y),
	};

	// ���ʃp�����[�^����UV�����W�Z�o
	float U[3], V[3];
	for (int i = 0; i < 3; ++i) {
		YsVec3 V1 = CP1[i] - CP0[i];
		YsVec3 V2 = CP2[i] - CP1[i];
		YsVec3 ABC;
		YsVec3::Cross(ABC, V1, V2);

		if (ABC.x == 0.0f) {
			// �|���S����UV��̃|���S�����k�ނ��Ă܂�
			//			_ASSERT(0);
			// �Ƃ肠����vN�����ɓK���ȕ���p��
			YsVec3::Cross(*outTangent, YsVec3(0, 1, 0), *vN);
			outTangent->Normalize();
			YsVec3::Cross(*outBinormal, *vN, *outTangent);
			outBinormal->Normalize();
			return;
		}
		U[i] = -ABC.y / ABC.x;
		V[i] = -ABC.z / ABC.x;
	}

	memcpy(outTangent, U, sizeof(float) * 3);
	memcpy(outBinormal, V, sizeof(float) * 3);

	// ���K��
	outTangent->Normalize();
	outBinormal->Normalize();
}
