// Debug2�r���h�Ŏ��s���x���グ�����ꍇ�́A���̃R�����g���͂���
// ��STL�̃G���[�`�F�b�N�Ȃǂ͊Â��Ȃ��Ă��܂��܂�
//#undef _DEBUG

// Direct3D11�̃f�o�b�O��ON�ɂ���
//#define DEBUG_DIRECT3D11

#ifdef _DEBUG
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#else
	#ifndef NDEBUG
		#define NDEBUG
	#endif
#endif

// �f�o�b�O�E�B���h�E��\������(�R�����g�ɂ���Ɣ�\��)
#define DEBUG_WND

// YskLib
#include "YskLib.h"
