#include "YskLib.h"

using namespace YskLib;

#define DXTRACE_ERR(str,hr)           (hr)

//==================================================================
// YsSound
//==================================================================
void YsSound::Play(bool Loop)
{
	if(m_lpBuf == nullptr)return;

	m_lpBuf->SetVolume(DSBVOLUME_MAX);
	m_lpBuf->SetCurrentPosition(0);
	DWORD Flag = 0;
	if(Loop) {
		Flag |= DSBPLAY_LOOPING;
	}
	HRESULT hr = m_lpBuf->Play(0, 0, Flag);
	if(hr == DSERR_BUFFERLOST) {
		m_lpBuf->Restore();
	}

	// 再生リストへ追加
	YsSndMgr.AddPlayList(YsToSPtr(this));
}

// 3D Sound Ver
void YsSound::Play3D(float x, float y, float z, bool Loop)
{
	if(m_lpBuf == nullptr)return;
	if(m_lpBuf3D == nullptr)return;

	m_lpBuf3D->SetPosition(x, y, z, DS3D_DEFERRED);//DS3D_DEFERRED DS3D_IMMEDIATE

	DWORD Flag = 0;
	if(Loop) {
		Flag |= DSBPLAY_LOOPING;
	}
	m_lpBuf->SetVolume(DSBVOLUME_MAX);
	m_lpBuf->SetCurrentPosition(0);
	HRESULT hr = m_lpBuf->Play(0, 0, Flag);
	if(hr == DSERR_BUFFERLOST) {
		m_lpBuf->Restore();
	}

	// 再生リストへ追加
	YsSndMgr.AddPlayList(shared_from_this());
}

void YsSound::SetVolume(int Vol)
{
	if(m_lpBuf == nullptr)return;
	m_lpBuf->SetVolume((long)(2000 * log10((float)(Vol) / 100.0f)));
}

void YsSound::SetDistance(float fMin, float fMax)
{
	if(m_lpBuf3D == nullptr)return;
	m_lpBuf3D->SetMinDistance(fMin, DS3D_DEFERRED);
	m_lpBuf3D->SetMaxDistance(fMax, DS3D_DEFERRED);
}

void YsSound::SetPos(float x, float y, float z)
{
	if(m_lpBuf3D == nullptr)return;
	m_lpBuf3D->SetPosition(x, y, z, DS3D_DEFERRED);
}

bool YsSound::IsPlay()
{
	if(m_lpBuf == nullptr)return false;
	DWORD dw;
	m_lpBuf->GetStatus(&dw);
	if(dw & DSBSTATUS_PLAYING) {
		// 再生中
		return true;
	}
	return false;
}

YsSound::~YsSound()
{
	YsSafe_Release(m_lpBuf);
	YsSafe_Release(m_lpBuf3D);
}

//==================================================================
// YsSoundData
//==================================================================
bool YsSoundData::LoadWaveFile(const std::string& fileName, bool b3D)
{
	if(!YsSndMgr.IsInit())return false;

	// Waveファイルを開く
	HRESULT hr;
	YsWaveFile cw;
	// 開く
	hr = cw.Open((LPTSTR)fileName.c_str(), nullptr, YsWaveFile::WAVEFILE_READ);
	if(FAILED(hr)) {
		return FALSE;
	}

	// 読み込み開始位置をファイルの先頭に設定
	cw.ResetFile();
	// バッファ作成
	if(CreateBuffer(&cw, b3D) == false){
		cw.Close();
		return false;
	}
	cw.Close();

	m_FileName = fileName;

	return true;
}

SPtr<YsSound> YsSoundData::CreateInstance()
{
	if(!YsSndMgr.IsInit())return nullptr;

	if(m_lpBuf == nullptr)return nullptr;

	SPtr<YsSound> inst(new YsSound());

	LPDIRECTSOUNDBUFFER lpDup;
	HRESULT hr;

	hr = YsSndMgr.GetDirectSound()->DuplicateSoundBuffer(m_lpBuf, &lpDup);
	if(FAILED(hr)) {
		return UPtr<YsSound>();
	}

	hr = lpDup->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&inst->m_lpBuf);

	if(m_lpBuf3D) {
		hr = lpDup->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&inst->m_lpBuf3D);
		if(FAILED(hr)) {
			return nullptr;
		}
	}

	lpDup->Release();

	return inst;
}

bool YsSoundData::CreateBuffer(YsWaveFile *lpcw, bool b3D)
{
	if(!YsSndMgr.IsInit())return false;

	HRESULT hr;

	DWORD datasize = lpcw->GetSize();

	// セカンダリバッファを作成し、書き込む
	DSBUFFERDESC dsc;
	ZeroMemory(&dsc, sizeof(DSBUFFERDESC));
	dsc.dwSize = sizeof(DSBUFFERDESC);
	dsc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
	dsc.dwBufferBytes = lpcw->GetSize();
	dsc.lpwfxFormat = lpcw->GetFormat();
	if(b3D) {
		dsc.dwFlags |= DSBCAPS_CTRL3D;
	}

	LPDIRECTSOUNDBUFFER lpDSB;
	hr = YsSndMgr.GetDirectSound()->CreateSoundBuffer(&dsc, &lpDSB, nullptr);
	if(FAILED(hr)) {
		return false;
	}

	hr = lpDSB->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_lpBuf);
	if(FAILED(hr)) {
		return false;
	}
	if(b3D) {
		m_lpBuf->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&m_lpBuf3D);
		if(FAILED(hr)) {
			return false;
		}
	}
	lpDSB->Release();

	// バッファをロック
	LPVOID pMem1, pMem2;
	DWORD dwSize1, dwSize2;
	hr = m_lpBuf->Lock(0, datasize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
	if(FAILED(hr)) {
		return false;
	}

	DWORD dw;
	// バッファにデータをコピー
	lpcw->Read((BYTE*)pMem1, dwSize1, &dw);
	if(0 != dwSize2) {
		lpcw->Read((BYTE*)pMem2, dwSize2, &dw);
	}

	// ロックを解除
	m_lpBuf->Unlock(pMem1, dwSize1, pMem2, dwSize2);

	return true;
}

void YsSoundData::Release()
{
	YsSafe_Release(m_lpBuf);
	YsSafe_Release(m_lpBuf3D);
	m_FileName = "";
}



//==================================================================
//
//
// SoundManager
//
//
//==================================================================
YsSoundManager::YsSoundManager()
{
	m_hWnd = nullptr;
	m_lpDirectSound = nullptr;
	m_lpDSBPrimary = nullptr;
}

YsSoundManager::~YsSoundManager()
{
	Release();
}

BOOL YsSoundManager::Init(HWND hwnd)
{
	m_hWnd = hwnd;
	LPGUID lpGuid = nullptr;

	// DirectSoundオブジェクトの作成
	HRESULT hr;
	hr = DirectSoundCreate8(lpGuid,&m_lpDirectSound,nullptr);
	if(FAILED(hr)){
		return FALSE;
	}

	// 協調レベルの設定
	hr = m_lpDirectSound->SetCooperativeLevel(m_hWnd,DSSCL_PRIORITY);
	if(FAILED(hr)){
		return FALSE;
	}

	// デバイスの性能の調査

	// スピーカの調査

	// プライマリバッファの作成
	DSBUFFERDESC dsc;
	ZeroMemory(&dsc,sizeof(DSBUFFERDESC));
	dsc.dwSize = sizeof(DSBUFFERDESC);
	dsc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	dsc.dwBufferBytes = 0;
	dsc.lpwfxFormat = nullptr;
	hr = m_lpDirectSound->CreateSoundBuffer(&dsc,&m_lpDSBPrimary,nullptr);
	if(FAILED(hr)){
		return FALSE;
	}

	// リスナーの作成
	hr = m_lpDSBPrimary->QueryInterface(IID_IDirectSound3DListener8,(LPVOID*)&m_lpListener);
	if(FAILED(hr)){
		return FALSE;
	}


	// リスナー設定
	m_lpListener->SetDopplerFactor(1.0f, DS3D_IMMEDIATE);

	// ロールオフ設定(距離に夜音の減衰()
//	m_lpListener->SetRolloffFactor(0.2f,DS3D_IMMEDIATE);

	return TRUE;
}


void YsSoundManager::Release()
{
	m_PlayList.clear();

	YsSafe_Release(m_lpDSBPrimary);
	YsSafe_Release(m_lpListener);

	// DirectSound8インターフェイス開放
	YsSafe_Release(m_lpDirectSound);
}
void YsSoundManager::StopAll()
{
	if(m_lpDirectSound == nullptr)return;

	// 再生リスト
	{
		auto it = m_PlayList.begin();
		while(it != m_PlayList.end()){
			(*it).second->Stop();
			++it;
		}
	}

}

void YsSoundManager::Run()
{
	// 再生リスト処理
	{
		auto it = m_PlayList.begin();
		while(it != m_PlayList.end()){
			// 再生中でないときは、リストから削除(どこからも参照されていなければ解放される)
			if(!(*it).second->IsPlay()){
				it = m_PlayList.erase(it);
				continue;
			}

			++it;
		}
	}

	// 3D関係の設定反映
	m_lpListener->CommitDeferredSettings();
}


//-----------------------------------------------------------------------------
// Name: YsWaveFile::YsWaveFile()
// Desc: Constructs the class.  Call Open() to open a wave file for reading.  
//       Then call Read() as needed.  Calling the destructor or Close() 
//       will close the file.  
//-----------------------------------------------------------------------------
YsWaveFile::YsWaveFile() {
	m_pwfx = nullptr;
	m_hmmio = nullptr;
	m_pResourceBuffer = nullptr;
	m_dwSize = 0;
	m_bIsReadingFromMemory = FALSE;
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::~YsWaveFile()
// Desc: Destructs the class
//-----------------------------------------------------------------------------
YsWaveFile::~YsWaveFile() {
	Close();

	if(!m_bIsReadingFromMemory)
		YsSafe_Delete_Array(m_pwfx);
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::Open()
// Desc: Opens a wave file for reading
//-----------------------------------------------------------------------------
HRESULT YsWaveFile::Open(LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags) {
	HRESULT hr;

	m_dwFlags = dwFlags;
	m_bIsReadingFromMemory = FALSE;

	if(m_dwFlags == WAVEFILE_READ) {
		if(strFileName == nullptr)
			return E_INVALIDARG;
		YsSafe_Delete_Array(m_pwfx);

		m_hmmio = mmioOpen(strFileName, nullptr, MMIO_ALLOCBUF | MMIO_READ);

		if(nullptr == m_hmmio) {
			HRSRC   hResInfo;
			HGLOBAL hResData;
			DWORD   dwSize;
			VOID*   pvRes;

			// Loading it as a file failed, so try it as a resource
			if(nullptr == (hResInfo = FindResource(nullptr, strFileName, TEXT("WAVE")))) {
				if(nullptr == (hResInfo = FindResource(nullptr, strFileName, TEXT("WAV"))))
					return DXTRACE_ERR(TEXT("FindResource"), E_FAIL);
			}

			if(nullptr == (hResData = LoadResource(nullptr, hResInfo)))
				return DXTRACE_ERR(TEXT("LoadResource"), E_FAIL);

			if(0 == (dwSize = SizeofResource(nullptr, hResInfo)))
				return DXTRACE_ERR(TEXT("SizeofResource"), E_FAIL);

			if(nullptr == (pvRes = LockResource(hResData)))
				return DXTRACE_ERR(TEXT("LockResource"), E_FAIL);

			m_pResourceBuffer = new CHAR[dwSize];
			memcpy(m_pResourceBuffer, pvRes, dwSize);

			MMIOINFO mmioInfo;
			ZeroMemory(&mmioInfo, sizeof(mmioInfo));
			mmioInfo.fccIOProc = FOURCC_MEM;
			mmioInfo.cchBuffer = dwSize;
			mmioInfo.pchBuffer = (CHAR*)m_pResourceBuffer;

			m_hmmio = mmioOpen(nullptr, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);
		}

		if(FAILED(hr = ReadMMIO())) {
			// ReadMMIO will fail if its an not a wave file
			mmioClose(m_hmmio, 0);
			return DXTRACE_ERR(TEXT("ReadMMIO"), hr);
		}

		if(FAILED(hr = ResetFile()))
			return DXTRACE_ERR(TEXT("ResetFile"), hr);

		// After the reset, the size of the wav file is m_ck.cksize so store it now
		m_dwSize = m_ck.cksize;
	} else {
		m_hmmio = mmioOpen(strFileName, nullptr, MMIO_ALLOCBUF |
			MMIO_READWRITE |
			MMIO_CREATE);
		if(nullptr == m_hmmio)
			return DXTRACE_ERR(TEXT("mmioOpen"), E_FAIL);

		if(FAILED(hr = WriteMMIO(pwfx))) {
			mmioClose(m_hmmio, 0);
			return DXTRACE_ERR(TEXT("WriteMMIO"), hr);
		}

		if(FAILED(hr = ResetFile()))
			return DXTRACE_ERR(TEXT("ResetFile"), hr);
	}

	return hr;
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::OpenFromMemory()
// Desc: copy data to YsWaveFile member variable from memory
//-----------------------------------------------------------------------------
HRESULT YsWaveFile::OpenFromMemory(BYTE* pbData, ULONG ulDataSize,
	WAVEFORMATEX* pwfx, DWORD dwFlags) {
	m_pwfx = pwfx;
	m_ulDataSize = ulDataSize;
	m_pbData = pbData;
	m_pbDataCur = m_pbData;
	m_bIsReadingFromMemory = TRUE;

	if(dwFlags != WAVEFILE_READ)
		return E_NOTIMPL;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::ReadMMIO()
// Desc: Support function for reading from a multimedia I/O stream.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_pwfx. 
//-----------------------------------------------------------------------------
HRESULT YsWaveFile::ReadMMIO() {
	MMCKINFO        ckIn;           // chunk info. for general use.
	PCMWAVEFORMAT   pcmWaveFormat;  // Temp PCM structure to load in.       

	m_pwfx = nullptr;

	if((0 != mmioDescend(m_hmmio, &m_ckRiff, nullptr, 0)))
		return DXTRACE_ERR(TEXT("mmioDescend"), E_FAIL);

	// Check to make sure this is a valid wave file
	if((m_ckRiff.ckid != FOURCC_RIFF) ||
		(m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
		return DXTRACE_ERR(TEXT("mmioFOURCC"), E_FAIL);

	// Search the input file for for the 'fmt ' chunk.
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if(0 != mmioDescend(m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK))
		return DXTRACE_ERR(TEXT("mmioDescend"), E_FAIL);

	// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
	// if there are extra parameters at the end, we'll ignore them
	if(ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT))
		return DXTRACE_ERR(TEXT("sizeof(PCMWAVEFORMAT)"), E_FAIL);

	// Read the 'fmt ' chunk into <pcmWaveFormat>.
	if(mmioRead(m_hmmio, (HPSTR)&pcmWaveFormat,
		sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
		return DXTRACE_ERR(TEXT("mmioRead"), E_FAIL);

	// Allocate the waveformatex, but if its not pcm format, read the next
	// word, and thats how many extra bytes to allocate.
	if(pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM) {
		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
		if(nullptr == m_pwfx)
			return DXTRACE_ERR(TEXT("m_pwfx"), E_FAIL);

		// Copy the bytes from the pcm structure to the waveformatex structure
		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		m_pwfx->cbSize = 0;
	} else {
		// Read in length of extra bytes.
		WORD cbExtraBytes = 0L;
		if(mmioRead(m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD))
			return DXTRACE_ERR(TEXT("mmioRead"), E_FAIL);

		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX) + cbExtraBytes];
		if(nullptr == m_pwfx)
			return DXTRACE_ERR(TEXT("new"), E_FAIL);

		// Copy the bytes from the pcm structure to the waveformatex structure
		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		m_pwfx->cbSize = cbExtraBytes;

		// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
		if(mmioRead(m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize)) + sizeof(WORD)),
			cbExtraBytes) != cbExtraBytes) {
			YsSafe_Delete(m_pwfx);
			return DXTRACE_ERR(TEXT("mmioRead"), E_FAIL);
		}
	}

	// Ascend the input file out of the 'fmt ' chunk.
	if(0 != mmioAscend(m_hmmio, &ckIn, 0)) {
		YsSafe_Delete(m_pwfx);
		return DXTRACE_ERR(TEXT("mmioAscend"), E_FAIL);
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::GetSize()
// Desc: Retuns the size of the read access wave file 
//-----------------------------------------------------------------------------
DWORD YsWaveFile::GetSize() {
	return m_dwSize;
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::ResetFile()
// Desc: Resets the internal m_ck pointer so reading starts from the 
//       beginning of the file again 
//-----------------------------------------------------------------------------
HRESULT YsWaveFile::ResetFile() {
	if(m_bIsReadingFromMemory) {
		m_pbDataCur = m_pbData;
	} else {
		if(m_hmmio == nullptr)
			return CO_E_NOTINITIALIZED;

		if(m_dwFlags == WAVEFILE_READ) {
			// Seek to the data
			if(-1 == mmioSeek(m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC),
				SEEK_SET))
				return DXTRACE_ERR(TEXT("mmioSeek"), E_FAIL);

			// Search the input file for the 'data' chunk.
			m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
			if(0 != mmioDescend(m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK))
				return DXTRACE_ERR(TEXT("mmioDescend"), E_FAIL);
		} else {
			// Create the 'data' chunk that holds the waveform samples.  
			m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
			m_ck.cksize = 0;

			if(0 != mmioCreateChunk(m_hmmio, &m_ck, 0))
				return DXTRACE_ERR(TEXT("mmioCreateChunk"), E_FAIL);

			if(0 != mmioGetInfo(m_hmmio, &m_mmioinfoOut, 0))
				return DXTRACE_ERR(TEXT("mmioGetInfo"), E_FAIL);
		}
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::Read()
// Desc: Reads section of data from a wave file into pBuffer and returns 
//       how much read in pdwSizeRead, reading not more than dwSizeToRead.
//       This uses m_ck to determine where to start reading from.  So 
//       subsequent calls will be continue where the last left off unless 
//       Reset() is called.
//-----------------------------------------------------------------------------
HRESULT YsWaveFile::Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead) {
	if(m_bIsReadingFromMemory) {
		if(m_pbDataCur == nullptr)
			return CO_E_NOTINITIALIZED;
		if(pdwSizeRead != nullptr)
			*pdwSizeRead = 0;

		if((BYTE*)(m_pbDataCur + dwSizeToRead) >
			(BYTE*)(m_pbData + m_ulDataSize)) {
			dwSizeToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
		}

		CopyMemory(pBuffer, m_pbDataCur, dwSizeToRead);

		if(pdwSizeRead != nullptr)
			*pdwSizeRead = dwSizeToRead;

		return S_OK;
	} else {
		MMIOINFO mmioinfoIn; // current status of m_hmmio

		if(m_hmmio == nullptr)
			return CO_E_NOTINITIALIZED;
		if(pBuffer == nullptr || pdwSizeRead == nullptr)
			return E_INVALIDARG;

		if(pdwSizeRead != nullptr)
			*pdwSizeRead = 0;

		if(0 != mmioGetInfo(m_hmmio, &mmioinfoIn, 0))
			return DXTRACE_ERR(TEXT("mmioGetInfo"), E_FAIL);

		UINT cbDataIn = dwSizeToRead;
		if(cbDataIn > m_ck.cksize)
			cbDataIn = m_ck.cksize;

		m_ck.cksize -= cbDataIn;

		for(DWORD cT = 0; cT < cbDataIn; cT++) {
			// Copy the bytes from the io to the buffer.
			if(mmioinfoIn.pchNext == mmioinfoIn.pchEndRead) {
				if(0 != mmioAdvance(m_hmmio, &mmioinfoIn, MMIO_READ))
					return DXTRACE_ERR(TEXT("mmioAdvance"), E_FAIL);

				if(mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
					return DXTRACE_ERR(TEXT("mmioinfoIn.pchNext"), E_FAIL);
			}

			// Actual copy.
			*((BYTE*)pBuffer + cT) = *((BYTE*)mmioinfoIn.pchNext);
			mmioinfoIn.pchNext++;
		}

		if(0 != mmioSetInfo(m_hmmio, &mmioinfoIn, 0))
			return DXTRACE_ERR(TEXT("mmioSetInfo"), E_FAIL);

		if(pdwSizeRead != nullptr)
			*pdwSizeRead = cbDataIn;

		return S_OK;
	}
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::Close()
// Desc: Closes the wave file 
//-----------------------------------------------------------------------------
HRESULT YsWaveFile::Close() {
	if(m_dwFlags == WAVEFILE_READ) {
		mmioClose(m_hmmio, 0);
		m_hmmio = nullptr;
		YsSafe_Delete_Array(m_pResourceBuffer);
	} else {
		m_mmioinfoOut.dwFlags |= MMIO_DIRTY;

		if(m_hmmio == nullptr)
			return CO_E_NOTINITIALIZED;

		if(0 != mmioSetInfo(m_hmmio, &m_mmioinfoOut, 0))
			return DXTRACE_ERR(TEXT("mmioSetInfo"), E_FAIL);

		// Ascend the output file out of the 'data' chunk -- this will cause
		// the chunk size of the 'data' chunk to be written.
		if(0 != mmioAscend(m_hmmio, &m_ck, 0))
			return DXTRACE_ERR(TEXT("mmioAscend"), E_FAIL);

		// Do this here instead...
		if(0 != mmioAscend(m_hmmio, &m_ckRiff, 0))
			return DXTRACE_ERR(TEXT("mmioAscend"), E_FAIL);

		mmioSeek(m_hmmio, 0, SEEK_SET);

		if(0 != (INT)mmioDescend(m_hmmio, &m_ckRiff, nullptr, 0))
			return DXTRACE_ERR(TEXT("mmioDescend"), E_FAIL);

		m_ck.ckid = mmioFOURCC('f', 'a', 'c', 't');

		if(0 == mmioDescend(m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK)) {
			DWORD dwSamples = 0;
			mmioWrite(m_hmmio, (HPSTR)&dwSamples, sizeof(DWORD));
			mmioAscend(m_hmmio, &m_ck, 0);
		}

		// Ascend the output file out of the 'RIFF' chunk -- this will cause
		// the chunk size of the 'RIFF' chunk to be written.
		if(0 != mmioAscend(m_hmmio, &m_ckRiff, 0))
			return DXTRACE_ERR(TEXT("mmioAscend"), E_FAIL);

		mmioClose(m_hmmio, 0);
		m_hmmio = nullptr;
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::WriteMMIO()
// Desc: Support function for reading from a multimedia I/O stream
//       pwfxDest is the WAVEFORMATEX for this new wave file.  
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_ck.  
//-----------------------------------------------------------------------------
HRESULT YsWaveFile::WriteMMIO(WAVEFORMATEX *pwfxDest) {
	DWORD    dwFactChunk; // Contains the actual fact chunk. Garbage until WaveCloseWriteFile.
	MMCKINFO ckOut1;

	dwFactChunk = (DWORD)-1;

	// Create the output file RIFF chunk of form type 'WAVE'.
	m_ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	m_ckRiff.cksize = 0;

	if(0 != mmioCreateChunk(m_hmmio, &m_ckRiff, MMIO_CREATERIFF))
		return DXTRACE_ERR(TEXT("mmioCreateChunk"), E_FAIL);

	// We are now descended into the 'RIFF' chunk we just created.
	// Now create the 'fmt ' chunk. Since we know the size of this chunk,
	// specify it in the MMCKINFO structure so MMIO doesn't have to seek
	// back and set the chunk size after ascending from the chunk.
	m_ck.ckid = mmioFOURCC('f', 'm', 't', ' ');
	m_ck.cksize = sizeof(PCMWAVEFORMAT);

	if(0 != mmioCreateChunk(m_hmmio, &m_ck, 0))
		return DXTRACE_ERR(TEXT("mmioCreateChunk"), E_FAIL);

	// Write the PCMWAVEFORMAT structure to the 'fmt ' chunk if its that type. 
	if(pwfxDest->wFormatTag == WAVE_FORMAT_PCM) {
		if(mmioWrite(m_hmmio, (HPSTR)pwfxDest,
			sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
			return DXTRACE_ERR(TEXT("mmioWrite"), E_FAIL);
	} else {
		// Write the variable length size.
		if((UINT)mmioWrite(m_hmmio, (HPSTR)pwfxDest,
			sizeof(*pwfxDest) + pwfxDest->cbSize) !=
			(sizeof(*pwfxDest) + pwfxDest->cbSize))
			return DXTRACE_ERR(TEXT("mmioWrite"), E_FAIL);
	}

	// Ascend out of the 'fmt ' chunk, back into the 'RIFF' chunk.
	if(0 != mmioAscend(m_hmmio, &m_ck, 0))
		return DXTRACE_ERR(TEXT("mmioAscend"), E_FAIL);

	// Now create the fact chunk, not required for PCM but nice to have.  This is filled
	// in when the close routine is called.
	ckOut1.ckid = mmioFOURCC('f', 'a', 'c', 't');
	ckOut1.cksize = 0;

	if(0 != mmioCreateChunk(m_hmmio, &ckOut1, 0))
		return DXTRACE_ERR(TEXT("mmioCreateChunk"), E_FAIL);

	if(mmioWrite(m_hmmio, (HPSTR)&dwFactChunk, sizeof(dwFactChunk)) !=
		sizeof(dwFactChunk))
		return DXTRACE_ERR(TEXT("mmioWrite"), E_FAIL);

	// Now ascend out of the fact chunk...
	if(0 != mmioAscend(m_hmmio, &ckOut1, 0))
		return DXTRACE_ERR(TEXT("mmioAscend"), E_FAIL);

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: YsWaveFile::Write()
// Desc: Writes data to the open wave file
//-----------------------------------------------------------------------------
HRESULT YsWaveFile::Write(UINT nSizeToWrite, BYTE* pbSrcData, UINT* pnSizeWrote) {
	UINT cT;

	if(m_bIsReadingFromMemory)
		return E_NOTIMPL;
	if(m_hmmio == nullptr)
		return CO_E_NOTINITIALIZED;
	if(pnSizeWrote == nullptr || pbSrcData == nullptr)
		return E_INVALIDARG;

	*pnSizeWrote = 0;

	for(cT = 0; cT < nSizeToWrite; cT++) {
		if(m_mmioinfoOut.pchNext == m_mmioinfoOut.pchEndWrite) {
			m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
			if(0 != mmioAdvance(m_hmmio, &m_mmioinfoOut, MMIO_WRITE))
				return DXTRACE_ERR(TEXT("mmioAdvance"), E_FAIL);
		}

		*((BYTE*)m_mmioinfoOut.pchNext) = *((BYTE*)pbSrcData + cT);
		(BYTE*)m_mmioinfoOut.pchNext++;

		(*pnSizeWrote)++;
	}

	return S_OK;
}
