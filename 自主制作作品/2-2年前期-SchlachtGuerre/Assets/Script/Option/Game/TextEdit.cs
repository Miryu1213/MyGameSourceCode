using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class TextEdit : MonoBehaviour
{
    GameObject GameMain;
    public GameObject state = null;
    public GameObject state2 = null;
    public GameObject state3 = null;
    public GameObject state4 = null;
    public GameObject name = null;
    public GameObject name2 = null;
    public GameObject hp = null;
    public GameObject mhp = null;
    public GameObject atk = null;
    public GameObject matk = null;
    public GameObject def = null;
    public GameObject mdef = null;
    public GameObject spd = null;
    public GameObject select = null;
    public GameObject srash = null;
    public GameObject selecter  = null;
    public GameObject charFlame = null;
    public GameObject charImage = null;
    public GameObject frame1 = null;
    public GameObject frame2 = null;

    //public GameObject [] state = new GameObject [5];

    //public Tuple<int, int>[] num = new Tuple<int, int>(5, 5)[5];
    public int count = 100;
    int chaNum = 0;
    bool drawFlag_ = true;
    bool EnFlag = false;

    private GameObject groupmng;

    // Start is called before the first frame update
    void Start()
    {
        GameMain = GameObject.Find("GameMng");
        try 
        {
            groupmng = GameObject.Find("GroupMng");
        }
        catch
        {
            //何もしない
        }
    }

    // Update is called once per frame
    void Update()
    {
        //-------------------------------------------------------
        //GameMngの中身の参照の仕方
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
        //------------------------------------------------------
        Text StateText = state.GetComponent<Text>();
        Text StateText2 = state2.GetComponent<Text>();

        Text HpText = hp.GetComponent<Text>();
        Text MhpText = mhp.GetComponent<Text>();
        Text AtkText = atk.GetComponent<Text>();
        Text MatkText = matk.GetComponent<Text>();
        Text DefText = def.GetComponent<Text>();
        Text MdefText = mdef.GetComponent<Text>();
        Text SpdText = spd.GetComponent<Text>();
        Text SrashText = srash.GetComponent<Text>();

        Text Name = name.GetComponent<Text>();
        Text NameText = name2.GetComponent<Text>();
        Text Select = select.GetComponent<Text>();
        Image Selecter = selecter.GetComponent<Image>();
        Image CharFlame = charFlame.GetComponent<Image>();
        Image CharImage = charImage.GetComponent<Image>();
        Image BigFlame = frame1.GetComponent<Image>();
        Image SmallFlame = frame2.GetComponent<Image>();

        Text score_text = state3.GetComponent<Text>();
        score_text.text = "";
        Text score_text2 = state4.GetComponent<Text>();
        score_text2.text = "";


        //ステータス表示用の仮配列
        stateNum[] num = new stateNum[4] {stateNum.hp,stateNum.atk, stateNum.def, stateNum.spd };
        stateNum[] subNum = new stateNum[2] { stateNum.matk, stateNum.mdef };

        if (drawFlag_)
        {
            //ステータス左辺
            foreach (stateNum state_ in num)
            {
                //DrawText(score_text, g1.GetCharData(chaNum, state_) + "\n");
            }

            //ステータス右辺
            foreach (stateNum state_ in subNum)
            {
                //DrawText(score_text2, g1.GetCharData(chaNum, state_) + "\n");
            }
            //drawFlag_ = false;
        }

        if (EnFlag)
        {
            //Name.enabled = !Name.enabled;
            changeEnabled(StateText);
            changeEnabled(StateText2);
            changeEnabled(Name);
            changeEnabled(NameText);
            changeEnabled(Select);
            changeEnabled(score_text);
            changeEnabled(score_text2);

            changeEnabled(HpText);
            changeEnabled(MhpText);
            changeEnabled(AtkText);
            changeEnabled(MatkText);
            changeEnabled(DefText);
            changeEnabled(MdefText);
            changeEnabled(SpdText);
            changeEnabled(SrashText);


            BigFlame.enabled = !BigFlame.enabled;
            SmallFlame.enabled = !SmallFlame.enabled;
            Selecter.enabled = !Selecter.enabled;
            CharFlame.enabled = !CharFlame.enabled;
            CharImage.enabled = !CharImage.enabled;

            EnFlag = !EnFlag;
        }

    }

    public void DrawText(Text Dtext,string text)
    {
        Dtext.text += text;
    }

    public void charPuls()
    {
        chaNum = (chaNum + 1) % 7;
    }

    public void charMinus()
    {
        if(chaNum != 0)
        {
            chaNum = (chaNum - 1) % 7;
        }
        else
        {
            chaNum = 6;
        }
    }

    public void ChangeFlag()
    {
        EnFlag = !EnFlag;
    }

    public void changeEnabled(Text Dtext)
    {
        //表示、非表示の変換
        Dtext.enabled = !Dtext.enabled;
    }
}


