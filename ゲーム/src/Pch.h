// Debug2ビルドで実行速度を上げたい場合は、このコメントをはずす
// ※STLのエラーチェックなどは甘くなってしまいます
//#undef _DEBUG

// Direct3D11のデバッグをONにする
//#define DEBUG_DIRECT3D11

#ifdef _DEBUG
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#else
	#ifndef NDEBUG
		#define NDEBUG
	#endif
#endif

// デバッグウィンドウを表示する(コメントにすると非表示)
#define DEBUG_WND

// YskLib
#include "YskLib.h"
