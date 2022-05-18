using UnityEngine;
using System;
public class GameOption : MonoBehaviour
{
    enum Function
    {
        //選択できる機能一覧
        Atack,      //攻撃
        //Skill,      //スキル
        Retreat     //撤退
    }
    private GameObject controllar;
    private GameObject marubtn;        //〇ボタン
    private GameObject objpos;
    private GameObject groupmng;
    public GameObject ResultPanel;
    private bool AtackFlag;
    public int count;
    private int eCount;
    public int numhp;
    private int plnum = 0;
    private int eynum = 0;
    GameObject GameMain;
    GameObject ImgMain;
    GameObject TextMain;
    private void Start()
    {
        GameMain = GameObject.Find("GameMng");
        ImgMain = GameObject.Find("GroupMng");
        TextMain = GameObject.Find("TextMng");
        controllar = GameObject.Find("Controllar");
        marubtn = GameObject.Find("btn_maru_inside");
        objpos = GameObject.Find("Obj_Pos");
        ResultPanel.SetActive(false);
        AtackFlag = false;
        numhp = 0;
        count = 0;
        eCount = 0;
        //Debug.Log(GameObject.Find("GroupMng").GetComponent<GameMng>().MyAttackF);
        //GameObject.Find("GroupMng").GetComponent<GameMng>().CharIDList;
        GameMng g1 = GameMain.GetComponent<GameMng>();
        GameMng g2 = ImgMain.GetComponent<GameMng>();

        //for (int i = 0; i < 10; i++)
        //{
        //    g1.Atkslt[i] = 30;
        //    g1.Atkslt2[i] = 30;
        //}
        for (int i = 0; i < 10; i++)
        {
            Debug.Log(g2.CharIDList[i]);
            if (g2.CharIDList[i] != "str")
            {
                if(i<5)
                {
                    plnum++;
                }
                else
                {
                    eynum++;
                }
            }
        }

        try
        {
            //情報引き継ぎのための初期化
            GameObject.Find("TurnMng").GetComponent<EnemyMove>().Init();
            groupmng = GameObject.Find("GroupMng");
            groupmng.GetComponent<AdaptImgMng>().Init();
            Debug.Log("init");
        }
        catch
        {
            //何もしない
        }
    }


    public void BtnOption()
    {
        if (FadeManager.Instance.isFading)
        {
            return;
        }
        if (objpos.GetComponent<Canvas>().enabled == false)
        {
            //Obj_Posがアクティブ状態でなければリターン
            return;
        }
        if (GetAtackFlag() == true)
        {
            //攻撃選択時はリターン
            return;
        }
        var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().y;
        if (nowpoint == (int)Function.Atack)
        {
            //攻撃
            Debug.Log("攻撃");
            SetAtackFlag(true);

            return;
        }
        //if (nowpoint == (int)Function.Skill)
        //{
        //    //スキル
        //    Debug.Log("スキル");
        //    return;
        //}
        if (nowpoint == (int)Function.Retreat)
        {
            //撤退
            Debug.Log("撤退");
            FadeManager.Instance.LoadLevel("MyTurnScene", 1.0f);
            return;
        }
    }
    public void Atack()
    {
        if (GetAtackFlag() == true)
        {
			var gameOption = marubtn.GetComponent<GameOption>();
            GameMng g1;
            if (groupmng)
            {
                //別シーンから移行したさいはこっちを使う
                g1 = groupmng.GetComponent<GameMng>();
            }
            else
            {
                g1 = GameMain.GetComponent<GameMng>();
            }
            //攻撃時の操作 ○が押されたときに実行
            //現在選択しているオブジェクトの取り出し
            TextMng textMng = TextMain.GetComponent<TextMng>();
            var textWriter = textMng.GetComponent<TextWriter>();
            var textEditer = textMng.GetComponent<TextEdit>();
            count = g1.countsev;
            for (int i = 0; i < plnum; i++)
            {
                g1.PspdSort[i] = g1.GetCharData(i, stateNum.spd);
            }
            Array.Sort(g1.PspdSort);
            int[] sav = new int[5];
            for (int i = plnum-1; i >= 0; i--)
            {
                for (int w = 0; w < plnum; w++)
                {
                    if (g1.PspdSort[i] == g1.GetCharData(w, stateNum.spd))
                    {
                        if (sav[w] != w)
                        {
                            g1.Atkslt[w] = w;
                            sav[w] = w;
                        }
                    }
                }
            }
            if (count < plnum)
            {
                var tmpSeleObj = controllar.GetComponent<SelectedMng>().GetSelectedObj();
                var chara = tmpSeleObj.GetComponent<chara>();
                g1.Atkslt2[count] = chara.charNumber;
                count++;
            }
            if (count == plnum)
            {
                for (int i = 0; i < plnum; i++)
                {
                    if(g1.GetCharData(g1.Atkslt[i], stateNum.hp)>0)
                    {
                        int numatk = g1.GetCharData(g1.Atkslt[i], stateNum.atk);
                        int numdef = g1.GetCharData(g1.Atkslt2[i], stateNum.def);
                        numhp = g1.GetCharData(g1.Atkslt2[i], stateNum.hp);
                        int num = numatk - numdef;
                        int minus = num <= 0 ? 1 : num;
                        numhp = numhp - minus;
                        g1.addData(g1.Atkslt2[i], stateNum.hp, numhp);
                        Debug.Log(numhp);
                        textWriter.AddTextDam(i, minus);
                        textWriter.AddTextAtk(i);
                        //　ログ（味方）
                    }
                }
                EAtack();
                int icnt = 0;
                int ycnt = 0;
                for (int y = 5; y < eynum + 5; y++)
                {
                    if (g1.GetCharData(y, stateNum.hp) <= 0)
                    {
                        ycnt++;
                        if (ycnt == eynum)
                        {
                            ResultPanel.SetActive(true);
                            g1.pWinFlag = true;
                        }
                    }
                }
                textEditer.ChangeFlag();
                textWriter.Check();
                    
                // ログ（敵）

                for (int i = 0; i < plnum; i++)
                {
                    if (i < plnum)
                    {
                        if (g1.GetCharData(i, stateNum.hp) <= 0)
                        {
                            icnt++;
                            if (icnt == plnum)
                            {
                                ResultPanel.SetActive(true);
                            g1.pWinFlag = false;
                        }
                        }
                    }
                }
                count = 0;
                gameOption.SetAtackFlag(false);
            }
            g1.countsev = count;
        }
    }
    public void Click_batu()
    {
        GameMng g1;
        if (groupmng)
        {
            //別シーンから移行したさいはこっちを使う
            g1 = groupmng.GetComponent<GameMng>();
        }
        else
        {
            g1 = GameMain.GetComponent<GameMng>();
        }
        count = g1.countsev;
        Debug.Log(count);
        //行動選択画面へ
        var gameOption = marubtn.GetComponent<GameOption>();
        if (gameOption.GetAtackFlag() == true)
        {
            if (count == 0)
            {
                Debug.Log("atackFlag:false");
                count = 0;
                gameOption.SetAtackFlag(false);
            }
            count--;
            if (count < 0)
            {
                count = 0;
            }
        }
        g1.countsev = count;
    }

    public bool GetAtackFlag()
    {
        return AtackFlag;
    }
    public void SetAtackFlag(bool flag)
    {
        AtackFlag = flag;
    }
    public void EAtack()
    {
        GameMng g1;
        if (groupmng)
        {
            //別シーンから移行したさいはこっちを使う
            g1 = groupmng.GetComponent<GameMng>();
        }
        else
        {
            g1 = GameMain.GetComponent<GameMng>();
        }
        System.Random r1 = new System.Random();
        for (int i = 5; i < eynum + 5; i++)
        {
            g1.EspdSort[i-5] = g1.GetCharData(i, stateNum.spd);
        }
        Array.Sort(g1.EspdSort);
        int[] sav = new int[5];
        for (int i = eynum-1; i >= 0; i--)
        {
            for (int w = 5; w < eynum + 5; w++)
            {
                if (g1.EspdSort[i] == g1.GetCharData(w, stateNum.spd))
                {
                    if (sav[w-5] != w)
                    {
                        g1.EAtkslt[w-5] = w;
                        sav[w-5] = w;
                    }
                }
            }
        }
        for (eCount = 0; eCount < plnum; eCount++)
        {
            if (g1.GetCharData(g1.EAtkslt[eCount], stateNum.hp) > 0)
            {
                int Pnum = r1.Next(0, plnum);
                numhp = g1.GetCharData(Pnum, stateNum.hp);
                int numdef = g1.GetCharData(Pnum, stateNum.def);
                int numatk = g1.GetCharData(g1.EAtkslt[eCount], stateNum.atk);
                int num = numatk - numdef;
                int minus = (num <= 0 ? 1 : num);
                numhp = numhp - minus;
                g1.addData(Pnum, stateNum.hp, numhp);
                Debug.Log(numhp);
            }
        }
    }
}
