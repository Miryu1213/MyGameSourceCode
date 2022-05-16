using UnityEngine;
using System.Collections;
using UnityEngine.UI; // UIコンポーネントの使用

public class Serect : MonoBehaviour
{
	Button cube;
	Button sphere;
	Button cylinder;

	void Start()
	{
		// ボタンコンポーネントの取得
		cube = GameObject.Find("/Canvas/Image/Button1").GetComponent<Button>();
		sphere = GameObject.Find("/Canvas/Image/Button2").GetComponent<Button>();
		cylinder = GameObject.Find("/Canvas/Image/Button3").GetComponent<Button>();

		// 最初に選択状態にしたいボタンの設定
		cube.Select();
	}
}