using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine;

public class atkDfImg : MonoBehaviour
{
    private Text targetText;
    GameObject GameMain;
    // Start is called before the first frame update
    void Start()
    {
        GameMain = GameObject.Find("GroupMng");
        GameMng g1 = GameMain.GetComponent<GameMng>();
        if(g1.MyAttackF)
        {
            this.targetText = this.GetComponent<Text>();
            this.targetText.text = "攻軍";
        }
        else
        {
            this.targetText = this.GetComponent<Text>();
            this.targetText.text = "守軍";
        }
    }
}
