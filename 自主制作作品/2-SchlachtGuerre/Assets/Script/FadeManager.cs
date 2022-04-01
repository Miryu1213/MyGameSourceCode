using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;

/// <summary>
/// シーン遷移時のフェードイン・アウトを制御するためのクラス
/// </summary>
public class FadeManager : SingletonMonoBehaviour<FadeManager>
{
    /// <summary>暗転用黒テクスチャ</summary>
    private Texture2D blackTexture;
    /// <summary>フェード中の透明度</summary>
    private float fadeAlpha = 0;
    /// <summary>フェード中かどうか</summary>
    public bool isFading = false;

    //マップシーン用進行データの引継ぎ
    private bool aggressionflag;
    //ゲームシーン用キャラIDの引継ぎ
    public string[] arraycharaid = new string[10];
    //前のシーンの名前
    public string BeforeScene;
    public void Awake()
    {
        if (this != Instance)
        {
            Destroy(this);
            return;
        }

        DontDestroyOnLoad(this.gameObject);

        //ここで黒テクスチャ作る
        this.blackTexture = new Texture2D(32, 32, TextureFormat.RGB24, false);
        this.blackTexture.ReadPixels(new Rect(0, 0, 32, 32), 0, 0, false);
        this.blackTexture.SetPixel(0, 0, Color.white);
        this.blackTexture.Apply();
    }

    public void OnGUI()
    {
        if (!this.isFading)
            return;

        //透明度を更新して黒テクスチャを描画
        GUI.color = new Color(0, 0, 0, this.fadeAlpha);
        GUI.DrawTexture(new Rect(0, 0, Screen.width, Screen.height), this.blackTexture);
    }

    /// <summary>
    /// 画面遷移
    /// </summary>
    /// <param name='scene'>シーン名</param>
    /// <param name='interval'>暗転にかかる時間(秒)</param>
    public void LoadLevel(string scene, float interval)
    {
        StartCoroutine(TransScene(scene, interval));
    }

    /// <summary>
    /// シーン遷移用コルーチン
    /// </summary>
    /// <param name='scene'>シーン名</param>
    /// <param name='interval'>暗転にかかる時間(秒)</param>
    private IEnumerator TransScene(string scene, float interval)
    {
        //だんだん暗く
        this.isFading = true;
        float time = 0;
        while (time <= interval)
        {
            this.fadeAlpha = Mathf.Lerp(0f, 1f, time / interval);
            time += Time.deltaTime;
            yield return 0;
        }

        //現在のシーン
        string nowScene = SceneManager.GetActiveScene().name;

        if (nowScene == "MyTurnScene")
        {
            //情報の引継ぎのため
            aggressionflag = GameObject.Find("btn_maru_inside").GetComponent<MyTurnOption>().AggressionFlag;
            SceneManager.sceneLoaded += MapSceneLoaded;
        }
        if ((nowScene == "MapScene" && scene == "GameScene") ||
            (nowScene == "MyTurnScene" && scene == "GameScene"))
        {
            //情報の引継ぎのため
            SceneManager.sceneLoaded += GameSceneLoaded;
        }

        //前回のシーン情報の引継ぎ
        BeforeScene = SceneManager.GetActiveScene().name;
        Debug.Log("前回のシーン:" + BeforeScene);

        //シーン切替
        //Application.LoadLevel(scene);
        SceneManager.LoadScene(scene);

        //だんだん明るく
        time = 0;
        while (time <= interval)
        {
            this.fadeAlpha = Mathf.Lerp(1f, 0f, time / interval);
            time += Time.deltaTime;
            yield return 0;
        }

        this.isFading = false;
    }

    //偵察でマップを開いたか、侵攻でマップを開いたか判別
    private void MapSceneLoaded(Scene next, LoadSceneMode mode)
    {
        try
        {
            // シーン切り替え後のスクリプトを取得
            var mapOptionMaru = GameObject.Find("btn_maru_inside").GetComponent<MapOption>();
            var mapOptionBatu = GameObject.Find("btn_batu_inside").GetComponent<MapOption>();

            Debug.Log("引継ぎ中...");

            // データを渡す処理(maruとbatuそれぞれに入れる)
            mapOptionMaru.SetAggressinnFlag(aggressionflag);
            mapOptionBatu.SetAggressinnFlag(aggressionflag);
        }
        catch
        {
            //何もしない
        }
        finally
        {
            // イベントから削除
            SceneManager.sceneLoaded -= MapSceneLoaded;
        }
    }

    private void GameSceneLoaded(Scene next, LoadSceneMode mode)
    {
        // シーン切り替え後のスクリプトを取得

        Debug.Log("引継ぎ中...");

        // データを渡す処理
        try
        {
            var gameMng = GameObject.Find("GroupMng").GetComponent<GameMng>();
            gameMng.ObjInit();
            //gameMng.CharIDList = arraycharaid;
        }
        catch
        {
            //何もしない
        }
        finally
        {
            // イベントから削除
            SceneManager.sceneLoaded -= MapSceneLoaded;
        }
    }
}