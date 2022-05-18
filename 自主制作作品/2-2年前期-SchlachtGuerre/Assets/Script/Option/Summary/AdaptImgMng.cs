using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class AdaptImgMng : MonoBehaviour
{
    const string InitArrayStr = "str";  //初期配列文字

    const int addPoint = 1;             //画面表示用追加数字

    const int MaxMapX = 8;
    const int MaxMapY = 7;

    //現在のグループ情報を取得し、対応した画像を張り付ける
    const int MaxCharacter = 79;

    //雇用画面最大数
    //const int MaxEmp = 5;

    //キャラ画像まとめ　左からキャラクターのID、キャラの画像
    private Dictionary<string, Sprite> CharaDir = new Dictionary<string, Sprite> { };

    //キャラ画像管理用
    Sprite[] CharaFGraphH = new Sprite[MaxCharacter];

    private GameObject controllar;
    GameObject Groupmng;
    GroupMng groupmng;
    GameMng gamemng;
    MapCtl mapctl;

    GameObject objpos;              //オブジェクトまとめ
    //GameObject[] ChaImgObjArr;    //貼り付け先オブジェクトまとめ

    //マイターンシーン用
    GameObject employment;

    private void Start()
    {
        //画像のロード
        for (int i = 0; i < MaxCharacter; i++)
        {
            string imgstr = "Character/fase-" + i.ToString();
            //Debug.Log(imgstr);
            CharaFGraphH[i] = Resources.Load<Sprite>(imgstr);
            if (CharaFGraphH[i] == null)
            {
                Debug.Log(i.ToString() + "番目の画像のロードに失敗");
            }
            else
            {
                //キャラ画像とIDの対応付け
                CharaDir.Add(i.ToString(), CharaFGraphH[i]);
            }
        }
        Init();
    }

    public void Init()
    {
        //オブジェクト系色々取得
        controllar = GameObject.Find("Controllar");
        Groupmng = GameObject.Find("GroupMng");
        groupmng = Groupmng.GetComponent<GroupMng>();
        gamemng = Groupmng.GetComponent<GameMng>();
        mapctl = Groupmng.GetComponent<MapCtl>();


        objpos = GameObject.Find("Obj_Pos");

        if (SceneManager.GetActiveScene().name == "MyTurnScene")
        {
            employment = GameObject.Find("employment");
        }
        //出力画面の取得
        //play_char = GameObject.Find("Play_Char");
    }

    //マップから参照する場合
    public void AdaptImage(IDictionary<string, Dictionary<int, string[]>> grouplist, GameObject gameObject)
    {
        int selenum = 0;
        try
        {
            //シーンによっては使っていないため
            selenum = controllar.GetComponent<SelectedMng>().GetSeleObjNum();
        }
        catch
        {
            //何もしない
        }
        //取得した画像を二次元に
        int idx = selenum % MaxMapX;
        int idy = (selenum / MaxMapX) % MaxMapY;
        var masterid = mapctl.GetMapID();
        AdaptImage(grouplist[masterid[idy, idx]], gameObject);
    }
    //対応した場所の画像を貼り付け(引数はマスターIDで指定したグループ,貼り付け先の親)
    public void AdaptImage(Dictionary<int, string[]> grouplist,GameObject gameObject)
    {
        var ObjChildren = new GameObject[gameObject.transform.childCount];
        for (int i = 0; i < gameObject.transform.childCount; i++)
        {
            //Debug.Log(i.ToString());
            ObjChildren[i] = gameObject.transform.GetChild(i).gameObject;
            //Debug.Log(ObjChildren[i].name);
        }
 
        int cnt1 = 0;
        int cnt2 = 0;
        int cnt3 = 0;
        int cnt4 = 0;
        int cnt5 = 0;

        foreach (var ObjList in ObjChildren)
        {
            Image ObjImage = ObjList.GetComponent<Image>();
            //チーム1の設定
            if (ObjList.name.Contains("Team1"))
            {
                if (grouplist[0][cnt1] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    ObjImage.sprite = CharaDir[grouplist[0][cnt1]];
                }
                cnt1++;
                continue;
            }
            //チーム2の設定
            if (ObjList.name.Contains("Team2"))
            {
                if (grouplist[1][cnt2] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    foreach(var test in grouplist)
                    {
                        foreach(var test2 in test.Value)
                        {
                            //Debug.Log(test2);
                        }
                    }
                    //Debug.Log(cnt2);
                    //var test = grouplist[1][cnt2];
                    //Debug.Log(test);
                    ObjImage.sprite = CharaDir[grouplist[1][cnt2]];
                }
                cnt2++;
                continue;
            }
            //チーム3の設定
            if (ObjList.name.Contains("Team3"))
            {
                if (grouplist[2][cnt3] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    ObjImage.sprite = CharaDir[grouplist[2][cnt3]];
                }
                cnt3++;
                continue;
            }
            //チーム4の設定
            if (ObjList.name.Contains("Team4"))
            {
                if (grouplist[3][cnt4] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    ObjImage.sprite = CharaDir[grouplist[3][cnt4]];
                }
                cnt4++;
                continue;
            }
            //チーム5の設定
            if (ObjList.name.Contains("Team5"))
            {
                if (grouplist[4][cnt5] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    ObjImage.sprite = CharaDir[grouplist[4][cnt5]];
                }
                cnt5++;
                continue;
            }
            //マスターフェイス
            if (ObjList.name.Contains("Master"))
            {
                string NowScene = SceneManager.GetActiveScene().name;
                if ( NowScene== "MyTurnScene" ||NowScene == "ClearScene" || NowScene == "GameOverScene")
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    //自分自身のグループのセット
                    AdaptMaster(ObjList, true);
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    AdaptMaster(ObjList, false);
                }
                continue;
            }
        }
    }

    //引数は貼り付け先、自分自身か否か
    public void AdaptMaster(GameObject gameObject, bool MyFlag)
    {
        if (MyFlag)
        {
            var myid = mapctl.GetMyId();
            gameObject.GetComponent<Image>().sprite = CharaDir[myid];
        }
        else
        {
            //選択場所の取得
            var selenum = controllar.GetComponent<SelectedMng>().GetSeleObjNum();
            //取得した画像を二次元に
            int idx = selenum % MaxMapX;
            int idy = (selenum / MaxMapX) % MaxMapY;
            var masterid = mapctl.GetMapID();

            gameObject.GetComponent<Image>().sprite = CharaDir[masterid[idy, idx]];
        }
    }

    //マスター画像の貼り付け
    public void AdaptMaster()
    {
        //貼り付け先取得
        var master = objpos.transform.Find("Master_face").gameObject;
        //選択場所の取得
        var selenum = controllar.GetComponent<SelectedMng>().GetSeleObjNum();
        //取得した画像を二次元に
        int idx = selenum % MaxMapX;
        int idy = (selenum / MaxMapX) % MaxMapY;
        var masterid = mapctl.GetMapID();

        master.GetComponent<Image>().sprite = CharaDir[masterid[idy, idx]];
    }

    //貼り付け先と張り付けるキャラのID
    public void AdaptOneImg(GameObject gameObject, string setname)
    {
        Debug.Log(gameObject.name + setname);
        gameObject.GetComponent<Image>().sprite = CharaDir[setname];
    }

    //雇用画面用
    public void AdaptEmp()
    {
        //使っていないIDの取得
        var NonId = groupmng.SearchNnId();

        //乱数
        var ramdam = new System.Random();
       
        //貼り付け先の取得
        for (int i = 0; i < employment.transform.childCount; i++)
        {
            var child = employment.transform.GetChild(i).gameObject;
            if (child.name.Contains("Team"))
            {
                //残IDに応じて表示をオフにする
                //残っているIDが3以下の場合
                if (NonId.Count <= 3)
                {
                    if (child.name.Contains("3"))
                    {
                        //child.GetComponent<Image>().color = Color.clear;
                    }
                    //残っているIDが2以下の場合
                    if (NonId.Count <= 2)
                    {
                        if (child.name.Contains("2"))
                        {
                            child.GetComponent<Image>().color = Color.clear;
                        }
                        //残っているIDが3以下の場合
                        if (NonId.Count <= 1)
                        {
                            if (child.name.Contains("1"))
                            {
                                child.GetComponent<Image>().color = Color.clear;
                            }
                            //残っているIDが0以下の場合
                            if (NonId.Count <= 0)
                            {
                                if (child.name.Contains("0"))
                                {
                                    child.GetComponent<Image>().color = Color.clear;
                                }
                            }
                        }
                    }
                }

                while (true)
                {
                    //残っているIDが0個の場合
                    if (NonId.Count == 0)
                    {
                        break;
                    }

                    //乱数を取得
                    var id = ramdam.Next(0, MaxCharacter);

                    if (NonId.Contains(id.ToString()))
                    {
                        //貼り付け
                        child.GetComponent<Image>().sprite = CharaDir[id.ToString()];
                        NonId.Remove(id.ToString());
                        break;
                    }
                }
                continue;
            }
            if (child.name.Contains("Master_"))
            {
                AdaptFromSelecter(child);
                continue;
            }
        }
    }

    //セレクターから画面出力元を決定
    public void AdaptFromSelecter(GameObject gameObject)
    {
        //セレクターの位置
        var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().y;

        Debug.Log(nowpoint.ToString());
       
        //セレクター位置検索
        //現在選択位置の画像を貼り付け
        string tmpstr = "Team (" + nowpoint.ToString() + ")";
        var ToImage = employment.transform.Find(tmpstr).gameObject.GetComponent<Image>();

        //相手方が透明だったときに透明にする
        if (ToImage.color == Color.clear)
        {
            gameObject.GetComponent<Image>().color = Color.clear;
        }
        else
        {
            gameObject.GetComponent<Image>().color = new Color(255, 255, 255, 255);
        }
        gameObject.GetComponent<Image>().sprite = employment.transform.Find(tmpstr).gameObject.GetComponent<Image>().sprite;
    }

    //セレクターからグループにアクセスし画面出力元を決定 引数　貼り付け先オブジェクト
    public void AdaptFromSelecterGroup(GameObject gameObject)
    {
        //セレクターの位置
        var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint();

        var group = groupmng.GetGroupId()[mapctl.GetMyId()];
        var CharaNum = group[nowpoint.x][nowpoint.y];

        //セレクター位置検索
        //現在選択位置の画像を貼り付け
        var ToImage = gameObject.GetComponent<Image>();

        //相手方が透明だったときに透明にする
        if (ToImage.color == Color.clear)
        {
            ToImage.color = Color.clear;
        }
        else
        {
            ToImage.color = new Color(255, 255, 255, 255);
        }

        if (CharaNum == InitArrayStr)
        {
            //選択位置にキャラが入っていない場合
            ToImage.color = Color.clear;
        }
        else
        {
            ToImage.color = new Color(255, 255, 255, 255);
            ToImage.sprite = CharaDir[CharaNum];
        }
    }

    //セレクターからグループにアクセスし画面出力元を決定 引数　貼り付け先オブジェクト マスターID
    public void AdaptFromSelecterGroup(GameObject gameObject,string masterid)
    {
        //セレクターの位置
        var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint();

        var group = groupmng.GetGroupId()[masterid];
        var CharaNum = group[nowpoint.x][nowpoint.y];

        //セレクター位置検索
        //現在選択位置の画像を貼り付け
        var ToImage = gameObject.GetComponent<Image>();

        //相手方が透明だったときに透明にする
        if (ToImage.color == Color.clear)
        {
            ToImage.color = Color.clear;
        }
        else
        {
            ToImage.color = new Color(255, 255, 255, 255);
        }

        if (CharaNum == InitArrayStr)
        {
            //選択位置にキャラが入っていない場合
            ToImage.color = Color.clear;
        }
        else
        {
            ToImage.color = new Color(255, 255, 255, 255);
            ToImage.sprite = CharaDir[CharaNum];
        }
    }

    //マップシーン
    public void AdaptMap()
    {
        var Mymasterid = mapctl.GetMyId(); //自分自身のID
        //現在選択場所のグループを取得
        Dictionary<int, string[]> grouplist = new Dictionary<int, string[]>();
       
        //探索元
        grouplist = groupmng.GetCharaGroup(Mymasterid);

        int cnt1 = 0;
        int cnt2 = 0;
        int cnt3 = 0;
        int cnt4 = 0;
        int cnt5 = 0;

        var childnum = objpos.transform.childCount;
        //貼り付け先
        GameObject[] children = new GameObject[childnum];
        for (int i = 0; i < childnum; i++)
        {
            children[i] = objpos.transform.GetChild(i).gameObject;
        }

        foreach (var ObjList in children)
        {
            var ObjImage = ObjList.GetComponent<Image>();
            //チーム1の設定
            if (ObjList.name.Contains("Team1"))
            {
                if (grouplist[0][cnt1] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    ObjImage.sprite = CharaDir[grouplist[0][cnt1]];
                }
                cnt1++;
                continue;
            }
            //チーム2の設定
            if (ObjList.name.Contains("Team2"))
            {
                if (grouplist[1][cnt2] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    ObjImage.sprite = CharaDir[grouplist[1][cnt2]];
                }
                cnt2++;
                continue;
            }
            //チーム3の設定
            if (ObjList.name.Contains("Team3"))
            {
                if (grouplist[2][cnt3] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    ObjImage.sprite = CharaDir[grouplist[2][cnt3]];
                }
                cnt3++;
                continue;
            }
            //チーム4の設定
            if (ObjList.name.Contains("Team4"))
            {
                if (grouplist[3][cnt4] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    ObjImage.sprite = CharaDir[grouplist[3][cnt4]];
                }
                cnt4++;
                continue;
            }
            //チーム5の設定
            if (ObjList.name.Contains("Team5"))
            {
                if (grouplist[4][cnt5] == InitArrayStr)
                {
                    ObjImage.color = Color.clear;
                }
                else
                {
                    if (ObjImage.color == Color.clear)
                    {
                        ObjImage.color = new Color(255, 255, 255, 255);
                    }
                    ObjImage.sprite = CharaDir[grouplist[4][cnt5]];
                }
                cnt5++;
                continue;
            }
            //マスターフェイス
            if (ObjList.name.Contains("Master"))
            {
                if (ObjImage.color == Color.clear)
                {
                    ObjImage.color = new Color(255, 255, 255, 255);
                }
                ObjImage.sprite = CharaDir[Mymasterid];
                continue;
            }
        }
    }

    public void AdaptMoney()
    {
        //資金の対応
        var Mymasterid = mapctl.GetMyId(); //自分自身のID
        var groupMng = Groupmng.GetComponent<GroupMng>();

        var money = GameObject.Find("Code").transform.Find("money").gameObject;
        money.GetComponent<Text>().text = groupMng.GetOneMoney(Mymasterid).ToString();
    }

    public void AdaptLosMoney(int losmoney)
    {
        //消費資金の対応
        var Mymasterid = mapctl.GetMyId(); //自分自身のID
        var groupMng = Groupmng.GetComponent<GroupMng>();

        var money = GameObject.Find("Code").transform.Find("losmoney").gameObject;
        money.GetComponent<Text>().text = losmoney.ToString();
    }

    public void AdaptStateNum(GameObject parentObj,string masterid)
    {
        //キャラステータス情報の画面表示
        var childnum = parentObj.transform.childCount;
        //貼り付け先
        GameObject[] children = new GameObject[childnum];
        for (int i = 0; i < childnum; i++)
        {
            children[i] = parentObj.transform.GetChild(i).gameObject;
        }

        foreach (var statenum in children)
        {
            var statename = statenum.GetComponent<Text>();
            if (statenum.name == "TotalPow")
            {
                //総戦力
                var groupid = groupmng.GetGroupId()[masterid];
                int count = 0;
                foreach(var group in groupid)
                {
                    foreach (var charaid in group.Value)
                    {
                        if (gamemng.GetCharaData(charaid, (stateNum)0) == -1)
                        {
                            //IDが存在しないとき
                            continue;
                        }
                        for (int i = 0; i < (int)stateNum.max; i++)
                        {
                            count += gamemng.GetCharaData(charaid, (stateNum)i) + addPoint;
                        }
                    }
                }
                statename.text = count.ToString();
                Debug.Log(statename);
                continue;
            }
            if (statenum.name == "M_Name")
            {
                //マスターネーム
                statename.text = gamemng.GetCharaName(masterid);
                Debug.Log(statename);
                continue;
            }
            if (statenum.name == "GroupSum")
            {
                //領地数
                statename.text = mapctl.GetMapIdNum(masterid).ToString();
                continue;
            }
            if (statenum.name == "TotalMoney")
            {
                //総資金
                int tmpmoney = groupmng.GetOneMoney(masterid) + mapctl.GetMapIdNum(masterid) * 9;
                statename.text = tmpmoney.ToString();
                continue;
            }
        }
    }

    public void AdaptCharaState(GameObject parentObj, string charaid)
    {
        //キャラデータの適応
        //キャラステータス情報の画面表示
        var childnum = parentObj.transform.childCount;
        //貼り付け先
        GameObject[] children = new GameObject[childnum];
        for (int i = 0; i < childnum; i++)
        {
            children[i] = parentObj.transform.GetChild(i).gameObject;
        }

        //キャラデータの取得
        var charadate = gamemng.GetCharaData(charaid);

        //キャラが存在しない場合
        if (charaid == InitArrayStr)
        {
            foreach (var statenum in children)
            {
                var statename = statenum.GetComponent<Text>();
                if (statenum.name.Contains("Name"))
                {
                    statename.text = "0";
                    continue;
                }
                if (statenum.name.Contains("MAXHP"))
                {
                    statename.text = "0";
                    continue;
                }
                if (statenum.name.Contains("HP"))
                {
                    statename.text = "0";
                    continue;
                }
                if (statenum.name.Contains("ATK"))
                {
                    statename.text = "0";
                    continue;
                }
                if (statenum.name.Contains("DEF"))
                {
                    statename.text = "0";
                    continue;
                }
                if (statenum.name.Contains("AGI"))
                {
                    statename.text = "0";
                    continue;
                }
                if (statenum.name.Contains("MATK"))
                {
                    statename.text = "0";
                    continue;
                }
                if (statenum.name.Contains("MDEF"))
                {
                    statename.text = "0";
                    continue;
                }
                if (statenum.name.Contains("TotalPow"))
                {
                    statename.text = "0";
                    continue;
                }
                if (statenum.name.Contains("COM"))
                {
                    statename.text = "0";
                    continue;
                }
            }
        }
        else
        {
            foreach (var statenum in children)
            {
                var statename = statenum.GetComponent<Text>();
                if (statenum.name.Contains("Name"))
                {
                    statename.text = gamemng.GetCharaName(charaid);
                    continue;
                }
                if (statenum.name.Contains("MAXHP"))
                {
                    statename.text = charadate[(int)stateNum.maxhp].ToString();
                    continue;
                }
                if (statenum.name.Contains("HP"))
                {
                    statename.text = charadate[(int)stateNum.hp].ToString();
                    continue;
                }
                if (statenum.name.Contains("ATK"))
                {
                    statename.text = charadate[(int)stateNum.atk].ToString();
                    continue;
                }
                if (statenum.name.Contains("DEF"))
                {
                    statename.text = charadate[(int)stateNum.def].ToString();
                    continue;
                }
                if (statenum.name.Contains("AGI"))
                {
                    statename.text = charadate[(int)stateNum.spd].ToString();
                    continue;
                }
                if (statenum.name.Contains("MATK"))
                {
                    statename.text = charadate[(int)stateNum.matk].ToString();
                    continue;
                }
                if (statenum.name.Contains("MDEF"))
                {
                    statename.text = charadate[(int)stateNum.mdef].ToString();
                    continue;
                }
                if (statenum.name.Contains("TotalPow"))
                {
                    statename.text = gamemng.GetCharaDataSum(charaid).ToString();
                    continue;
                }
                if (statenum.name.Contains("COM"))
                {
                    statename.text = charadate[(int)stateNum.com].ToString();
                    continue;
                }
            }
        }
    }

    public void AdaptTotalPow(GameObject ToObject, string[] charaid)
    {
        //軍団データの適応
       
        int totalcnt = 0;
        //キャラデータの取得
        for (int i = 0; i < 5; i++)
        {
            var charadate = gamemng.GetCharaData(charaid[i]);

            //キャラが存在しない場合
            if (charaid[i] == InitArrayStr)
            {
                continue;
            }

            for (int j = 0; j < (int)stateNum.max; j++)
            {
                totalcnt += charadate[j] + addPoint;
            }
        }
        ToObject.GetComponent<Text>().text = totalcnt.ToString();
    }
    public void AdaptName(GameObject parentObj, string[] charaid)
    {
        //キャラの名前の適応
        var childnum = parentObj.transform.childCount;
        //貼り付け先
        GameObject[] children = new GameObject[childnum];
        for (int i = 0; i < childnum; i++)
        {
            children[i] = parentObj.transform.GetChild(i).gameObject;
        }

        for (int i = 0; i < 5; i++)
        {
            if (charaid[i] == InitArrayStr)
            {
                //キャラが入っていないとき
                children[i].GetComponent<Text>().text = "";
                continue;
            }
            children[i].GetComponent<Text>().text = gamemng.GetCharaName(charaid[i]);
        }
    }

    public void AdaptVs(GameObject parentObj, string atkMasterid, string defMasterid)
    {
        var atkMasterface = parentObj.transform.Find("atk_Master_face").gameObject.GetComponent<Image>();
        var atkname = parentObj.transform.Find("atk_name").gameObject.GetComponent<Text>();
        var atktotal = parentObj.transform.Find("atk_total").gameObject.GetComponent<Text>();

        var defMasterface = parentObj.transform.Find("def_Master_face").gameObject.GetComponent<Image>();
        var defname = parentObj.transform.Find("def_name").gameObject.GetComponent<Text>();
        var deftotal = parentObj.transform.Find("def_total").gameObject.GetComponent<Text>();

        var groupid = groupmng.GetGroupId();

        //画像
        atkMasterface.sprite = CharaDir[atkMasterid];
        defMasterface.sprite = CharaDir[defMasterid];
        //名前
        atkname.text = gamemng.GetCharaName(atkMasterid);
        defname.text = gamemng.GetCharaName(defMasterid);
        //合計
        atktotal.text = groupmng.GetTotalinGroup(atkMasterid).ToString();
        deftotal.text = groupmng.GetTotalinGroup(defMasterid).ToString();
    }

    public IEnumerator AdaptRamdam(GameObject rootObj)
    {
        var parentObj =rootObj.transform.Find("move").Find("SumName").gameObject;

        var rightface = parentObj.transform.Find("Master_face2").gameObject.GetComponent<Image>();
        var rightname = parentObj.transform.Find("Name2").gameObject.GetComponent<Text>();
        var righttotal = parentObj.transform.Find("Total2").gameObject.GetComponent<Text>();

        var leftface = parentObj.transform.Find("Master_face").gameObject.GetComponent<Image>();
        var leftname = parentObj.transform.Find("Name").gameObject.GetComponent<Text>();
        var lefttotal = parentObj.transform.Find("Total").gameObject.GetComponent<Text>();

        rootObj.transform.Find("move").gameObject.SetActive(true);

        //順番に表示
        for (int i = 0; i < MaxCharacter; i++)
        {
            //画像
            rightface.sprite = CharaDir[i.ToString()];
            //名前
            rightname.text = gamemng.GetCharaName(i.ToString());
            //合計
            righttotal.text = gamemng.GetCharaDataSum(i.ToString()).ToString();

            if (i + 1 < MaxCharacter)
            {
                //画像
                leftface.sprite = CharaDir[(i + 1).ToString()];
                //名前
                leftname.text = gamemng.GetCharaName((i + 1).ToString());
                //合計
                lefttotal.text = gamemng.GetCharaDataSum((i + 1).ToString()).ToString();
            }
            else
            {
                //画像
                leftface.sprite = CharaDir[(i - 4).ToString()];
                //名前
                leftname.text = gamemng.GetCharaName((i - 4).ToString());
                //合計
                lefttotal.text = gamemng.GetCharaDataSum((i - 4).ToString()).ToString();
            }
            yield return new WaitForSeconds(0.04f);
        }
        //rootObj.transform.Find("move").gameObject.SetActive(false);
        //rootObj.transform.Find("vs").gameObject.SetActive(true);

        controllar.transform.Find("btn_maru_inside").GetComponent<MyTurnOption>().Finish(false);
    }
    public void AdaptGame(GameObject parentObj)
    {
        GameMng g1;
        g1 = GameObject.Find("GroupMng").GetComponent<GameMng>();

        //適応
        for (int i = 0; i < 5; i++)
        {
            for (int a = 0; a < (int)stateNum.max; a++)
            {
                g1.PcharList[i, a] = g1.GetCharData(i, (stateNum)a);
            }
        }

        for (int i = 5; i < 10; i++)
        {
            for (int a = 0; a < (int)stateNum.max; a++)
            {
                g1.EcharList[i - 5, a] = g1.GetCharData(i, (stateNum)a);
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

        //ゲームシーン用のAdapt
        var seleObj = controllar.GetComponent<SelectedMng>().GetSelectedObj();
        if (seleObj.GetComponent<Image>().color == new Color(0, 0, 0, 0))
        {
            var tmpobj = parentObj.transform.Find("Master_face").gameObject;
            tmpobj.GetComponent<Image>().color = new Color(0, 0, 0, 0);
            AdaptCharaState(parentObj.transform.Find("strdown").gameObject, InitArrayStr);
        }
        else
        {
            var selename = seleObj.GetComponent<Image>().sprite.name;
            selename.Replace(".png", "");
            //parentObj.transform.Find("Master_face").gameObject.GetComponent<Image>().sprite = CharaDir[selename];
            //Debug.Log(parentObj);
            var tmpobj = parentObj.transform.Find("Master_face").gameObject;
            tmpobj.GetComponent<Image>().color = new Color(255, 255, 255, 255);
            tmpobj.GetComponent<Image>().sprite = CharaDir[selename];
            AdaptCharaState(parentObj.transform.Find("strdown").gameObject, selename);
        }
    }

}
