using UnityEngine;
using System.Collections;
using UnityEngine.UI; // ←※これを忘れずに入れる

public class HpBarCtrl : MonoBehaviour
{
    //　追加する際はキャラナンバーを追加すること
    GameObject GameMain;
    GameObject groupmng;
    public Slider[] slider = new Slider[10];

    private void Start()
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
        finally
        {
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
            for (int i = 0; i < 10; i++)
            {
                slider[i].maxValue = g1.GetCharData(i, stateNum.maxhp);
                slider[i].value = g1.GetCharData(i, stateNum.hp);
                Debug.Log(g1.GetCharData(i, stateNum.maxhp));
            }
        }
    }
    void Update()
    {
        try
        {
            groupmng = GameObject.Find("GroupMng");
        }
        catch
        {
            //何もしない
        }
        finally
        {
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
            for (int i = 0; i < 10; i++)
            {
                slider[i].value = g1.GetCharData(i, stateNum.hp);
            }
        }
    }
}
