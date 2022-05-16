using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

//キャラの行動コマンド
public enum command
{
    damage, //攻撃
    kill,   //撃破
    deth,   //死亡    
    item,   //アイテム使用
    win,    //勝利    
    lose,   //敗北
    start,  //バトルシーン開始
    ell
}

//キャラのステータスのenum

public class game : MonoBehaviour
{
    //キャラの行動、ステータス情報
    public command cmd;
    public int hp = 100;

    //他のスクリプトの参照
    public GameObject GameMain;
    chara script;

    //キャラのステータス配列の作成
    public int[,] chara = new int[7, 10];
    //7…キャラ数(とりあえず試作版のキャラ数＋予備で１)
    //10…ステータスの種類数  //この辺defineで定義しておきたい

    // Start is called before the first frame update

    void Start()
    {
        GameMain = GameObject.Find("GameObject");
        script = GameMain.GetComponent<chara>();

        cmd = command.deth;
        //キャラのステータスの初期化
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    //試作
    public void CountUp()
    {
        hp++;
    }
   
    //現在のコマンドの取得
    public command GetCommand()
    {
        return cmd;
    }

    //次コマンドのセット
    public command Setcommand(command state)
    {
        cmd = state;
        return cmd;
    }

    
}
