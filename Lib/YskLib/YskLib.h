//===============================================================
//!
//! @file YskLib.h
//! @brief YskLib�̃w�b�_
//!
//! YskLib�̊�{�w�b�_�t�@�C���B�����include�����OK�B
//!
//! @author ���c
//!
//===============================================================

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"dxgi.lib")
//#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"dxguid.lib")

#pragma comment(lib,"dsound.lib")

//=====================================================================
// DirectX Tool Kit
//=====================================================================
#ifdef _DEBUG
	// Visual Studio 2013
	#if _MSC_VER == 1800
		#pragma comment(lib,"DirectXTK/Lib/Debug/DirectXTK.lib")
	// Visual Studio 2015
	#else if _MSC_VER == 1900
		#pragma comment(lib,"DirectXTK/Lib_2015/Debug/DirectXTK.lib")
	#endif
#else
	// Visual Studio 2013
	#if _MSC_VER == 1800
		#pragma comment(lib,"DirectXTK/Lib/Release/DirectXTK.lib")
	// Visual Studio 2015
	#else if _MSC_VER == 1900
		#pragma comment(lib,"DirectXTK/Lib_2015/Release/DirectXTK.lib")
	#endif
#endif
//=====================================================================
// Bullet Physics Engine
//=====================================================================
#ifdef _DEBUG
	// Visual Studio 2013
	#if _MSC_VER == 1800
		#pragma comment(lib,"BulletPhysicsEngine/lib/Debug/BulletCollision_debug.lib")
		#pragma comment(lib,"BulletPhysicsEngine/lib/Debug/BulletDynamics_debug.lib")
		#pragma comment(lib,"BulletPhysicsEngine/lib/Debug/LinearMath_debug.lib")
	// Visual Studio 2015
	#else if _MSC_VER == 1900
		#pragma comment(lib,"BulletPhysicsEngine/lib_2015/Debug/BulletCollision_debug.lib")
		#pragma comment(lib,"BulletPhysicsEngine/lib_2015/Debug/BulletDynamics_debug.lib")
		#pragma comment(lib,"BulletPhysicsEngine/lib_2015/Debug/LinearMath_debug.lib")
	#endif
#else
	// Visual Studio 2013
	#if _MSC_VER == 1800
		#pragma comment(lib,"BulletPhysicsEngine/lib/Release/BulletCollision.lib")
		#pragma comment(lib,"BulletPhysicsEngine/lib/Release/BulletDynamics.lib")
		#pragma comment(lib,"BulletPhysicsEngine/lib/Release/LinearMath.lib")
	// Visual Studio 2015
	#else if _MSC_VER == 1900
		#pragma comment(lib,"BulletPhysicsEngine/lib_2015/Release/BulletCollision.lib")
		#pragma comment(lib,"BulletPhysicsEngine/lib_2015/Release/BulletDynamics.lib")
		#pragma comment(lib,"BulletPhysicsEngine/lib_2015/Release/LinearMath.lib")
	#endif
#endif

//=====================================================================
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"imm32.lib")

#ifndef __YskLib_h__
#define __YskLib_h__

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <mmsystem.h>
#include <math.h>
#include <commctrl.h>
#include <ctime>

// DirectXTK
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

// DirectX11
#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11.h>
#include <SimpleMath.h>

// DirectX9
#define DIRECTINPUT_VERSION  0x0900
#include <d3dx9.h>
//#include "dxerr.h"
#include "dinput.h"
#include "dxgitype.h"
#include "dsound.h"

// STL
#include <map>
#include <unordered_map>
#include <array>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <shlobj.h>
#include <new>
#include <future>


// Bullet
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"


// doxygen�p�O���[�v�ݒ�
//! @defgroup Graphics_Important �O���t�B�b�N�֌W[*��v�@�\*](��{)
//! @defgroup Graphics �O���t�B�b�N�֌W(��{)
//! @defgroup Graphics_Model_Important �O���t�B�b�N�֌W[*��v�@�\*](���f��)
//! @defgroup Graphics_Model �O���t�B�b�N�֌W(���f��)
//! @defgroup Graphics_Shader �O���t�B�b�N�֌W(�V�F�[�_)
//! @defgroup Animation �A�j���[�V�����֌W
//! @defgroup Collision �����蔻��֌W
//! @defgroup Math �Z�p�֌W
//! @defgroup Physics �����G���W���֌W
//! @defgroup Sound �T�E���h�֌W
//! @defgroup Etc ���̑�

namespace YskLib{

	//===============================================
	//! @brief ���\�[�X���N���X
	//!
	//! ���\�[�X�n�̃N���X�̊�{�ƂȂ�N���X
	//!
	//! @ingroup Etc
	//===============================================
	class YsIResource {
	public:
		virtual ~YsIResource(){}

		//! @brief ���\�[�X�^�C�v���擾
		//! @return ���\�[�X�̌^�����Ԃ�
		virtual std::string GetTypeName() const = 0;
	};

}

//==================================================
//
// YskLib
//
//==================================================


//==================================================
//! @brief YsDirectXGraphics�̃C���X�^���X�擾 �Z�k�L�q��
//! @ingroup Graphics
//==================================================
#define YsDx YsDirectXGraphics::GetInstance()

//==================================================
//! @brief SoundManager�̃C���X�^���X�擾 �Z�k�L�q��
//! @ingroup Sound
//==================================================
#define YsSndMgr YsSoundManager::GetInstance()

#include "YsDebugWnd.h"

#include "YsHelper.h"

#include "YsLocking.h"
#include "YsFpsControl.h"
#include "YsCsv.h"
#include "YsMTRand.h"


#include "YsVec2.h"
#include "YsVec3.h"
#include "YsVec4.h"
#include "YsQuat.h"
#include "YsMatrix.h"
#include "YsCamera.h"

#include "YsUVAnimation.h"
#include "YsKeyframeAnimation.h"

#include "YsDataStorage.h"
#include "YsResourceStorage.h"

#include "YsVertexType.h"

#include "YsCollision.h"

#include "YsTexture.h"
#include "YsStates.h"

#include "YsPolygon.h"
#include "YsBillBoard.h"
#include "YsLaser.h"
#include "YsMesh.h"
#include "YsSingleModel.h"
#include "YsGameModel.h"
#include "YsModelObject.h"

#include "YsConstantBuffer.h"
#include "YsVertexShader.h"
#include "YsPixelShader.h"
#include "YsGeometryShader.h"

#include "YsSprite.h"
#include "YsDirectXGraphics.h"

#include "YsBulletPhysicsEngine.h"

#include "YsSound.h"

/*
// �f�o�b�O�R���p�C�����̓��������[�N���o
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif
*/


#endif
