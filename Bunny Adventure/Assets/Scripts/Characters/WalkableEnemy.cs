using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class WalkableEnemy : EnemyCharacter
{
    [SerializeField] protected float m_MaxWalkSpeed = 0.5f;

    //요약 : 걷기 메서드(액션)
    protected void Walk(int direction)
    {
        m_Animator.SetBool("Walking", true);
        m_Rigidbody2D.velocity = new Vector2(direction * m_MaxWalkSpeed, m_Rigidbody2D.velocity.y);
    }

    protected override void Awake()
    {
        base.Awake();
        m_Rigidbody2D.gravityScale = 3.0f;
    }
}
