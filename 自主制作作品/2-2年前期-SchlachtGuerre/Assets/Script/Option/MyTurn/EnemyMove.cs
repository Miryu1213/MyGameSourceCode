using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class EnemyMove : SingletonMonoBehaviour<EnemyMove>
{
    const int MaxMapX = 8;
    const int MaxMapY = 7;

    private GameObject Groupmng;
    private GroupMng groupmng;      //グループマネージャー内スクリプト
    private MapCtl mapctl;          //グループマネージャー内マップ系スクリプト
    private AdaptImgMng adaptmng;   //グループマネージャー内画像貼り付け系スクリプト

    private GameObject textbar;
    private GameObject blocks;

    private string tmpmasterid = ""; //マスターID一時保存用
    public KeyValuePair<int, int> tmpMapId = new KeyValuePair<int, int>();  //引継ぎ用マップID一時保存 

    bool IsGameScene = false;        //現在のシーンがゲームシーンか
    //敵の動き
    public void Start()
    {
        Init();
    }

    public void Init()
    {
        //グループマネージャーの取得
        Groupmng = GameObject.Find("GroupMng");
        groupmng = Groupmng.GetComponent<GroupMng>();
        mapctl = Groupmng.GetComponent<MapCtl>();
        adaptmng = Groupmng.GetComponent<AdaptImgMng>();

        textbar = GameObject.Find("TextBar");
        blocks = GameObject.Find("Blocks");
        Debug.Log(tmpmasterid);
    }

    //返り値  first…侵攻相手が自分か否か second…成功か否か
    public KeyValuePair<bool,bool> Move(string masterid, bool flag)
    {
        if (!groupmng.GetGroupId().ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
            return new KeyValuePair<bool, bool>(false, false);
        }

        //マスターIDの一時保存
        tmpmasterid = masterid;
        Debug.Log(tmpmasterid);

        //資金の増加
        groupmng.CntUpMoney(masterid);

        //マップの取得
        var mapdata = mapctl.GetMapID();

        var group = groupmng.GetGroupId()[masterid];
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                var charaid = group[i][j];
                var charaexp = Groupmng.GetComponent<GameMng>().GetCharaData(charaid, stateNum.exp);
                System.Random rand = new System.Random();
                var point = rand.Next(10);
                Groupmng.GetComponent<GameMng>().SetCharaData(charaid, stateNum.exp, charaexp + 35 + point);
            }
        }

        //ここに敵の行動を書く
        //周囲に他領土があったら攻める。
        List<KeyValuePair<int, int>> atacklist = new List<KeyValuePair<int, int>>();  //攻めれる場所リスト
        for (var i = 0; i < mapdata.Length; i++)
        {
            //IDの計算
            int x = i % MaxMapX;
            int y = (i / MaxMapX) % MaxMapY;
            //自領地に接しているか
            
            if (0 <= x - 1)
            {
                //左
                if (mapdata[y, x - 1] == masterid)
                {
                    //その場所が自分の場合は省く
                    if (mapdata[y, x] != masterid)
                    {
                        atacklist.Add(new KeyValuePair<int, int>(y, x));
                    }
                }
            }
            if (x + 1 < MaxMapX)
            {
                //右
                if (mapdata[y, x + 1] == masterid)
                {
                    //その場所が自分の場合は省く
                    if (mapdata[y, x] != masterid)
                    {
                        atacklist.Add(new KeyValuePair<int, int>(y, x));
                    }
                }
            }
            if (0 <= y - 1)
            {
                //上
                if (mapdata[y - 1, x] == masterid)
                {
                    //その場所が自分の場合は省く
                    if (mapdata[y, x] != masterid)
                    {
                        atacklist.Add(new KeyValuePair<int, int>(y, x));
                    }
                }
            }
            if (y + 1 < MaxMapY)
            {
                //下
                if (mapdata[y + 1, x] == masterid)
                {
                    //その場所が自分の場合は省く
                    if (mapdata[y, x] != masterid)
                    {
                        atacklist.Add(new KeyValuePair<int, int>(y, x));
                    }
                }
            }
        }

        if (0 < atacklist.Count)
        {
            //攻撃箇所の選択
            System.Random rand = new System.Random();
            var point = rand.Next(atacklist.Count);
            //Debug.Log(masterid);
            //Debug.Log(atacklist[point].Value);
            //Debug.Log(atacklist[point].Key);
            Debug.Log("侵攻");

            //プレイヤーへの進行
            if (mapctl.GetMapID()[atacklist[point].Key, atacklist[point].Value] == mapctl.GetMyId())
            {
                if (flag == true)
                {
                    return new KeyValuePair<bool, bool>(false, true);
                }

                Debug.Log("侵攻 敵から味方");
                textbar.GetComponent<Text>().text = "侵攻されました！";

                //侵攻箇所を赤く
                var y = atacklist[point].Key;
                var x = atacklist[point].Value;
                var num = y * MaxMapX + x;
                if (num == 0)
                {
                    //例外処理
                    blocks.transform.Find("Button").gameObject.GetComponent<Image>().color = Color.red;
                }
                else
                {
                    blocks.transform.Find("Button (" + num.ToString() + ")").gameObject.GetComponent<Image>().color = Color.red;
                }

                //var test = groupmng.GetComponent<MapCtl>().GetMapID();
                //Debug.Log(test);

                //場所の一時保存
                tmpMapId = atacklist[point];

                //mapctl.SetMapID(atacklist[point].Value, atacklist[point].Key, masterid);

                //ゲームシーンへ
                IsGameScene = true;
                //侵攻されたとき
                return new KeyValuePair<bool, bool>(true, true);
            }
            else
            {
                Debug.Log("侵攻 敵から敵");

                mapctl.SetMapID(atacklist[point].Value, atacklist[point].Key, masterid);
                //var tmpobj = GameObject.Find("EneVsEne").transform.Find("vs").Find("SumName").gameObject;
                //adaptmng.AdaptVs(tmpobj, masterid, mapctl.GetMapID()[atacklist[point].Key, atacklist[point].Value]);
                return new KeyValuePair<bool, bool>(false, true);
            }
        }
        else
        {
            Debug.Log("攻撃箇所がありません");
        }

        //敵の侵攻
        Debug.Log("Move...");

        return new KeyValuePair<bool, bool>(false, false);
    }

    public string GetTmpMasterId()
    {
        return tmpmasterid;
    }
    public void SetTmpMasterId(string str)
    {
        tmpmasterid = str;
    }

    public bool GetIsGameScene()
    {
        return IsGameScene;
    }
    public void SetIsGameScene(bool flag)
    {
        IsGameScene = flag;
    }
}
