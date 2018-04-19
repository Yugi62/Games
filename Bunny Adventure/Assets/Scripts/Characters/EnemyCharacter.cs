using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(CircleCollider2D))]
public abstract class EnemyCharacter : Character
{
    [SerializeField] protected float m_Hp;

    protected Transform m_PlayerTransform;
    private CircleCollider2D m_CircleCollider2D;    

    protected override void Awake()
    {
        base.Awake();
        m_CircleCollider2D = GetComponent<CircleCollider2D>();
        m_CircleCollider2D.isTrigger = true;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.tag == "Player")
        {
            m_PlayerTransform = collision.transform;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.tag == "Player")
        {
            m_PlayerTransform = null;
        }
    }
}
