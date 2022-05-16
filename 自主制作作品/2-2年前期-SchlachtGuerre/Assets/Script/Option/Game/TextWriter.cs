using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class TextWriter : MonoBehaviour
{
    // Start is called before the first frame update
    public UIText uitext;
    public GameObject textFront = null;
    GameObject txtMng;
    buttleCmd cmd = buttleCmd.atk;
    string TextA;
    void Start()
    {
        //textMng = GetComponent<TextMng>();
    }

    public void Check()
    {
        Text FrontText = textFront.GetComponent<Text>();
        FrontText.enabled = true;
        uitext.DrawText(TextA);
        StartCoroutine("Cotest");

    }

    public void AddTextAtk(int num)
    {
        txtMng = GameObject.Find("TextMng");
        TextMng textMng = txtMng.GetComponent<TextMng>();
        uitext.played = true;
        string NumA = string.Format("キャラクター{0}", num);
        TextA = string.Format(textMng.CheckCmd(buttleCmd.atk), NumA)  + "\n" + TextA;
    }

    public void AddTextDam(int num,int num2)
    {
        txtMng = GameObject.Find("TextMng");
        TextMng textMng = txtMng.GetComponent<TextMng>();
        uitext.played = true;
        string NumA = string.Format("キャラクター{0}", num);
        string NumB = string.Format("{0}", num2);
        TextA = string.Format(textMng.CheckCmd(buttleCmd.dam), NumA,NumB) + "\n" + TextA;
    }

    public void AddTextSkill(int num)
    {
        txtMng = GameObject.Find("TextMng");
        TextMng textMng = txtMng.GetComponent<TextMng>();
        uitext.played = true;
        //string NumA = string.Format("キャラクター{0}", num);
        //TextA = string.Format(textMng.CheckCmd(cmd), NumA) + "\n" + TextA;
    }

    public void AddTextDeth(int num)
    {
        txtMng = GameObject.Find("TextMng");
        TextMng textMng = txtMng.GetComponent<TextMng>();
        uitext.played = true;
        //string NumA = string.Format("キャラクター{0}", num);
        //TextA = string.Format(textMng.CheckCmd(cmd), NumA) + "\n" + TextA;
    }

    public void AddTextWin(int num)
    {
        txtMng = GameObject.Find("TextMng");
        TextMng textMng = txtMng.GetComponent<TextMng>();
        uitext.played = true;
        //string NumA = string.Format("キャラクター{0}", num);
        //TextA = string.Format(textMng.CheckCmd(cmd), NumA) + "\n" + TextA;
    }

    public void AddTextLose(int num)
    {
        txtMng = GameObject.Find("TextMng");
        TextMng textMng = txtMng.GetComponent<TextMng>();
        uitext.played = true;
        //string NumA = string.Format("キャラクター{0}", num);
        //TextA = string.Format(textMng.CheckCmd(cmd), NumA) + "\n" + TextA;
    }

    public void AddTextKill(int num)
    {
        txtMng = GameObject.Find("TextMng");
        TextMng textMng = txtMng.GetComponent<TextMng>();
        uitext.played = true;
    }

    public void AddTextStart(int num)
    {
        txtMng = GameObject.Find("TextMng");
        TextMng textMng = txtMng.GetComponent<TextMng>();
        uitext.played = true;
        //string NumA = string.Format("キャラクター{0}", num);
        //TextA = string.Format(textMng.CheckCmd(cmd), NumA) + "\n" + TextA;
    }

    public void DeleteText()
    {
        Text FrontText = textFront.GetComponent<Text>();
        uitext.played = false;
        FrontText.enabled = false;
        TextA = "";
    }

    public bool Playing()
    {
        return uitext.playing;
    }

    public bool Played()
    {
        return uitext.played;
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    // クリック待ちのコルーチン
    IEnumerator Skip()
    {
        while (uitext.playing) yield return 0;
        while (!uitext.IsClicked()) yield return 0;
    }

    // 文章を表示させるコルーチン
    IEnumerator Cotest()
    {
        yield return StartCoroutine("Skip");
    }
}
