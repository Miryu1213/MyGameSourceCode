using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIText : MonoBehaviour
{
    // nameText:喋っている人の名前
    // talkText:喋っている内容やナレーション
    public Text nameText;
    public Text talkText;

    public bool playing = false;
    public bool played = false;
    public float textSpeed = 0.1f;
    string oldText = "";

    void Start() { }

    // クリックで次のページを表示させるための関数
    public bool IsClicked()
    {
        if (Input.GetMouseButtonDown(0)) return true;
        return false;
    }

    // ナレーション用のテキストを生成する関数
    public void DrawText(string text)
    {
        nameText.text = talkText.text;
        StartCoroutine("CoDrawText", text);
    }
    
    // テキストがヌルヌル出てくるためのコルーチン
    IEnumerator CoDrawText(string text)
    {
        //talkText.text = "\n" + oldText;
        playing = true;
        played = playing;
        float time = 0;
        while (true)
        {
            yield return 0;
            time += Time.deltaTime;

            // クリックされると一気に表示
            if (IsClicked()) break;

            int len = Mathf.FloorToInt(time / textSpeed);
            if (len > text.Length) break;
            talkText.text = text.Substring(0, len) + "\n" + oldText;
        }
        //talkText.text = text +"\n"+ talkText.text;
        //oldText = text +"\n" + oldText;

        playing = false;
        yield return 0;

    }
}