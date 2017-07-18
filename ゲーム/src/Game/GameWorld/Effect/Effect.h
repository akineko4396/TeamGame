#pragma once

//=================================================
// �G�t�F�N�g��{�N���X
//=================================================
class BaseEffectObj : public BaseGameObj {
public:
	virtual void Update()=0;
	virtual void Draw()=0;

	// �C���X�^���X���Ǘ�
	static void AllUpdate();
	static void AllDraw();
	static void PushObject(SPtr<BaseEffectObj> _obj){ m_List.push_back(_obj); }
	static std::vector<SPtr<BaseEffectObj>> GetList(){ return m_List; }
	static void FullClear(){ m_List.clear(); }

	bool GetKillFlg(){ return m_KillFlg; }

protected:
	static std::vector<SPtr<BaseEffectObj>>m_List;

	bool m_KillFlg = false;
};


