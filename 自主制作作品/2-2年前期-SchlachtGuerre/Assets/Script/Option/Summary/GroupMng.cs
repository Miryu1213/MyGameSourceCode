using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GroupMng : SingletonMonoBehaviour<GroupMng>
{
    const int MaxCharacter = 35;
    const int MaxGroupNumber = 5;       //グループ自体の最大数
    const int MaxNumInGroup = 5;        //グループ内の最大数
    const string InitArrayStr = "str";  //初期配列文字

    //グループ管理用map 引数は左から                  グループの主のID,  グループの番号,  グループ内の番号,  キャラのID
    public IDictionary<string, Dictionary<int, string[]>> groupId = new Dictionary<string, Dictionary<int, string[]>>();
    //領地資金管理用
    private IDictionary<string, int> Money = new Dictionary<string, int>();

    GameObject groupmng;

    string tmpid = "";  //Id一時保存用

    private void Start()
    {
        groupmng = GameObject.Find("GroupMng");
        //グループ系の初期化
        //マスターID(0～12)
        foreach (var masterid in groupmng.GetComponent<MapCtl>().GetMapID())
        {
            if (groupId.ContainsKey(masterid))
            {
                //そのマスターデータがすでに入っている場合
                continue;
            }
            //グループ自体（5個)
            for (int j = 0; j < MaxGroupNumber; j++)
            {
                InitSetCharaToGroup(masterid, j, InitArrayStr);
            }
        }
        //初期化済みグループIDに本データを代入
        SetCharaToGroup("0", 0, "0");
        SetCharaToGroup("0", 0, "11");
        SetCharaToGroup("0", 0, "29");

        SetCharaToGroup("1", 0, "1");
        SetCharaToGroup("1", 0, "33");
        SetCharaToGroup("1", 0, "25");

        SetCharaToGroup("2", 0, "2");
        SetCharaToGroup("2", 0, "21");
        SetCharaToGroup("2", 0, "17");

        SetCharaToGroup("3", 0, "3");
        SetCharaToGroup("3", 0, "12");
        SetCharaToGroup("3", 0, "13");

        SetCharaToGroup("4", 0, "4");
        SetCharaToGroup("4", 0, "15");

        SetCharaToGroup("5", 0, "5");
        SetCharaToGroup("5", 0, "28");
        SetCharaToGroup("5", 0, "30");

        SetCharaToGroup("6", 0, "6");
        SetCharaToGroup("6", 0, "10");

        SetCharaToGroup("7", 0, "7");

        SetCharaToGroup("8", 0, "8");

        SetCharaToGroup("9", 0, "9");
        SetCharaToGroup("9", 0, "27");
        SetCharaToGroup("9", 0, "32");
        SetCharaToGroup("9", 0, "26");

        SetCharaToGroup("9", 1, "22");
        SetCharaToGroup("9", 1, "23");
        SetCharaToGroup("9", 1, "24");

        //SetCharaToGroup("9", 0, "13");
        //SetCharaToGroup("9", 0, "14");

        WatchGroupid();

        //資金系の初期化
        foreach (var masterdata in groupId)
        {
            Money.Add(masterdata.Key, 0);
        }
    }

    //初期化用キャラセット
    public void InitSetCharaToGroup(string masterid, int groupnumber, string charaid)
    {
        //特定のキャラを部隊へ
        if (MaxGroupNumber <= groupnumber)
        {
            Debug.Log("groupnumberが軍団最大数を超えています");
            return;
        }
        //masteridの代入
        if (!groupId.ContainsKey(masterid))
        {
            //そのマスターキーがなかった場合
            groupId.Add(masterid, new Dictionary<int, string[]>());
        }

        //グループidの代入
        if (!groupId[masterid].ContainsKey(groupnumber))
        {
            //そのグループナンバーがなかった場合         
            groupId[masterid].Add(groupnumber, new string[5] { InitArrayStr, InitArrayStr, InitArrayStr, InitArrayStr, InitArrayStr });
        }
        else
        {
            groupId[masterid][groupnumber] = new string[5] { InitArrayStr, InitArrayStr, InitArrayStr, InitArrayStr, InitArrayStr };
        }
    }

    //キャラクターを部隊にセット
    public bool SetCharaToGroup(string masterid, int groupnumber,int groupInnumber, string charaid)
    {
        //特定のキャラを部隊へ
        if (!groupId.ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
            return false;
        }
        if (MaxGroupNumber <= groupnumber)
        {
            Debug.Log("groupnumberが軍団最大数を超えています");
            return false;
        }
        if (MaxNumInGroup <= groupInnumber)
        {
            Debug.Log("groupnumberが軍団最大数を超えています");
            return false;
        }
        if (!groupId[masterid].ContainsKey(groupnumber))
        {
            Debug.Log(groupnumber + "のキーが存在しません");
        }

        //セットするIDが使用されていないか
        //var NonList = SearchNnId();
        //bool Flag = false;
        //foreach (var NonId in NonList)
        //{
        //    if (NonId == charaid)
        //    {
        //        //一致の確認
        //        Flag = true;
        //    }
        //}
        //if (!Flag)
        //{
        //    //すでに使用されているIDのとき
        //    Debug.Log("使用されているIDです");
        //    return false;
        //}

        groupId[masterid][groupnumber][groupInnumber] = charaid;

        return true;
    }

    //キャラクターを部隊にセット（一番後ろに）
    public bool SetCharaToGroup(string masterid, int groupnumber, string charaid)
    {
        //特定のキャラを部隊へ
        if (!groupId.ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
            return false;
        }
        if (MaxGroupNumber <= groupnumber)
        {
            Debug.Log("groupnumberが軍団最大数を超えています");
            return false;
        }
        if (!groupId[masterid].ContainsKey(groupnumber))
        {
            Debug.Log(groupnumber + "のキーが存在しません");
        }

        //セットするIDが使用されていないか
        var NonList = SearchNnId();
        bool Flag = false;
        foreach (var NonId in NonList)
        {
            if (NonId == charaid)
            {
                //一致の確認
                Flag = true;
            }
        }
        if (!Flag)
        {
            //すでに使用されているIDのとき
            Debug.Log("使用されているIDです");
            return false;
        }

        for (int id = 0; id < MaxNumInGroup; id++)
        {
            //空白個所にキャラIDを入れる
            if (groupId[masterid][groupnumber][id] == InitArrayStr)
            {
                if (groupId[masterid][groupnumber][id] != charaid)
                {
                    groupId[masterid][groupnumber][id] = charaid;
                    Debug.Log("masterid" + masterid + "groupnumber" + groupnumber.ToString() + "id" + id.ToString() + "charaid" + charaid);
                }

                return true;
            }
        }
        return false;
    }

    //グループ全体の取得
    public IDictionary<string, Dictionary<int, string[]>> GetGroupId()
    {
        return groupId;
    }

    //グループの部隊全体の取得 失敗した場合null
    public string[] GetCharaFromGroup(string masterid, int groupnumber)
    {
        if (!groupId.ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
            string[] tmpstr = new string[5];
            return tmpstr;
        }
        if (MaxGroupNumber <= groupnumber)
        {
            Debug.Log("groupnumberが軍団最大数を超えています");
            string[] tmpstr = new string[5];
            return tmpstr;
        }

        return groupId[masterid][groupnumber];
    }

    //領主のグループを取得
    public Dictionary<int, string[]> GetCharaGroup(string masterid)
    {
        if (!groupId.ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
            Dictionary<int, string[]> tmpstr = new Dictionary<int, string[]>();
            return tmpstr;
        }

        return groupId[masterid];
    }

    //グループのうちの一体の取得 失敗した場合空白
    public string GetOneCharaFromGroup(string masterid, int groupnumber, int number)
    {
        if (!groupId.ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
            return "";
        }
        if (MaxGroupNumber <= groupnumber)
        {
            Debug.Log("groupnumberが軍団最大数を超えています");
            return "";
        }
        if (MaxNumInGroup <= number)
        {
            Debug.Log("numberが軍団内最大人数を超えています");
            return "";
        }
        return groupId[masterid][groupnumber][number];
    }

    //デバッグ用表示
    public void WatchGroupid()
    {
        foreach (var masterid in groupId)
        {
            foreach (var tmpgroupid in masterid.Value)
            {
                foreach (var idingroup in tmpgroupid.Value)
                {
                    Debug.Log("masterid:" + masterid.Key + "GroupNumber" + tmpgroupid.Key.ToString() + "GroupInNumber" + idingroup);
                }
            }
        }
    }

    public void WatchMasterid()
    {
        foreach (var masterid in groupId)
        {
            Debug.Log(masterid.Key);
        }
    }

    //一つだけの領地の資金の取得
    public int GetOneMoney(string masterid)
    {
        if (!groupId.ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
        }
        return Money[masterid];
    }

    //使っていないキャラIDの検索
    public List<string> SearchNnId()
    {
        //最初に全部入ったリストを用意し、一致したものを削除していく
        List<string> checklist = new List<string>();
        for (int i = 0; i < MaxCharacter; i++)
        {
            checklist.Add(i.ToString());
        }

        foreach (var masterid in GetGroupId())
        {
            foreach (var tmpgroupid in masterid.Value)
            {
                foreach (var idingroup in tmpgroupid.Value)
                {
                    checklist.Remove(idingroup);
                }
            }
        }

        return checklist;
    }

    public string[] GetMostHPinGroup(string masterid)
    {
        //軍団の合計体力が一番高いものを返す
        GameMng gamemng = groupmng.GetComponent<GameMng>();
        var group = GetGroupId()[masterid];
        string[] MaxHpGroup = group[0];
        int MaxTotalHp = 0;
        for (int i = 0; i < MaxGroupNumber; i++)
        {
            int totalHP = 0;
            for (int j = 0; j < MaxNumInGroup; j++)
            {
                totalHP += gamemng.GetCharaData(group[i][j], stateNum.hp);
            }
            if (totalHP > MaxTotalHp)
            {
                MaxTotalHp = totalHP;
                MaxHpGroup = group[i];
            }
        }
        return MaxHpGroup;
    }

    public string[] GetMostTotalinGroup(string masterid)
    {
        //軍団の総戦力が一番高いものを返す
        GameMng gamemng = groupmng.GetComponent<GameMng>();
        var group = GetGroupId()[masterid];
        string[] MaxTotalGroup = group[0];
        int MaxTotal = 0;
        for (int i = 0; i < MaxGroupNumber; i++)
        {
            int total = 0;
            for (int j = 0; j < MaxNumInGroup; j++)
            {
                for (int n = 0; n < (int)stateNum.max; n++)
                {
                    total += gamemng.GetCharaData(group[i][j], (stateNum)n);
                }
            }
            if (total > MaxTotal)
            {
                MaxTotal = total;
                MaxTotalGroup = group[i];
            }
        }
        return MaxTotalGroup;
    }

    public int GetTotalinGroup(string masterid)
    {
        //軍団の総戦力が合計を返す
        GameMng gamemng = groupmng.GetComponent<GameMng>();
        var group = GetGroupId()[masterid];
        int total = 0;
        for (int i = 0; i < MaxGroupNumber; i++)
        {
            for (int j = 0; j < MaxNumInGroup; j++)
            {
                for (int n = 0; n < (int)stateNum.max; n++)
                {
                    total += gamemng.GetCharaData(group[i][j], (stateNum)n);
                }
            }
        }
        return total;
    }

    public void HealHpInAllGroup()
    {
        //全てのHpを回復
        GameMng gamemng = groupmng.GetComponent<GameMng>();
        for (int i = 0; i < MaxCharacter; i++)
        {
            var charahp = gamemng.GetCharaData(i.ToString(), stateNum.hp);
            var charamaxhp = gamemng.GetCharaData(i.ToString(), stateNum.maxhp);
            if (charahp + 20 > charamaxhp)
            {
                //マックスのとき
                gamemng.SetCharaData(i.ToString(), stateNum.hp, charamaxhp);
            }
            else
            {
                //通常
                gamemng.SetCharaData(i.ToString(), stateNum.hp, charahp + 20);
            }
            
        }
    }

    public void HealHpInAllGroupByMinus()
    {
        //全てのHpを回復
        GameMng gamemng = groupmng.GetComponent<GameMng>();
        for (int i = 0; i < MaxCharacter; i++)
        {
            var charahp = gamemng.GetCharaData(i.ToString(), stateNum.hp);
            var charamaxhp = gamemng.GetCharaData(i.ToString(), stateNum.maxhp);
            if (charahp <= 0)
            {
                //マックスのとき
                gamemng.SetCharaData(i.ToString(), stateNum.hp, 20);
            }
        }
    }

    //ターン開始時の資金追加
    public void CntUpMoney(string masterid)
    {
        if (!groupId.ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
            return;
        }
        //資金のカウントアップ
        //自領地数の計算
        int count = groupmng.GetComponent<MapCtl>().GetMapIdNum(masterid);
        
        Money[masterid] += 10 + count;
    }

    public void CntUpMoney(string masterid,int UpCount)
    {
        if (!groupId.ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
            return;
        }
        //資金のカウントアップ
        Money[masterid] += UpCount;
    }

    public void CntDownMoney(string masterid,int DownMoney)
    {
        if (!groupId.ContainsKey(masterid))
        {
            Debug.Log(masterid + "のIDが存在しません");
            return;
        }
        Money[masterid] -= DownMoney;
    }
    public void SetTmpId(string str)
    {
        tmpid = str;
    }
    public string GetTmpId()
    {
        return tmpid;
    }
}