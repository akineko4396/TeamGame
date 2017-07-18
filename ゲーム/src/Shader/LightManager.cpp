#include "main.h"

#include "LightManager.h"

void LightManager::Init()
{
	// �V�F�[�_�p�̒萔�o�b�t�@�쐬
	m_cb12_SampleLight.Create(12);
	m_cb12_SampleLight.SetVS();		// ���_�V�F�[�_�p�C�v���C���փZ�b�g
	m_cb12_SampleLight.SetPS();		// �s�N�Z���V�F�[�_�p�C�v���C���փZ�b�g

}

void LightManager::Release()
{
	m_cb12_SampleLight.Release();

}

void LightManager::Update()
{
	//-----------------------------------
	// ���s����
	//-----------------------------------
	//  �����ɂȂ��Ă郉�C�g�͍폜����
	{
		auto it = m_DirLightList.begin();
		while (it != m_DirLightList.end()) {
			if ((*it).expired()) {
				it = m_DirLightList.erase(it);
			}
			else {
				++it;
			}
		}
	}

	// �萔�o�b�t�@�ɏ�������
	m_cb12_SampleLight.m_Data.DL_Cnt = 0;
	for (auto& light : ShMgr.m_LightMgr.m_DirLightList) {
		if (light.expired())continue;	// �����ȃ��C�g�̓X�L�b�v

		int idx = m_cb12_SampleLight.m_Data.DL_Cnt;
		if (idx >= ShMgr.m_LightMgr.MAX_DIRLIGHT)break;	// ���x���ȏ�Ȃ�I��

		m_cb12_SampleLight.m_Data.DL[idx].Color = light.lock()->Diffuse;
		m_cb12_SampleLight.m_Data.DL[idx].Dir = light.lock()->Direction;

		m_cb12_SampleLight.m_Data.DL_Cnt++;

	}

	//-----------------------------------
	// �|�C���g���C�g
	//-----------------------------------
	//  �����ɂȂ��Ă郉�C�g�͍폜����
	{
		auto it = m_PointLightList.begin();
		while (it != m_PointLightList.end()) {
			if ((*it).expired()) {
				it = m_PointLightList.erase(it);
			}
			else {
				++it;
			}
		}
	}

	// �萔�o�b�t�@�ɏ�������
	m_cb12_SampleLight.m_Data.PL_Cnt = 0;
	for (auto& light : ShMgr.m_LightMgr.m_PointLightList) {
		if (light.expired())continue;	// �����ȃ��C�g�̓X�L�b�v

		int idx = m_cb12_SampleLight.m_Data.PL_Cnt;
		if (idx >= ShMgr.m_LightMgr.MAX_POINTLIGHT)break;	// ���x���ȏ�Ȃ�I��

		m_cb12_SampleLight.m_Data.PL[idx].Color = light.lock()->Diffuse;
		m_cb12_SampleLight.m_Data.PL[idx].Pos = light.lock()->Position;
		m_cb12_SampleLight.m_Data.PL[idx].Radius = light.lock()->Radius;

		m_cb12_SampleLight.m_Data.PL_Cnt++;

	}

	//-----------------------------------
	// �X�|�b�g���C�g
	//-----------------------------------
	//  �����ɂȂ��Ă郉�C�g�͍폜����
	{
		auto it = m_SpotLightList.begin();
		while (it != m_SpotLightList.end()) {
			if ((*it).expired()) {
				it = m_SpotLightList.erase(it);
			}
			else {
				++it;
			}
		}
	}

	// �萔�o�b�t�@�ɏ�������
	m_cb12_SampleLight.m_Data.SL_Cnt = 0;
	for (auto& light : ShMgr.m_LightMgr.m_SpotLightList) {
		if (light.expired())continue;	// �����ȃ��C�g�̓X�L�b�v

		int idx = m_cb12_SampleLight.m_Data.SL_Cnt;
		if (idx >= ShMgr.m_LightMgr.MAX_POINTLIGHT)break;	// ���x���ȏ�Ȃ�I��

		m_cb12_SampleLight.m_Data.SL[idx].Color = light.lock()->Diffuse;
		m_cb12_SampleLight.m_Data.SL[idx].Pos = light.lock()->Position;
		m_cb12_SampleLight.m_Data.SL[idx].Range = light.lock()->Range;
		m_cb12_SampleLight.m_Data.SL[idx].Dir = light.lock()->Direction;
		m_cb12_SampleLight.m_Data.SL[idx].Dir.Normalize();
		m_cb12_SampleLight.m_Data.SL[idx].MinAng = YsToRadian(light.lock()->MinAngle);
		m_cb12_SampleLight.m_Data.SL[idx].MaxAng = YsToRadian(light.lock()->MaxAngle);

		m_cb12_SampleLight.m_Data.SL_Cnt++;

	}

	// ���ۂɒ萔�o�b�t�@�֏�������
	m_cb12_SampleLight.WriteData();

	m_cb12_SampleLight.SetPS(9);
}
