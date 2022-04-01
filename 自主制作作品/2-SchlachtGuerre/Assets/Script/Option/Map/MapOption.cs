using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class MapOption : MonoBehaviour
{
    const int MaxMapX = 8;
    const int MaxMapY = 7;

    Vector2Int RecAmount = new Vector2Int(123, 84);      //配置画面用移動量
    Vector2Int RecMaxCnt = new Vector2Int(5, 5);         //配置画面用移動可能数
    private GameObject Recselecter;                      //配置画面用選択
    private Vector3 RecseleDefPos;                       //配置用セレクター初期位置

    private bool AggressionFlag;    //true侵攻 false偵察    MyTurnSceneから値を引き継ぐ
    private GameObject obj_pos;     //Obj_Pos
    private GameObject controllar;  //コントローラー
    private GameObject recon;       //偵察画面
    private GameObject fademng;     //フェードマネージャ

    private GameObject textbar;     //上部テキストバー
    private Text textbarText;       //テキストバー内テキスト

    private GameObject selecter;    //選択バー
    private Vector3 selecterdefPos;

    private GameObject groupMng;
    private GroupMng groupmng;    //グループマネージャー内スクリプト
    private MapCtl mapctl;          //グループマネージャー内マップ系スクリプト
    private AdaptImgMng adaptmng;   //グループマネージャー内画像貼り付け系スクリプト

    private GameObject play_char;

    private AudioSource MaruSE;            //選択SE
    private AudioSource BatuSE;            //キャンセルSE
   
    private void Start()
    {
        //SE取得
        MaruSE = GameObject.Find("ClickMaru").GetComponent<AudioSource>();
        BatuSE = GameObject.Find("ClickBatu").GetComponent<AudioSource>();

        //オブジェクト系取得
        controllar = GameObject.Find("Controllar");

        //テキスト系
        textbar = GameObject.Find("TextBar");
        textbarText = textbar.GetComponent<Text>();

        selecter = GameObject.Find("Selecter");
        selecterdefPos = selecter.GetComponent<RectTransform>().localPosition;
        obj_pos = GameObject.Find("Obj_Pos");
        obj_pos.GetComponent<Canvas>().enabled = false;
        recon = GameObject.Find("Recon");
        recon.GetComponent<Canvas>().enabled = false;

        Recselecter = GameObject.Find("RecSelecter");
        RecseleDefPos = Recselecter.GetComponent<RectTransform>().localPosition;

        //グループマネージャー系
        groupMng = GameObject.Find("GroupMng");
        groupmng = groupMng.GetComponent<GroupMng>();
        mapctl = groupMng.GetComponent<MapCtl>();
        adaptmng = groupMng.GetComponent<AdaptImgMng>();

        play_char = GameObject.Find("Play_Char");

        fademng = GameObject.Find("FadeManager");

        //ConfirmFlag = false;
        if (AggressionFlag == true)
        {
            //侵攻
            textbarText.text = "侵攻";
        }
        else
        {
            //偵察
            textbarText.text = "偵察";
        }

        //Adapt系の初期化
        adaptmng.Init();

        //MapCtlの適応
        mapctl.AdaptMap();

        //画像系の適応
        //スタート時に行うためできない場合があるため
        var group = groupmng.GetGroupId();
        adaptmng.AdaptImage(group, play_char);
    }
    public void Confirm()
    {
        if (FadeManager.Instance.isFading)
        {
            return;
        }

        //SEの再生
        MaruSE.PlayOneShot(MaruSE.clip);

        //確認画面
        if (obj_pos.GetComponent<Canvas>().enabled == true)
        {
            //一時的に侵攻押したら領地が変わるように
            //選択した場所
            var seleObjNum = controllar.GetComponent<SelectedMng>().GetSeleObjNum();
            int x = seleObjNum % MaxMapX;
            int y = (seleObjNum / MaxMapX) % MaxMapY;

            //マップの取得
            var mapdata = mapctl.GetMapID();

            if (mapctl.GetMapID()[y, x] == mapctl.GetMyId())
            {
                textbarText.text = "自領地です";
                Debug.Log("自領地です");
                return;
            }
            var groupid = groupmng.GetGroupId();
            var myid = mapctl.GetMyId();
            var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().y;

            //自領地に接しているか
            bool MyFlag = false;
            if (0 <= x - 1)
            {
                //左
                if (mapdata[y, x - 1] == myid)
                {
                    //その場所が自分の場合OK
                    MyFlag = true;
                }
            }
            if (x + 1 < MaxMapX)
            {
                //右
                if (mapdata[y, x + 1] == myid)
                {
                    //その場所が自分の場合OK
                    MyFlag = true;
                }
            }
            if (0 <= y - 1)
            {
                //上
                if (mapdata[y - 1, x] == myid)
                {
                    //その場所が自分の場合OK
                    MyFlag = true;
                }
            }
            if (y + 1 < MaxMapY)
            {
                //下
                if (mapdata[y + 1, x] == myid)
                {
                    //その場所が自分の場合OK
                    MyFlag = true;
                }
            }
            if (!MyFlag)
            {
                textbarText.text = "自領地に接していません";
                return;
            }

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

            //相手側(体力が高い順)
            var enemygroup = groupmng.GetMostHPinGroup(mapctl.GetMapID()[y, x]);

            //値の保持
            for (int i = 0; i < 5; i++)
            {
                groupMng.GetComponent<GameMng>().CharIDList[i] = mygroup[i];
            }
            for (int i = 5; i < 10; i++)
            {
                groupMng.GetComponent<GameMng>().CharIDList[i] = enemygroup[i - 5];
            }

            var gamemng = groupMng.GetComponent<GameMng>();

            gamemng.MyAttackF = true;

            gamemng.WatchData();

            //データ引継ぎ
            gamemng.Init();

            KeyValuePair<int, int> pair = new KeyValuePair<int, int>(x, y);
            mapctl.tmpMapId = pair;
            //mapctl.SetMapID(x, y, mapctl.GetMyId());

            //クリアしているかの確認
            if (mapctl.CheckClear())
            {
                FadeManager.Instance.LoadLevel("ClearScene", 0.2f);
            }
            else
            {
                //バトルシーンへ
                FadeManager.Instance.LoadLevel("GameScene", 1.0f);
                
            }

            return;
        }
        else
        {
            //初期画面
            if (AggressionFlag == true)
            {
                //侵攻
                textbarText.text = "侵攻させる部隊の選択";
                obj_pos.GetComponent<Canvas>().enabled = true;
                //画面適応
                adaptmng.AdaptMap();

                //画面表示
                for (int i = 0; i < 5; i++)
                {
                    var totalpowobj = obj_pos.transform.Find("TotalPow").gameObject;
                    adaptmng.AdaptTotalPow(
                        totalpowobj.transform.Find("Total (" + i.ToString() + ")").gameObject,
                        groupmng.GetGroupId()[mapctl.GetMyId()][i]);
                }
            }
            else
            {
                //偵察
                recon.GetComponent<Canvas>().enabled = true;
                //選択した場所
                var seleObjNum = controllar.GetComponent<SelectedMng>().GetSeleObjNum();
                int idx = seleObjNum % MaxMapX;
                int idy = (seleObjNum / MaxMapX) % MaxMapY;
                var masterid = mapctl.GetMapID()[idy, idx];
                //var EnemyGroup = groupmng.GetComponent<GroupMng>().GetGroupId()[masterid];
                ChangeSelecter("偵察", RecAmount, RecMaxCnt, Recselecter);

                //画像の適応
                adaptmng.AdaptFromSelecterGroup(recon.transform.Find("Master_face").gameObject,masterid);

                //ステータス
                var nowPoint = controllar.GetComponent<SelectedMng>().Getnowpoint();
                var group = groupmng.GetGroupId()[masterid];
                var Charaid = group[nowPoint.x][nowPoint.y];
                adaptmng.AdaptCharaState(recon.transform.Find("strdown").gameObject, Charaid);

                //画面対応(名前)
                for (int i = 0; i < 5; i++)
                {
                    adaptmng.AdaptName(recon.transform.Find("Group" + i).gameObject, group[i]);
                }
            }

            return;
        }
    }
    
    public void ChengeMapScene()
    {
        //SEの再生
        BatuSE.PlayOneShot(BatuSE.clip);
        //×クリック時
        if (obj_pos.GetComponent<Canvas>().enabled)
        {
            //Obj_Posが描画されているとき,Selecterを初期位置に
            selecter.GetComponent<RectTransform>().localPosition = selecterdefPos;
            Vector2Int tmpvec = new Vector2Int(0, 0);
            controllar.GetComponent<SelectedMng>().Setnowpoint(tmpvec);

            obj_pos.GetComponent<Canvas>().enabled = false;
            if (AggressionFlag == true)
            {
                //侵攻
                textbarText.text = "侵攻";
            }
            else
            {
                //偵察
                textbarText.text = "偵察";
            }
        }
        else if(recon.GetComponent<Canvas>().enabled)
        {
            //偵察
            //偵察画面が描画されているとき,Selecterを初期位置に
            Recselecter.GetComponent<RectTransform>().localPosition = RecseleDefPos;
            Vector2Int tmpvec = new Vector2Int(0, 0);
            controllar.GetComponent<SelectedMng>().Setnowpoint(tmpvec);

            recon.GetComponent<Canvas>().enabled = false;
           
            textbarText.text = "偵察";
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
                var data = selecter.GetTmp();
                //変更フラグがオンになっている場合
                if (changeFlag)
                {
                    //現在位置を一時保存していたものへ戻す
                    var SeleMng = controllar.GetComponent<SelectedMng>();
                    SeleMng.Setnowpoint(SeleMng.GetTmppoint());

                    //セレクターの対象を変える
                    selecter.MoveObject = data.Value.Value;
                    selecter.MaxCnt = data.Key;
                    selecter.AmountOfMovement = data.Value.Key;

                    //変更フラグをオフに
                    selecter.SetChangeFlag(false);
                }
            }
        }
        else
        {
            //Obj_Posが描画されてないとき×押下時マイターンシーンへ
            Debug.Log("Map→MyTurn");
            //FadeManager.Instance.LoadLevel("MyTurnScene", 1.0f);
            SceneManager.LoadScene("MyTurnScene");
        }
        return;
    }

    public void SetAggressinnFlag(bool flag)
    {
        AggressionFlag = flag;
    }
    public bool GetAggressinnFlag()
    {
        return AggressionFlag;
    }
    private void ChangeSelecter(string text, Vector2Int AmountMove, Vector2Int maxcnt, GameObject Toselecter)
    {
        //セレクターの変更

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
}
