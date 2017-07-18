//===================================================================
//
//
// YsDataWeakStorage�e���v���[�g�N���X�̃����o�֐��̎�������
//
//
//===================================================================

// �f�[�^�ǉ�
template<typename T, typename LockType>
template<typename X>
SPtr<X> YsDataWeakStorage<T, LockType>::AddData_Type(const std::string& Name, bool* pbCreate)			// ����
{

	// ����I�ɕs�v�m�[�h������
	m_Cnt++;
	if (m_Cnt % 10 == 0) {
		Optimize();
	}

	SPtr<X> lpData;
	std::string sKey = Name;

	// �r������
	YsLockGuard<LockType> lock(m_Lock);

	//===============================
	// ���łɓo�^����Ă��邩��Check
	//===============================
	std::unordered_map<std::string, WPtr<T>>::iterator p = m_DataMap.find(sKey);
	if (m_DataMap.end() != p) {	// ����
								// �j�󂳂�Ă�Ȃ�A��x����
		if (p->second.expired()) {
			m_DataMap.erase(p);
		}
		// ���݂���Ȃ�A�����Ԃ�
		else {
			if (pbCreate)*pbCreate = false;
			return SPtr_dynamic_cast<X>(p->second.lock());
		}
	}

	//===========================
	// �f�[�^���쐬
	//===========================
	lpData = SPtr<X>(new X());

	m_DataMap.insert(std::pair<std::string, WPtr<T>>(sKey, lpData));

	if (pbCreate)*pbCreate = true;

	return lpData;
}

// �w�薼�̃f�[�^�擾
template<typename T, typename LockType>
SPtr<T> YsDataWeakStorage<T, LockType>::GetData(const std::string& Name)
{
	// ����I�ɕs�v�m�[�h������
	m_Cnt++;
	if (m_Cnt % 10 == 0) {
		Optimize();
	}

	std::string sKey;
	if (Name == nullptr) {
		sKey = "";
	}
	else {
		sKey = Name;
	}

	// �r������
	YsLockGuard<LockType> lock(m_Lock);

	// ���݊m�F
	std::unordered_map<std::string, WPtr<T>>::iterator p = m_DataMap.find(sKey);
	if (p == m_DataMap.end())return SPtr<T>();

	// �j��m�F
	if (p->second.expired()) {
		m_DataMap.erase(p);
		return SPtr<T>();
	}

	// ���݂���Ȃ�return
	return p->second.lock();
}

// �w��L�[�̃f�[�^���Ǘ��}�b�v����폜
template<typename T, typename LockType>
void YsDataWeakStorage<T, LockType>::DelData(const std::string& Name)
{
	// �r������
	{
		YsLockGuard<LockType> lock(m_Lock);

		// ���݊m�F
		std::unordered_map<std::string, WPtr<T>>::iterator p = m_DataMap.find(sKey);
		if (p == m_DataMap.end())return;	// ���݂��Ȃ��Ȃ�I��

											// �폜
		m_DataMap.erase(p);
	}

	Optimize();
}

// ���
template<typename T, typename LockType>
void YsDataWeakStorage<T, LockType>::Release() {
	// �r������
	YsLockGuard<LockType> lock(m_Lock);

	std::unordered_map<std::string, WPtr<T>>::iterator p = m_DataMap.begin();
	while (p != m_DataMap.end())
	{
		p->second.reset();
		p = m_DataMap.erase(p);
	}
}

// �œK��
template<typename T, typename LockType>
void YsDataWeakStorage<T, LockType>::Optimize()
{
	// �r������
	YsLockGuard<LockType> lock(m_Lock);

	std::unordered_map<std::string, WPtr<T>>::iterator p = m_DataMap.begin();
	while (p != m_DataMap.end())
	{
		if (p->second.expired()) {
			p = m_DataMap.erase(p);
		}
		else {
			++p;
		}
	}
}
