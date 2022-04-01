using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class MapCtl : MonoBehaviour
{
    const int MaxMapX = 8;
    const int MaxMapY = 7;

    const int MaxNationalFlag = 13;

    private string MyID = "";   //自分のキー

    //侵攻用一時保存ID
    public KeyValuePair<int, int> tmpMapId = new KeyValuePair<int, int>();

    GameObject groupmng;
    //マップ管理用
    //初期領地 中のstringが領主のキー 
    private string[,] MapID = new string[MaxMapY, MaxMapX]
     {
     { "0","0","0","8","8","8","6","6" },
     { "0","7","7","8","8","8","6","3" },
     { "4","7","7","9","9","9","3","3" },
     { "4","4","4","9","9","9","9","3" },
     { "2","4","4","9","9","9","9","9" },
     { "2","5","1","1","9","9","9","9" },
     { "5","5","5","1","1","1","9","9" }
    };
    //デバッグ用
    //{
    // { "0","0","0","0","0","0","0","0" },
    // { "0","0","0","0","0","0","0","0" },
    // { "0","0","0","0","0","0","0","0" },
    // { "0","0","0","0","0","0","0","0" },
    // { "0","0","0","0","0","0","0","0" },
    // { "0","0","0","0","0","0","0","9" },
    // { "0","0","0","0","0","0","0","8" }
    //};

    //旗まとめ　左から領主のキー、旗の画像
    private Dictionary<string, Sprite> NationalFlagD;

    //旗管理用
    Sprite[] FlagGraphH = new Sprite[MaxNationalFlag];

    public void Start()
    {
        //グループマネージャーの取得
        groupmng = GameObject.Find("GroupMng");

        //旗画像のロード
        for (int i = 0; i < MaxNationalFlag; i++)
        {
            string imgstr = "Flag/" + i.ToString();
            Debug.Log(imgstr);
            FlagGraphH[i] = Resources.Load<Sprite>(imgstr);
            if(FlagGraphH[i]==null)
            {
                Debug.Log(i.ToString() +"番目の画像のロードに失敗");
            }
        }
        //領主のキーと旗画像ハンドルの対応付け
        NationalFlagD = new Dictionary<string, Sprite>
        {
             { "0",FlagGraphH[0] },
             { "1",FlagGraphH[1] },
             { "2",FlagGraphH[2] },
             { "3",FlagGraphH[3] },
             { "4",FlagGraphH[4] },
             { "5",FlagGraphH[5] },
             { "6",FlagGraphH[6] },
             { "7",FlagGraphH[7] },
             { "8",FlagGraphH[8] },
             { "9",FlagGraphH[9] },
             { "10",FlagGraphH[10] },
             { "11",FlagGraphH[11] },
             { "12",FlagGraphH[12] }
        };
        //旗を適応
        AdaptMap();
    }

    public string[,] GetMapID()
    {
        return MapID;
    }
    public bool SetMapID(int x,int y,string setID)
    {
        //範囲内チェック
        if (!(0 <= x && x < MaxMapX))
        {
            Debug.Log("x:" + x.ToString() + "は範囲外です");
            return false;
        }
        if (!(0 <= y && y < MaxMapY))
        {
            Debug.Log("y:" + y.ToString() + "は範囲外です");
            return false;
        }
        //範囲内チェック
        int IntSetId = Convert.ToInt32(setID);
        if (!(0 <= IntSetId && IntSetId < MaxNationalFlag))
        {
            Debug.Log("setID:" + setID + "は範囲外です");
            return false;
        }

        //自領地に接しているか
        bool Flag = false;
        if (0 <= x - 1)
        {
            //左
            if (MapID[y, x - 1] == setID)
            {
                Flag = true;
            }
        }
        if (x + 1 < MaxMapX)
        {
            //右
            if (MapID[y, x + 1] == setID)
            {
                Flag = true;
            }
        }
        if (0 <= y - 1)
        {
            //上
            if (MapID[y - 1, x] == setID)
            {
                Flag = true;
            }
        }
        if (y + 1 < MaxMapY)
        {
            //下
            if (MapID[y + 1, x] == setID)
            {
                Flag = true;
            }
        }

        if (Flag)
        {
            Debug.Log("x:" + x.ToString() + "y:" + y.ToString() + "setID" + setID);
            MapID[y, x] = setID;
        }
        else
        {
            Debug.Log("x: " + x.ToString() + "y: " + y.ToString() + "setID:" + setID + "は自領地に接していません");
        }

        return AdaptMap();
    }

    //Mapデータを画面に適応
    public bool AdaptMap()
    {
        IDictionary<string, bool> masterDictinary = new Dictionary<string, bool>();    //領主リスト存在フラグ
        //消滅した領主主をグループデータから削除
        foreach (var groupdata in groupmng.GetComponent<GroupMng>().GetGroupId())
        {
            masterDictinary.Add(groupdata.Key, false);
        }

        Debug.Log("AdaptMap...");
        //深さ優先検索なので順番に並んでいる
        var mapData = GameObject.Find("Blocks").GetComponentsInChildren<Button>();

        for (int i = 0; i < mapData.Length; i++)
        {
            int idx = i % MaxMapX;
            int idy = (i / MaxMapX) % MaxMapY;
            //Debug.Log(NationalFlagD[MapID[idy, idx]]);
            //画像の変更
            mapData[i].GetComponent<Image>().sprite = NationalFlagD[MapID[idy, idx]];

            //グループデータとの一致を確認
            if (masterDictinary[MapID[idy, idx]] == false)
            {
                masterDictinary[MapID[idy, idx]] = true;
            }
        }

        bool Flag = false;
        //一致していないものの探索
        foreach (var groupdata in masterDictinary)
        {
            if (groupdata.Value == false)
            {
                //一致していないものがある場合削除
                EraseMasterId(groupdata.Key);
                Flag = true;
            }
        }
        if(Flag)
        {
            return true;
        }
        return false;
    }

    public bool AdaptMap(GameObject parentObj)
    {
        IDictionary<string, bool> masterDictinary = new Dictionary<string, bool>();    //領主リスト存在フラグ
        //消滅した領主主をグループデータから削除
        foreach (var groupdata in groupmng.GetComponent<GroupMng>().GetGroupId())
        {
            masterDictinary.Add(groupdata.Key, false);
        }

        Debug.Log("AdaptMap...");
        //深さ優先検索なので順番に並んでいる
        var mapData = parentObj.GetComponentsInChildren<Button>();

        for (int i = 0; i < mapData.Length; i++)
        {
            int idx = i % MaxMapX;
            int idy = (i / MaxMapX) % MaxMapY;
            //Debug.Log(NationalFlagD[MapID[idy, idx]]);
            //画像の変更
            mapData[i].GetComponent<Image>().sprite = NationalFlagD[MapID[idy, idx]];

            //グループデータとの一致を確認
            if (masterDictinary[MapID[idy, idx]] == false)
            {
                masterDictinary[MapID[idy, idx]] = true;
            }
        }

        bool Flag = false;
        //一致していないものの探索
        foreach (var groupdata in masterDictinary)
        {
            if (groupdata.Value == false)
            {
                //一致していないものがある場合削除
                EraseMasterId(groupdata.Key);
                Flag = true;
            }
        }
        if (Flag)
        {
            return true;
        }
        return false;
    }
    public bool CheckClear()
    {
        uint checknum = 100;       //判断用 初期は適当な数字
        bool Flag = true;         //クリアフラグ
        //全領地獲得時クリア画面へ
        foreach(var mapdata in MapID)
        {
            uint mapnum = Convert.ToUInt32(mapdata);
            if (checknum == 100)
            {
                checknum = mapnum;
            }
            if (checknum != mapnum)
            {
                Flag = false;
            }

        }
        if (Flag)
        {
            return true;
        }
        return false;
    }

    public void EraseMasterId(string masterid)
    {
        //グループデータからそのマスターIdを削除
        var groupdata = groupmng.GetComponent<GroupMng>().GetGroupId();
        if(!groupdata.ContainsKey(masterid))
        {
            Debug.Log(masterid + "が存在しません");
            return;
        }
        if(masterid == MyID)
        {
            Debug.Log(masterid + "自身のIDを削除しました");
            return;
        }

        if(!groupdata.Remove(masterid))
        {
            Debug.Log(masterid + "の削除に失敗");
        }
        Debug.Log(masterid + "を削除しました");
    }

    public void SetMyId(string id)
    {
        if (0 <= Convert.ToInt32(id) && Convert.ToInt32(id) < MaxNationalFlag)
        {
            MyID = id;
        }
        //初期資金
        groupmng.GetComponent<GroupMng>().CntUpMoney(GetMyId(), 20);
    }

    public string GetMyId()
    {
        return MyID;
    }

    public int GetMapIdNum(string masterid)
    {
        //領地数の計算
        int count = 0;
        foreach (var mapdata in GetMapID())
        {
            if (mapdata == masterid)
            {
                count++;
            }
        }
        return count;
    }
}
