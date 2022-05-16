using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Collections.Generic;

//ボタン以外のときの選択
public class Selecter : MonoBehaviour
{
    //各機能ごとの減少資金数
    const int AggPoint = 3;    //侵攻
    const int EmpPoint = 9;    //雇用
    const int RewardPoint = 2; //恩賞
    const int TraPoint = 2;    //訓練

    const int MaxMapX = 8;
    const int MaxMapY = 7;

    public Vector2Int MaxCnt = new Vector2Int();              //最大移動数
    public Vector2Int AmountOfMovement = new Vector2Int();    //移動量
    public GameObject MoveObject;          //動く対象物
    public MoveDirection Direction;        //動く向き

    private GameObject controllar;  //下部コントローラー

    private GameObject Groupmng;    //グループ管理用
    private MapCtl mapctl;          //グループマネージャー内マップ系スクリプト
    private AdaptImgMng adaptmng;   //グループマネージャー内画像貼り付け系スクリプト

    private GameObject textbar;     //上部テキストバー
    private Text textbarText;       //テキストバー内テキスト

    private bool changeFlag;        //変更確認用フラグ
    private GameObject objpos;      //基本オブジェクトまとめ

    //例外処理用
    private GameObject option;  //オプション画面
    private GameObject employment;     //雇用
    private GameObject empmaster;     //雇用
    private GameObject rec;     //雇用成功時
    private GameObject inv;     //防衛
    private GameObject recon;   //配置
    private GameObject maru;
    private GameObject manual;

    private AudioSource MoveSE;            //移動用SE

    //選択系アイテム一時保存用
    Vector2Int TmpMaxCnt;
    Vector2Int TmpAmount;
    GameObject TmpMoveObj;
    
    private void Start()
    {
        //SE取得
        MoveSE = GameObject.Find("MoveSE").GetComponent<AudioSource>();

        //現在のシーン
        string nowScene = SceneManager.GetActiveScene().name;

        changeFlag = false;
        controllar = GameObject.Find("Controllar");
        if (nowScene == "MyTurnScene")
        {
            option = GameObject.Find("Option");
            employment = GameObject.Find("employment");
            empmaster = employment.transform.Find("Master_face").gameObject;
            rec = GameObject.Find("Recruitment");
            inv = GameObject.Find("Invaded");
            recon = GameObject.Find("Recon");
            maru = GameObject.Find("btn_maru_inside");
            manual = GameObject.Find("Manual");

            //グループマネージャー系
            Groupmng = GameObject.Find("GroupMng");
            mapctl = Groupmng.GetComponent<MapCtl>();
            adaptmng = Groupmng.GetComponent<AdaptImgMng>();
        }
        if (nowScene == "MapScene")
        {
            recon = GameObject.Find("Recon");

            //グループマネージャー系
            Groupmng = GameObject.Find("GroupMng");
            mapctl = Groupmng.GetComponent<MapCtl>();
            adaptmng = Groupmng.GetComponent<AdaptImgMng>();
        }

        try
        {
            objpos = GameObject.Find("Obj_Pos");

            //テキスト系
            textbar = GameObject.Find("TextBar");
            textbarText = textbar.GetComponent<Text>();
        }
        catch
        {
            //何もしない
        }
    }

    public void MoveSelecter()
    {
        //現在のシーン
        string nowScene = SceneManager.GetActiveScene().name;

        MoveDirection direction = Direction;
        if (direction == MoveDirection.None)
        {
            //例外処理
            Debug.Log("Directionを確認してください");
            return;
        }
        if (MoveObject == null)
        {
            //例外処理
            Debug.Log("オブジェクトを指定してください");
            return;
        }
        if (MoveObject.GetComponentInParent<Canvas>().enabled == false)
        {
            //ムーブオブジェクトが親（Canvas）がオフになっているとき
            return;
        }

        if (nowScene == "MyTurnScene")
        {
            //例外処理
            if (rec.GetComponent<Canvas>().enabled)
            {
                return;
            }
            if (!objpos.GetComponent<Canvas>().enabled)
            {
                //各種画面
               
                //省く場所(上から下へ数えるとき)
                List<int> pointlist = new List<int> { };
                CaliculatePosY(direction, pointlist, 0);
               
                if (recon.GetComponent<Canvas>().enabled)
                {
                    //配置画面
                    CaliculatePosX(direction);
                    //画像の更新
                    adaptmng.AdaptFromSelecterGroup(recon.transform.Find("Master_face").gameObject);

                    //ステータス
                    var nowPoint = controllar.GetComponent<SelectedMng>().Getnowpoint();
                    var group = Groupmng.GetComponent<GroupMng>().GetGroupId()[mapctl.GetMyId()];
                    var Charaid = group[nowPoint.x][nowPoint.y];

                    adaptmng.AdaptCharaState(recon.transform.Find("strdown").gameObject, Charaid);
                }
                else
                {
                    if (!inv.GetComponent<Canvas>().enabled)
                    {
                        //画像の更新
                        adaptmng.AdaptFromSelecter(empmaster);
                        if (employment.GetComponent<Canvas>().enabled)
                        {
                            //現在選択位置の名前を取得
                            var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().y;
                            string tmpstr = "Team (" + nowpoint.ToString() + ")";
                            string SetName = employment.transform.Find(tmpstr).gameObject.GetComponent<Image>().sprite.name;
                            SetName = SetName.Replace("fase-", "");
                            adaptmng.AdaptCharaState(employment.transform.Find("strdown").gameObject, SetName);
                        }
                    }
                }
            }
            else
            {
                //Option画面
                if (option.GetComponent<Canvas>().enabled)
                {
                    if(manual.GetComponent<Canvas>().enabled)
                    {
                        return;
                    }
                    List<int> pointList = new List<int> { };
                    CaliculatePosY(direction, pointList, 0);
                    //上部テキストの切り替え
                    ChangeText();
                }
                else
                {

                    if (!recon.GetComponent<Canvas>().enabled)
                    {
                        //初期画面
                        //省く場所(上から下へ数えるとき)
                        List<int> pointList = new List<int> { 1, 6, 7 };
                        CaliculatePosY(direction, pointList, 3);
                        if (!inv.GetComponent<Canvas>().enabled)
                        {
                            //上部テキストの切り替え
                            ChangeText();
                        }
                    }
                    else
                    {
                        //省く場所(上から下へ数えるとき)
                        List<int> pointlist = new List<int> { 1, 6, 7 };
                        CaliculatePosY(direction, pointlist, 3);
                    }
                }
            }
        }
        else if (nowScene == "GameScene")
        {
            //Gameのみ
            //攻撃選択時はリターン
            //ここの中括弧はあえて
            {
                var gameOption = controllar.transform.Find("btn_maru_inside").GetComponent<GameOption>();
                if (gameOption.GetAtackFlag() == true)
                {
                    return;
                }
            }
            //省く場所(上から下へ数えるとき)
            List<int> pointlist = new List<int> { 1 };
            CaliculatePosY(direction, pointlist, 1);
        }
        else
        {
            if (nowScene == "SelectScene")
            {
                if (objpos.GetComponent<Canvas>().enabled == false)
                {
                    //Obj_Pos（選択画面）がfalseのときは通さない
                    return;
                }
                CaliculatePosX(direction);
            }
            else
            {
                if (nowScene == "MapScene")
                {
                    if (objpos.GetComponent<Canvas>().enabled == false && recon.GetComponent<Canvas>().enabled == false)
                    {
                        //Obj_Pos（選択画面）と偵察画面がfalseのときは通さない
                        return;
                    }
                }

                if (nowScene == "TitleScene")
                {
                    if(GameObject.Find("Manual").GetComponent<Canvas>().enabled || GameObject.Find("Credit").GetComponent<Canvas>().enabled)
                    {
                        return;
                    }
                    List<int> pointlist = new List<int> { };
                    CaliculatePosY(direction, pointlist, 0);
                }
                else
                {
                    //選択した場所
                    var seleObjNum = controllar.GetComponent<SelectedMng>().GetSeleObjNum();
                    int idx = seleObjNum % MaxMapX;
                    int idy = (seleObjNum / MaxMapX) % MaxMapY;
                    var masterid = mapctl.GetMapID()[idy, idx];

                    List<int> pointlist = new List<int> { };
                    CaliculatePosY(direction, pointlist, 0);
                    CaliculatePosX(direction);
                    //画像の更新
                    adaptmng.AdaptFromSelecterGroup(recon.transform.Find("Master_face").gameObject, masterid);
                    if (recon.GetComponent<Canvas>().enabled)
                    {
                        //ステータス
                        var nowPoint = controllar.GetComponent<SelectedMng>().Getnowpoint();
                        var group = Groupmng.GetComponent<GroupMng>().GetGroupId()[masterid];
                        var Charaid = group[nowPoint.x][nowPoint.y];

                        adaptmng.AdaptCharaState(recon.transform.Find("strdown").gameObject, Charaid);
                    }
                }
            }
        }
        //SE
        MoveSE.PlayOneShot(MoveSE.clip);
    }

    private void CaliculatePosY(MoveDirection direction, List<int> pointlist, int amountPoint)
    {
        var SeleMng = controllar.GetComponent<SelectedMng>();

        if (direction == MoveDirection.Up)
        {
            //上
            Vector2Int point = SeleMng.Getnowpoint();
            point.y -= 1;
            var pos = MoveObject.GetComponent<RectTransform>().localPosition;
            if (point.y < 0)
            {
                //上から下へ
                if (MaxCnt.y != 0)
                {
                    point.y = MaxCnt.y - 1;
                    pos.y -= (AmountOfMovement.y * (MaxCnt.y - 1)) + AmountOfMovement.y * amountPoint;
                }
            }
            else
            {
                bool flag = false;
                //一つ飛び
                for (var i = 0; i < pointlist.Count; i++)
                {
                    if (pointlist[i] == point.y)
                    {
                        pos.y += AmountOfMovement.y * 2;
                        flag = true;
                        break;
                    }
                }
                if (!flag)
                {
                    //通常
                    pos.y += AmountOfMovement.y;
                }
            }
            SeleMng.Setnowpoint(point);
            MoveObject.GetComponent<RectTransform>().localPosition = pos;
        }
        if (direction == MoveDirection.Down)
        {
            //下
            Vector2Int point = SeleMng.Getnowpoint();
            point.y += 1;
            var pos = MoveObject.GetComponent<RectTransform>().localPosition;
            if (MaxCnt.y <= point.y)
            {
                //下から上へ
                point.y = 0;
                pos.y += (AmountOfMovement.y * (MaxCnt.y - 1)) + AmountOfMovement.y * amountPoint;
            }
            else
            {
                bool flag = false;
                for (var i = 0; i < pointlist.Count; i++)
                {
                    var PointI = pointlist[i] + 1;
                    if (PointI == point.y)
                    {
                        pos.y -= AmountOfMovement.y * 2;
                        flag = true;
                        break;
                    }
                }
                if (!flag)
                {
                    //通常
                    pos.y -= AmountOfMovement.y;
                }
            }
            SeleMng.Setnowpoint(point);
            MoveObject.GetComponent<RectTransform>().localPosition = pos;
        }
    }

    private void CaliculatePosX(MoveDirection direction)
    {
        var SeleMng = controllar.GetComponent<SelectedMng>();

        if (direction == MoveDirection.Left)
        {
            //左
            Vector2Int point = SeleMng.Getnowpoint();
            point.x -= 1;
            var pos = MoveObject.GetComponent<RectTransform>().localPosition;
            if (point.x < 0)
            {
                //左から右へ
                if (MaxCnt.x != 0)
                {
                    point.x = MaxCnt.x - 1;
                    pos.x += (AmountOfMovement.x * (MaxCnt.x - 1));
                }
            }
            else
            {
                //通常
                pos.x -= AmountOfMovement.x;
            }
            SeleMng.Setnowpoint(point);
            MoveObject.GetComponent<RectTransform>().localPosition = pos;
        }
        if (direction == MoveDirection.Right)
        {
            Debug.Log("OK");
            //右
            Vector2Int point = SeleMng.Getnowpoint();
            point.x += 1;
            var pos = MoveObject.GetComponent<RectTransform>().localPosition;
            if (MaxCnt.x <= point.x)
            {
                //右から左へ
                point.x = 0;
                pos.x -= (AmountOfMovement.x * (MaxCnt.x - 1));
            }
            else
            {
                //通常
                pos.x += AmountOfMovement.x;
            }
            Debug.Log(point);
            SeleMng.Setnowpoint(point);
            MoveObject.GetComponent<RectTransform>().localPosition = pos;
        }
    }
    private void ChangeText()
    {
        //現在のシーン
        string nowScene = SceneManager.GetActiveScene().name;
        if (nowScene != "MyTurnScene")
        {
            //MyTurnSceneのみ
            return;
        }
        var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint();
        if (!changeFlag)
        {
            if (nowpoint.y == (int)MyTurnOption.Function.Reconnaissance)
            {
                //偵察
                textbarText.text = "敵国の偵察";

                //消費金の表示
                adaptmng.AdaptLosMoney(0);

                return;
            }
            if (nowpoint.y == (int)MyTurnOption.Function.Aggression)
            {
                //侵攻
                textbarText.text = "隣接する他国に侵攻";

                //消費金の表示
                adaptmng.AdaptLosMoney(AggPoint);

                return;
            }
            if (nowpoint.y == (int)MyTurnOption.Function.Employment)
            {
                //雇用
                textbarText.text = "兵士を雇用";

                //消費金の表示
                adaptmng.AdaptLosMoney(EmpPoint);

                return;
            }
            if (nowpoint.y == (int)MyTurnOption.Function.Reward)
            {
                //恩賞
                textbarText.text = "配下へ恩賞を与える";
                maru.GetComponent<MyTurnOption>().DeleteTraningNum();

                //消費金の表示
                adaptmng.AdaptLosMoney(RewardPoint);

                return;
            }
            if (nowpoint.y == (int)MyTurnOption.Function.Training)
            {
                //訓練
                textbarText.text = "兵士を訓練";
                maru.GetComponent<MyTurnOption>().ShowTraningNum();

                //消費金の表示
                adaptmng.AdaptLosMoney(TraPoint);

                return;
            }
            if (nowpoint.y == (int)MyTurnOption.Function.Purchase)
            {
                //購入
                textbarText.text = "武具の購入";
                maru.GetComponent<MyTurnOption>().DeleteTraningNum();

                //消費金の表示
                adaptmng.AdaptLosMoney(0);

                return;
            }
            if (nowpoint.y == (int)MyTurnOption.Function.Equipment)
            {
                //装備
                textbarText.text = "武具の装備";

                //消費金の表示
                adaptmng.AdaptLosMoney(0);

                return;
            }
            if (nowpoint.y == (int)MyTurnOption.Function.Expulsion)
            {
                //配置
                textbarText.text = "兵士の配置を変更";

                //消費金の表示
                adaptmng.AdaptLosMoney(0);

                return;
            }
            if (nowpoint.y == (int)MyTurnOption.Function.Option)
            {
                //機能
                textbarText.text = "セーブ/マニュアル/終了";

                //消費金の表示
                adaptmng.AdaptLosMoney(0);

                return;
            }
            if (nowpoint.y == (int)MyTurnOption.Function.End)
            {
                //終了
                textbarText.text = "ターン命令を完了";

                //消費金の表示
                adaptmng.AdaptLosMoney(0);

                return;
            }
            else
            {
                Debug.Log("Nowpointの値が不正です:" + nowpoint.ToString());
                return;
            }
        }
        else
        {
            if (option.GetComponent<Canvas>().enabled)
            {
                //機能画面の画面表示
                if (nowpoint.y == (int)MyTurnOption.OptionMenu.Save)
                {
                    //セーブ
                    textbarText.text = "現在の勢力情報を保存";
                    return;
                }
                if (nowpoint.y == (int)MyTurnOption.OptionMenu.Manual)
                {
                    //マニュアル
                    textbarText.text = "操作の説明を表示";
                    return;
                }
                if (nowpoint.y == (int)MyTurnOption.OptionMenu.ToTitle)
                {
                    //タイトルへ
                    textbarText.text = "タイトル画面へ戻ります";
                    return;
                }
                else
                {
                    Debug.Log("Nowpointの値が不正です");
                    return;
                }
            }
        }
    }

    public bool GetChangeFlag()
    {
        return changeFlag;
    }
    public void SetChangeFlag(bool flag)
    {
        changeFlag = flag;
    }

    public KeyValuePair<Vector2Int,KeyValuePair<Vector2Int,GameObject>> GetTmp()
    {
        KeyValuePair<Vector2Int, GameObject> tmptmp = new KeyValuePair<Vector2Int, GameObject>(TmpAmount, TmpMoveObj);
        //一時保存まとめを返す
        KeyValuePair<Vector2Int, KeyValuePair<Vector2Int, GameObject>> tmp = new KeyValuePair<Vector2Int, KeyValuePair<Vector2Int, GameObject>>(TmpMaxCnt, tmptmp);
        return tmp;
    }
    public void SetTmp(KeyValuePair<Vector2Int, KeyValuePair<Vector2Int, GameObject>> tmp)
    {
        TmpMaxCnt = tmp.Key;
        TmpAmount = tmp.Value.Key;
        TmpMoveObj = tmp.Value.Value;
    }

    public KeyValuePair<Vector2Int, KeyValuePair<Vector2Int, GameObject>> GetNowSelecter()
    {
        KeyValuePair<Vector2Int, GameObject> tmptmp = new KeyValuePair<Vector2Int, GameObject>(AmountOfMovement, MoveObject);
        //一時保存まとめを返す
        KeyValuePair<Vector2Int, KeyValuePair<Vector2Int, GameObject>> tmp = new KeyValuePair<Vector2Int, KeyValuePair<Vector2Int, GameObject>>(MaxCnt, tmptmp);
        return tmp;
    }

    public void SetNowSelecter(KeyValuePair<Vector2Int, KeyValuePair<Vector2Int, GameObject>> tmp)
    {
        MaxCnt = tmp.Key;
        AmountOfMovement = tmp.Value.Key;
        MoveObject = tmp.Value.Value;
    }
}
