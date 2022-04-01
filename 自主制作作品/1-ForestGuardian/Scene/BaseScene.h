#pragma once
#include<memory>
#include<list>
#include"SceneID.h"
#include"Camera.h"
//#include"obj/obj.h"

class obj;
class BaseScene;
//ユニークポインタ-消滅時それが管理しているものも自動で消滅
using uniqueBaseScene = std::unique_ptr<BaseScene>;		//ユニークポインタとして管理
using ObjPtr = std::shared_ptr<obj>;
using ObjList = std::list<ObjPtr>;		//objクラスをシェアードポインタとして管理、それをリスト化

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual uniqueBaseScene UpDate(uniqueBaseScene own) = 0;
	virtual SceneID GetSceneID() = 0;		//現在のシーンIDの取得
	void Draw();
	void ExtendDraw(int firstX, int firstY, int lastX, int lastY, bool inv);	//サイズを指定しての描画
	virtual void DrawOwnScreen(void) = 0;

	ObjList::iterator AddobjList(ObjPtr&& objptr);		//objlistにオブジェクトを追加
	ObjList objList_;		//オブジェクトの一覧
private:

protected:
	virtual bool Init(void) = 0;

	int screenScrID_;		//各シーンでの描画用ハンドル

	std::shared_ptr<Camera> camera_;

	bool UpDateObj(void);		//オブジェクト系の更新

	bool DrawObj(void);
};



