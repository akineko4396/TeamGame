#include "YskLib.h"

using namespace YskLib;
using namespace DirectX;

YsSprite::~YsSprite()
{
	Release();
}

void YsSprite::Init(ID3D11DeviceContext* devContext)
{
	Release();

	m_pSB = new SpriteBatch(devContext);

	m_pFont = new DirectX::SpriteFont(YsDx.GetDev(), L"data/DebugFont.spritefont");//フォント表示

	setlocale(LC_ALL, "japanese");

	m_bsAlpha.Set_Alpha();
	m_bsAlpha.Create();

	m_IsBegin = false;
}

void YsSprite::Release()
{
	YsSafe_Delete(m_pSB);
	YsSafe_Delete(m_pFont);
	m_SaveStates = nullptr;
}

void YsSprite::Begin(bool bImmediateMode, YsBlendState* blendState)
{
	// 各ステートの保存
	m_SaveStates = UPtr<States>( new States());
	m_SaveStates->bs.SetAll_GetState();
	m_SaveStates->rs.SetAll_GetState();
	m_SaveStates->ss.SetAll_GetStatePS(0);
	m_SaveStates->ds.SetAll_GetState();

	// セットするステート
	if(blendState == nullptr){
		blendState = &m_bsAlpha;
	}

	// SpriteBatch開始
	m_pSB->Begin(
		bImmediateMode ? SpriteSortMode::SpriteSortMode_Immediate : SpriteSortMode::SpriteSortMode_Deferred,
		blendState->GetState()
	);

	m_IsBegin = true;
}

void YsSprite::End()
{
	if(m_IsBegin == false)return;

	// SpriteBatch終了
	m_pSB->End();

	// 各ステートを戻す
	m_SaveStates->bs.SetState();
	m_SaveStates->rs.SetState();
	m_SaveStates->ss.SetStatePS();
	m_SaveStates->ds.SetState();

	// 記憶してたステートを解放
	m_SaveStates = nullptr;

	// セットされているテクスチャの解除
	ID3D11ShaderResourceView* srv = nullptr;
	YsDx.GetDevContext()->PSSetShaderResources(0, 1, &srv);

	m_IsBegin = false;
}

void YsSprite::Draw(ID3D11ShaderResourceView* tex, const YsMatrix& mat, const YsVec4* Color)
{
	if (tex == nullptr)return;

	bool isbegin = m_IsBegin;
	if(isbegin == false){
		Begin(true);
	}

	YsVec3 vX(1, 0, 0);
	vX.TransformNormal(mat);
	vX.Normalize();
	float ang = YsToDegree(acosf(vX.x));
	if(vX.y < 0){
		ang = 360 - ang;
	}

	XMVECTORF32 color = { Color->x, Color->y, Color->z, Color->w};

	XMFLOAT2 pos(mat.X(), mat.Y());
	XMFLOAT2 origin(0, 0);	// 回転・拡大中心点
	XMFLOAT2 scale(mat.GetXScale(), mat.GetYScale());
	m_pSB->Draw(tex, pos, nullptr, color, YsToRadian(ang), origin, scale);

	if(isbegin == false){
		End();
	}
}

void YsSprite::Draw(ID3D11ShaderResourceView* tex, const YsMatrix& mat, int sx, int sy, int sw, int sh, const YsVec4* Color)
{
	if(tex == nullptr)return;

	bool isbegin = m_IsBegin;
	if(isbegin == false){
		Begin(true);
	}

	YsVec3 vX(1, 0, 0);
	vX.TransformNormal(mat);
	vX.Normalize();
	float ang = YsToDegree(acosf(vX.x));
	if(vX.y < 0){
		ang = 360 - ang;
	}

	XMVECTORF32 color = { Color->x, Color->y, Color->z, Color->w };
	RECT rcSrc = { sx, sy, sx + sw, sy + sh };

	XMFLOAT2 pos(mat.X(), mat.Y());
	XMFLOAT2 origin(0, 0);	// 回転・拡大中心点
	XMFLOAT2 scale(mat.GetXScale(), mat.GetYScale());
	m_pSB->Draw(tex, pos, &rcSrc, color, YsToRadian(ang), origin, scale);

	if(isbegin == false){
		End();
	}
}


void YsSprite::Draw(ID3D11ShaderResourceView* tex, int x, int y, int w, int h, const YsVec4* Color)
{
	if (tex == nullptr)return;

	// Begin()を読んでいない場合は呼ぶ
	bool isbegin = m_IsBegin;
	if(isbegin == false){
		Begin(true);
	}

	XMVECTORF32 color = { Color->x, Color->y, Color->z, Color->w };

	RECT rc = {x,y,x+w,y+h};
	m_pSB->Draw(tex, rc, color);

	// 自動でBegin()を読んだ場合のみ、最後にEnd()を呼ぶ
	if(isbegin == false){
		End();
	}
}

void YsSprite::Draw(ID3D11ShaderResourceView* tex, int x, int y, int w, int h, int sx, int sy, int sw, int sh, const YsVec4* Color)
{
	if(tex == nullptr)return;

	// Begin()を読んでいない場合は呼ぶ
	bool isbegin = m_IsBegin;
	if(isbegin == false){
		Begin(true);
	}

	XMVECTORF32 color = { Color->x, Color->y, Color->z, Color->w };

	RECT rcDest = { x, y, x + w, y + h };
	RECT rcSrc = { sx, sy, sx + sw, sy + sh };
	m_pSB->Draw(tex, rcDest, &rcSrc, color);

	// 自動でBegin()を読んだ場合のみ、最後にEnd()を呼ぶ
	if(isbegin == false){
		End();
	}

}


void YsSprite::DrawFont(const std::string& text, const YsMatrix& mat, const YsVec4* Color)
{
	bool isbegin = m_IsBegin;
	if(isbegin == false){
		Begin(true);
	}

	// wchar変換
	int len = (int)text.size();
	WCHAR* wStr = new WCHAR[len+1];
	wStr[len] = 0;
	size_t wLen;
	mbstowcs_s(&wLen, wStr, len+1, text.c_str(), _TRUNCATE);

	// 
	YsVec3 vX(1, 0, 0);
	vX.TransformNormal(mat);
	vX.Normalize();
	float ang = YsToDegree(acosf(vX.x));
	if(vX.y < 0){
		ang = 360 - ang;
	}

	XMVECTORF32 color = { Color->x, Color->y, Color->z, Color->w };

	XMFLOAT2 pos(floor(mat.X()), floor(mat.Y()));
	XMFLOAT2 origin(0, 0);	// 回転・拡大中心点
	XMFLOAT2 scale(mat.GetXScale(), mat.GetYScale());

	m_pFont->DrawString(m_pSB, wStr, pos, color, YsToRadian(ang), origin, scale);

	delete[] wStr;

	if(isbegin == false){
		End();
	}
}

void YsSprite::DrawFont(const std::string& text, int x, int y, const YsVec4* Color)
{
	bool isbegin = m_IsBegin;
	if(isbegin == false){
		Begin(true);
	}

	XMVECTORF32 color = { Color->x, Color->y, Color->z, Color->w };

	// wchar変換
	int len = (int)text.size();
	WCHAR* wStr = new WCHAR[len+1];
	wStr[len] = 0;
	size_t wLen;
	mbstowcs_s(&wLen, wStr, len+1, text.c_str(), _TRUNCATE);

	XMFLOAT2 pos((float)x, (float)y);

	m_pFont->DrawString(m_pSB, wStr, pos, color);

	delete[] wStr;

	if(isbegin == false){
		End();
	}
}
