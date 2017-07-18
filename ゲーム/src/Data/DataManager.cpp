#include "main.h"
#include "Game/GameWorld/GameWorld.h"
#include "DataManager.h"
#include "FilePath.h"


//int DataManager::TxtDataLoad(const std::string& _FileName)
//{
//	std::ifstream ifs;
//
//	//	�ǂݍ���
//	ifs.open(_FileName.c_str());
//
//	//	�ǂݍ��ݎ��s
//	if (!ifs)
//	{
//		//	���b�Z�[�W������
//		MessageBox(NULL, "�e�L�X�g�t�@�C�����ǂݍ��߂Ă��܂���", "�x��", MB_OK);
//
//		//	���������Ԃ�
//		return 0;
//	}
//
//	int num_cnt = 0;
//
//	for (;;)
//	{
//		std::string line;
//
//		//	��s�ǂݍ���
//		getline(ifs, line);
//
//		//	�ǂݍ��ݏI��
//		if (ifs.fail())
//		{
//			break;
//		}
//
//
//		/*---------------------------------------------------------------------
//
//		��؂肽��������ݒ�Avector�z��Ɋi�[(�؂��肽�������͕����w���)
//
//		��
//		string sp = "abc";
//
//		sp = GetSplit(line, "ac");
//
//		sp�ɂ�"abc"�̂���a��c���؂���ꂽb�݂̂��A���Ă���
//
//		---------------------------------------------------------------------*/
//		std::vector<std::string> sp = GetSplit(line, "(),");
//
//
//		//	�f�t�H���g�X�e�[�^�X
//		YsVec3 vPos = YsVec3(0.0f, 0.0f, 0.0f);
//		YsVec3 vScale = YsVec3(1.0f, 1.0f, 1.0f);
//		YsVec3 vAngle = YsVec3(0.0f, 0.0f, 0.0f);
//		INT		 Id = -1;
//
//
//		//	�ŏ��̃A�h���X���C�e���[�^�Ɋi�[
//		auto FileIt = sp.begin();
//
//
//		/*-----------------------------------------------------
//
//		�w�肳�ꂽ�^�O�����������ꍇ�A���̃^�O���̐��l��
//		�p�ӂ��ꂽ�X�e�[�^�X�p�̕ϐ��ɑ��
//		���l��+2�̈ʒu�Ɏw��
//
//
//		�^�O
//		{
//		Pos				:	���W�x�N�g��
//		Scale			:	�g��x�N�g��
//		Angle			:	�p�x�x�N�g��
//		Id			:	���ʔԍ�
//		}
//
//		-----------------------------------------------------*/
//
//		while (FileIt != sp.end())
//		{
//			if ((*FileIt) == "Pos")
//			{
//				FileIt += 2;
//				vPos.x = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vPos.y = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vPos.z = static_cast<float>(atof((*FileIt).c_str()));
//			}
//			else if ((*FileIt) == "Scale")
//			{
//				FileIt += 2;
//				vScale.x = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vScale.y = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vScale.z = static_cast<float>(atof((*FileIt).c_str()));
//			}
//			else if ((*FileIt) == "Angle")
//			{
//				FileIt += 2;
//				vAngle.x = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vAngle.y = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vAngle.z = static_cast<float>(atof((*FileIt).c_str()));
//			}
//			/*else if ((*FileIt) == "Id")
//			{
//				FileIt += 2;
//				Id = atoi((*FileIt).c_str());
//			}*/
//
//
//			++FileIt;
//		}
//
//		//-----------------------------------------------------
//		//
//		//	Class�^�O�̒����m�F���āA�w�肵��Class���������
//		//	�C���X�^���X������ObjectManager�Ƀv�b�V������
//		//	�X�e�[�^�X�͐�ɐݒ�l��������
//		//
//		//-----------------------------------------------------
//		FileIt = sp.begin();
//
//		while (FileIt != sp.end())
//		{
//			//====================
//			// �L�����N�^
//			//====================
//			//�v���C���[
//			{
//				if ((*FileIt) == "Player")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(PLAYER_PATH);
//
//					CreateGameObject<Player>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::PLAYER);
//					num_cnt++;
//				}
//			}
//
//			//====================
//			//�I�u�W�F�N�g
//			//====================
//			//�X�e�[�W
//			{
//				if ((*FileIt) == "Ground")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(GROUND_PATH);
//
//					CreateGameObject<Ground>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::GROUND);
//					num_cnt++;
//				}
//			}
//
//			//���_
//			{
//				if ((*FileIt) == "Stronghold")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Stronghold>(num_cnt, vPos, vScale, vAngle, drawModel,OBJECT_LIST::ID::STRONGHOLD);
//					num_cnt++;
//				}
//			}
//
//			//��
//			{
//				if ((*FileIt) == "Wall")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Wall>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::WALL);
//					num_cnt++;
//				}
//			}
//
//			//���Ί�
//			{
//				if ((*FileIt) == "Sling")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Sling>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::SLING);
//					num_cnt++;
//				}
//			}
//
//			//��C
//			{
//				if ((*FileIt) == "Cannon")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Cannon>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::CANNON);
//					num_cnt++;
//				}
//			}
//
//			//�Ή����ˊ�
//			{
//				if ((*FileIt) == "Flamethrower")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Flamethrower>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::FLAMETHROWER);
//					num_cnt++;
//				}
//			}
//
//			//�v���X�@
//			{
//				if ((*FileIt) == "Press")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Press>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::PRESS);
//					num_cnt++;
//				}
//			}
//
//			//�A�[��
//			{
//				if ((*FileIt) == "Arm")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(ARM_PATH);
//
//					CreateGameObject<Arm>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::ARM);
//					num_cnt++;
//				}
//			}
//
//			//�x���g�R���x�A
//			{
//				if ((*FileIt) == "Belt")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(BELT_PATH);
//
//					CreateGameObject<BeltConveyor>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::BELTCONVEYOR);
//					num_cnt++;
//				}
//			}
//
//			//�u���b�W
//			{
//				if ((*FileIt) == "Bridge")
//				{
//					// �`��p���b�V���ǂݍ���
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(BRIDGE_PATH);
//
//					CreateGameObject<Bridge>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::BRIDGE);
//					num_cnt++;
//				}
//			}
//
//			++FileIt;
//
//		}
//	}
//
//	return num_cnt;
//}
//
//void DataManager::TxtDataSave(const std::string& _FileName, int _num)
//{
//
//	std::ofstream ofs;
//
//	// �ǂݍ���
//	ofs.open(_FileName.c_str());
//
//	//	�ǂݍ��ݎ��s
//	if (!ofs)
//	{
//		//	���b�Z�[�W������
//		MessageBox(NULL, "�e�L�X�g�t�@�C�����ǂݍ��߂Ă��܂���", "�x��", MB_OK);
//
//		//	���������Ԃ�
//		return;
//	}
//
//	// �e�L�X�g���̃f�[�^������
//	ofs.trunc;
//
//	//��Ɨp�ϐ�
//	static int num_cnt;
//	YsVec3 Pos = YsVec3(0.0f, 0.0f, 0.0f);
//	YsVec3 Scale = YsVec3(1.0f, 1.0f, 1.0f);
//	YsVec3 Angle = YsVec3(0.0f, 0.0f, 0.0f);
//
//	//���X�g�������ׂĉ�
//	for (num_cnt = 0; num_cnt < _num; num_cnt++) {
//
//		//���f�[�^���i�[
//		Pos = HELP.GetObject_All(num_cnt)->GetPos();
//		Scale = HELP.GetObject_All(num_cnt)->GetScale();
//		if (HELP.GetObject_All(num_cnt)->GetObjId() == OBJECT_LIST::ID::PLAYER) {
//			Angle = HELP.GetObject_All(num_cnt)->GetAxis();
//		}
//		else {
//			Angle = HELP.GetObject_All(num_cnt)->GetAngle();
//		}
//
//
//		//�N���X���ۑ�
//		switch (HELP.GetObject_All(num_cnt)->GetObjId()) {
//			//�v���C���[
//		case OBJECT_LIST::ID::PLAYER:
//			ofs << "ClassName(Player)" << TXT_DATA_SAVE << std::endl; break;
//			//�G�l�~�[
//		case OBJECT_LIST::ID::ENEMY:
//			ofs << "ClassName(Enemy)" << TXT_DATA_SAVE << std::endl; break;
//			//�X�e�[�W
//		case OBJECT_LIST::ID::GROUND:
//			ofs << "ClassName(Ground)" << TXT_DATA_SAVE << std::endl; break;
//			//���_
//		case OBJECT_LIST::ID::STRONGHOLD:
//			ofs << "ClassName(Stronghold)" << TXT_DATA_SAVE << std::endl; break;
//			//��
//		case OBJECT_LIST::ID::WALL:
//			ofs << "ClassName(Wall)" << TXT_DATA_SAVE << std::endl; break;
//			//���Ί�
//		case OBJECT_LIST::ID::SLING:
//			ofs << "ClassName(Sling)" << TXT_DATA_SAVE << std::endl; break;
//			//��C
//		case OBJECT_LIST::ID::CANNON:
//			ofs << "ClassName(Cannon)" << TXT_DATA_SAVE << std::endl; break;
//			//�Ή����ˊ�
//		case OBJECT_LIST::ID::FLAMETHROWER:
//			ofs << "ClassName(Flamethrower)" << TXT_DATA_SAVE << std::endl; break;
//			//�v���X�@
//		case OBJECT_LIST::ID::PRESS:
//			ofs << "ClassName(Press)" << TXT_DATA_SAVE << std::endl; break;
//			//�A�[��
//		case OBJECT_LIST::ID::ARM:
//			ofs << "ClassName(Arm)" << TXT_DATA_SAVE << std::endl; break;
//			//�x���g�R���x�A
//		case OBJECT_LIST::ID::BELTCONVEYOR:
//			ofs << "ClassName(Belt)" << TXT_DATA_SAVE << std::endl; break;
//			//�u���b�W
//		case OBJECT_LIST::ID::BRIDGE:
//			ofs << "ClassName(Bridge)" << TXT_DATA_SAVE << std::endl; break;
//		}
//	}
//
//	//�t�@�C���N���[�Y
//	ofs.close();
//}
//
///*trim------------------------------------------->
//
//�w�肳�ꂽ�������؂���
//�؂�����������̑O��̂���Ȃ������������
//
//_Str				: �؂��肽��������̌�
//_TrimCharacterList	: ����Ȃ�������
//
//return : string�^
//
//<-----------------------------------------------*/
//static std::string trim(const std::string& _Str, const char* _TrimCharacterList = " \t\v\r\n")
//{
//	std::string result;
//
//	// ��������g�������镶���ȊO��������ʒu���������܂��B
//	std::string::size_type left = _Str.find_first_not_of(_TrimCharacterList);
//
//	if (left != std::string::npos)
//	{
//		// ��������g�������镶���ȊO�����������ꍇ�́A�����悤�ɉE��������������܂��B
//		std::string::size_type right = _Str.find_last_not_of(_TrimCharacterList);
//
//		// �߂�l�����肵�܂��B�����ł͉E�����猟�����Ă��A�g�������镶���ȊO���K�����݂���̂Ŕ���s�v�ł��B
//		result = _Str.substr(left, right - left + 1);
//	}
//
//	return result;
//}
//
//
//std::vector<std::string> DataManager::GetSplit(
//	std::string _Str,
//	std::string _Split)
//{
//	//	�������i�[���邽�߂̕ϐ�
//	std::vector<std::string> vecRet;
//
//
//	//	�������������̂����̂����ꂩ���q�b�g�����
//	while (_Str.find_first_of(_Split) != std::string::npos)
//	{
//
//		//	�؂��肽���������ݒ�
//		_Str = trim(_Str, " ");
//		_Str = trim(_Str, "\t\v\r\n");
//
//		//	�؂��肽�����������擾
//		int Cp = _Str.find_first_of(_Split);
//
//		//	����������݂̂������ꍇ
//		if (Cp <= 0)
//		{
//			Cp += 1;
//		}
//
//		//	�擪�󕶎����i�[����Ȃ��悤��
//		if (Cp > 0)
//		{
//
//			//	�i�[
//			vecRet.push_back(_Str.substr(0, Cp));
//
//		}
//
//		//	��؂蕶�����񕶎��ȏ�ɑΉ�
//		_Str = _Str.substr(Cp);
//	}
//
//	//	���X�g��Ԃ�
//	return vecRet;
//}







SPtr<CharacterBase> DataManager::CsvDataLoad(const std::string& _FileName)
{

	//===========================
	// CSV�f�[�^�ǂݍ���
	//===========================
	csv.Load(_FileName);

	SPtr<CharacterBase> p;

	//�ǂݍ��݁E�z�u(�O�s�ڂ̓^�C�g���s�Ȃ̂ŁA�P�s�ڂ���)
	for (UINT i = 1; i < csv.m_Tbl.size(); i++) {
		// �f�[�^�擾
		YsVec3 pos;
		pos.x = std::stof(csv.GetCol(i, "X���W"));
		pos.y = std::stof(csv.GetCol(i, "Y���W"));
		pos.z = std::stof(csv.GetCol(i, "Z���W"));
		float size = std::stof(csv.GetCol(i, "�T�C�Y"));
		YsVec3 rotate;
		rotate.x = std::stof(csv.GetCol(i, "�p�xX"));
		rotate.y = std::stof(csv.GetCol(i, "�p�xY"));
		rotate.z = std::stof(csv.GetCol(i, "�p�xZ"));
		std::string className = csv.GetCol(i, "�N���X");

		YsVec3 scale = YsVec3(size, size, size);

		//���f���p
		std::string modelName;
		SPtr<YsGameModel> drawModel;

		//���s��
		YsMatrix mat;

		//�@���W���Z�b�g����
		mat.CreateMove(pos);

		//�@�g��T�C�Y���Z�b�g����
		mat.Scale_Local(size, size, size);

		//�@��]�ʂ��Z�b�g����
		mat.RotateY_Local(rotate.y);
		mat.RotateX_Local(rotate.y);
		mat.RotateZ_Local(rotate.y);

		//====================
		// �L�����N�^
		//====================
		//�v���C���[
		{
			if (className == "Player")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Player> add = CharacterBase::CreateCharaTask<Player>();

				//���b�V���ǂݍ���
				modelName = PLAYER_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);

				//�v���C��[�̏���ۑ�
				p = add;
			}
		}

		//�{�X
		{
			if (className == "Boss")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Boss> add = CharacterBase::CreateCharaTask<Boss>();

				//���b�V���ǂݍ���
				modelName = BOSS_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//�X�P���g��
		{
			if (className == "Skeleton")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Skeleton> add = CharacterBase::CreateCharaTask<Skeleton>();

				//���b�V���ǂݍ���
				modelName = SKELETON_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//�f�[����
		{
			if (className == "Demon")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Demon> add = CharacterBase::CreateCharaTask<Demon>();

				//���b�V���ǂݍ���
				modelName = DEMON_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//====================
		//�I�u�W�F�N�g
		//====================
		//�X�e�[�W
		{
			if (className == "Ground")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Ground> add = CharacterBase::CreateCharaTask<Ground>();

				//���b�V���ǂݍ���
				modelName = GROUND_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//���_
		{
			if (className == "Stronghold")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Stronghold> add = CharacterBase::CreateCharaTask<Stronghold>();

				//���b�V���ǂݍ���
				modelName = STRONGHOLD_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//��
		{
			if (className == "Wall")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Wall> add = CharacterBase::CreateCharaTask<Wall>();

				//���b�V���ǂݍ���
				modelName = WALL_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//���Ί�
		{
			if (className == "Sling")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Sling> add = CharacterBase::CreateCharaTask<Sling>();

				//���b�V���ǂݍ���
				modelName = SLING_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//��C
		{
			if (className == "Cannon")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Cannon> add = CharacterBase::CreateCharaTask<Cannon>();

				//���b�V���ǂݍ���
				modelName = CANNON_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//�Ή����ˊ�
		{
			if (className == "Flamethrower")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Flamethrower> add = CharacterBase::CreateCharaTask<Flamethrower>();

				//���b�V���ǂݍ���
				modelName = FLAMETHROWER_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//�v���X�@
		{
			if (className == "Press")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Press> add = CharacterBase::CreateCharaTask<Press>();

				//���b�V���ǂݍ���
				modelName = PRESS_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//�A�[��
		{
			if (className == "Arm")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Arm> add = CharacterBase::CreateCharaTask<Arm>();

				//���b�V���ǂݍ���
				modelName = ARM_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//�x���g�R���x�A
		{
			if (className == "BeltConveyor")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<BeltConveyor> add = CharacterBase::CreateCharaTask<BeltConveyor>();

				//���b�V���ǂݍ���
				modelName = BELTCONVEYOR_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}

		//�u���b�W
		{
			if (className == "Bridge")
			{
				//�@���X�g�ɃC���X�^���X�������I�u�W�F�N�g���v�b�V������
				SPtr<Bridge> add = CharacterBase::CreateCharaTask<Bridge>();

				//���b�V���ǂݍ���
				modelName = BRIDGE_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//��]�ʃZ�b�g�i�I�u�W�F�N�g�p�j
				add->SetAngle(rotate);

				//�@�I�u�W�F�N�g�̏�����������
				add->Init(mat, drawModel);
			}
		}
	}

	return p;
}

void DataManager::CsvDataSave(const std::string& _FileName)
{
	std::ofstream ofs;

	// �ǂݍ���
	ofs.open(_FileName.c_str());

	//	�ǂݍ��ݎ��s
	if (!ofs)
	{
		//	���b�Z�[�W������
		MessageBox(NULL, "�e�L�X�g�t�@�C�����ǂݍ��߂Ă��܂���", "�x��", MB_OK);

		//	���������Ԃ�
		return;
	}

	// �e�L�X�g���̃f�[�^������
	ofs.trunc;

	//��Ɨp�ϐ�
	YsVec3 Pos = YsVec3(0.0f, 0.0f, 0.0f);
	float size = 1.0f;
	YsVec3 Angle = YsVec3(0.0f, 0.0f, 0.0f);

	std::vector<SPtr<CharacterBase>> cList = CharacterBase::GetList();

	//��ޖ����ɕۑ�
	ofs << "X���W," << "Y���W," << "Z���W," << "�T�C�Y," << "�p�xX," << "�p�xY," << "�p�xZ," << "�N���X" << std::endl;

	//���X�g�������ׂĉ�
	for (UINT i = 0; i < cList.size(); i++) {

		//���f�[�^���i�[
		Pos = cList[i]->GetPos();
		size = cList[i]->GetScale().x;
		if (cList[i]->GetObjId() == OBJECT_LIST::ID::PLAYER) {
			Angle = cList[i]->GetAxis();
		}
		else {
			Angle = cList[i]->GetAngle();
		}


		//�N���X���ۑ�
		switch (cList[i]->GetObjId()) {
			//�v���C���[
		case OBJECT_LIST::ID::PLAYER:
			ofs << CSV_DATA_SAVE << "Player" << std::endl; break;
			//�{�X
		case OBJECT_LIST::ID::BOSS:
			ofs << CSV_DATA_SAVE << "Boss" << std::endl; break;
			//�X�P���g��
		case OBJECT_LIST::ID::SKELETON:
			ofs << CSV_DATA_SAVE << "Skeleton" << std::endl; break;
			//�f�[����
		case OBJECT_LIST::ID::DEMON:
			ofs << CSV_DATA_SAVE << "Demon" << std::endl; break;
			//�X�e�[�W
		case OBJECT_LIST::ID::GROUND:
			ofs << CSV_DATA_SAVE << "Ground" << std::endl; break;
			//���_
		case OBJECT_LIST::ID::STRONGHOLD:
			ofs << CSV_DATA_SAVE << "Stronghold" << std::endl; break;
			//��
		case OBJECT_LIST::ID::WALL:
			ofs << CSV_DATA_SAVE << "Wall" << std::endl; break;
			//���Ί�
		case OBJECT_LIST::ID::SLING:
			ofs << CSV_DATA_SAVE << "Sling" << std::endl; break;
			//��C
		case OBJECT_LIST::ID::CANNON:
			ofs << CSV_DATA_SAVE << "Cannon" << std::endl; break;
			//�Ή����ˊ�
		case OBJECT_LIST::ID::FLAMETHROWER:
			ofs << CSV_DATA_SAVE << "Flamethrower" << std::endl; break;
			//�v���X�@
		case OBJECT_LIST::ID::PRESS:
			ofs << CSV_DATA_SAVE << "Press" << std::endl; break;
			//�A�[��
		case OBJECT_LIST::ID::ARM:
			ofs << CSV_DATA_SAVE << "Arm" << std::endl; break;
			//�x���g�R���x�A
		case OBJECT_LIST::ID::BELTCONVEYOR:
			ofs << CSV_DATA_SAVE << "BeltConveyor" << std::endl; break;
			//�u���b�W
		case OBJECT_LIST::ID::BRIDGE:
			ofs << CSV_DATA_SAVE << "Bridge" << std::endl; break;
		}
	}

	cList.clear();

	//�t�@�C���N���[�Y
	ofs.close();
}