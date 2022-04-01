using UnityEngine;
using System.Collections;
// Imageを使うためにインポート。忘れずに
using UnityEngine.UI;
using System;

public class CharImage : MonoBehaviour
{
    GameObject GameMain;
    Button xButton;

    int Imgnum = 99;

    int pe = 0;

    // 画像を動的に変えたいボタンの宣言
    public Sprite[] Imgchar = new Sprite[34];

    SpriteState xState = new SpriteState();

    void Awake()
    {
        xButton = this.GetComponent<Button>();
    }

    void Start()
    {
        GameMain = GameObject.Find("GroupMng");
        GameMng g1 = GameMain.GetComponent<GameMng>();
        xState = xButton.spriteState;
        Debug.Log(g1.CharIDList[1]);
        var chara = xButton.GetComponent<chara>();

        for (int i = 0; i < 10; i++)
        {
            Debug.Log(g1.CharIDList[i]);
            if (g1.CharIDList[i] != "str")
            {
                if (i == chara.charNumber)
                {
                    Imgnum = Int32.Parse(g1.CharIDList[i]);
                }
            }
        }
        LocalizeJp();
    }
    private void Update()
    {
        if (Imgnum == 99)
        {
            xButton.GetComponent<Image>().color = new Color(0, 0, 0, 0);
        }
    }
    //ローカライズ日本語かそれ以外
    void LocalizeJp()
    {
        GameMng g1 = GameMain.GetComponent<GameMng>();
        if(Imgnum == 99)
        {
            xButton.GetComponent<Image>().sprite = Imgchar[0];
            xButton.GetComponent<Image>().color = new Color(0, 0, 0, 0);
        }
        else
        {
            xButton.GetComponent<Image>().sprite = Imgchar[Imgnum];
        }
       

        xState.pressedSprite = Imgchar[0];
        xButton.spriteState = xState;
        xState.pressedSprite = Imgchar[0];
        xButton.spriteState = xState;

    }
}

