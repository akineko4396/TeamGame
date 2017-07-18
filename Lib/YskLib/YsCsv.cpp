#include "YskLib.h"

using namespace YskLib;

// ���{�ꔻ��
//#define isSJIS(a) ((BYTE)a >= 0x81 && (BYTE)a <= 0x9f || (BYTE)a >= 0xe0 && (BYTE)a<=0xfc)

// ���{�ꔻ��
static bool isSJIS(char a) {
	return ((BYTE)a >= 0x81 && (BYTE)a <= 0x9f || (BYTE)a >= 0xe0 && (BYTE)a <= 0xfc);
}

bool YsCsv::Load(const std::string& filename,bool bDelTitle)
{
	FILE* fp = fopen(filename.c_str(),"rb");
	if(fp == nullptr)return false;

	fseek(fp, 0L, SEEK_END);	//�����čŏ�����t�@�C���|�C���^�̈ʒu���Ō�ɂ���
	int filesize = ftell(fp);	//�t�@�C���T�C�Y����
	fseek(fp, 0L, SEEK_SET);    //�ŏ��ɖ߂��Ď��̏����ɔ�����
	char* buf = new char[filesize+1];
	fread(buf,filesize,1,fp);
	fclose(fp);

	buf[filesize] = '\0';

	std::string ttt = buf;
	bool bRet = LoadFromText(buf, bDelTitle);

	delete buf;
	return bRet;

}

static bool IsKAIGYO(const char* str)
{
	if(*str == '\n'){
		return true;
	}
	else if(strncmp(str, "\r\n", 2) == 0){
		return true;
	}
	return false;
}

bool YsCsv::LoadFromText(const std::string& text, bool bDelTitle)
{
	m_Tbl.clear();

	std::vector<std::string> tmp;

	std::string str;
	const char* s = text.c_str();
	bool bAdd=true;
	while(1){
		// �I�[
		if(*s == '\0'){
			break;
		}
		tmp.clear();



		const char* pRead = s;
		while(1){
			// ���s�Ȃ玟��
			if(*s == '\n'){
				s++;
				break;
			}
			else if(strncmp(s, "\r\n", 2) == 0){
				s+=2;
				break;
			}
			if(*pRead == ',')pRead++;

			if(*pRead == '\0'){
				s = pRead;
				break;
			}

			if(*pRead == '"' && strncmp(pRead,"\"\"",2) != 0){	// ������w�肪����I
				pRead++;
				const char* pEnd = pRead;
				while(1){
					if(isSJIS(*pEnd)){
						pEnd++;
					}
					else{
						if(*pEnd == '"' && strncmp(pEnd,"\"\"",2) != 0){	// ������w�肪����I
							int n = pEnd - pRead;
							str.resize(n);
							strncpy(&str[0],pRead,n);
//							str[n] = '\0';
							tmp.push_back(str);
							pRead = pEnd;
							pRead++;	// "
							s = pRead;
							break;
						}
						pEnd++;
					}
				}
			}
			else{
				const char* pEnd = pRead;
				while(1){
					if(isSJIS(*pEnd)){
						pEnd++;
					}
					else{
						if(*pEnd == ',' || IsKAIGYO(pEnd) || *pEnd == '\0'){	// ��؂�
							int n = pEnd - pRead;
							str.resize(n);
							strncpy(&str[0],pRead,n);
//							str[n] = '\0';
							tmp.push_back(str);
							pRead = pEnd;
//							pRead++;
							s = pRead;
							if(str == "ImpactEffect"){
								int a = 0;
							}
							break;
						}
						/*
						else if(*pEnd == '\0'){ // �I�[�܂ŗ��Ă��܂����B�������̃f�[�^�Ȃ̂Œǉ��͂��Ȃ�
							bAdd = false;
							pRead = pEnd;
							s = pRead;
							break;
						}
						*/
						pEnd++;
					}
				}
			}
		}

		if(bDelTitle){
			bDelTitle = false;
		}
		else{
			// �s�ɒǉ�
			if(bAdd){
				m_Tbl.push_back(tmp);
				tmp.clear();
			}
		}
	}

	return true;
}

//  �������u������
static std::string Replace( std::string String1, std::string String2, std::string String3 )
{
	std::string::size_type  Pos( String1.find( String2 ) );

	while( Pos != std::string::npos )
	{
		String1.replace( Pos, String2.length(), String3 );
		Pos = String1.find( String2, Pos + String3.length() );
	}

	return String1;
}

bool YsCsv::Save(const std::string& filename)
{
	std::ofstream ofs(filename, std::ios_base::binary);
	if( !ofs ) {
		return false;
	}

	for(UINT y=0;y<m_Tbl.size();y++){
		for(UINT x=0;x<m_Tbl[y].size();x++){
			bool bStr = false;
			// ","��������
			if(std::string::npos != m_Tbl[y][x].find(",")){
				bStr = true;
			}
			// ���s����
			if(std::string::npos != m_Tbl[y][x].find("\n")){
				bStr = true;
			}
			// ���{�ꔻ��
			/*
			for(UINT si=0;si<inData[y][x].size();si++){
				if(isSJIS(inData[y][x][si])){
					bStr = true;
					break;
				}
			}
			*/

			if(bStr){
				ofs << "\"";
			}

			std::string sTmp = m_Tbl[y][x];
			if(bStr){
				sTmp = Replace(sTmp, "\"", "\"\"");
			}
			ofs << sTmp;

			if(bStr){
				ofs << "\"";
			}

			if(x < m_Tbl[y].size()-1){
				ofs << ",";
			}
		}
		ofs << std::endl;
	}

	return true;
}
