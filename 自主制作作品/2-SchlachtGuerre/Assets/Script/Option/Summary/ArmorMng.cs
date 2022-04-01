using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ArmorMng : MonoBehaviour
{
    public enum ArmorName
    {
        長剣,
        短剣,
        双剣,
        兜,
        鎧,
        MAX
    };

    public IDictionary<int, List<int>> ArmorMap = new Dictionary<int, List<int>>();

    public IDictionary<int, int> MyArmor = new Dictionary<int, int>();

    private void Start()
    {
        //ステータス分作成
        for (int i = 0; i < (int)ArmorName.MAX; i++)
        {
            ArmorMap.Add(i, new List<int>((int)stateNum.max));
            MyArmor.Add(i, 0);
        }

        //細かいデータの作成
        //長剣
        ArmorMap[0][(int)stateNum.atk] = 5;
        //短剣
        ArmorMap[1][(int)stateNum.atk] = 2;
        ArmorMap[1][(int)stateNum.spd] = 3;
        //双剣
        ArmorMap[2][(int)stateNum.atk] = 4;
        ArmorMap[2][(int)stateNum.spd] = 3;
        ArmorMap[2][(int)stateNum.def] = -3;
        //兜
        ArmorMap[3][(int)stateNum.def] = 2;
        ArmorMap[3][(int)stateNum.mdef] = 5;
        //鎧
        ArmorMap[4][(int)stateNum.def] = 5;
        ArmorMap[4][(int)stateNum.mdef] = 2;
    }

    public List<int> GetAmour(int id)
    {
        return ArmorMap[id];
    }

    public int GetMyAmour(int id)
    {
        return MyArmor[id];
    }

    public void SetCntDownAmour(int id)
    {
        MyArmor[id]--;
    }
}
