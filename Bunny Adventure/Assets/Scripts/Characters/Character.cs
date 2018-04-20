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
    protected Animator m_Animator;
    protected Rigidbody2D m_Rigidbody2D;
    protected ActionParam m_ActionParam;

    protected delegate void m_ActionDelegate(ActionParam actionParam);
    protected m_ActionDelegate m_ActionMethods;

    protected LayerMask m_GroundLayer = (-1);               //땅 레이어(전체)
    protected Transform m_GroundCheck;                      //땅 검사를 위한 값
    protected bool m_IsGrounded;                            //땅에 닿는 여부
    protected const float k_GroundedRadious = 0.01f;        //땅 검사 반지름

    protected Transform m_CellingCheck;                     //천장 검사를 위한 값
    protected bool m_IsCelling;                             //천장에 닿는 여부
    protected const float k_CellingRadious = 0.3f;          //천장 검사 반지름

    protected virtual void Awake()
    {
        m_Animator = GetComponent<Animator>();              //컴포넌트 추가
        m_Rigidbody2D = GetComponent<Rigidbody2D>();        //컴포넌트 추가
        m_Rigidbody2D.freezeRotation = true;                //Z축 회전 방지
        m_ActionParam = new ActionParam();

        m_GroundCheck = transform.FindChild("GroundCheck");
        m_CellingCheck = transform.FindChild("CellingCheck");
    }

    protected virtual void FixedUpdate()
    {
        m_IsGrounded = CheckOverlapCircle(m_GroundCheck, k_GroundedRadious, m_GroundLayer);
        m_IsCelling = CheckOverlapCircle(m_CellingCheck, k_CellingRadious, m_GroundLayer);
    }

    protected bool CheckOverlapCircle(Transform transform, float radious, LayerMask layerMask)
    {
        Collider2D[] colliders = Physics2D.OverlapCircleAll(transform.position, radious, layerMask);
        for (int i = 0; i < colliders.Length; i++)
        {
            if (colliders[i].gameObject != gameObject)
                return true;
        }
        return false;
    }
}