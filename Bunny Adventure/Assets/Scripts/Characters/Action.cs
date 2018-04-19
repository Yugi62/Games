using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//액션 메서드 파라미터
public struct ActionParam
{
    public Rigidbody2D m_Rigidbody2D;
    public Animator m_Animator;

    public int m_Direction;              //방향
    public float? m_MaxWalkSpeed;        //최대 걷기 속도
    public float? m_MaxRunSpeed;         //최대 뛰기 속도
    public float? m_JumpForce;           //점프량 
}

public class Action : MonoBehaviour
{
    //공격 메서드(액션)
    public static void Attack(ActionParam actionParam)
    {

    }

    //죽음 메서드(액션)
    public static void Dead(ActionParam actionParam)
    {

    }

    //회전 메서드(액션)
    public static void Flip(ActionParam actionParam)
    {
        if (actionParam.m_Direction != 0)
            actionParam.m_Rigidbody2D.transform.localScale = new Vector3(actionParam.m_Direction, 1f, 1f);        
    }

    //걷기 메서드(액션)
    public static void Walk(ActionParam actionParam)
    {
        if (actionParam.m_MaxWalkSpeed.HasValue)
        {
            actionParam.m_Animator.SetBool("Walking", true);
            actionParam.m_Rigidbody2D.velocity = new Vector2(actionParam.m_Direction * actionParam.m_MaxWalkSpeed.Value, actionParam.m_Rigidbody2D.velocity.y);
        }
    }

    //뛰기 메서드(액션)
    public static void Run(ActionParam actionParam)
    {
        if (actionParam.m_MaxRunSpeed.HasValue)
        {
            actionParam.m_Animator.SetBool("Running", true);
            actionParam.m_Rigidbody2D.velocity = new Vector2(actionParam.m_Direction * actionParam.m_MaxRunSpeed.Value, actionParam.m_Rigidbody2D.velocity.y);
        }
    }

    //점프 메서드(액션)
    public static void Jump(ActionParam actionParam)
    {
        if (actionParam.m_JumpForce.HasValue)
        {
            actionParam.m_Animator.SetBool("Jumping", true);
            actionParam.m_Rigidbody2D.AddForce(new Vector2(0, actionParam.m_JumpForce.Value));
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

    }
}
