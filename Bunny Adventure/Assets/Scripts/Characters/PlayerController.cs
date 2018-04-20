using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof (PlayerCharacter))]
public class PlayerController : MonoBehaviour
{
    private PlayerCharacter m_PlayerCharacter;

    private void Awake()
    {
        m_PlayerCharacter = GetComponent<PlayerCharacter>();
    }

    private void FixedUpdate()
    {
        ControlParam controlParam = new ControlParam();

        if (Input.GetKey(KeyCode.RightArrow))
            controlParam.m_Direction = 1;
        if (Input.GetKey(KeyCode.LeftArrow))
            controlParam.m_Direction = -1;
        if (Input.GetKey(KeyCode.UpArrow))
            controlParam.m_Up = true;
        if (Input.GetKey(KeyCode.DownArrow))
            controlParam.m_Down = true;
        if (Input.GetKey(KeyCode.LeftShift))
            controlParam.m_Run = true;

        m_PlayerCharacter.Control(controlParam);
    }
}
