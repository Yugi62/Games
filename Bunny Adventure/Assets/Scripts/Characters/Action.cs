using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//액션 메서드 파라미터
public struct ActionParam
{
    public Transform m_Transform;
    public Rigidbody2D m_Rigidbody2D;
    public Animator m_Animator;

    public int m_Direction;             //방향
    public float m_MaxWalkSpeed;        //최대 걷기 속도
    public float m_MaxRunSpeed;         //최대 뛰기 속도
    public float m_JumpForce;           //점프량

    public bool m_IsGrounded;           //땅에 닿은 여부
}

public static class Action
{
    //공격 메서드(액션)
    public static void Attack()
    {
        
    }

    //죽음 메서드(액션)
    public static void Dead()
    {

    }

    //회전 메서드(액션)
    public static void Flip(Rigidbody2D rigidbody2D, int direction)
    {
        if (direction != 0)
            rigidbody2D.transform.localScale = new Vector3(direction, 1f, 1f);        
    }

    //이동 메서드(액션)
    public static void Move(Rigidbody2D rigidbody2D, Animator animator, string animation, float speed, int direction)
    {
        if (speed != 0)
        {
            animator.SetBool(animation, true);
            rigidbody2D.velocity = new Vector2(direction * speed,
                                               rigidbody2D.velocity.y);
        }
    }

    //점프 메서드(액션)
    public static void Jump(ActionParam actionParam)
    {
        if (actionParam.m_JumpForce != 0 && actionParam.m_IsGrounded)
        {
            actionParam.m_Animator.SetBool("Jumping", true);
            actionParam.m_Rigidbody2D.AddForce(new Vector2(0, actionParam.m_JumpForce));
        }
    }

    //낙하 메서드(액션)
    public static void Fall(ActionParam actionParam)
    {
        actionParam.m_Animator.SetBool("Falling", true);
    }

    //벽타기 메서드(액션)
    public static void WallClimb(ActionParam actionParam)
    {
        if (actionParam.m_Direction != 0 && !actionParam.m_IsGrounded)
        {
            RaycastHit2D hit = Physics2D.BoxCast(
                    actionParam.m_Transform.position,
                    new Vector2(0.1f, actionParam.m_Transform.localScale.y * 0.8f),
                    0f,
                    Vector2.right *
                    actionParam.m_Transform.localScale.x,
                    0.5f,
                    (1 << LayerMask.NameToLayer("Tile")));

            if (hit)
            {
                actionParam.m_Animator.SetBool("WallClimbing", true);
                actionParam.m_Rigidbody2D.velocity = new Vector2(0, -0.5f);
            }
        }
    }
    
    //벽점프 메서드(액션)
    public static void WallJump(ActionParam actionParam)
    {      
        if (actionParam.m_JumpForce != 0 && !actionParam.m_IsGrounded)
        {
            RaycastHit2D hit = Physics2D.BoxCast(
                    actionParam.m_Transform.position,
                    new Vector2(0.1f, actionParam.m_Transform.localScale.y * 0.8f),
                    0f,
                    Vector2.right *
                    actionParam.m_Transform.localScale.x,
                    0.5f,
                    (1 << LayerMask.NameToLayer("Tile")));

            if (hit)
            {
                actionParam.m_Rigidbody2D.velocity = new Vector2(5f * hit.normal.x, 8f);

                actionParam.m_Direction *= -1;
                Action.Flip(actionParam);
            }
        }
    }
}
