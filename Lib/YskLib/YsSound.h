//===============================================================
//! @file YsSound.h
//! @brief サウンド関係
//!
//!	YsSoundManagerクラス	… 再生中のサウンド管理など。\n
//!	YsSoundDataクラス		… サウンドの読み込みや、実際に再生するインスタンスの再生など。\n
//!	YsSoundクラス			… 実際再生されるサウンドインスタンス。YsSoundDataクラスから生成される。\n
//!
//! @author 鎌田
//===============================================================
#ifndef __YsSound_h__
#define __YsSound_h__

namespace YskLib{

//=======================================================
//! @brief 再生用サウンドインスタンスクラス
//!
//!  実際に再生するサウンドオブジェクト				\n
//!  YsSoundDataクラスのCreateInstance()で作成可能	\n
//!
//! @ingroup Sound
//=======================================================
class YsSound : public std::enable_shared_from_this<YsSound>{
	friend class YsSoundData;
public:
	//==================================================================
	// 再生
	//==================================================================
	//! @brief 2D再生
	//! @param[in]	Loop	… ループ再生するか？
	void Play(bool Loop = false);

	//! @brief 3D再生　※モノラルサウンドのみ3D再生可能です
	//! @param[in]	x	… x座標
	//! @param[in]	y	… y座標
	//! @param[in]	z	… z座標
	//! @param[in]	Loop	… ループ再生するか？
	void Play3D(float x, float y, float z, bool Loop = false);

	//==================================================================
	// 停止
	//==================================================================
	//! @brief 停止
	void Stop() {
		if(m_lpBuf)m_lpBuf->Stop();
	}

	//==================================================================
	// 設定
	//==================================================================
	//! @brief ボリューム設定
	//! @param[in]	Vol	… ボリューム設定(0～100 %)
	void SetVolume(int Vol);

	//! @brief 3Dサウンド減衰設定
	//! @param[in]	Min	… サウンドが減衰を始める距離
	//! @param[in]	Max	… それ以上減衰しなくなるリスナーからの距離
	void SetDistance(float fMin, float fMax);
	
	//! @brief 3Dサウンド座標設定
	//! @param[in]	x	… x座標
	//! @param[in]	y	… y座標
	//! @param[in]	z	… z座標
	void SetPos(float x, float y, float z);

	//==================================================================
	// 取得
	//==================================================================
	//! @brief 再生中か？
	//! @return 再生中ならtrue
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
//! @brief サウンドデータクラス
//!
//!　サウンドの元データとなるクラス				\n
//!　これ自体では再生できません					\n
//!　CreateInstance()で実際に再生するサウンドYsSoundクラスのインスタンスを生成	\n
//!
//! @ingroup Sound
//=======================================================
class YsSoundData : public YsIResource {
	friend class YsSound;
public:
	//==================================================================
	//! @brief WAVEサウンド読み込み
	//! @param[in]	filename	… ファイル名
	//! @param[in]	b3D			… 3Dサウンドにするか？
	//==================================================================
	bool LoadWaveFile(const std::string& fileName, bool b3D);

	//==================================================================
	//! @brief 解放
	//==================================================================
	void Release();

	//==================================================================
	//! @brief 再生用のサウンドインスタンス作成
	//! @return 新たに作成した再生用サウンドインスタンス
	//==================================================================
	SPtr<YsSound> CreateInstance();

	//==================================================================
	//! @brief リソースタイプ取得
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
//! @brief サウンド管理 シングルトンクラス
//!
//! 
//! @ingroup Sound
//================================================
class YsSoundManager
{
friend class YsSound;
public:
	//==================================================================
	// 取得系
	//==================================================================
	//! @brief DirectSoundオブジェクト取得
	LPDIRECTSOUND8				GetDirectSound(){return m_lpDirectSound;}

	//! @brief 3Dリスナー(聞き手)取得
	LPDIRECTSOUND3DLISTENER8	GetListener(){return m_lpListener;}

	//! @brief 初期化されているか？
	bool						IsInit() { return (m_lpDirectSound != nullptr); }

	//! @brief 現在再生中のサウンド数取得
	int							GetNumPlaying() { return (int)m_PlayList.size(); }

	//==================================================================
	// 初期化・解放
	//==================================================================
	//! @brief 初期化
	//! @param[in]	hwnd	… ウィンドウハンドル
	BOOL Init(HWND hwnd);

	//! @brief 解放
	void Release();

	//==================================================================
	// 共通
	//==================================================================
	//! @brief すべて停止
	void StopAll();

	//==================================================================
	// 処理
	//==================================================================
	//! @brief 処理　定期的に実行してください
	//! 再生中のサウンドの管理やリスナーの設定繁栄などを行っています
	void Run();

	//==================================================================
	~YsSoundManager();

private:
	HWND						m_hWnd;
	LPDIRECTSOUND8				m_lpDirectSound;				//!< DirectSound
	LPDIRECTSOUNDBUFFER			m_lpDSBPrimary;
	LPDIRECTSOUND3DLISTENER8	m_lpListener;					//!< リスナー

	std::map<UINT, SPtr<YsSound>>	m_PlayList;

private:
	// 複製リスト追加。実際に再生されているものになる。
	void AddPlayList(SPtr<YsSound> snd){
		if(snd == nullptr)return;
		m_PlayList[(UINT)snd.get()] = snd;
	}

//===================================================================
// シングルトン
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
//! @brief Waveファイルローダークラス
//!
//! Waveサウンドファイルからデータを読み込む
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

	// WAVEファイルのヘッダ
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
