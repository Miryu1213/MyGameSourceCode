#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include <list>
#include "BaseScene.h"
#include "Obj/Type/ArmyType.h"
#include "Obj/Type/ObjType.h"
#include "SystemObj/Signal.h"
#include "UI/UI.h"
#include "UI/Place.h"
#include "../common/Directing/MoveObject.h"

class Obj;
class Smoke;

class GameScene :
    public BaseScene
{
public:
    GameScene(const char* mapname, bool isTutorial);
    ~GameScene() override;

private:
    bool Init(void) override;
    //チュートリアルから移行したさいの初期化
    void InitFromTutorial(void) override;
    //音楽再生
    void InitMusic(void) override;

    UniqueScene UpDate(UniqueScene ownScene) override;
    //UIの更新
    void UpDateUI(void);

    void Draw(void) override;
    //オフセットでずらす用描画
    void BeforeDraw(void);
    //UIの描画
    void DrawUI(void);
    //TMXの描画
    void SaveDrawnTMX(void);
    //ルートチェンジ中の描画
    void DrawRouteChange(void);

    void DrawBreakedCore(void);

    Scene GetSceneID(void) override
    {
        return Scene::GAME;
    };

    //objlistへユニットを生成
    void GenerateUnit(std::list<UniqueObj>::iterator& objitr);
    //ルートと施設とコアの初期配置
    void SetPlantAndCore(void);
    //Plantをobjlistへ登録
    void PlantEntryObjList(Vector2 pos, std::vector<Vector2> route, ArmyType armytype, ObjType objtype);
    //プラントの配置
    void DeployPlant(void);
    //プラント設置時の土煙作成
    void CreateSmokeWhenDeployingPlant(Vector2 plantPos);

    //UI処理
    void CreateUI(void);
    //経過時間の倍速値ををカウントアップ
    bool CntUpDoubleElapTime(void);
    //経過時間の倍速値ををカウントダウン
    bool CntDownDoubleElapTime(void);
    //ルートチェンジ
    bool RouteChange(void);

    const std::string& GetMusicName(void) override
    {
        return musicName_;
    }

    //オブジェクト全般管理
    std::list<UniqueObj> objList_; 
    //プラント保存用プール
    std::list<UniqueObj> plantAllyPool_;
    std::list<UniqueObj> plantEnemyPool_;

    //画面UI管理リスト
    std::list<std::unique_ptr<UI>> btnList_;
    //シグナル管理リスト
    std::list<std::unique_ptr<Signal>> signalList_;
    //演出管理
    MoveObjectVoidVoid* deployPlantFunc_;

    //配置時の土煙
    std::list<Smoke*> smokeVArray_;

    //オフセット用グラフィックハンドル
    int offsetGh_;
    //マップ用ハンドル
    int mapGh_;

    //縦横の最大量
    Vector2 limitNumBlock_;

    std::string musicName_;
    std::string mapName_;

    //チュートリアルか否か    
    bool isTutorial_;

    bool isBackGroundLoading_;

    bool effectFlag_;
    bool oneFlag_;
    bool waitEndFlag_;
    bool convergenceEndFlag_;
    double expAnimTime_;
    Vector2 corePos_;
};
