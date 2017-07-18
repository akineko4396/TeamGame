#pragma once

//=================================================
// �f�[�^��ǂݏ�������N���X
//=================================================

class CharacterBase;

class DataManager
{
public:

	/*DataLoad------------------------------------

	�O���t�@�C����ǂݍ���
	�Q�[���Ɏg�p����L�����N�^��I�u�W�F�N�g��
	�C���X�^���X�����ă��X�g�Ɋi�[����

	_FileName		:	�t�@�C���p�X

	return			:	�I�u�W�F�N�g����Ԃ�

	--------------------------------------------*/
	//int TxtDataLoad(const std::string& _FileName);	//�e�L�X�g�t�@�C��Ver.

	SPtr<CharacterBase> CsvDataLoad(const std::string& _FileName);	//csv�t�@�C��Ver.

	/*DataLoad------------------------------------

	�O���t�@�C���ɏ�������
	�Q�[���Ɏg�p�����L�����N�^��I�u�W�F�N�g��
	�e�L�X�g�t�@�C���ɏ����o��

	_FileName		:	�t�@�C���p�X

	return			:	�߂�l�Ȃ�

	--------------------------------------------*/
	//void TxtDataSave(const std::string& _FileName, int _num);

	void CsvDataSave(const std::string& _FileName);

	/*GetSplit--------------------------------->

	�w�肵��������؂�����
	�f�[�^�ƂȂ镶�����Ԃ�
	�߂�l�Ƃ��Đ؂������������Ԃ�

	_Str	:	���ƂȂ镶����
	_Split	:	�؂��肽��������(������)

	return	:	vector�^

	<----------------------------------------*/
	/*std::vector<std::string> GetSplit(
		std::string _Str,
		std::string _Split
		);
		*/
private:
	//�f�[�^�擾�p
	YsCsv csv;
};