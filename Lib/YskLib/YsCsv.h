//===============================================================
//! @file YsCsv.h
//! @brief Csv�`���̃t�@�C���֌W
//!
//! @author ���c
//===============================================================
#ifndef __YsCsv_h__
#define __YsCsv_h__

namespace YskLib{

//===============================================
//! @brief CSV�t�@�C�����o�̓N���X
//! �E���{�ꕶ�����J���}����s�Ɗ��Ⴂ���Ȃ��悤�ɑΉ������Ă܂�
//! @ingroup Etc
//===============================================
class YsCsv{
public:
	//========================================================
	//! brief CSV�f�[�^
	//========================================================
	std::vector<std::vector<std::string>> m_Tbl;

	//========================================================
	//! brief CSV�t�@�C������Am_Tbl�ɓǂݍ���
	//! @param[in]	filename	�c CSV�t�@�C����
	//! @param[in]	bDelTitle	�c �ŏ��̂P�s�ڂ͏ȗ����邩�H
	//========================================================
	bool Load(const std::string& filename, bool bDelTitle = false);
	
	//========================================================
	//! brief CSV�`�������񂩂�Am_Tbl�ɓǂݍ���
	//! @param[in]	text		�c CSV�`���̕�����
	//! @param[in]	bDelTitle	�c �ŏ��̂P�s�ڂ͏ȗ����邩�H
	//========================================================
	bool LoadFromText(const std::string& text, bool bDelTitle = false);

	//========================================================
	//! brief m_Tbl���t�@�C���֕ۑ�
	//! @param[in]	filename	�c CSV�t�@�C����
	//========================================================
	bool Save(const std::string& filename);

	//========================================================
	//! brief �w�肵����Ɏw�蕶���񂪂��邩�A�S�s����������
	//! @param[in]	nCol	�c �w���
	//! @param[in]	str		�c ����������
	//! @return �����F�sIndex�@�����F-1
	//========================================================
	int Search_Col(int nCol, const std::string& str){
		for(int i=0;i<(int)m_Tbl.size();i++){
			if(nCol >= (int)m_Tbl[i].size())continue;

			if(str == m_Tbl[i][nCol]){
				return i;
			}
		}
		return -1;
	}

	//========================================================
	//! brief �w�肵���s�Ɏw�蕶���񂪂��邩�A�S�����������
	//! @param[in]	nRow	�c �w��s
	//! @param[in]	str		�c ����������
	//! @return �����F��Index�@�����F-1
	//========================================================
	int Search_Row(int nRow, const std::string& str){
		if(nRow >= (int)m_Tbl.size())return -1;

		for(int i = 0; i<(int)m_Tbl[nRow].size(); i++){
			if(str == m_Tbl[nRow][i]){
				return i;
			}
		}
		return -1;
	}

	//========================================================
	//! brief 0�s�ڂ��^�O�����Ƃ��Ďg���Atag�ƈ�v���镶���񂪉���ڂ����߁AnRow�s�ڂ�tag��ڂ̕������Ԃ�
	//! @param[in]	nRow	�c �w��s
	//! @param[in]	tag		�c ��𔻒肷�邽�߂̕�����
	//! @return nRow�s�ڂ�tag��ڂ̕������Ԃ��@tag�̗񂪑��݂��Ȃ��ꍇ�́A""���Ԃ�
	//========================================================
	std::string GetCol(int nRow, const std::string& tag){
		int col = Search_Row(0, tag);
		if(col == -1)return "";

		return m_Tbl[nRow][col];
	}

	//! @brief �f�[�^���N���A����
	void Clear(){
		m_Tbl.clear();
	}
};

}
#endif