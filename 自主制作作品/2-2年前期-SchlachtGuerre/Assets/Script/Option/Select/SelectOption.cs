using UnityEngine;
using UnityEngine.UI;

public class SelectOption : MonoBehaviour
{
    private GameObject controllar;  //コントローラー
    private GameObject obj_pos;     //配置オブジェクトまとめ（前列）
    private GameObject statenum;    //数値系

    private GameObject textbar;     //上部テキストバー
    private Text textbarText;       //テキストバー内テキスト

    private GameObject groupMng;    //グループマネージャー
    private MapCtl mapctl;          //グループマネージャー内マップ系スクリプト
    private AdaptImgMng adaptmng;   //グループマネージャー内画像貼り付け系スクリプト

    private GameObject turnmng;     //ターンマネージャー

    private bool pushbutton;        //長押し防止用

    private AudioSource MaruSE;            //選択SE
    private AudioSource BatuSE;            //キャンセルSE

    private void Start()
    {
        //SE取得
        MaruSE = GameObject.Find("ClickMaru").GetComponent<AudioSource>();
        BatuSE = GameObject.Find("ClickBatu").GetComponent<AudioSource>();

        controllar = GameObject.Find("Controllar");
        obj_pos = GameObject.Find("Obj_Pos");
        statenum = obj_pos.transform.Find("StateNum").gameObject;

        //テキスト系
        textbar = GameObject.Find("TextBar");
        textbarText = textbar.GetComponent<Text>();

        //敵の行動引き継ぎのため保持
        turnmng = GameObject.Find("TurnMng");
        DontDestroyOnLoad(turnmng);

        obj_pos.GetComponent<Canvas>().enabled = false;
        pushbutton = false;

        groupMng = GameObject.Find("GroupMng");
        //グループマネージャー系
        mapctl = groupMng.GetComponent<MapCtl>();
        adaptmng = groupMng.GetComponent<AdaptImgMng>();
        //初期化
        adaptmng.Init();
        turnmng.GetComponent<EnemyMove>().Init();

        //グループマネージャーの保持
        try
        {
            DontDestroyOnLoad(groupMng);
            DontDestroyOnLoad(turnmng);
            Debug.Log("GroupMngを保持しました");
        }
        catch
        {
            //何もしない
            Debug.Log("GroupMngが存在しません");
        }
    }
    public void ClickOption()
    {
        if (FadeManager.Instance.isFading)
        {
            return;
        }
        //はい、いいえ選択画面
        Debug.Log("Click");
        if (obj_pos.GetComponent<Canvas>().enabled == false)
        {
            //Obj_Posがアクティブ状態でなければリターン
            //Debug.Log("Obj_Posがノンアクティブ状態です");
            return;
        }
        pushbutton = true;
        var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().x;
        if (nowpoint == 0)
        {
            //はい
            int MaxMapX = 8;
            int MaxMapY = 7;
            //選択したものをMyIDに格納
            var seleObjNum = controllar.GetComponent<SelectedMng>().GetSeleObjNum();
            int idx = seleObjNum % MaxMapX;
            int idy = (seleObjNum / MaxMapX) % MaxMapY;
            mapctl.SetMyId(mapctl.GetMapID()[idy, idx]);

            //Debug.Log(groupMng.GetComponent<MapCtl>().GetMyId());
            
            FadeManager.Instance.LoadLevel("MyTurnScene", 1.0f);
            return;
        }
        if (nowpoint == 1)
        {
            //いいえ
            //Obj_Posをノンアクティブにする。
            obj_pos.GetComponent<Canvas>().enabled = false;
            textbarText.text = "プレイキャラ選択";
            return;
        }
        else
        {
            Debug.Log("Nowpointの値が不正です");
            return;
        }
    }
    public void StartClickOption()
    {
        //SEの再生
        MaruSE.PlayOneShot(MaruSE.clip);

        //初期画面
        Debug.Log("Start");
        if (obj_pos.GetComponent<Canvas>().enabled)
        {
            //Obj_Posがアクティブであればリターン
            Debug.Log("Obj_Posがアクティブ状態です");
            return;
        }
        var NowSeleObj = controllar.GetComponent<SelectedMng>().GetSelectedObj();
        if (NowSeleObj == null)
        {
            //選択できてなかった場合
            Debug.Log("SelectObjが設定されていません");
            return;
        }

        if (pushbutton == true)
        {
            Debug.Log("pushbutton:true");
            pushbutton = false;
            return;
        }

        //選択情報を渡す

        //Obj_Posをアクティブにする。
        Debug.Log("アクティブにしました");
        obj_pos.GetComponent<Canvas>().enabled = true;
        textbarText.text = "開始してよろしいですか？";

        //画面適応
        int MaxMapX = 8;
        int MaxMapY = 7;
        var seleObjNum = controllar.GetComponent<SelectedMng>().GetSeleObjNum();
        int idx = seleObjNum % MaxMapX;
        int idy = (seleObjNum / MaxMapX) % MaxMapY;
        adaptmng.AdaptStateNum(statenum, mapctl.GetMapID()[idy, idx]);

        //画面設定
        adaptmng.AdaptMaster();

        pushbutton = false;
    }
    public void NnActiveObj_Pos()
    {
        //SEの再生
        BatuSE.PlayOneShot(BatuSE.clip);

        if (obj_pos.GetComponent<Canvas>().enabled == false)
        {
            //Obj_Posがアクティブ状態でなければリターン
            //Debug.Log("Obj_Posがノンアクティブ状態です");
            return;
        }
        obj_pos.GetComponent<Canvas>().enabled = false;
        textbarText.text = "プレイキャラ選択";
    }

    public GameObject GetObjPos()
    {
        return obj_pos;
    }
}
