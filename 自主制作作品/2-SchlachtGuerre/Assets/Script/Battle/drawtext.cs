using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class drawtext : MonoBehaviour
{
    GameObject GameMain;
    game script;

    int[] i;
    int hp;
    public GameObject text_object = null;
    command state;

    // Start is called before the first frame update
    void Start()
    {
        GameMain = GameObject.Find("GameObject");
        script = GameMain.GetComponent<game>();
    }

    // Update is called once per frame
    void Update()
    {
        //表示テキストの設定をする
        //SetCommand(command.deth);
        Text score_text = text_object.GetComponent<Text>();
        script.CountUp();
        
        hp = script.hp;
        state = script.GetCommand();

        //表示したいテキスト
        DrawText(score_text, state);
    }

    public command GetCommand()
    {
        return state;
    }

    public void DrawText(Text Dtext,command state)
    {
        switch (state)
        {
            case command.damage:
                Dtext.text = "\n";
                break;
            case command.kill:
                Dtext.text = "兵士Aを倒した\n";
                break;
            case command.deth:
                Dtext.text = "兵士Aの体力が" + script.chara[2, 4] + "になった\n" + "兵士Aは戦闘不能となった\n";
                break;
            case command.item:
                Dtext.text = "兵士Aは薬草を使った\n" + "兵士Aの体力が３０回復した\n";
                break;
            case command.win:
                Dtext.text = "敵軍Aとの勝負に勝った\n" + "兵士たちは経験値20を手に入れた\n" + "兵士Cは勲章を手に入れた\n";
                break;
            case command.lose:
                Dtext.text = "敵軍Aとの勝負に負けた\n";
                break;
            case command.start:
                Dtext.text = "戦闘開始!\n";
                break;
            default:
                script.Setcommand(command.ell);
                Dtext.text = "\n";
                break;
        }
    }


}
