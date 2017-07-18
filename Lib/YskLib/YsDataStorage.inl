//===================================================================
//
//
// YsDataWeakStorageテンプレートクラスのメンバ関数の実装部分
//
//
//===================================================================

// データ追加
template<typename T, typename LockType>
template<typename X>
SPtr<X> YsDataWeakStorage<T, LockType>::AddData_Type(const std::string& Name, bool* pbCreate)			// 生成
{

	// 定期的に不要ノードを消す
	m_Cnt++;
	if (m_Cnt % 10 == 0) {
		Optimize();
	}

	SPtr<X> lpData;
	std::string sKey = Name;

	// 排他制御
	YsLockGuard<LockType> lock(m_Lock);

	//===============================
	// すでに登録されているかをCheck
	//===============================
	std::unordered_map<std::string, WPtr<T>>::iterator p = m_DataMap.find(sKey);
	if (m_DataMap.end() != p) {	// 存在
								// 破壊されてるなら、一度消す
		if (p->second.expired()) {
			m_DataMap.erase(p);
		}
		// 存在するなら、それを返す
		else {
			if (pbCreate)*pbCreate = false;
			return SPtr_dynamic_cast<X>(p->second.lock());
		}
	}

	//===========================
	// データを作成
	//===========================
	lpData = SPtr<X>(new X());

	m_DataMap.insert(std::pair<std::string, WPtr<T>>(sKey, lpData));

	if (pbCreate)*pbCreate = true;

	return lpData;
}

// 指定名のデータ取得
template<typename T, typename LockType>
SPtr<T> YsDataWeakStorage<T, LockType>::GetData(const std::string& Name)
{
	// 定期的に不要ノードを消す
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

	// 排他制御
	YsLockGuard<LockType> lock(m_Lock);

	// 存在確認
	std::unordered_map<std::string, WPtr<T>>::iterator p = m_DataMap.find(sKey);
	if (p == m_DataMap.end())return SPtr<T>();

	// 破壊確認
	if (p->second.expired()) {
		m_DataMap.erase(p);
		return SPtr<T>();
	}

	// 存在するならreturn
	return p->second.lock();
}

// 指定キーのデータを管理マップから削除
template<typename T, typename LockType>
void YsDataWeakStorage<T, LockType>::DelData(const std::string& Name)
{
	// 排他制御
	{
		YsLockGuard<LockType> lock(m_Lock);

		// 存在確認
		std::unordered_map<std::string, WPtr<T>>::iterator p = m_DataMap.find(sKey);
		if (p == m_DataMap.end())return;	// 存在しないなら終了

											// 削除
		m_DataMap.erase(p);
	}

	Optimize();
}

// 解放
template<typename T, typename LockType>
void YsDataWeakStorage<T, LockType>::Release() {
	// 排他制御
	YsLockGuard<LockType> lock(m_Lock);

	std::unordered_map<std::string, WPtr<T>>::iterator p = m_DataMap.begin();
	while (p != m_DataMap.end())
	{
		p->second.reset();
		p = m_DataMap.erase(p);
	}
}

// 最適化
template<typename T, typename LockType>
void YsDataWeakStorage<T, LockType>::Optimize()
{
	// 排他制御
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
