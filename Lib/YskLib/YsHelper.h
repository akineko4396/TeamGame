//===============================================================
//! @file Helper.h
//! @brief ���̑��⏕�I�Ȃ���
//!
//! @author ���c
//===============================================================
#ifndef __YsHelper_h__
#define __YsHelper_h__

namespace YskLib
{

class YsVec2;
class YsVec3;

//! @addtogroup Etc
//! @{


//! @brief ���W�A���p �� �x �ϊ�
#define YsToRadian(x) ((x)*0.017453f)

//! @brief �x �� ���W�A���p �ϊ�
#define YsToDegree(x) ((x)*57.3f)

// ���S�ȉ���n

//! @brief ���S��Release����}�N��
template<typename T>
inline void YsSafe_Release(T*& p) {
	if (p) {
		p->Release();
		p = nullptr;
	}
}

//! @brief new���ꂽ�ϐ������S�ɉ������}�N��
template<typename T>
inline void YsSafe_Delete(T*& p) {
	if (p) {
		delete p;
		p = nullptr;
	}
}

//! @brief new���ꂽ�z������S�ɉ������}�N��
template<typename T>
inline void YsSafe_Delete_Array(T*& p) {
	if (p) {
		delete[] p;
		p = nullptr;
	}
}

//===============================================
// �X�}�[�g�|�C���^ �Z�k��
//===============================================
#define SPtr				std::shared_ptr
#define WPtr				std::weak_ptr
#define SPtr_dynamic_cast	std::dynamic_pointer_cast
#define SPtr_static_cast	std::static_pointer_cast
#define UPtr				std::unique_ptr
// weak_ptr���琶�|�C���^���擾(lock()��荂��)
// ��1�t���[���ɉ������lock()���s���Ă��܂��Ƃ��Ȃ�d��
// �@lock()�݂�����shared_ptr�͂���񂪁A���|�C���^�ł����ꍇ�̓R��
#define WPtr_Get(wp)	(wp.expired() == false ? wp._Get() : nullptr)

//===============================================
// ���|�C���^����shared_ptr�֕ϊ�����
// ���������Astd::enable_shared_from_this���p�������N���X�Ɍ���(����ȊO�̃N���X�̓R���p�C���G���[�ɂȂ�)
// �����R���̃N���X�́Amake_shared�֐���new�ō��ꂽshared_ptr�Ɍ���(���ʂ̕ϐ��̏ꍇ�̓_��)
// ���܂肱�̐��|�C���^(ptr)�́A�ǂ�����shared_ptr�ŕێ�����Ă���K�v������
// �ڂ�����std::enable_shared_from_this�𒲂ׂĂ�������
//  ptr	�c �ϊ����ƂȂ鐶�|�C���^
//  �߂�l : �ϊ����shared_ptr
//===============================================
template<typename T>
inline SPtr<T> YsToSPtr(T* ptr) {
	return SPtr_static_cast<T>(ptr->shared_from_this());
}

//! @brief RGBA�w��(0�`255)����ABGR�`����UINT�쐬
#define RGBA(r,g,b,a)	((UINT)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))
//! @brief RGBA�w��(0.0�`1.0)����ABGR�`����UINT�쐬
#define fRGBA(r,g,b,a)	RGBA((DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f),(DWORD)((a)*255.f))



//=====================================================================
//! @brief �t�@�C�����Ɗg���q�̊ԂɁA�w�蕶����(ext)������
//! @param[in]	FileName	�c ���ƂȂ�t�@�C����
//! @param[in]	ext			�c �}��������������
//! @return ext���}�����ꂽ������
//=====================================================================
std::string YsConvertExtFileName(const std::string& FileName, const std::string& ext);


//===================================================
//! @brief �@���}�b�v�p��Tangent,Binormal���Z�o����
//===================================================
void YsCalcTangentAndBinormal(
	YsVec3* p0, YsVec2* uv0,
	YsVec3* p1, YsVec2* uv1,
	YsVec3* p2, YsVec2* uv2,
	const YsVec3* vN, YsVec3* outTangent, YsVec3* outBinormal
);

//! @}

}
#endif