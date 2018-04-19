using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*캐릭터 움직임 순서도
 *
 *Update() -> Control() -> 1. ResetAnimatorParam()
 *                      -> 2. Action Methods       -> 1. Check
*/

[RequireComponent(typeof(Animator))]
[RequireComponent(typeof(Rigidbody2D))]
public abstract class Character : MonoBehaviour
{
    protected Animator m_Animator;                             //Init Animator
    protected Rigidbody2D m_Rigidbody2D;                       //Init Rigidbody2D

    protected delegate void m_ActionDelegate(ActionParam actionParam);
    protected m_ActionDelegate m_ActionMethods;

    //애니메이터 파라미터 리셋 메서드
    protected abstract void ResetAnimatorParam();
    //특정 조건에 따라 액션 메서드를 호출하는 메서드
    //public abstract void Control();

    //공격 메서드(액션)
    protected virtual void Attack()
    {

    }
    //죽음 메서드(액션)
    protected virtual void Dead()
    {

    }
    //회전 메서드(액션)
    protected void Flip(int direction)
    {
        if (direction != 0)
            transform.localScale = new Vector3(direction, 1f, 1f);
    }
    //이동 메서드(액션)
    protected void Move(int direction, float speed, string animation)
    {
        m_Animator.SetBool(animation, true);
        m_Rigidbody2D.velocity = new Vector2(direction * speed, m_Rigidbody2D.velocity.y);     
    }
    //점프 메서드(액션)
    protected void Jump(float speed, string animation)
    {
        m_Animator.SetBool(animation, true);
        m_Rigidbody2D.AddForce(new Vector2(0, speed));
    }
    //낙하 메서드(액션)
    protected void Fall(string animation)
    {
        m_Animator.SetBool(animation, true);
    }

    protected virtual void Awake()
    {
        m_Animator = GetComponent<Animator>();              //컴포넌트 추가
        m_Rigidbody2D = GetComponent<Rigidbody2D>();        //컴포넌트 추가
        m_Rigidbody2D.freezeRotation = true;                //Z축 회전 방지
    }
}
