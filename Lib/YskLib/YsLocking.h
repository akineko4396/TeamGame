#ifndef __YsLocking_h__
#define __YsLocking_h__

namespace YskLib
{

//===============================================================
//! @file YsLocking.h
//! @brief �X���b�h�̔r������Ȃǂ������Ȃ��@�\
//!
//! @author ���c
//===============================================================




//========================================
//! @brief�}���`�X���b�h�r������
// �N���e�B�J���Z�N�V�����ł̔r������
//!
//! @ingroup Etc
//========================================
class YsLock {
public:
	// ���b�N�J�n
	void Lock() {
		EnterCriticalSection(&m_Lock);
	}
	// ���b�N����
	void Unlock() {
		LeaveCriticalSection(&m_Lock);
	}

	YsLock() {
		InitializeCriticalSection(&m_Lock);
	}
	~YsLock() {
		DeleteCriticalSection(&m_Lock);
	}

private:
	// �N���e�B�J���Z�N�V�����Ń��b�N����
	CRITICAL_SECTION	m_Lock;
};

//========================================
//! @brief �r������Ȃ�
//!
//! @ingroup Etc
//========================================
class YsNoLock {
public:
	// ���b�N�J�n
	void Lock() {
	}
	// ���b�N����
	void Unlock() {
	}
};


//========================================
//! @brief �r������N���X���g���₷������N���X
//!
//! @ingroup Etc
//========================================
template<typename LockType>
class YsLockGuard {
public:
	YsLockGuard(LockType& lockObj) : m_refLock(lockObj) {
		lockObj.Lock();
	}
	~YsLockGuard() {
		m_refLock.Unlock();
	}

private:
	LockType&	m_refLock;
};


}

#endif
