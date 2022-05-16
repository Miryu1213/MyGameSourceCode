using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Shaking : MonoBehaviour
{
    public Image rakurai;
    public float waveRange;
    public float waveSpeed;
    public int leftCount = 1;

    private float radian;
    private Vector3 center;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
    }

    public void Shakingcar()
    {
        center = rakurai.transform.position;
        StartCoroutine(run());
    }
    IEnumerator run()
    {
        float TIME_VIB = 1.0f;
        float NUM_VIB = 10.0f;

        int step = 0;
        while (true)
        {
            radian += waveSpeed * Time.deltaTime;

            waveRange -= Time.deltaTime * 1.3f;
            waveRange = Mathf.Clamp(waveRange, 0.0f, waveRange);
            waveSpeed -= Time.deltaTime;

            rakurai.transform.position = center +
                new Vector3(Mathf.Sin(radian) * waveRange / 2.0f, Mathf.Sin(radian / 2.0f) * waveRange / 6.0f, 0.0f);
            yield return new WaitForSeconds(TIME_VIB / NUM_VIB);
            step += 1;
            if (step > NUM_VIB)
            {
                rakurai.transform.position = center;
                yield break;
            }
        }
        yield break;
    }
}

