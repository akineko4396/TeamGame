#include "YskLib.h"

using namespace YskLib;

//==========================================================================================
//
//
// �Q�[���p���@�\���f���N���X
//
//
//==========================================================================================

YsGameModel::YsGameModel()
{

}

YsGameModel::~YsGameModel()
{
	Release();
}

void YsGameModel::CreateRefMeshTbl()
{
	m_RefModelTbl_SkinOnly.clear();
	m_RefModelTbl_SkinOnly.shrink_to_fit();
	m_RefModelTbl_StaticOnly.clear();
	m_RefModelTbl_StaticOnly.shrink_to_fit();
	for(UINT i=0;i<m_ModelTbl.size();i++){
		if(m_ModelTbl[i]->GetMesh()->IsSkinMesh()){
			m_RefModelTbl_SkinOnly.push_back(m_ModelTbl[i]);
		}
		else{
			m_RefModelTbl_StaticOnly.push_back(m_ModelTbl[i]);
		}
	}
}

bool YsGameModel::LoadMesh(const std::string& fileName, bool bakeCurve)
{
	// �w�b�_�`�F�b�N�`�F�b�N
	char head[4];
	FILE* fp = fopen(fileName.c_str(), "r");
	if(fp == nullptr)return false;

	fread(head, 3, 1, fp);
	head[3] = '\0';
	fclose(fp);

	// XED�t�@�C��
	if(strcmp(head, "XED") == 0) {
		return LoadXEDFile(fileName, bakeCurve);
	}
	// X�t�@�C��
	else if(strcmp(head, "xof") == 0) {
		return LoadXFile(fileName);
	}
	else{
		return false;
	}
}

//==========================================================================================
// �X�L�����b�V����ǂݍ���
//==========================================================================================
bool YsGameModel::LoadXFile(const std::string& fileName)
{
	// ���
	Release();

	// �p�X�A�t�@�C�������o
	char szDir[_MAX_PATH];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(fileName.c_str(), nullptr, szDir, szFname, szExt);
	m_FileName = szFname;
	m_FileName += szExt;
	m_FilePath = szDir;

	// XED�ǂݍ���
	if (YsLoadXFile(fileName, m_ModelTbl, m_BoneTree, m_AnimeList)) {

		// �X�L�����b�V���ƃX�^�e�B�b�N���b�V���̎Q�Ƃ𕪂����e�[�u���쐬
		CreateRefMeshTbl();

		return true;
	}
	return false;
}

bool YsGameModel::LoadXEDFile(const std::string& fileName, bool bakeCurve)
{
	// ���
	Release();

	// �p�X�A�t�@�C�������o
	char szDir[_MAX_PATH];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(fileName.c_str(), nullptr, szDir, szFname, szExt);
	m_FileName = szFname;
	m_FileName += szExt;
	m_FilePath = szDir;

	// XED�ǂݍ���
	if(YsLoadXEDFile(fileName, m_ModelTbl, m_BoneTree, m_AnimeList, m_RigidBodyTbl, bakeCurve)) {

		// �X�L�����b�V���ƃX�^�e�B�b�N���b�V���̎Q�Ƃ𕪂����e�[�u���쐬
		CreateRefMeshTbl();

		//------------------------------------------------------------------------------------------
		// KAMADA SkinMesh Editor�ł́A�}�e���A���̃e�L�X�g��������́@���K�v�Ȃ��Ȃ�폜���Ă悢
		// �S���b�V��
		for (auto& model : m_ModelTbl) {
			// �S�}�e���A��
			for (auto& mate : model->GetMaterials()) {
				// �Ƃ肠����CSV�Ƃ��ĉ�͂���
				YsCsv csv;
				csv.LoadFromText(mate.ExtScript);
				// �c���[�v
				for(UINT ci = 0;ci < csv.m_Tbl.size();ci++){
					const std::string strType = csv.m_Tbl[ci][0];
					// 
					if(strType == "�Ȃɂ��`"){

					}
				}
			}
		}
		//------------------------------------------------------------------------------------------

		return true;
	}
	return false;
	
}

//==========================================================================================
// �X�L�����b�V�������
//==========================================================================================
void YsGameModel::Release()
{
	// �{�[�����
	m_BoneTree.clear();
	m_BoneTree.shrink_to_fit();

	// �A�j���[�V�����f�[�^���
	DeleteAnimation();

	// ���b�V�����
	m_ModelTbl.clear();
	m_ModelTbl.shrink_to_fit();
	m_RefModelTbl_SkinOnly.clear();
	m_RefModelTbl_SkinOnly.shrink_to_fit();
	m_RefModelTbl_StaticOnly.clear();
	m_RefModelTbl_StaticOnly.shrink_to_fit();

	m_FilePath.clear();
	m_FileName.clear();
}
