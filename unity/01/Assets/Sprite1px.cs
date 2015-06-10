using UnityEngine;
using System.Collections;

public class Sprite1px : MonoBehaviour {

	SpriteRenderer spriteRenderer;

	// spritesの内容はUnityのInspectorから設定すること
	public Sprite[] sprites = new Sprite[4];


	int idx = 0;

	void Start ()
	{
		spriteRenderer = gameObject.GetComponent<SpriteRenderer>();
	}

	void Update () 
	{
		int idx = (int)(Time.fixedTime / 0.25) % 4;
		spriteRenderer.sprite = sprites[idx];
	}
}
