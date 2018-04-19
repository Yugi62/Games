using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class FlyableEnemy : EnemyCharacter
{
    [SerializeField] protected float m_MaxFlySpeed;

    //요약 : 날기 메서드
    protected abstract void Fly();

    protected override void Awake()
    {
        base.Awake();
        m_Rigidbody2D.gravityScale = 0.0f;
    }
}
