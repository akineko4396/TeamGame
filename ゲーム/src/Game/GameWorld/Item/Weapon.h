#pragma once


//============================================
// �����{�N���X
//============================================
class BaseWeapon : public BaseGameObj {
public:
	virtual ~BaseWeapon(){}

	WPtr<CharacterBase>m_wpChara;

	virtual void Update(){}
	virtual void Draw(){}
	// �U�������o�^����
	virtual void RegisterAttack(int time,
		std::function<void(const SPtr<BaseHitObj>& obj)> onHitProc
		){}


	// �C���X�^���X���Ǘ�
	static void AllUpdate();
	static void AllDraw();
	static void PushObject(SPtr<BaseWeapon> _obj){ m_List.push_back(_obj); }
	static std::vector<SPtr<BaseWeapon>> GetList(){ return m_List; }
	static void FullClear(){ m_List.clear(); }

	bool GetKillFlg(){ return m_KillFlg; }

protected:
	static std::vector<SPtr<BaseWeapon>>m_List;

	bool m_KillFlg = false;
};
