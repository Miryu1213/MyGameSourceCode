using UnityEngine;
using UnityEngine.UI;
using System;
using UnityEngine.SceneManagement;

//管理用クラス
public class SelectedMng : SingletonMonoBehaviour<SelectedMng>
{
    private GameObject selectedObj;     //現在選択中のオブジェクト
    private GameObject OldselectedObj;  //一つ前の選択オブジェクト
    private Vector2Int nowpoint;        //現在の選択バー位置(座標ではない)
    private Vector2Int tmppoint;        //現在選択中のバー位置一時保存用
    void Awake()
    {
        Init();
    }

    public void Init()
    {
        GameObject blocks = GameObject.Find("Blocks");
        //Debug.Log(blocks.transform.Find("Button").gameObject.name);
        try
        {
            selectedObj = blocks.transform.Find("Button").gameObject;
            OldselectedObj = selectedObj;
            if (SceneManager.GetActiveScene().name != "MyTurnScene")
            {
                selectedObj.GetComponent<Image>().color = new Color(100, 0, 10);
            }
        }
        catch
        {
            selectedObj = null;
        }
        finally
        {
            if (selectedObj == null)
            {
                Debug.Log("selectedobj:null");
            }
            else
            {
                nowpoint = new Vector2Int(0, 0);
            }
        }
    }
    public GameObject GetSelectedObj()
    {
        return selectedObj;
    }
    public void SetSelectObj(GameObject gameObject)
    {
        OldselectedObj = selectedObj;
        selectedObj = gameObject;
        if (SceneManager.GetActiveScene().name != "MyTurnScene")
        {
            selectedObj.GetComponent<Image>().color = new Color(100, 0, 10);
        }
        OldselectedObj.GetComponent<Image>().color = Color.white;
    }

    public int GetSeleObjNum()
    {
        //現在選択している場所をint型に変換し返却
        //※加工前のデータ
        string selename = selectedObj.name;
        if (selename == "Button")
        {
            //例外処理
            selename = "0";
        }
        else
        {
            selename = selename.Replace("Button (", "");
            selename = selename.Replace(")", "");
        }
        Debug.Log(selename);
        return Convert.ToInt32(selename);
    }

    public Vector2Int Getnowpoint()
    {
        return nowpoint;
    }
    public void Setnowpoint(Vector2Int num)
    {
        nowpoint = num;
    }

    public Vector2Int GetTmppoint()
    {
        return tmppoint;
    }
    public void SetTmppoint(Vector2Int num)
    {
        tmppoint = num;
    }
}
