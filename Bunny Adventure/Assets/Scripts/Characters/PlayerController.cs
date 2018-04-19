using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Key
{
    public int m_Direction;
    public bool m_UpArrow;
    public bool m_DownArrow;
    public bool m_LeftShift;

    public Key()
    {
        if (Input.GetKey(KeyCode.RightArrow))
            m_Direction = 1;
        if (Input.GetKey(KeyCode.LeftArrow))
            m_Direction = -1;
        if (Input.GetKey(KeyCode.LeftShift) && m_Direction != 0)
            m_LeftShift = true;
        if (Input.GetKey(KeyCode.UpArrow))
            m_UpArrow = true;
        if (Input.GetKey(KeyCode.DownArrow))
            m_DownArrow = true;
    }
}

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
        Key key = new Key();

        m_PlayerCharacter.Control(key);
    }
}
