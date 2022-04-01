using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine;

public class atkDfImg1 : MonoBehaviour
{
    private Text targetText1;
    GameObject GameMain1;
    // Start is called before the first frame update
    void Start()
    {
        GameMain1 = GameObject.Find("GroupMng");
        GameMng g1 = GameMain1.GetComponent<GameMng>();
        if (g1.MyAttackF)
        {
            this.targetText1 = this.GetComponent<Text>();
            this.targetText1.text = "守軍";
        }
        else
        {
            this.targetText1 = this.GetComponent<Text>();
            this.targetText1.text = "攻軍";
        }
    }
}
