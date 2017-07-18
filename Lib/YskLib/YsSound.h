//===============================================================
//! @file YsSound.h
//! @brief �T�E���h�֌W
//!
//!	YsSoundManager�N���X	�c �Đ����̃T�E���h�Ǘ��ȂǁB\n
//!	YsSoundData�N���X		�c �T�E���h�̓ǂݍ��݂�A���ۂɍĐ�����C���X�^���X�̍Đ��ȂǁB\n
//!	YsSound�N���X			�c ���ۍĐ������T�E���h�C���X�^���X�BYsSoundData�N���X���琶�������B\n
//!
//! @author ���c
//===============================================================
#ifndef __YsSound_h__
#define __YsSound_h__

namespace YskLib{

//=======================================================
//! @brief �Đ��p�T�E���h�C���X�^���X�N���X
//!
//!  ���ۂɍĐ�����T�E���h�I�u�W�F�N�g				\n
//!  YsSoundData�N���X��CreateInstance()�ō쐬�\	\n
//!
//! @ingroup Sound
//=======================================================
class YsSound : public std::enable_shared_from_this<YsSound>{
	friend class YsSoundData;
public:
	//==================================================================
	// �Đ�
	//==================================================================
	//! @brief 2D�Đ�
	//! @param[in]	Loop	�c ���[�v�Đ����邩�H
	void Play(bool Loop = false);

	//! @brief 3D�Đ��@�����m�����T�E���h�̂�3D�Đ��\�ł�
	//! @param[in]	x	�c x���W
	//! @param[in]	y	�c y���W
	//! @param[in]	z	�c z���W
	//! @param[in]	Loop	�c ���[�v�Đ����邩�H
	void Play3D(float x, float y, float z, bool Loop = false);

	//==================================================================
	// ��~
	//==================================================================
	//! @brief ��~
	void Stop() {
		if(m_lpBuf)m_lpBuf->Stop();
	}

	//==================================================================
	// �ݒ�
	//==================================================================
	//! @brief �{�����[���ݒ�
	//! @param[in]	Vol	�c �{�����[���ݒ�(0�`100 %)
	void SetVolume(int Vol);

	//! @brief 3D�T�E���h�����ݒ�
	//! @param[in]	Min	�c �T�E���h���������n�߂鋗��
	//! @param[in]	Max	�c ����ȏ㌸�����Ȃ��Ȃ郊�X�i�[����̋���
	void SetDistance(float fMin, float fMax);
	
	//! @brief 3D�T�E���h���W�ݒ�
	//! @param[in]	x	�c x���W
	//! @param[in]	y	�c y���W
	//! @param[in]	z	�c z���W
	void SetPos(float x, float y, float z);

	//==================================================================
	// �擾
	//==================================================================
	//! @brief �Đ������H
	//! @return �Đ����Ȃ�true
	bool IsPlay();

	//==================================================================
	~YsSound();

private:
	// 
	YsSound() : m_lpBuf(0), m_lpBuf3D(0){
	}

private:
	LPDIRECTSOUNDBUFFER8	m_lpBuf;
	LPDIRECTSOUND3DBUFFER8	m_lpBuf3D;
};

class YsWaveFile;
//=======================================================
//! @brief �T�E���h�f�[�^�N���X
//!
//!�@�T�E���h�̌��f�[�^�ƂȂ�N���X				\n
//!�@���ꎩ�̂ł͍Đ��ł��܂���					\n
//!�@CreateInstance()�Ŏ��ۂɍĐ�����T�E���hYsSound�N���X�̃C���X�^���X�𐶐�	\n
//!
//! @ingroup Sound
//=======================================================
class YsSoundData : public YsIResource {
	friend class YsSound;
public:
	//==================================================================
	//! @brief WAVE�T�E���h�ǂݍ���
	//! @param[in]	filename	�c �t�@�C����
	//! @param[in]	b3D			�c 3D�T�E���h�ɂ��邩�H
	//==================================================================
	bool LoadWaveFile(const std::string& fileName, bool b3D);

	//==================================================================
	//! @brief ���
	//==================================================================
	void Release();

	//==================================================================
	//! @brief �Đ��p�̃T�E���h�C���X�^���X�쐬
	//! @return �V���ɍ쐬�����Đ��p�T�E���h�C���X�^���X
	//==================================================================
	SPtr<YsSound> CreateInstance();

	//==================================================================
	//! @brief ���\�[�X�^�C�v�擾
	//==================================================================
	virtual std::string GetTypeName() const override {
		std::string str = "YsSoundData : ";
		str += m_FileName;
		return str;
	}

	//==================================================================
	YsSoundData() : m_lpBuf(0), m_lpBuf3D(0){
	}
	~YsSoundData() {
		Release();
	}

private:
	LPDIRECTSOUNDBUFFER8	m_lpBuf;
	LPDIRECTSOUND3DBUFFER8	m_lpBuf3D;
	std::string				m_FileName;

	bool CreateBuffer(YsWaveFile* lpcw, bool b3D);
};

//================================================
//!
//! @brief �T�E���h�Ǘ� �V���O���g���N���X
//!
//! 
//! @ingroup Sound
//================================================
class YsSoundManager
{
friend class YsSound;
public:
	//==================================================================
	// �擾�n
	//==================================================================
	//! @brief DirectSound�I�u�W�F�N�g�擾
	LPDIRECTSOUND8				GetDirectSound(){return m_lpDirectSound;}

	//! @brief 3D���X�i�[(������)�擾
	LPDIRECTSOUND3DLISTENER8	GetListener(){return m_lpListener;}

	//! @brief ����������Ă��邩�H
	bool						IsInit() { return (m_lpDirectSound != nullptr); }

	//! @brief ���ݍĐ����̃T�E���h���擾
	int							GetNumPlaying() { return (int)m_PlayList.size(); }

	//==================================================================
	// �������E���
	//==================================================================
	//! @brief ������
	//! @param[in]	hwnd	�c �E�B���h�E�n���h��
	BOOL Init(HWND hwnd);

	//! @brief ���
	void Release();

	//==================================================================
	// ����
	//==================================================================
	//! @brief ���ׂĒ�~
	void StopAll();

	//==================================================================
	// ����
	//==================================================================
	//! @brief �����@����I�Ɏ��s���Ă�������
	//! �Đ����̃T�E���h�̊Ǘ��⃊�X�i�[�̐ݒ�ɉh�Ȃǂ��s���Ă��܂�
	void Run();

	//==================================================================
	~YsSoundManager();

private:
	HWND						m_hWnd;
	LPDIRECTSOUND8				m_lpDirectSound;				//!< DirectSound
	LPDIRECTSOUNDBUFFER			m_lpDSBPrimary;
	LPDIRECTSOUND3DLISTENER8	m_lpListener;					//!< ���X�i�[

	std::map<UINT, SPtr<YsSound>>	m_PlayList;

private:
	// �������X�g�ǉ��B���ۂɍĐ�����Ă�����̂ɂȂ�B
	void AddPlayList(SPtr<YsSound> snd){
		if(snd == nullptr)return;
		m_PlayList[(UINT)snd.get()] = snd;
	}

//===================================================================
// �V���O���g��
private:
	YsSoundManager();

public:
	static YsSoundManager &GetInstance() {
		static YsSoundManager Instance;
		return Instance;
	}
	//===================================================================
};




//===================================================================
//! @brief Wave�t�@�C�����[�_�[�N���X
//!
//! Wave�T�E���h�t�@�C������f�[�^��ǂݍ���
//!
//! @ingroup Sound
//===================================================================
class YsWaveFile {
public:
	enum {
		WAVEFILE_READ = 1,
		WAVEFILE_WRITE = 2,
	};

public:
	WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure
	HMMIO         m_hmmio;       // MM I/O handle for the WAVE
	MMCKINFO      m_ck;          // Multimedia RIFF chunk
	MMCKINFO      m_ckRiff;      // Use in opening a WAVE file
	DWORD         m_dwSize;      // The size of the wave file
	MMIOINFO      m_mmioinfoOut;
	DWORD         m_dwFlags;
	BOOL          m_bIsReadingFromMemory;
	BYTE*         m_pbData;
	BYTE*         m_pbDataCur;
	ULONG         m_ulDataSize;
	CHAR*         m_pResourceBuffer;

protected:
	HRESULT ReadMMIO();
	HRESULT WriteMMIO(WAVEFORMATEX *pwfxDest);

public:
	YsWaveFile();
	~YsWaveFile();

	HRESULT Open(LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags);
	HRESULT OpenFromMemory(BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags);
	HRESULT Close();

	HRESULT Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead);
	HRESULT Write(UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote);

	DWORD   GetSize();
	HRESULT ResetFile();
	WAVEFORMATEX* GetFormat() { return m_pwfx; };

	// WAVE�t�@�C���̃w�b�_
	struct WAVEFILEHEADER {
		char         cRIFF[4];
		int          iSizeRIFF;
		char         cType[4];
		char         cFmt[4];
		int          iSizeFmt;
		WAVEFORMATEX WaveFmt;
		char         cData[4];
		int          iSizeData;
	};

};


}

#endif
