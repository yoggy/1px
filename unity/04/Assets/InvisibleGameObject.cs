using UnityEngine;
using System.Collections;

public class InvisibleGameObject : MonoBehaviour
{
	void Start ()
	{
		gameObject.SetActiveRecursively(false);
	}
		
	void Update ()
	{
	}
}