using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MyTurnOption : MonoBehaviour
{
    public enum Function
    {
        //マイターンで行える機能一覧
        Reconnaissance,        //偵察
        Aggression,            //侵攻
        Employment,            //雇用
        Reward,                //恩賞
        Training,              //訓練
        Purchase,              //購入
        Equipment,             //装備
        Expulsion,             //追放
        Option,                //機能
        End,                   //終了
        Max
    }

    public enum OptionMenu
    {
        //オプションメニュー
        Save,                   //セーブ
        Manual,                 //マニュアル
        ToTitle                 //タイトルへ
    }

    //各機能ごとの減少資金数
    const int AggPoint = 3;    //侵攻
    const int EmpPoint = 9;    //雇用
    const int RewardPoint = 2; //恩賞
    const int TraPoint = 2;    //訓練
    //const int PurPoint = 0;    //購入
    //const int EquPoint = 0;    //装備

    const int MaxMasterID = 13;     //マスターID最大数
    const int MaxGroupNumber = 5;   //グループ自体の最大数
    string MyID;                    //自分自身のID

    public bool AggressionFlag;     //true侵攻 false偵察    MapSceneへ値を引き継ぐ
    private GameObject controllar;  //下部コントローラー
    private GameObject objpos;      //セレクター
    private GameObject play_char;   //キャラ画像まとめ
    private GameObject code;        //軍団情報系

    Vector2Int InitVecter2 = new Vector2Int(100, 100);              //配置用初期ベクター2
    private Vector2Int tmpselectpoint = new Vector2Int(100, 100);   //配置用現在選択している場所

    private bool rewardFlag = false;    //恩賞フラグ

    private GameObject bg_pos;      //初期画面
    private GameObject employment;  //雇用画面
    private GameObject recruitment; //採用成功画面
    private GameObject purchase;    //購入用画面
    private GameObject equipment;   //装備用画面
    private GameObject Option;      //機能用画面
    private GameObject blocks;      //領地画面
    private GameObject Invaded;     //進行された時の画面
    private GameObject recon;       //追放、恩賞画面
    private GameObject enevsene;    //敵VS敵の画面
    private GameObject manual;      //マニュアル画面
    //private GameObject bgimage;     //背景
    private GameObject nowScreen;   //現在の画面

    private GameObject maruObj;     //MyTurnOptionインスタンス場所
    private MyTurnOption maruop;    //〇内スクリプト

    private GameObject textbar;     //上部テキストバー
    private Text textbarText;       //テキストバー内テキスト

    private GameObject Groupmng;    //グループ管理用
    private GroupMng groupmng;      //グループマネージャー内スクリプト
    private MapCtl mapctl;          //グループマネージャー内マップ系スクリプト
    private AdaptImgMng adaptmng;   //グループマネージャー内画像貼り付け系スクリプト
    private GameMng gamemng;        //グループマネージャー内ゲームマネージャー

    private GameObject turnmng;     //ターン管理用
    private EnemyMove enemymove;    //ターンマネージャー内敵動き系スクリプト

    private GameObject fademng;     //フェードマネージャ

    //Vector2Int defAmount = new Vector2Int(0, 48);        //初期画面の移動量
    private Vector3 DefseleDefPos;                       //初期画面用セレクター初期位置

    Vector2Int EmpAmount = new Vector2Int( 0,120 );      //雇用画面用移動量
    Vector2Int EmpMaxCnt = new Vector2Int(0, 4);         //雇用画面用移動可能数
    private GameObject Empselecter;                      //雇用画面用選択
    private Vector3 EmpseleDefPos;                       //雇用用セレクター初期位置

    Vector2Int InvAmount = new Vector2Int(0, 100);       //防衛画面用移動量
    Vector2Int InvMaxCnt = new Vector2Int(0, 5);         //防衛画面用移動可能数
    private GameObject Invselecter;                      //防衛画面用選択
    private Vector3 InvseleDefPos;                       //防衛用セレクター初期位置

    Vector2Int RecAmount = new Vector2Int(123, 84);      //配置画面用移動量
    Vector2Int RecMaxCnt = new Vector2Int(5, 5);         //配置画面用移動可能数
    private GameObject Recselecter;                      //配置画面用選択
    private Vector3 RecseleDefPos;                       //配置用セレクター初期位置

    Vector2Int OptionAmount = new Vector2Int(0, 32);    //オプション画面用移動量
    Vector2Int OptionMaxCnt = new Vector2Int(0, 3);     //オプション画面用移動可能数
    private GameObject Opselecter;                      //オプション画面用選択
    private Vector3 OpseleDefPos;                       //オプション用セレクター初期位置

    private AudioSource MaruSE;                         //選択SE
    private AudioSource BatuSE;                         //キャンセルSE

    private void Start()
    {
        //SE取得
        MaruSE = GameObject.Find("ClickMaru").GetComponent<AudioSource>();
        BatuSE = GameObject.Find("ClickBatu").GetComponent<AudioSource>();

        //各オブジェクトの取得
        controllar = GameObject.Find("Controllar");
        AggressionFlag = false;

        objpos = GameObject.Find("Obj_Pos");
        fademng = GameObject.Find("FadeManager");

        //〇系
        maruObj = GameObject.Find("btn_maru_inside");
        maruop = maruObj.GetComponent<MyTurnOption>();

        //グループマネージャー系
        Groupmng = GameObject.Find("GroupMng");
        groupmng = Groupmng.GetComponent<GroupMng>();
        mapctl = Groupmng.GetComponent<MapCtl>();
        adaptmng = Groupmng.GetComponent<AdaptImgMng>();
        gamemng = Groupmng.GetComponent<GameMng>();

        //ターンマネージャー系
        turnmng = GameObject.Find("TurnMng");
        enemymove = turnmng.GetComponent<EnemyMove>();

        //テキスト系
        textbar = GameObject.Find("TextBar");
        textbarText = textbar.GetComponent<Text>();

        play_char = GameObject.Find("Play_Char");
        blocks = GameObject.Find("Blocks");
        Invaded = GameObject.Find("Invaded");

        //MYIDの取得
        MyID = mapctl.GetMyId();

        //各画面用セレクター
        DefseleDefPos = new Vector3(-315, 518, 0);

        Empselecter = GameObject.Find("EmpSelecter");
        EmpseleDefPos = Empselecter.GetComponent<RectTransform>().localPosition;

        Invselecter = GameObject.Find("InvSelecter");
        InvseleDefPos = Invselecter.GetComponent<RectTransform>().localPosition;

        Opselecter = GameObject.Find("OpSelecter");
        OpseleDefPos = Opselecter.GetComponent<RectTransform>().localPosition;

        Recselecter = GameObject.Find("RecSelecter");
        RecseleDefPos = Recselecter.GetComponent<RectTransform>().localPosition;

        bg_pos = GameObject.Find("BG_pos");
        bg_pos.GetComponent<Canvas>().enabled = false;
        objpos.GetComponent<Canvas>().enabled = false;

        code = bg_pos.transform.Find("Code").gameObject;

        nowScreen = bg_pos;

        //初期画面以外の画面を見えないように
        employment = GameObject.Find("employment");
        employment.GetComponent<Canvas>().enabled = false;
        purchase = GameObject.Find("Buy");
        purchase.GetComponent<Canvas>().enabled = false;
        equipment = GameObject.Find("Equipment");
        equipment.GetComponent<Canvas>().enabled = false;
        Option = GameObject.Find("Option");
        Option.GetComponent<Canvas>().enabled = false;
        recruitment = GameObject.Find("Recruitment");
        recruitment.GetComponent<Canvas>().enabled = false;
        recon = GameObject.Find("Recon");
        recon.GetComponent<Canvas>().enabled = false;
        enevsene = GameObject.Find("EneVsEne");
        enevsene.GetComponent<Canvas>().enabled = false;
        manual = GameObject.Find("Manual");
        manual.GetComponent<Canvas>().enabled = false;
        //bgimage = GameObject.Find("BGImage");
        //bgimage.GetComponent<Canvas>().enabled = false;

        blocks = GameObject.Find("Blocks");
        //blocks.GetComponent<Canvas>().enabled = false;
        Invaded = GameObject.Find("Invaded");
        if (Invaded.GetComponent<Canvas>().enabled)
        {
            //例外処理
            objpos.GetComponent<Canvas>().enabled = false;
        }

        //初期化
        adaptmng.Init();
        enemymove.Init();

        var group = groupmng.GetGroupId();
        //画面の対応付け（キャラ画像）
        adaptmng.AdaptImage(group[MyID], play_char);

        //画面対応（総戦力などの数値系）
        adaptmng.AdaptStateNum(code, MyID);

        //画面の対応付け（旗画像）
        mapctl.AdaptMap();

        //資金系対応付け
        adaptmng.AdaptMoney();

        //防衛後ゲームシーンから戻ってきた場合
        if (maruObj.GetComponent<MyTurnOption>().enemymove.GetIsGameScene())
        {
            if (!gamemng.pWinFlag)
            {
                //相手が勝利した場合
                var settmpid = enemymove.tmpMapId;
                mapctl.SetMapID(settmpid.Value, settmpid.Key, enemymove.GetTmpMasterId());
                //自領地がなくなったとき
                if (mapctl.GetMapIdNum(MyID) == 0)
                {
                    GoGameOverScene();
                    return;
                }
            }
            //減ったHPを対応
            var idlist = gamemng.CharIDList;
            //味方
            for (int i = 0; i < 5; i++)
            {
                gamemng.SetCharaData(idlist[i], stateNum.hp, gamemng.PcharList[i, (int)stateNum.hp]);
                gamemng.SetCharaData(idlist[i], stateNum.exp, gamemng.PcharList[i, (int)stateNum.exp] + 30);
                gamemng.SetCharaData(idlist[i], stateNum.com, gamemng.PcharList[i, (int)stateNum.com] + 5);
            }
            //敵
            for (int i = 5; i < 10; i++)
            {
                gamemng.SetCharaData(idlist[i], stateNum.hp, gamemng.EcharList[i - 5, (int)stateNum.hp]);
                gamemng.SetCharaData(idlist[i], stateNum.exp, gamemng.EcharList[i - 5, (int)stateNum.exp] + 20);
                //gamemng.SetCharaData(idlist[i], stateNum.com, gamemng.EcharList[i, (int)stateNum.com] + 5);
            }

            //HPがマイナスだったときの処理
            groupmng.HealHpInAllGroupByMinus();

            //経験値Up
            gamemng.UpCharaStateOnExp();

            //不満度Up
            gamemng.DownCharaStateOnCom();

            //画面対応（総戦力などの数値系）
            adaptmng.AdaptStateNum(code, MyID);

            enemymove.SetIsGameScene(false);
            Finish(true);
        }
        else
        {
            bg_pos.GetComponent<Canvas>().enabled = true;
            objpos.GetComponent<Canvas>().enabled = true;
        }
        //侵攻後ゲームシーンから戻ってきた場合
        if (gamemng.MyAttackF)
        {
            if (gamemng.pWinFlag)
            {
                //自分が勝利した場合
                var settmpid = mapctl.tmpMapId;
                mapctl.SetMapID(settmpid.Key, settmpid.Value, MyID);
                //クリアしているかの確認
                if (mapctl.CheckClear())
                {
                    FadeManager.Instance.LoadLevel("ClearScene", 0.5f);
                    return;
                }
            }
            //減ったHPを対応
            var idlist = gamemng.CharIDList;
            //味方
            for (int i = 0; i < 5; i++)
            {
                gamemng.SetCharaData(idlist[i], stateNum.hp, gamemng.PcharList[i, (int)stateNum.hp]);
                gamemng.SetCharaData(idlist[i], stateNum.exp, gamemng.PcharList[i, (int)stateNum.exp] + 30);
                gamemng.SetCharaData(idlist[i], stateNum.com, gamemng.PcharList[i, (int)stateNum.com] + 5);
            }
            //敵
            for (int i = 5; i < 10; i++)
            {
                gamemng.SetCharaData(idlist[i], stateNum.hp, gamemng.EcharList[i - 5, (int)stateNum.hp]);
                gamemng.SetCharaData(idlist[i], stateNum.exp, gamemng.EcharList[i - 5, (int)stateNum.exp] + 20);
                //gamemng.SetCharaData(idlist[i], stateNum.com, gamemng.EcharList[i, (int)stateNum.com] + 5);
            }

            //HPがマイナスだったときの処理
            groupmng.HealHpInAllGroupByMinus();

            //経験値Up
            gamemng.UpCharaStateOnExp();

            //不満度Up
            gamemng.DownCharaStateOnCom();

            //画面対応（総戦力などの数値系）
            adaptmng.AdaptStateNum(code, MyID);
        }
    }
    public void ClickOption()
    {
        if (FadeManager.Instance.isFading)
        {
            return;
        }

        //SEの再生
        MaruSE.PlayOneShot(MaruSE.clip);

        if (objpos.GetComponent<Canvas>().enabled && nowScreen == bg_pos)
        {
            var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().y;
            if (nowpoint == (int)Function.Reconnaissance)
            {
                //偵察
                Debug.Log("偵察");
                AggressionFlag = false;
                //SceneManager.LoadScene("MapScene");
                FadeManager.Instance.LoadLevel("MapScene", 0.2f);
                return;
            }
            if (nowpoint == (int)Function.Aggression)
            {
                //侵攻
                Debug.Log("侵攻");

                //資金が足りないとき
                if (AggPoint >groupmng.GetOneMoney(MyID))
                {
                    textbarText.text = "資金が不足しています";
                    return;
                }

                //資金の減少
                groupmng.CntDownMoney(MyID, AggPoint);

                AggressionFlag = true;

                //資金の画面適応
                adaptmng.AdaptMoney();

                enemymove.SetIsGameScene(false);

                //SceneManager.LoadScene("MapScene");
                FadeManager.Instance.LoadLevel("MapScene", 0.2f);
                return;
            }
            if (nowpoint == (int)Function.Employment)
            {
                //雇用
                Debug.Log("雇用");

                //資金が足りないとき
                if (EmpPoint > groupmng.GetOneMoney(MyID))
                {
                    textbarText.text = "資金が不足しています";
                    return;
                }

                //資金の減少
                groupmng.CntDownMoney(MyID, EmpPoint);

                bg_pos.GetComponent<Canvas>().enabled = false;
                employment.GetComponent<Canvas>().enabled = true;
                nowScreen = employment;
                //Obj_Posをfalseに
                objpos.GetComponent<Canvas>().enabled = false;

                //セレクターのチェンジ
                ChangeSelecter("雇用する兵の選択", EmpAmount, EmpMaxCnt, Empselecter);

                //キャラの表示
                adaptmng.AdaptEmp();

                //資金の画面適応
                adaptmng.AdaptMoney();

                //キャラステータスの適応

                //現在選択位置の名前を取得
                string tmpstr = "Team (0)";
                string SetName = employment.transform.Find(tmpstr).gameObject.GetComponent<Image>().sprite.name;
                SetName = SetName.Replace("fase-", "");
                Debug.Log(SetName);
                adaptmng.AdaptCharaState(employment.transform.Find("strdown").gameObject, SetName);

                for (int i = 0; i < 4; i++)
                {
                    string tmpname = "Team (" + i.ToString() + ")";
                    if (employment.transform.Find(tmpname).gameObject.GetComponent<Image>().color != Color.clear)
                    {
                        try
                        {
                            string changeName = employment.transform.Find(tmpname).gameObject.GetComponent<Image>().sprite.name;
                            changeName = changeName.Replace("fase-", "");
                            adaptmng.AdaptCharaState(employment.transform.Find("statedate" + i.ToString()).gameObject, changeName);
                        }
                        catch
                        {

                        }
                    }
                }

                return;
            }
            if (nowpoint == (int)Function.Reward)
            {
                //恩賞
                Debug.Log("恩賞");

                //資金が足りないとき
                if (RewardPoint > groupmng.GetOneMoney(MyID))
                {
                    textbarText.text = "資金が不足しています";
                    return;
                }

                bg_pos.GetComponent<Canvas>().enabled = false;
                recon.GetComponent<Canvas>().enabled = true;
                nowScreen = recon;

                //Obj_Posをfalseに
                objpos.GetComponent<Canvas>().enabled = false;

                //セレクターの対象を変更
                ChangeSelecter("恩賞を与える兵の選択", RecAmount, RecMaxCnt, Recselecter);

                //画面対応
                adaptmng.AdaptFromSelecterGroup(recon.transform.Find("Master_face").gameObject);

                //資金の画面適応
                adaptmng.AdaptMoney();

                //ステータス
                var nowPoint = controllar.GetComponent<SelectedMng>().Getnowpoint();
                var group = groupmng.GetGroupId()[mapctl.GetMyId()];
                var Charaid = group[nowPoint.x][nowPoint.y];
                adaptmng.AdaptCharaState(recon.transform.Find("strdown").gameObject, Charaid);

                //画面対応(名前)
                for (int i = 0; i < 5; i++)
                {
                    adaptmng.AdaptName(recon.transform.Find("Group" + i).gameObject, group[i]);
                }

                rewardFlag = true;

                return;
            }
            if (nowpoint == (int)Function.Training)
            {
                //訓練
                Debug.Log("訓練");

                //資金が足りないとき
                if (TraPoint > groupmng.GetOneMoney(MyID))
                {
                    textbarText.text = "資金が不足しています";
                    return;
                }

                //資金の減少
                groupmng.CntDownMoney(MyID, TraPoint);

                var group = groupmng.GetGroupId()[MyID];
                for (int i = 0; i < MaxGroupNumber; i++)
                {
                    for (int j = 0; j < MaxGroupNumber; j++)
                    {
                        var charaid = group[i][j];
                        var charaexp =Groupmng.GetComponent<GameMng>().GetCharaData(charaid, stateNum.exp);
                        System.Random rand = new System.Random();
                        var point = rand.Next(7);
                        Groupmng.GetComponent<GameMng>().SetCharaData(charaid, stateNum.exp, charaexp + 4 + point);
                    }
                }

                //トレーニングの画面適応
                ShowTraningNum();

                //能力アップ
                gamemng.UpCharaStateOnExp();

                //画面対応（総戦力などの数値系）
                adaptmng.AdaptStateNum(code, MyID);

                //資金の画面適応
                adaptmng.AdaptMoney();

                return;
            }
            if (nowpoint == (int)Function.Purchase)
            {
                //購入
                Debug.Log("購入");
                bg_pos.GetComponent<Canvas>().enabled = false;
                purchase.GetComponent<Canvas>().enabled = true;
                nowScreen = purchase;
                Debug.Log(nowScreen.name);

                //Obj_Posをfalseに
                objpos.GetComponent<Canvas>().enabled = false;

                return;
            }
            if (nowpoint == (int)Function.Equipment)
            {
                //装備
                Debug.Log("装備");
                bg_pos.GetComponent<Canvas>().enabled = false;
                equipment.GetComponent<Canvas>().enabled = true;
                nowScreen = equipment;

                //Obj_Posをfalseに
                objpos.GetComponent<Canvas>().enabled = false;

                return;
            }
            if (nowpoint == (int)Function.Expulsion)
            {
                //追放
                Debug.Log("配置");
                bg_pos.GetComponent<Canvas>().enabled = false;
                recon.GetComponent<Canvas>().enabled = true;
                nowScreen = recon;

                //Obj_Posをfalseに
                objpos.GetComponent<Canvas>().enabled = false;

                //セレクターの対象を変更
                ChangeSelecter("配置を変更する兵の選択", RecAmount, RecMaxCnt, Recselecter);

                //画面対応
                adaptmng.AdaptFromSelecterGroup(recon.transform.Find("Master_face").gameObject);

                //ステータス
                var nowPoint = controllar.GetComponent<SelectedMng>().Getnowpoint();
                var group = groupmng.GetGroupId()[mapctl.GetMyId()];
                var Charaid = group[nowPoint.x][nowPoint.y];
                adaptmng.AdaptCharaState(recon.transform.Find("strdown").gameObject, Charaid);

                //画面対応(名前)
                for (int i = 0; i < 5; i++)
                {
                    adaptmng.AdaptName(recon.transform.Find("Group" + i).gameObject, group[i]);
                }

                return;
            }
            if (nowpoint == (int)Function.Option)
            {
                //機能
                Debug.Log("機能");
                Option.GetComponent<Canvas>().enabled = true;
                nowScreen = Option;

                //セレクターの対象を変更
                ChangeSelecter("現在の勢力情報を保存", OptionAmount, OptionMaxCnt, Opselecter, true);

                return;
            }
            if (nowpoint == (int)Function.End)
            {
                //終了
                //画面対応
                bg_pos.GetComponent<Canvas>().enabled = false;
                objpos.GetComponent<Canvas>().enabled = false;
                enevsene.GetComponent<Canvas>().enabled = true;
                enevsene.transform.Find("vs").gameObject.SetActive(false);

                mapctl.AdaptMap(enevsene);

                //HPの回復
                groupmng.HealHpInAllGroup();

                textbarText.text = "敵思考中…";

                StartCoroutine(adaptmng.AdaptRamdam(enevsene));
                adaptmng.AdaptMoney();
                return;
            }
            else
            {
                Debug.Log("Nowpointの値が不正です");
                return;
            }
        }
        else
        {
            //雇用画面
            if(employment.GetComponent<Canvas>().enabled)
            {
                Debug.Log("雇用…");
                //現在の場所
                var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().y;
                //現在選択位置の名前を取得
                string tmpstr = "Team (" + nowpoint.ToString() + ")";
                string SetName = employment.transform.Find(tmpstr).gameObject.GetComponent<Image>().sprite.name;
                SetName = SetName.Replace("fase-", "");
                Debug.Log(SetName);
                for (int i = 0; i < MaxGroupNumber; i++)
                {
                    if (groupmng.SetCharaToGroup(MyID, i, SetName))
                    {
                        var group = groupmng.GetGroupId();
                        //セットできた場合
                        adaptmng.AdaptImage(group[MyID], play_char);
                        textbarText.text = "採用に成功しました！";

                        //貼り付け先の探索
                        recruitment.GetComponent<Canvas>().enabled = true;
                        for (int child = 0; child < recruitment.transform.childCount; child++)
                        {
                            var ToObj = recruitment.transform.GetChild(child).gameObject;
                            Debug.Log(ToObj.name);
                            if (ToObj.name.Contains("Master"))
                            {
                                adaptmng.AdaptOneImg(ToObj, SetName);
                                //ステータス
                                adaptmng.AdaptCharaState(recruitment.transform.Find("strdown").gameObject, SetName);
                                break;
                            }
                        }
                        break;
                    }
                }
                return;
            }

            //防衛画面
            OpInvaded();

            //侵攻された画面
            Attension();

            if(recon.GetComponent<Canvas>().enabled)
            {
                if (!rewardFlag)
                {
                    //配置画面
                    var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint();
                    if (tmpselectpoint == InitVecter2)
                    {
                        //現在選択している場所のテキストを青くする
                        recon.transform.Find("Group" + nowpoint.x).Find("Text" + nowpoint.y + " (" + nowpoint.x + ")").GetComponent<Text>().color = Color.blue;
                        tmpselectpoint = nowpoint;
                    }
                    else
                    {
                        //スワップ
                        var group = groupmng.GetGroupId()[MyID];
                        var FirstCharaNum = group[tmpselectpoint.x][tmpselectpoint.y];    //最初に選択した場所
                        var SecondCharaNum = group[nowpoint.x][nowpoint.y];               //二番目に選択した場所

                        groupmng.SetCharaToGroup(MyID, tmpselectpoint.x, tmpselectpoint.y, SecondCharaNum);
                        groupmng.SetCharaToGroup(MyID, nowpoint.x, nowpoint.y, FirstCharaNum);
                        Debug.Log("OK");

                        //最初に選択したテキストを元に戻す
                        recon.transform.Find("Group" + tmpselectpoint.x).Find("Text" + tmpselectpoint.y + " (" + tmpselectpoint.x + ")").GetComponent<Text>().color = Color.black;

                        //一時保存データの初期化
                        tmpselectpoint = InitVecter2;

                        //画像の更新
                        adaptmng.AdaptFromSelecterGroup(recon.transform.Find("Master_face").gameObject);
                        //初期画面の画像の更新
                        adaptmng.AdaptImage(groupmng.GetGroupId()[MyID], play_char);
                        //画面対応(名前)
                        for (int i = 0; i < 5; i++)
                        {
                            adaptmng.AdaptName(recon.transform.Find("Group" + i).gameObject, group[i]);
                        }
                    }
                }
                else
                {
                    //恩賞画面
                    var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint();
                    var group = groupmng.GetGroupId()[MyID];
                    var CharaNumber = group[nowpoint.x][nowpoint.y];               //選択した場所
                    var characom = Groupmng.GetComponent<GameMng>().GetCharaData(CharaNumber, stateNum.com);

                    //キャラが存在しないとき
                    if (CharaNumber == "str")
                    {
                        textbarText.text = "存在する兵を選択してください";
                        return;
                    }

                    //資金が足りないとき
                    if (RewardPoint > groupmng.GetOneMoney(MyID))
                    {
                        textbarText.text = "資金が不足しています";
                        return;
                    }

                    if (characom <= 0)
                    {
                        textbarText.text = "これ以上の恩賞は不要です";
                        return;
                    }
                    else
                    {
                        //資金の減少
                        groupmng.CntDownMoney(MyID, RewardPoint);
                        adaptmng.AdaptMoney();

                        gamemng.UpCharaStateOnCom(CharaNumber);
                        Groupmng.GetComponent<GameMng>().SetCharaData(CharaNumber, stateNum.com, characom - 5);
                        adaptmng.AdaptCharaState(recon.transform.Find("strdown").gameObject, CharaNumber);
                    }
                }
            }

            //オプション画面
            if (Option.GetComponent<Canvas>().enabled)
            {
                var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().y;
                if (nowpoint == (int)OptionMenu.Save)
                {
                    //セーブ
                    Debug.Log("セーブ");
                }
                if (nowpoint == (int)OptionMenu.Manual)
                {
                    //マニュアル
                    Debug.Log("マニュアル");
                    manual.GetComponent<Canvas>().enabled = true;
                }
                if (nowpoint == (int)OptionMenu.ToTitle)
                {
                    //タイトルへ
                    Debug.Log("タイトルへ");
                    FadeManager.Instance.LoadLevel("TitleScene", 0.2f);
                }
                return;
            }
        }

    }

    //侵攻されたときの画面
    private void Attension()
    {
        if (blocks.GetComponent<Canvas>().enabled)
        {
            Invaded.GetComponent<Canvas>().enabled = true;
            //セレクターの対象を変更
            ChangeSelecter("防衛軍勢を選択してください", InvAmount, InvMaxCnt, Invselecter);

            //画面表示
            for (int i = 0; i < 5; i++)
            {
                var totalpowobj = Invaded.transform.Find("TotalPow").gameObject;
                adaptmng.AdaptTotalPow(
                    totalpowobj.transform.Find("Total (" + i.ToString() + ")").gameObject,
                    groupmng.GetGroupId()[MyID][i]);
            }
        }
    }

    //終了の処理
    public void Finish(bool secondflag)
    {
        Debug.Log("終了");
        //終了後に敵のルーチンを回す
        //グループIDの取得
        var groupid = groupmng.GetGroupId();

        bool finishFlag = true;

        if (secondflag == true)
        {
            Debug.Log(enemymove.GetTmpMasterId());
            //TmpマスターIDからループを始める
            if (enemymove.GetTmpMasterId() != "")
            {
                //二回目以降
                Debug.Log("二回目以降ー");
                var nowid = enemymove.GetTmpMasterId();
                for (var id = Convert.ToInt32(nowid) + 1; id < MaxMasterID; id++)
                {
                    if (id.ToString() == MyID)
                    {
                        //自分だったとき
                        //資金カウント
                        groupmng.CntUpMoney(MyID);
                        Debug.Log("カウント");
                        continue;
                    }

                    Debug.Log(id.ToString() + "aa");
                    //一致の確認
                    bool MatchFlag = false;
                    foreach (var masterid in groupid)
                    {
                        if (id.ToString() == masterid.Key)
                        {
                            Debug.Log(id.ToString() + "OK");
                            MatchFlag = true;
                            break;
                        }
                    }
                    if (!MatchFlag)
                    {
                        continue;
                    }
                    //二回目以降
                    var EneMoveRtnPt = enemymove.Move(id.ToString(), true);
                    if (EneMoveRtnPt.Value)
                    {
                        if (EneMoveRtnPt.Key)
                        {
                            //Debug.Log(id.ToString() + "二回目");
                            ////自領地への侵攻
                            ////侵攻されたとき
                            //blocks.GetComponent<Canvas>().enabled = true;
                            //bg_pos.GetComponent<Canvas>().enabled = false;
                            //objpos.GetComponent<Canvas>().enabled = false;
                            //enevsene.GetComponent<Canvas>().enabled = false;

                            //textbarText.text = "侵攻されました！";

                            ////画面対応
                            //var tmpobj = blocks.transform.Find("item").gameObject;
                            //adaptmng.AdaptImage(groupid[id.ToString()], tmpobj);
                            //adaptmng.AdaptOneImg(tmpobj.transform.Find("Master_face").gameObject, id.ToString());
                            //adaptmng.AdaptImage(groupid[MyID], Invaded);

                            //groupmng.SetTmpId(id.ToString());

                            //finishFlag = false;

                            break;
                        }
                        else
                        {
                            //敵から敵

                            //bg_pos.GetComponent<Canvas>().enabled = true;
                            //objpos.GetComponent<Canvas>().enabled = true;

                            //enevsene.GetComponent<Canvas>().enabled = false;

                            //textbarText.text = "敵国の偵察";
                            //bg_pos.GetComponent<Canvas>().enabled = false;
                            //objpos.GetComponent<Canvas>().enabled = false;
                            //enevsene.GetComponent<Canvas>().enabled = true;

                            //textbarText.text = "敵 対 敵";
                        }
                    }
                }
            }
            else
            {
                Debug.Log("Out");
            }
        }
        else
        {
            //初回のみ
            //ループの関係上1から順番に並んでいる
            for (var id = 0; id < MaxMasterID; id++)
            {
                bool MatchFlag = false;
                //一致の確認
                foreach (var masterid in groupid)
                {
                    if (id.ToString() == masterid.Key)
                    {
                        Debug.Log(id.ToString() + "OK");
                        MatchFlag = true;
                        break;
                    }
                }
                if(!MatchFlag)
                {
                    continue;
                }

                if (MyID == id.ToString())
                {
                    //資金カウント
                    groupmng.CntUpMoney(MyID);
                    Debug.Log("カウント");
                    //自分は省く
                    continue;
                }

                var EneMoveRtnPt = enemymove.Move(id.ToString(), false);
                if (EneMoveRtnPt.Value)
                {
                    if (EneMoveRtnPt.Key)
                    {
                        //敵から自分
                        blocks.GetComponent<Canvas>().enabled = true;
                        bg_pos.GetComponent<Canvas>().enabled = false;
                        objpos.GetComponent<Canvas>().enabled = false;
                        enevsene.GetComponent<Canvas>().enabled = false;

                        textbarText.text = "侵攻されました！";

                        var tmpobj = blocks.transform.Find("item").gameObject;
                        //画面対応
                        adaptmng.AdaptImage(groupid[id.ToString()], tmpobj);
                        adaptmng.AdaptOneImg(tmpobj.transform.Find("Master_face").gameObject, id.ToString());
                        adaptmng.AdaptImage(groupid[MyID], Invaded);

                        groupmng.SetTmpId(id.ToString());

                        finishFlag = false;

                        break;
                    }
                    else
                    {
                        Debug.Log("敵から敵；；；");
                        //敵から敵

                        //bg_pos.GetComponent<Canvas>().enabled = true;
                        //objpos.GetComponent<Canvas>().enabled = true;

                        //enevsene.GetComponent<Canvas>().enabled = false;

                        //textbarText.text = "敵国の偵察";

                        //var tmpobj = blocks.transform.Find("item").Find("SumName").gameObject;
                        //adaptmng.AdaptImage(groupid[id.ToString()], tmpobj);
                        //adaptmng.AdaptOneImg(tmpobj.transform.Find("Master_face").gameObject, id.ToString());
                    }
                }
            }
        }
        //処理が終了したときにTmpマスターIDを削除する
        //turnmng.GetComponent<EnemyMove>().SetTmpMasterId("");
        if (finishFlag)
        {
            //行動終了
            bg_pos.GetComponent<Canvas>().enabled = true;
            objpos.GetComponent<Canvas>().enabled = true;
            enevsene.GetComponent<Canvas>().enabled = false;
            enemymove.SetIsGameScene(false);
            textbarText.text = "敵国の偵察";
            //セレクター関連
            controllar.GetComponent<SelectedMng>().Setnowpoint(new Vector2Int(0, 0));
            objpos.transform.Find("Selecter").localPosition = DefseleDefPos;
        }

        adaptmng.AdaptMoney();

        groupmng.WatchMasterid();
    }

    //防衛画面
    private void OpInvaded()
    {
        if (Invaded.GetComponent<Canvas>().enabled)
        {
            //侵攻されたとき
            var groupid = groupmng.GetGroupId();
            var myid = mapctl.GetMyId();
            var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().y;
            //自分自身
            var mygroup = groupid[myid][nowpoint];

            //空白の禁止
            for (int i = 0; i < 5; i++)
            {
                if (mygroup[i] != "str")
                {
                    //キャラが存在するとき
                    break;
                }
                if (i == 4)
                {
                    textbarText.text = "兵が配置されていません";
                    return;
                }
            }

            //相手側(HPが高い順)
            var eneid = groupmng.GetTmpId();
            var enemygroup = groupmng.GetMostHPinGroup(eneid.ToString());
            //値の保持
            for (int i = 0; i < 5; i++)
            {
                gamemng.CharIDList[i] = mygroup[i];
            }
            for (int i = 5; i < 10; i++)
            {
                gamemng.CharIDList[i] = enemygroup[i - 5];
            }
            gamemng.MyAttackF = false;

            gamemng.WatchData();
            //データ引継ぎ
            gamemng.Init();

            FadeManager.Instance.LoadLevel("GameScene", 1.0f);
        }
    }

    //最初の画面へ
    public void ClickBatu()
    {
        //例外処理
        if (blocks.GetComponent<Canvas>().enabled == true || Invaded.GetComponent<Canvas>().enabled == true ||
            enevsene.GetComponent<Canvas>().enabled == true)
        {
            return;
        }

        //SEの再生
        BatuSE.PlayOneShot(BatuSE.clip);

        //例外処理
        if (recruitment.GetComponent<Canvas>().enabled)
        {
            recruitment.GetComponent<Canvas>().enabled = false;
        }
        if (manual.GetComponent<Canvas>().enabled)
        {
            manual.GetComponent<Canvas>().enabled = false;
        }

        //画面切り替え処理まとめ
        SwitchScreen(maruop.nowScreen);

        maruop.nowScreen.GetComponent<Canvas>().enabled = false;
        maruop.bg_pos.GetComponent<Canvas>().enabled = true;
        maruop.nowScreen = maruop.bg_pos;

        var SeleMng = controllar.GetComponent<SelectedMng>();

        //セレクターの対象を変える
        //子の要素の取得
        Transform children = controllar.GetComponentInChildren<Transform>();
        //子要素がいなければ終了
        if (children.childCount == 0)
        {
            return;
        }
        foreach (Transform ob in children)
        {
            var Selesqu = ob.GetComponent<Selecter>();
            //動かす量等のデータの取得
            bool changeFlag;
            try
            {
                changeFlag = Selesqu.GetChangeFlag();
            }
            catch
            {
                //取得できなかったとき
                continue;
            }
            var data = Selesqu.GetTmp();
            //変更フラグがオンになっている場合
            if (changeFlag)
            {
                //現在位置を一時保存していたものへ戻す
                SeleMng.Setnowpoint(SeleMng.GetTmppoint());

                //セレクターの対象を変える
                Selesqu.MoveObject = data.Value.Value;
                Selesqu.MaxCnt = data.Key;
                Selesqu.AmountOfMovement = data.Value.Key;

                //変更フラグをオフに
                Selesqu.SetChangeFlag(false);
            }
        }

        //座標を戻す
        GoBackDefPos(Empselecter, EmpseleDefPos);
        GoBackDefPos(Opselecter, OpseleDefPos);
        GoBackDefPos(Recselecter, RecseleDefPos);
        GoBackDefPos(Invselecter, InvseleDefPos);

        //Obj_Posをtrueに
        objpos.GetComponent<Canvas>().enabled = true;
    }

    private void GoBackDefPos(GameObject obj,Vector3 defpos)
    {
        obj.GetComponent<RectTransform>().localPosition = defpos;
    }

    private void ChangeSelecter(string text,Vector2Int AmountMove,Vector2Int maxcnt,GameObject Toselecter, bool Flag = false)
    {
        //セレクターの変更

        if (!Flag)
        {
            //Obj_Posをfalseに
            objpos.GetComponent<Canvas>().enabled = false;
        }

        //セレクターの対象を変える
        //子の要素の取得
        Transform children = controllar.GetComponentInChildren<Transform>();
        //子要素がいなければ終了
        if (children.childCount == 0)
        {
            return;
        }

        bool OneFlag = false;
        //一時保存
        foreach (Transform ob in children)
        {
            var selecter = ob.GetComponent<Selecter>();
            //動かす量等のデータの取得
            bool changeFlag;
            try
            {
                changeFlag = selecter.GetChangeFlag();
            }
            catch
            {
                //取得できなかったとき
                continue;
            }
            if (!changeFlag)
            {
                if (!OneFlag)
                {
                    var SeleMng = controllar.GetComponent<SelectedMng>();
                    //現在の場所を一時保存
                    SeleMng.SetTmppoint(SeleMng.Getnowpoint());
                    //Debug.Log(SeleMng.Getnowpoint());
                    //現在の場所を0にする
                    Vector2Int tmpvec = new Vector2Int(0, 0);
                    SeleMng.Setnowpoint(tmpvec);
                    OneFlag = true;
                }
                //変更フラグをオン
                selecter.SetChangeFlag(true);
                //上部表記の切り替え
                textbarText.text = text;
            }
            //一時保存系に格納
            selecter.SetTmp(selecter.GetNowSelecter());
            //セレクターの対象を変える
            KeyValuePair<Vector2Int, GameObject> tmptmp = new KeyValuePair<Vector2Int, GameObject>(AmountMove, Toselecter);
            //一時保存まとめを返す
            KeyValuePair<Vector2Int, KeyValuePair<Vector2Int, GameObject>> tmp = new KeyValuePair<Vector2Int, KeyValuePair<Vector2Int, GameObject>>(maxcnt, tmptmp);
            selecter.SetNowSelecter(tmp);
        }
        return;
    }

    private void SwitchScreen(GameObject gameObject)
    {
        if (gameObject.name == "employment")
        {
            //雇用画面
            if (gameObject.GetComponent<Canvas>().enabled)
            {
                //上部表記の変更
                textbarText.text = "兵士を雇用";
                return;
            }
        }
        
        if (gameObject.name == "Recon")
        {
            //配置画面
            if (gameObject.GetComponent<Canvas>().enabled)
            {
                //配置の場所を初期化
                maruop.tmpselectpoint = InitVecter2;

                //恩賞フラグの初期化
                maruop.rewardFlag = false;

                //上部表記の変更
                textbarText.text = "兵士の配置を変更";
                return;
            }
        }

        if (gameObject.name == "Option")
        {
            //オプション画面
            if (gameObject.GetComponent<Canvas>().enabled)
            {
                //上部表記の変更
                textbarText.text = "セーブ/マニュアル/終了";
                return;
            }
        }
    }

    public void ShowTraningNum()
    {
        //訓練時の数字表示
        GameObject[] CharaList = new GameObject[play_char.transform.childCount];
        for (int i = 0; i < play_char.transform.childCount; i++)
        {
            CharaList[i] = play_char.transform.GetChild(i).gameObject;
        }
        int count = 0;
        foreach(var child in CharaList)
        {
            if(child.name.Contains("Master"))
            {
                //例外処理
                continue;
            }
            Debug.Log(count);
            //キャラデータの取得
            var group = groupmng.GetGroupId()[MyID];
            var charaid = group[(count / MaxGroupNumber)][count % MaxGroupNumber];
            if (charaid == "str")
            {
                //キャラが存在しないとき
                child.transform.GetChild(0).gameObject.GetComponent<Text>().text = "0";
            }
            else
            {
                var charaexp = gamemng.GetCharaData(charaid, stateNum.exp);
                child.transform.GetChild(0).gameObject.GetComponent<Text>().text = charaexp.ToString();
            }
            count++;
        }
    }
    public void DeleteTraningNum()
    {
        //訓練時の数字削除
        GameObject[] CharaList = new GameObject[play_char.transform.childCount];
        for (int i = 0; i < play_char.transform.childCount; i++)
        {
            CharaList[i] = play_char.transform.GetChild(i).gameObject;
        }
        foreach (var child in CharaList)
        {
            if (child.name.Contains("Master"))
            {
                //例外処理
                continue;
            }
            child.transform.GetChild(0).gameObject.GetComponent<Text>().text = "";
        }
    }

    public void GoGameOverScene()
    {
        FadeManager.Instance.LoadLevel("GameOverScene", 1.0f);
        return;
    }

    public void GoClearScene()
    {
        FadeManager.Instance.LoadLevel("ClearScene", 1.0f);
        return;
    }
}
