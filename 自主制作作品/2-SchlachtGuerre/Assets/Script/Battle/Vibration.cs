using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Vibration : MonoBehaviour
{
    // 振動させたいImageです
    public Image rakurai;
    Vector3 def;
    // 振動の大きさ
    public int velocity;
    // Start is called before the first frame update
    void Start()
    {

    }
    // Update is called once per frame
     void Update()
    {
    }
    public void vibrate()
    {
        def = rakurai.GetComponent<RectTransform>().localPosition;
        Debug.Log("buiblard");
        StartCoroutine(run());
    }

    IEnumerator run()
    {
        float TIME_VIB = 1.0f;
        float NUM_VIB = 10.0f;
        
        int step = 0;
        while (true)
        {
            //-velocity ~ velocity の乱数
            int value1 = Random.Range(velocity * -1, velocity + 1);
            //int value2 = Random.Range(velocity * -1, velocity + 1);


            // value1, value2 の分だけ移動させる
            Vector3 m_pos = rakurai.GetComponent<RectTransform>().localPosition;
            m_pos.x += value1;
            //m_pos.y += value2;
            rakurai.GetComponent<RectTransform>().localPosition = m_pos;
            yield return new WaitForSeconds(TIME_VIB / NUM_VIB);
            step += 1;
            if (step > NUM_VIB)
            {
                Debug.Log("break");
                rakurai.GetComponent<RectTransform>().localPosition = def;
                yield break;
            }
        }
        yield break;
    }
}


