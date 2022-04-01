using UnityEngine;

public class GameOverOption : MonoBehaviour
{
    GameObject groupmng;

    GameObject play_char;
    GameObject statenum;
    private void Start()
    {
        groupmng = GameObject.Find("GroupMng");
        var adaptmng = groupmng.GetComponent<AdaptImgMng>();

        play_char = GameObject.Find("Play_Char");
        statenum = GameObject.Find("StateNum");

        //初期化
        adaptmng.Init();
        //画面適応
        var group = groupmng.GetComponent<GroupMng>().GetGroupId();
        var myid = groupmng.GetComponent<MapCtl>().GetMyId();
        adaptmng.AdaptImage(group[myid], play_char);

        //数字系画面対応
        adaptmng.AdaptStateNum(statenum, myid);
    }

    public void SceneChange()
    {
        if (FadeManager.Instance.isFading)
        {
            return;
        }
        //タイトルシーンへ
        FadeManager.Instance.LoadLevel("TitleScene", 1.0f);
        return;
    }
}
