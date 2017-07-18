//===============================================================
//! @file YsDataStorage.h
//! @brief �f�[�^�Ǘ��֌W
//!
//! @author ���c
//===============================================================
#ifndef __YsDataManager_h__
#define __YsDataManager_h__

namespace YskLib
{

//================================================================================
//! @brief �ėp�f�[�^�ۊǃN���X
//!
//! (Singleton������FlyWeight�p�^�[���̂悤�Ȋ���)	\n
//!  weak_ptr�Ǘ�:�쐬��shared_ptr�ŕԂ��B���L�҂����Ȃ��Ȃ������_�Ńf�[�^�͎�����������(weak_ptr�Ȃ̂�)�B		\n
//!	             map���Ƀm�[�h�͎c�邪�AAddData()��GetData()�����Ƃ��ɁA����ς݂̃f�[�^�������map�����������B	\n
//!				 �܂���Optimize()�ł��\�B																			\n
//!
//!  �E�}���`�X���b�h�Ή�(LockType�ɔr������N���X�̌^���w�肷��)
//!
//! @ingroup Etc
//================================================================================
template<typename T, typename LockType>
class YsDataWeakStorage{
private:
	class LockGuard;

public:
	//==========================================================================
	//! @brief �f�[�^�ǉ�
	//!
	//! �w��L�[���̃f�[�^�����݂��Ȃ���Ύw��^X��V�K�ǉ����Ԃ�	\n
	//! ���ɑ��݂���ꍇ�͂����Ԃ�								\n
	//! ����������^�w��Ver											\n
	//!
	//! @param[in]	Name		�c �L�[��
	//! @param[in]	pbCreate	�c ���������ꍇ��true�@���ɑ��݂���ꍇ��false������
	//! @return ���݂���F�����̃f�[�^�@���݂��Ȃ��F�V�K�쐬�����f�[�^
	//==========================================================================
	template<typename X>
	SPtr<X> AddData_Type(const std::string& Name, bool* pbCreate = nullptr);
	
	//==========================================================================
	//! @brief �f�[�^�ǉ�
	//!
	//! �w��L�[���̃f�[�^�����݂��Ȃ���Ό^T��V�K�ǉ����Ԃ�	\n
	//! ���ɑ��݂���ꍇ�͂����Ԃ�							\n
	//!
	//! @param[in]	Name		�c �L�[��
	//! @param[in]	pbCreate	�c ���������ꍇ��true�@���ɑ��݂���ꍇ��false������
	//! @return ���݂���F�����̃f�[�^�@���݂��Ȃ��F�V�K�쐬�����f�[�^
	//==========================================================================
	SPtr<T> AddData(const std::string& Name, bool* pbCreate = nullptr)	
	{
		return AddData_Type<T>(Name, pbCreate);
	}

	//=======================================================
	//! @brief �w�薼�̃f�[�^�擾
	//! @return ���݂���F�f�[�^�@���݂��Ȃ��Fnullptr
	//=======================================================
	SPtr<T> GetData(const std::string& Name);

	//=======================================================
	//! @brief �w��L�[�̃f�[�^���Ǘ��}�b�v����폜
	//! @param[in]	Name		�c �L�[��
	//=======================================================
	void DelData(const std::string& Name);

	//=======================================================
	//! @brief ���
	//! �S�Ẵf�[�^������
	//=======================================================
	void Release();

	//=======================================================
	//! @brief �œK��
	//!	�j��ς݂̃m�[�h������
	//=======================================================
	void Optimize();

	//=======================================================
	//! @brief �m�[�h���擾
	//=======================================================
	UINT GetNodeNum(){
		// �r������
		YsLockGuard<LockType> lock(m_Lock);

		return m_DataMap.size();
	}

	//=======================================================
	//! @brief �S���\�[�X�}�b�v���擾
	//! ���}�b�v���͔̂r������ŃR�s�[���ĕԂ��Ă��邪�A�����f�[�^�̓}���`�X���b�h�A�N�Z�X���ɂ͒���
	//=======================================================
	std::unordered_map< std::string, const WPtr<T> > GetDataMap() {
		// �r������
		YsLockGuard<LockType> lock(m_Lock);

		return (std::unordered_map< std::string, const WPtr<T> >&)m_DataMap;
	}


public:

	// 
	YsDataWeakStorage() : m_Cnt(0){
	}
	// 
	~YsDataWeakStorage(){
		Release();
	}

private:
	std::unordered_map< std::string, WPtr<T> >	m_DataMap;		//!< �o�^map
	int											m_Cnt;			//!< ����I�ȍœK���p�J�E���^

	// �}���`�X���b�h�p�r������I�u�W�F�N�g
	//  Math��YsLocking.h�̃N���X���w�肷��
	LockType									m_Lock;


private:
	// �R�s�[�֎~�p
	YsDataWeakStorage(const YsDataWeakStorage<T, LockType>& src) {}
	void operator=(const YsDataWeakStorage<T, LockType>& src) {}
};

#include "YsDataStorage.inl"


}
#endif