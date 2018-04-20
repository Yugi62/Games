using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*캐릭터 움직임 순서도
 *
 *Update() -> Control() -> 1. ResetAnimatorParam()
 *                      -> 2. Action Methods       -> 1. Check
*/

public struct ControlParam
{
    public int m_Direction;       //방향
    public bool m_Up;             //위      
    public bool m_Down;           //아래

    public bool m_Attack;         //공격
    public bool m_Run;            //달리기
}

[RequireComponent(typeof(Animator))]
[RequireComponent(typeof(Rigidbody2D))]
public abstract class Character : MonoBehaviour
{
    //캐릭터 필수 요소
    private Animator m_Animator;
    private Rigidbody2D m_Rigidbody2D;

    private LayerMask m_GroundLayer = (-1);                 //땅 레이어(전체)

    private Transform m_GroundCheck;                        //땅 검사를 위한 값
    private bool m_IsGrounded;                              //땅에 닿는 여부
    private const float k_GroundedRadious = 0.01f;          //땅 검사 반지름

    private Transform m_CellingCheck;                       //천장 검사를 위한 값
    private bool m_IsCelling;                               //천장에 닿는 여부
    private const float k_CellingRadious = 0.3f;            //천장 검사 반지름


    //캐릭터 능력 수치
    [SerializeField] private float m_MaxWalkSpeed;          //최대 걷기 속도
    [SerializeField] private float m_MaxRunSpeed;           //최대 달리기 속도
    [SerializeField] private float m_JumpForce;             //점프 높이

    private bool m_AirControl = false;                              //공중에서 이동 가능 여부


    protected virtual void Awake()
    {
        m_Animator = GetComponent<Animator>();              //컴포넌트 추가
        m_Rigidbody2D = GetComponent<Rigidbody2D>();        //컴포넌트 추가
        m_Rigidbody2D.freezeRotation = true;                //Z축 회전 방지

        m_GroundCheck = transform.FindChild("GroundCheck");
        m_CellingCheck = transform.FindChild("CellingCheck");
    }

    private void FixedUpdate()
    {
        m_IsGrounded = CheckOverlapCircle(m_GroundCheck, k_GroundedRadious, m_GroundLayer);
        m_IsCelling = CheckOverlapCircle(m_CellingCheck, k_CellingRadious, m_GroundLayer);
    }



    public void Control(ControlParam controlParam)
    {
        ResetAnimatorParam();

        //Flip
        if (controlParam.m_Direction != 0)
            Action.Flip(m_Rigidbody2D, controlParam.m_Direction);


        //Move(Walk, Run)
        if (controlParam.m_Run && controlParam.m_Direction != 0 && (m_AirControl || m_IsGrounded))
            Action.Move(m_Rigidbody2D, m_Animator, "Running", m_MaxRunSpeed, controlParam.m_Direction);
        else if (controlParam.m_Direction != 0 && (m_AirControl || m_IsGrounded))
            Action.Move(m_Rigidbody2D, m_Animator, "Walking", m_MaxWalkSpeed, controlParam.m_Direction);


        //Jump  
        if (controlParam.m_Up && m_IsGrounded)
            Action.Jump(m_Rigidbody2D, m_Animator, "Jumping", m_JumpForce);


        //WallClimb, WallJump
        if (controlParam.m_Direction != 0 && !m_IsGrounded)
        {
            RaycastHit2D hit = Physics2D.BoxCast(
            transform.position, new Vector2(0.1f, transform.localScale.y * 0.8f),
            0f,
            Vector2.right * transform.localScale.x,
            0.5f,
            (1 << LayerMask.NameToLayer("Tile")));

            if (hit && controlParam.m_Up)
            {
                Action.WallJump(m_Rigidbody2D, hit, controlParam.m_Direction);
            }
            else if (hit)
            {
                Action.WallClimb(m_Rigidbody2D, m_Animator, "WallClimbing", -0.5f);
            }
        }
    }

    private void ResetAnimatorParam()
    {
        //정지 상태일 때 Idle로 복귀
        if (m_IsGrounded && m_Animator.GetBool("Jumping"))
            m_Animator.SetBool("Jumping", false);

        if (m_IsGrounded && m_Animator.GetBool("WallClimbing"))
            m_Animator.SetBool("WallClimbing", false);

        if (m_Animator.GetBool("Walking"))
            m_Animator.SetBool("Walking", false);

        if (m_Animator.GetBool("Running"))
            m_Animator.SetBool("Running", false);
    }



    private bool CheckOverlapCircle(Transform transform, float radious, LayerMask layerMask)
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