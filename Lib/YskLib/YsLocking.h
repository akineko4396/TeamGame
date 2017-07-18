#ifndef __YsLocking_h__
#define __YsLocking_h__

namespace YskLib
{

//===============================================================
//! @file YsLocking.h
//! @brief スレッドの排他制御などをおこなう機能
//!
//! @author 鎌田
//===============================================================




//========================================
//! @briefマルチスレッド排他制御
// クリティカルセクションでの排他制御
//!
//! @ingroup Etc
//========================================
class YsLock {
public:
	// ロック開始
	void Lock() {
		EnterCriticalSection(&m_Lock);
	}
	// ロック解除
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
	// クリティカルセクションでロックする
	CRITICAL_SECTION	m_Lock;
};

//========================================
//! @brief 排他制御なし
//!
//! @ingroup Etc
//========================================
class YsNoLock {
public:
	// ロック開始
	void Lock() {
	}
	// ロック解除
	void Unlock() {
	}
};


//========================================
//! @brief 排他制御クラスを使いやすくするクラス
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
