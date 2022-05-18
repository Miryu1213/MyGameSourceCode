using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System;

public enum buttleCmd
{ 
    atk,    //攻撃
    dam,    //攻撃
    kill,   //撃破
    deth,   //死亡
    item,   //アイテム使用
    skill,  //スキル使用
    win,    //勝利
    lose,   //敗北
    start,  //バトルシーン開始
    ell
}

public enum stateNum
{
    maxhp,  //最大HP
    hp,     //現存HP
    atk,    //物理攻撃
    matk,   //魔法攻撃
    def,    //物理防御
    mdef,   //魔法防御
    spd,    //俊敏
    exp,    //経験値
    lv,     //レベル
    mvp,    //貢献度
    com,    //不満
    max
}

public enum charName
{
    ナベリス,
    ゼパル,
    ボディス,
    サレオス,
    バティン,
    プルソン,
    モラクス,
    イポス,
    アイム,
    フラウノス,
    オリアス,
    ウァプラ,
    サガン,
    ウァラク,
    アンドラス,
    アンドフス,
    キマリス,
    アムドゥ,
    フォルネ,
    グラシャ,
    ブネ,
    ロノウェ,
    ベリト,
    アスタ,
    フォラス,
    アスモダイ,
    ガープ,
    バルバトス,
    パイモン,
    ブエル,
    グシオン,
    シトリー,
    レラジェ,
    エリゴス,
    カイム,
    ウー,
    ソナ,
    サラ,
    ナー,
    ゴル,
    サギ,
    ゴイ,
    サリー,
    クイ,
    ミミ,
    ミル,
    アプティパ,
    テール,
    テンコウ,
    アイ,
    アキクエ,
    ディバイ,
    インル,
    トーラス,
    カマノイタチ,
    カイルア,
    ゴーライナ,
    イアン,
    ライティア,
    スフィア,
    ニロ,
    ニャカス,
    ハーロイ,
    マタ,
    マイロ,
    マリー,
    ナリー,
    キアース,
    プラープ,
    リロー,
    リミー,
    シノカミ,
    ハリトノエ,
    ジョウハリ,
    ユキノエ,
    ユキカミ,
    サミノエ,
    ナナノ,
    ナハチ,
    アモン
}

public class GameMng : SingletonMonoBehaviour<GameMng>
{
    const int MaxCharacter = 79;

    //キャラの行動、ステータス情報
    public buttleCmd cmd_;
    public int hp = 100;

    //他のスクリプトの参照
    public GameObject GameMain;
    GameObject TextMain;
    chara script;

    //自分が侵攻したか否か true-自分
    public bool MyAttackF; 

    //キャラのステータス配列の作成
    public int[,] chara = new int[10, 12];

    //キャラのシーン引継ぎ用の配列 0～4が味方 ※キャラがいない箇所には"str"という文字列が入っています
    public string[] CharIDList = new string[10];

    //キャラがいないところには全部0が入ってるはずです。
    public int[,] PcharList = new int[5,12];
    public int[,] EcharList = new int[5,12];
    public bool pWinFlag = false;  //勝敗情報  f…負け/t…勝ち

    //キャラの攻撃先の選択配列 上(攻撃元)、下(攻撃先)
    public int[] Atkslt = new int[5];
    public int[] Atkslt2 = new int[5];

    public int[] EAtkslt = new int[5];

    public int[] PspdSort = new int[5];
    public int[] EspdSort = new int[5];
    public int countsev  = 0; 
    public int countsev2 = 0;
    public int countsev3 = 0;

    private AdaptImgMng adaptmng;
    private GameObject blocks;
   
    //------------------------------------------------------------------------------
    public IDictionary<string, List<int>> StetasMap = new Dictionary<string, List<int>>();
        //ステータスリストをオブジェクトIDで管理する用のmap
        //first…オブジェクトID, second…ステータスの格納されたlist
    //public List<int> Stetas = new List<int>();
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    private IDictionary<string, string> CharaNameMap = new Dictionary<string, string>();
    //キャラネーム関連
    //first…オブジェクトID, second…キャラクターの名前
    //------------------------------------------------------------------------------

    // Start is called before the first frame update
    void Awake()
    {
        if (SceneManager.GetActiveScene().name == "GameScene")
        {
            //ゲームシーン用の処理のため
            GameMain = GameObject.Find("GameObject");
            TextMain = GameObject.Find("TextMng");
            script = GameMain.GetComponent<chara>();
            adaptmng = GameObject.Find("GroupMng").GetComponent<AdaptImgMng>();
            blocks = GameObject.Find("Blocks");
        }
        //AddCharData(21);
        //AddCharData(22);

        for (int i = 0; i < MaxCharacter; i++)
        {
            //キャラのセット
            SetMapData(i.ToString());

            ////キャラのステータスをセット(一時的に)
            //for (int j = 0; j < (int)stateNum.max; j++)
            //{
            //    SetCharaData(i.ToString(), (stateNum)j, 100 + i);
            //}

            string cID = ((charName)i).ToString();

            //このステータスの部分をどうするか
            SetCharaData(i.ToString(), stateNum.hp, 100);
            SetCharaData(i.ToString(), stateNum.maxhp, 100);
            SetCharaData(i.ToString(), stateNum.atk, 15);
            SetCharaData(i.ToString(), stateNum.def, 5);
            SetCharaData(i.ToString(), stateNum.matk, 15);
            SetCharaData(i.ToString(), stateNum.mdef, 5);
            SetCharaData(i.ToString(), stateNum.spd, 15);
            SetCharaData(i.ToString(), stateNum.exp, 0);

            //キャラクターの名前
            AddCharaName(i, cID);
        }
        for (int i = 0; i < 10; i++)
        {
            SetCharaData(i.ToString(), stateNum.atk, 25);
            SetCharaData(i.ToString(), stateNum.spd, 20);
        }

        SetCharaData("9", stateNum.atk, 35);
        SetCharaData("9", stateNum.def, 10);
        SetCharaData("9", stateNum.spd, 35);

        SetCharaData("5", stateNum.atk, 35);

        SetCharaData("30", stateNum.atk, 35);
        SetCharaData("30", stateNum.spd, 40);

        SetCharaData("27", stateNum.def, 20);

        SetCharaData("28", stateNum.def, 15);

        SetCharaData("8", stateNum.atk, 100);
        SetCharaData("8", stateNum.def, 15);
        SetCharaData("8", stateNum.spd, 30);

        SetCharaData("7", stateNum.atk, 15);
        SetCharaData("7", stateNum.def, 45);

        SetCharaData("22", stateNum.atk, 30);

        //StetasMap["player"][1] … playerのステータス一つ目(HP)を参照
        //Stetasにデータを追加してからStetasMapへ格納。

        //キャラのステータスの初期化(反映)
        for (int x = 0; x < 10; x++)
        {
            addData(x, stateNum.hp, 100);
            addData(x, stateNum.maxhp,100);
            addData(x, stateNum.atk, 15);
            addData(x, stateNum.matk, 15);
            addData(x, stateNum.def, 5);
            addData(x, stateNum.mdef, 5);
            addData(x, stateNum.spd, 10);
        }
    }

    private void Start()
    {
        //引き継ぎキャラ確認用
        foreach (var id in CharIDList)
        {
            Debug.Log(id);
        }
    }

    public void Init()
    {
        //引継ぎデータのデータの反映
        for (int x = 0; x < 10; x++)
        {
            addData(x, stateNum.hp, GetCharaData(CharIDList[x],stateNum.hp));
            addData(x, stateNum.maxhp, GetCharaData(CharIDList[x], stateNum.maxhp));
            addData(x, stateNum.atk, GetCharaData(CharIDList[x], stateNum.atk));
            addData(x, stateNum.matk, GetCharaData(CharIDList[x], stateNum.matk));
            addData(x, stateNum.def, GetCharaData(CharIDList[x], stateNum.def));
            addData(x, stateNum.mdef, GetCharaData(CharIDList[x], stateNum.mdef));
            addData(x, stateNum.spd, GetCharaData(CharIDList[x], stateNum.spd));
            addData(x, stateNum.exp, GetCharaData(CharIDList[x], stateNum.exp));
            addData(x, stateNum.com, GetCharaData(CharIDList[x], stateNum.com));
        }
        adaptmng = GameObject.Find("GroupMng").GetComponent<AdaptImgMng>();
        blocks = GameObject.Find("Blocks");
    }

    //シーン移行
    public void ResultReturn()
    {
        GameMng g1;
        g1 = GameObject.Find("GroupMng").GetComponent<GameMng>();
       
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

        FadeManager.Instance.LoadLevel("MyTurnScene", 1.0f);
    }


    // Update is called once per frame
    void Update()
    {
        if (SceneManager.GetActiveScene().name != "GameScene")
        {
            //ゲームシーン以外省く
            return;
        }
        //画面適応
        if (adaptmng && blocks)
        {
            adaptmng.AdaptGame(blocks);
        }

        TextMng textMng = TextMain.GetComponent<TextMng>();
        var textWriter = textMng.GetComponent<TextWriter>();
        var textEditer = textMng.GetComponent<TextEdit>();

        if (!textWriter.Playing() && textWriter.Played())
        {
            textWriter.DeleteText();
            textEditer.ChangeFlag();
        }
    }

    public void ObjInit()
    {
        //オブジェクト系の再読み込み用
        GameMain = GameObject.Find("GameObject");
        TextMain = GameObject.Find("TextMng");
        script = GameMain.GetComponent<chara>();
    }  

    //----------------------------------------------------------------------
    //旧キャラデータの部分
    //キャラデータの配列から一部分だけを取り出す関数
    public int GetCharData(int num, stateNum stt)
    {
        return chara[num, (int)stt];
    }

    //キャラクターのダメージ計算をする関数
    public int DamageCal(int atk, int hp)
    {
        return (hp - atk);
    }
    //キャラ配列に情報を書き込む
    public void addData(int no, stateNum stetas_, int num)
    {
        chara[no, (int)stetas_] = num;
    }
    //----------------------------------------------------------------------


    //----------------------------------------------------------------------
    //新規キャラデータの部分

    //キャラデータの配列追加
    //public void AddCharData(int nom)
    //{
    //    Stetas.Add(nom);
    //}

    //キャラデータのペアでの追加
    public void SetMapData(string str)
    {
        //キャラデータが既存しているかどうかのチェック
        if(!StetasMap.ContainsKey(str))
        {
            StetasMap.Add(str, new List<int>((int)stateNum.max) { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
            return;
        }

        //キャラの連番管理
        for (int cnt = 1; cnt < 99; cnt++)
        {
            if(!StetasMap.ContainsKey(str + cnt))
            {
                StetasMap.Add(str + cnt, new List<int>((int)stateNum.max) { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
                return;
            }
        }
    }

    //キャラクターの細かいデータの追加
    public void SetCharaData(string id, stateNum state, int statepoint)
    {
        //キャラデータが既存しているかどうかのチェック
        if (!StetasMap.ContainsKey(id))
        {
            Debug.Log("IDが不正です" + id);
            return;
        }
       StetasMap[id][(int)state] = statepoint;
    }

    //キャラデータの取得
    public int GetCharaData(string id, stateNum state)
    {
        //キャラデータが既存しているかどうかのチェック
        if (!StetasMap.ContainsKey(id))
        {
            Debug.Log("IDが不正です ID:" + id);
            return -1;
        }
        return StetasMap[id][(int)state];
    }
    public List<int> GetCharaData(string id)
    {
        //キャラデータが既存しているかどうかのチェック
        if (!StetasMap.ContainsKey(id))
        {
            Debug.Log("IDが不正です ID:" + id);
            return new List<int>();
        }
        return StetasMap[id];
    }
    public int GetCharaDataSum(string id)
    {
        //キャラデータが既存しているかどうかのチェック
        if (!StetasMap.ContainsKey(id))
        {
            Debug.Log("IDが不正です ID:" + id);
            return 0;
        }
        int totalint = 0;
        for (int i = 0; i < (int)stateNum.max; i++)
        {
            totalint += StetasMap[id][i];
        }
        return totalint;
    }

    public void UpCharaStateOnExp()
    {
        //経験値に応じてステータスをUp
        for (int i = 0; i < MaxCharacter; i++)
        {
            //経験値
            var UpstateonExp = GetCharaData(i.ToString(), stateNum.exp) / 15;
            SetCharaData(i.ToString(), stateNum.maxhp, GetCharaData(i.ToString(), stateNum.maxhp) + UpstateonExp);
            SetCharaData(i.ToString(), stateNum.atk, GetCharaData(i.ToString(), stateNum.atk) + UpstateonExp);
            SetCharaData(i.ToString(), stateNum.matk, GetCharaData(i.ToString(), stateNum.matk) + UpstateonExp);
            SetCharaData(i.ToString(), stateNum.def, GetCharaData(i.ToString(), stateNum.def) + UpstateonExp);
            SetCharaData(i.ToString(), stateNum.mdef, GetCharaData(i.ToString(), stateNum.mdef) + UpstateonExp);
            SetCharaData(i.ToString(), stateNum.spd, GetCharaData(i.ToString(), stateNum.spd) + UpstateonExp);
        }
    }

    public void DownCharaStateOnCom()
    {
        //不満度に応じてステータスをDown
        for (int i = 0; i < MaxCharacter; i++)
        {
            //経験値
            var DownstateonCom = GetCharaData(i.ToString(), stateNum.com) / 3;
            if (0 <= GetCharaData(i.ToString(), stateNum.atk) - DownstateonCom)
            {
                SetCharaData(i.ToString(), stateNum.atk, GetCharaData(i.ToString(), stateNum.atk) - DownstateonCom);
            }
            else
            {
                //値がマイナスになるとき
                SetCharaData(i.ToString(), stateNum.atk, 0);
            }

            if (0 <= GetCharaData(i.ToString(), stateNum.matk) - DownstateonCom)
            {
                SetCharaData(i.ToString(), stateNum.matk, GetCharaData(i.ToString(), stateNum.matk) - DownstateonCom);
            }
            else
            {
                //値がマイナスになるとき
                SetCharaData(i.ToString(), stateNum.matk, 0);
            }

            if (0 <= GetCharaData(i.ToString(), stateNum.def) - DownstateonCom)
            {
                SetCharaData(i.ToString(), stateNum.def, GetCharaData(i.ToString(), stateNum.def) - DownstateonCom);
            }
            else
            {
                //値がマイナスになるとき
                SetCharaData(i.ToString(), stateNum.def, 0);
            }

            if (0 <= GetCharaData(i.ToString(), stateNum.mdef) - DownstateonCom)
            {
                SetCharaData(i.ToString(), stateNum.mdef, GetCharaData(i.ToString(), stateNum.mdef) - DownstateonCom);
            }
            else
            {
                //値がマイナスになるとき
                SetCharaData(i.ToString(), stateNum.mdef, 0);
            }

            if (0 <= GetCharaData(i.ToString(), stateNum.spd) - DownstateonCom)
            {
                SetCharaData(i.ToString(), stateNum.spd, GetCharaData(i.ToString(), stateNum.spd) - DownstateonCom);
            }
            else
            {
                //値がマイナスになるとき
                SetCharaData(i.ToString(), stateNum.spd, 0);
            }
        }
    }
    public void UpCharaStateOnCom(string charaid)
    {
        var DownstateonCom = GetCharaData(charaid, stateNum.com) / 3;
        DownstateonCom = -DownstateonCom;
        if (0 <= GetCharaData(charaid, stateNum.atk) - DownstateonCom)
        {
            SetCharaData(charaid, stateNum.atk, GetCharaData(charaid, stateNum.atk) - DownstateonCom);
        }
        else
        {
            //値がマイナスになるとき
            SetCharaData(charaid, stateNum.atk, 0);
        }

        if (0 <= GetCharaData(charaid, stateNum.matk) - DownstateonCom)
        {
            SetCharaData(charaid, stateNum.matk, GetCharaData(charaid, stateNum.matk) - DownstateonCom);
        }
        else
        {
            //値がマイナスになるとき
            SetCharaData(charaid, stateNum.matk, 0);
        }

        if (0 <= GetCharaData(charaid, stateNum.def) - DownstateonCom)
        {
            SetCharaData(charaid, stateNum.def, GetCharaData(charaid, stateNum.def) - DownstateonCom);
        }
        else
        {
            //値がマイナスになるとき
            SetCharaData(charaid, stateNum.def, 0);
        }

        if (0 <= GetCharaData(charaid, stateNum.mdef) - DownstateonCom)
        {
            SetCharaData(charaid, stateNum.mdef, GetCharaData(charaid, stateNum.mdef) - DownstateonCom);
        }
        else
        {
            //値がマイナスになるとき
            SetCharaData(charaid, stateNum.mdef, 0);
        }

        if (0 <= GetCharaData(charaid, stateNum.spd) - DownstateonCom)
        {
            SetCharaData(charaid, stateNum.spd, GetCharaData(charaid, stateNum.spd) - DownstateonCom);
        }
        else
        {
            //値がマイナスになるとき
            SetCharaData(charaid, stateNum.spd, 0);
        }
    }
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    //キャラクターの名前関連
    public void AddCharaName(int number,string name)
    {
        CharaNameMap.Add(number.ToString(), name);
    }

    public string GetCharaName(string id)
    {
        if(!CharaNameMap.ContainsKey(id))
        {
            Debug.Log(id + "が存在しません");
            return "str";
        }
        return CharaNameMap[id];
    }
    //----------------------------------------------------------------------

    public void WatchData()
    {
        //引き継ぎキャラ確認用
        foreach (var id in CharIDList)
        {
            Debug.Log(id);
        }
    }
}
