using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class chara : MonoBehaviour
{

    public GameObject GameMain;
    game script;

    public int charNumber = 0;

    // Start is called before the first frame update
    void Start()
    {
        GameMain = GameObject.Find("GameObject");
        script = GameMain.GetComponent<game>();
    }

    // Update is called once per frame
    void Update()
    {

    }

    public int GetCharID()
    {
        return charNumber;
    }

    public void SetCharID(int id)
    {
        charNumber = id;
        Debug.Log(charNumber);
    }

}
