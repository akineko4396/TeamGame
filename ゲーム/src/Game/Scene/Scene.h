#ifndef __Scene_h__
#define __Scene_h__

//============================================================
// シーン基本クラス
//  こいつを継承して新しいシーンを作る
//============================================================
class BaseScene{
public:
	BaseScene(){
	}
	virtual ~BaseScene(){};

	virtual int Update() = 0;	// シーン更新処理
	virtual void Draw() = 0;	// シーン描画処理
};

#endif
