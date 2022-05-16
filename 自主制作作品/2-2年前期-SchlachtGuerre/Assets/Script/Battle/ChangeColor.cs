using UnityEngine;
using System.Collections;

public class ChangeColor : MonoBehaviour
{

    //選択判別用フラグ
    public bool select_flg;

    //選択時の色変更用
    private Color default_color;
    private Color select_color;

    //色変更対象のオブジェクトのマテリアル格納用
    protected Material mat;

    void Start()
    {
        //フラグ、色、マテリアルの初期化
        select_flg = false;
        default_color = Color.gray;
        select_color = Color.red;
        mat = this.gameObject.GetComponent<Renderer>().material;
    }

    void Update()
    {
        mat.color = default_color;

        //フラグがtrueの場合(RayCast.csで変更される)
        if (select_flg)
        {
            //選択から外れたとき用
            select_flg = false;
            //オブジェクトの色を変更する
            mat.color = select_color;
        }
    }
}
