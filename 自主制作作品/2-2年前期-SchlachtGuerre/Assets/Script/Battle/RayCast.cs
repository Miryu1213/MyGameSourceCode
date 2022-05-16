using UnityEngine;
using System.Collections;

public class RayCast : MonoBehaviour
{

    void Start()
    {

    }

    void Update()
    {
        //カメラ上にあるマウスの位置に飛ばすレイを作成する
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

        //レイがコライダーに当たった場合に情報を格納する
        RaycastHit hit = new RaycastHit();

        //作成したレイを投げる。コライダーに当たった場合はtrueが返る
        if (Physics.Raycast(ray, out hit))
        {
            //レイが当たったオブジェクトがChangeColorコンポーネントをアタッチしていた場合、フラグをtrueにする
            if (hit.collider.gameObject.GetComponent<ChangeColor>())
            {
                hit.collider.gameObject.GetComponent<ChangeColor>().select_flg = true;
            }
        }
    }
}