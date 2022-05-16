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
    //�`���[�g���A������ڍs���������̏�����
    void InitFromTutorial(void) override;
    //���y�Đ�
    void InitMusic(void) override;

    UniqueScene UpDate(UniqueScene ownScene) override;
    //UI�̍X�V
    void UpDateUI(void);

    void Draw(void) override;
    //�I�t�Z�b�g�ł��炷�p�`��
    void BeforeDraw(void);
    //UI�̕`��
    void DrawUI(void);
    //TMX�̕`��
    void SaveDrawnTMX(void);
    //���[�g�`�F���W���̕`��
    void DrawRouteChange(void);

    void DrawBreakedCore(void);

    Scene GetSceneID(void) override
    {
        return Scene::GAME;
    };

    //objlist�փ��j�b�g�𐶐�
    void GenerateUnit(std::list<UniqueObj>::iterator& objitr);
    //���[�g�Ǝ{�݂ƃR�A�̏����z�u
    void SetPlantAndCore(void);
    //Plant��objlist�֓o�^
    void PlantEntryObjList(Vector2 pos, std::vector<Vector2> route, ArmyType armytype, ObjType objtype);
    //�v�����g�̔z�u
    void DeployPlant(void);
    //�v�����g�ݒu���̓y���쐬
    void CreateSmokeWhenDeployingPlant(Vector2 plantPos);

    //UI����
    void CreateUI(void);
    //�o�ߎ��Ԃ̔{���l�����J�E���g�A�b�v
    bool CntUpDoubleElapTime(void);
    //�o�ߎ��Ԃ̔{���l�����J�E���g�_�E��
    bool CntDownDoubleElapTime(void);
    //���[�g�`�F���W
    bool RouteChange(void);

    const std::string& GetMusicName(void) override
    {
        return musicName_;
    }

    //�V�O�i�����X�g�̎擾
    const std::list<std::unique_ptr<Signal>>& GetSigNalList(void) override
    {
        return signalList_;
    }

    //�I�u�W�F�N�g�S�ʊǗ�
    std::list<UniqueObj> objList_; 
    //�v�����g�ۑ��p�v�[��
    std::list<UniqueObj> plantAllyPool_;
    std::list<UniqueObj> plantEnemyPool_;

    //���UI�Ǘ����X�g
    std::list<std::unique_ptr<UI>> btnList_;
    //�V�O�i���Ǘ����X�g
    std::list<std::unique_ptr<Signal>> signalList_;
    //���o�Ǘ�
    MoveObjectVoidVoid* deployPlantFunc_;

    //�z�u���̓y��
    std::list<Smoke*> smokeVArray_;

    //�I�t�Z�b�g�p�O���t�B�b�N�n���h��
    int offsetGh_;
    //�}�b�v�p�n���h��
    int mapGh_;

    //�c���̍ő��
    Vector2 limitNumBlock_;

    std::string musicName_;
    std::string mapName_;

    //�`���[�g���A�����ۂ�    
    bool isTutorial_;

    //���[�h��
    bool isBackGroundLoading_;

    bool effectFlag_;           //�G�t�F�N�g�\���t���O
    bool oneEffectFlag_;        //��x�݂̂̃G�t�F�N�g
    bool waitEndFlag_;          //�ҋ@���ԏI��
    bool convergenceEndFlag_;   //�����I���t���O   
    double expAnimTime_;        //�����p�^�C��
    Vector2 corePos_;           //�R�A���W�ۑ�
};
