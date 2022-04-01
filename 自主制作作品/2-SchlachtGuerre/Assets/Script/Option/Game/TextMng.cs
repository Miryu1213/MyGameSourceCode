using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class TextMng : MonoBehaviour
{
    GameObject GameMain;

    Dictionary<string,string> textTable = new Dictionary<string, string>()
        {
          {"atk", "{0}の攻撃"},
          {"atk2", "{0}の攻撃"},
          {"dam2", "{0]に{1]ダメージ与えた"},
          {"dam", "{0}は{1}ダメージ与えた"},
          {"deth", "{0}は戦闘不能となった"},
          {"kill", "{0}を倒した"},
          {"skill", "使えるスキルがない！"},
          {"win","戦いに勝利した。"},
          {"lose" ,"戦いに敗北した。"},
          {"start","戦闘開始。" },
          {"ell","" }
        };

    buttleCmd btlCmd;

    // Start is called before the first frame update
    void Start()
    {
        GameMain = GameObject.Find("GameMng");
        GameMng g1 = GameMain.GetComponent<GameMng>();

        //Cmdの切り替え(各所に埋め込んでAddしていく形)
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public string CheckCmd(buttleCmd cmd)
    {
        string cmdName =  cmd.ToString();
        return textTable[cmdName];
    }
}
