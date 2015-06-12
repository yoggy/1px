using UnityEngine;
using System.Collections;

public class LightController1px : MonoBehaviour {
	
	Light [] lights = new  Light[3];
	int idx = 0;

	void Start ()
	{
		lights[0] = GetLight("red");
		lights[1] = GetLight("green");
		lights[2] = GetLight("blue");
	}

	void Update () 
	{
		int idx = (int)(Time.fixedTime / 0.25) % 4;
		EnableLight (idx);
	}

	Light GetLight(string name) 
	{
		GameObject obj = GameObject.Find (name);
		Light light = obj.GetComponent<Light>();
		return light;
	}

	void EnableLight(int idx)
	{
		switch (idx) {
		case 0:
			lights[0].enabled = true;
			lights[1].enabled = false;
			lights[2].enabled = false;
			break;
		case 1:
			lights[0].enabled = false;
			lights[1].enabled = true;
			lights[2].enabled = false;
			break;
		case 2:
			lights[0].enabled = false;
			lights[1].enabled = false;
			lights[2].enabled = true;
			break;
		default:
			lights[0].enabled = false;
			lights[1].enabled = false;
			lights[2].enabled = false;
			break;
		}
	}
}
