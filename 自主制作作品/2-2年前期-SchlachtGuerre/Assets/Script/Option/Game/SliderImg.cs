using UnityEngine;
using System.Collections;
// Imageを使うためにインポート。忘れずに
using UnityEngine.UI;
using System;

public class SliderImg : MonoBehaviour
{
    GameObject GameMain;

    public int xSlidernum;
    public Image m_image;

    int Imgnum = 99;

    int pe = 0;

    void Start()
    {
        GameMain = GameObject.Find("GroupMng");
        GameMng g1 = GameMain.GetComponent<GameMng>();

        for (int i = 0; i < 10; i++)
        {
            Debug.Log(g1.CharIDList[i]);
            if (g1.CharIDList[i] != "str")
            {
                if (i == xSlidernum)
                {
                    Imgnum = i;
                }
            }
        }
        LocalizeJp();
    }
    private void Update()
    {
        if (Imgnum == 99)
        {
            m_image.CrossFadeAlpha(0, 0, true);
        }
    }
    //ローカライズ日本語かそれ以外
    void LocalizeJp()
    {
        GameMng g1 = GameMain.GetComponent<GameMng>();
        if (Imgnum == 99)
        {
            m_image.CrossFadeAlpha(0, 0, true);
        }
    }
}
