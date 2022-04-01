using UnityEngine;

public class TitleOption : MonoBehaviour
{
    const int GameStart_NO = 0; //スタート
    const int Manual_NO = 1;    //マニュアル
    const int Credit_NO = 2;    //クレジット
    const int End_NO = 3;       //終了

    private GameObject controllar;
    private GameObject startobj;    //最初の描画オブジェクト
    private GameObject titleobj;    //選択画面でのオブジェクト

    private GameObject manual;      //マニュアル
    private GameObject credit;      //クレジット

    private AudioSource MaruSE;            //選択SE

    private void Start()
    {
        //SE取得
        MaruSE = GameObject.Find("ClickMaru").GetComponent<AudioSource>();

        controllar = GameObject.Find("Controllar");

        startobj = GameObject.Find("StartObj");
        titleobj = GameObject.Find("Obj_Pos");
        titleobj.GetComponent<Canvas>().enabled = false;

        manual = GameObject.Find("Manual");
        manual.GetComponent<Canvas>().enabled = false;
        credit = GameObject.Find("Credit");
        credit.GetComponent<Canvas>().enabled = false;
    }
    public void ClickOption()
    {
        if(FadeManager.Instance.isFading)
        {
            return;
        }
             
        //SEの再生
        MaruSE.PlayOneShot(MaruSE.clip);

        if (titleobj.GetComponent<Canvas>().enabled == false || manual.GetComponent<Canvas>().enabled || credit.GetComponent<Canvas>().enabled)
        {
            //Obj_Posがアクティブ状態でなければリターン
            return;
        }
        var nowpoint = controllar.GetComponent<SelectedMng>().Getnowpoint().y;
        if (nowpoint == GameStart_NO)
        {
            //ゲームスタート
            FadeManager.Instance.LoadLevel("SelectScene", 1.0f);
            return;
        }
        if (nowpoint == Manual_NO)
        {
            //マニュアル
            manual.GetComponent<Canvas>().enabled = true;
            return;
        }
        if (nowpoint == Credit_NO)
        {
            //クレジット
            credit.GetComponent<Canvas>().enabled = true;
            return;
        }
        if (nowpoint == End_NO)
        {
            //ゲーム終了
#if UNITY_EDITOR
            //UnityEditorのとき
            UnityEditor.EditorApplication.isPlaying = false;
#elif UNITY_STANDALONE
            //アプリケーションのとき
             UnityEngine.Application.Quit();
#endif
            return;
        }
        else
        {
            Debug.Log("Nowpointの値が不正です");
            return;
        }
    }
    public void titlestart()
    {
        if (startobj.activeInHierarchy)
        {
            //最初の画面
            //○が押されたとき
            startobj.SetActive(false);
            titleobj.GetComponent<Canvas>().enabled = true;
        }
        else
        {
            //選択画面
            //何もしない
        }
    }

    public void ClickBatu()
    {
        if (manual.GetComponent<Canvas>().enabled)
        {
            manual.GetComponent<Canvas>().enabled = false;
        }
        if (credit.GetComponent<Canvas>().enabled)
        {
            credit.GetComponent<Canvas>().enabled = false;
        }
    }
}
