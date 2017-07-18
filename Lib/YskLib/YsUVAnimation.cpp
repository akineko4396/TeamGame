#include "YskLib.h"

using namespace YskLib;

// アニメーション進行
void YsUVAnimator::Animation(float fSpeed, bool bLoop)
{
	// アニメ進行
	m_fAnimePos += fSpeed;
	if (m_fAnimePos >= m_AnimeMaxNum) {
		if (bLoop) {
			m_fAnimePos = m_fAnimePos - m_AnimeMaxNum*((int)(m_fAnimePos / m_AnimeMaxNum));
		}
		else {
			m_fAnimePos = (float)(m_AnimeMaxNum - 1);
		}
	}
}
