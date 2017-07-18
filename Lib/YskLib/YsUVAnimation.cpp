#include "YskLib.h"

using namespace YskLib;

// �A�j���[�V�����i�s
void YsUVAnimator::Animation(float fSpeed, bool bLoop)
{
	// �A�j���i�s
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
