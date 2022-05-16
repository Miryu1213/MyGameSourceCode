using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Currentselected : MonoBehaviour
{
    private GameObject tmpSeleObj;      //現在選択中のオブジェクト(一時保存用)
    public MoveDirection Direction;
    private GameObject controllar;

    private GameObject objpos;
    private GameObject recon;

    private GameObject groupmng;

    string nextname;                    //次のオブジェクト名
    string Tmpnextname;                 //次のオブジェクト名（加工後）

    private GameObject play_char;

    private AudioSource MoveSE;            //移動用SE
   
    private void Start()
    {
        Init();
    }
    public void Init()
    {
        //SE取得
        MoveSE = GameObject.Find("MoveSE").GetComponent<AudioSource>();

        //オブジェクト系色々取得
        play_char = GameObject.Find("Play_Char");
        groupmng = GameObject.Find("GroupMng");
        controllar = GameObject.Find("Controllar");
        if (SceneManager.GetActiveScene().name == "GameScene" || SceneManager.GetActiveScene().name == "MapScene" || SceneManager.GetActiveScene().name == "SelectScene")
        {
            objpos = GameObject.Find("Obj_Pos");
        }
        if(SceneManager.GetActiveScene().name == "MapScene")
        {
            recon = GameObject.Find("Recon");
        }

        //画像の切り替え
        if (SceneManager.GetActiveScene().name == "SelectScene")
        {
            try
            {
                //スタート時に行うためできない場合があるため
                var group = groupmng.GetComponent<GroupMng>().GetGroupId();
                groupmng.GetComponent<AdaptImgMng>().AdaptImage(group, play_char);
            }
            catch
            {
                //何もしない
            }
        }

        try
        {
            tmpSeleObj = controllar.GetComponent<SelectedMng>().GetSelectedObj();
        }
        catch
        {
            Debug.Log("Currentselectedエラー");
            return;
        }
    }
    public void CurSelected()
    {
        //例外処理
        if (SceneManager.GetActiveScene().name == "SelectScene")
        {
            if (objpos.GetComponent<Canvas>().enabled)
            {
                //選択画面が動いていたら
                return;
            }
        }
        if (SceneManager.GetActiveScene().name == "GameScene")
        {
            //攻撃選択時のみ
            var gameOption = controllar.transform.Find("btn_maru_inside").GetComponent<GameOption>();
            if (gameOption.GetAtackFlag() == false)
            {
                return;
            }
        }
        if(SceneManager.GetActiveScene().name == "MapScene")
        {
            if (objpos.GetComponent<Canvas>().enabled || recon.GetComponent<Canvas>().enabled)
            {
                return;
            }
        }

        MoveDirection direction = Direction;
        //選択オブジェクトの更新
        tmpSeleObj = controllar.GetComponent<SelectedMng>().GetSelectedObj();
        if (direction == MoveDirection.None)
        {
            //例外処理
            Debug.Log("direction:nonエラー");
            return;
        }
        if (tmpSeleObj == null)
        {
            //例外処理
            Debug.Log("tmpseleObj:null");
            return;
        }
       
        if (direction == MoveDirection.Up)
        {
            //上
            //Debug.Log(tmpSeleObj.name);
            try
            {
                nextname = tmpSeleObj.GetComponent<Button>().navigation.selectOnUp.ToString();
            }
            catch
            {
                //上がNULLだった場合
                return;
            }
        }
        if (direction == MoveDirection.Down)
        {
            //下
            try
            {
                nextname = tmpSeleObj.GetComponent<Button>().navigation.selectOnDown.ToString();
            }
            catch
            {
                //下がNULLだった場合
                return;
            }
        }
        if (direction == MoveDirection.Left)
        {
            //左
            try
            {
                nextname = tmpSeleObj.GetComponent<Button>().navigation.selectOnLeft.ToString();
            }
            catch
            {
                //左がNULLだった場合
                return;
            }
        }
        if (direction == MoveDirection.Right)
        {
            //右
            try
            {
                nextname = tmpSeleObj.GetComponent<Button>().navigation.selectOnRight.ToString();
            }
            catch
            {
                //右がNULLだった場合
                return;
            }
        }
        //名前検索から次のオブジェクトをGaobInit側に代入する
        Tmpnextname = nextname.Replace(" (UnityEngine.UI.Button)", "");
        controllar.GetComponent<SelectedMng>().SetSelectObj(GameObject.Find(Tmpnextname));

        //画像の切り替え
        if (SceneManager.GetActiveScene().name == "SelectScene" || SceneManager.GetActiveScene().name == "MapScene")
        {
            var group = groupmng.GetComponent<GroupMng>().GetGroupId();
            groupmng.GetComponent<AdaptImgMng>().AdaptImage(group, play_char);
        }

        //SE
        MoveSE.PlayOneShot(MoveSE.clip);

        Debug.Log(Tmpnextname);
        //Debug.Log(nextname);
    }
}
