using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerCharacter : Character
{
    [SerializeField] private float m_MaxWalkSpeed = 4.0f;     //최대 걷기 속도
    [SerializeField] private float m_MaxRunSpeed = 6.0f;      //최대 달리기 속도
    [SerializeField] private float m_JumpForce = 250.0f;      //점프 높이

    public GameObject m_WallScratch;

    //사다리 영역 안에 있는가?
    private bool m_InLadder;
    //사다리를 타고 있는가?
    private bool m_IsLaddered;

    //겹쳐진 콜라이더
    private Collider2D m_TriggeredCollider;
    
    protected override void Awake()
    {
        base.Awake();
        m_Rigidbody2D.gravityScale = 3.0f;

        //사용 가능 '행동 메서드' 목록
        m_ActionMethods += Action.Flip;
        m_ActionMethods += Action.Run;
        m_ActionMethods += Action.Walk;
        m_ActionMethods += Action.Jump;
        m_ActionMethods += Action.WallClimb;
        m_ActionMethods += Action.WallJump;
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

    public void Control(Key key)
    {
        ResetAnimatorParam();

        m_ActionParam.m_Transform = transform;
        m_ActionParam.m_Rigidbody2D = m_Rigidbody2D;
        m_ActionParam.m_Animator = m_Animator;

        m_ActionParam.m_Direction = key.m_Direction;
        m_ActionParam.m_MaxRunSpeed = (key.m_Direction != 0 && key.m_LeftShift) ? m_MaxRunSpeed : 0f;
        m_ActionParam.m_MaxWalkSpeed = (key.m_Direction != 0 && !key.m_LeftShift) ? m_MaxWalkSpeed : 0f;
        m_ActionParam.m_JumpForce = (key.m_UpArrow) ? m_JumpForce : 0f;

        m_ActionParam.m_IsGrounded = m_IsGrounded;
             
        m_ActionMethods(m_ActionParam);
    }

    private void OnTriggerEnter2D(Collider2D collider2D)
    {
        m_TriggeredCollider = collider2D;
        Item item = collider2D.gameObject.GetComponent<Item>();

        if (item != null)
        {
            if (item.GetId() == "Ladder")
            {
                m_InLadder = true;
            }
        }
    }

    private void OnTriggerExit2D(Collider2D collider2D)
    {
        m_TriggeredCollider = null;
        Item item = collider2D.gameObject.GetComponent<Item>();

        if (item != null)
        {
            if (item.GetId() == "Ladder")
            {
                m_InLadder = false;

                if (m_IsLaddered)
                {
                    m_Animator.SetBool("Climbing", false);
                    m_Animator.speed = 1.0f;

                    m_IsLaddered = false;
                    m_GroundLayer = (-1);
                }
            }
        }
    }
}





/*

        if (!Physics2D.Raycast(transform.position, Vector2.right* transform.localScale.x, m_WallJumpDistance, (1 << LayerMask.NameToLayer("Tile"))))
            m_IsWallClimbing = false;

        if (m_IsGrounded)
            m_Rigidbody2D.velocity = Vector2.zero;

        if (m_IsLaddered)
            m_Animator.speed = 0.0f;

        if (!m_AirControl)
        {
            m_WallJumpCoolDown += Time.deltaTime;

            if (m_WallJumpCoolDown >= 0.2f)
            {
                m_WallJumpCoolDown = 0f;
                m_AirControl = true;
            }
        }

        if (!m_IsWallClimbing)
        {
            if (transform.FindChild("WallScratch"))
            {
                Destroy(transform.FindChild("WallScratch").gameObject);
            }
        }


        //Left / Right
        if (key.m_Direction != 0)
        {
            RaycastHit2D hit = Physics2D.BoxCast(
                transform.position,
                new Vector2(0.1f, transform.localScale.y * 0.8f),
                0f,
                Vector2.right *
                transform.localScale.x,
                m_WallJumpDistance,
                (1 << LayerMask.NameToLayer("Tile")));

            if (!m_IsGrounded && hit.collider != null && !m_InLadder)
            {
                m_Rigidbody2D.velocity = new Vector2(key.m_Direction, -0.5f);

                if (!transform.FindChild("WallScratch"))
                {
                    Vector3 hand = transform.FindChild("Hand").position;

GameObject obj = Instantiate(m_WallScratch, hand, m_WallScratch.transform.rotation, transform);
obj.name = "WallScratch";

                    m_IsWallClimbing = true;
                    m_Animator.SetBool("WallClimbing", true);
                }
            }

            else if (key.m_LeftShift)
            {
                if (!m_IsLaddered && m_AirControl)
                    Flip(key.m_Direction);

                if ((m_IsGrounded || m_AirControl) && m_IsLaddered == false)
                {
                    Move(key.m_Direction, m_MaxRunSpeed, "Running");
                }
            }
            else
            {
                if (!m_IsLaddered && m_AirControl)
                    Flip(key.m_Direction);

                if ((m_IsGrounded || m_AirControl) && m_IsLaddered == false)
                {
                    Move(key.m_Direction, m_MaxWalkSpeed, "Walking");
                }
            }
        }

        //Up
        if (key.m_UpArrow)
        {
            RaycastHit2D hit = Physics2D.Raycast(transform.position, Vector2.right * transform.localScale.x, m_WallJumpDistance, (1 << LayerMask.NameToLayer("Tile")));
            if (!m_IsGrounded && hit.collider != null && !m_InLadder)
            {
                m_Rigidbody2D.velocity = new Vector2(5f * hit.normal.x, 8f);
m_AirControl = false;

                Flip(-key.m_Direction);
            }
            else if (m_IsGrounded && !m_IsCelling)
            {
                Jump(m_JumpForce, "Jumping");
            }
            else if (m_InLadder)
            {
                Ladder ladder = m_TriggeredCollider.GetComponent<Ladder>();

                if (m_IsCelling && !m_IsLaddered)
                {
                    m_Animator.SetBool("Climbing", true);

                    m_WhatIsGround = (-1) - ((1 << 9) | (1 << 8));

                    ladder.LadderRide();
                    ladder.LadderMove(1);

                    m_IsLaddered = true;
                    m_IsGrounded = false;
                }
                else if (m_IsLaddered)
                {
                    m_WhatIsGround = (-1) - ((1 << 9) | (1 << 8));
                    m_Animator.speed = 3.0f;
                    ladder.LadderMove(1);
                }
            }
        }
        //Down
        if (key.m_DownArrow)
        {
            if (m_InLadder)
            {
                Ladder ladder = m_TriggeredCollider.GetComponent<Ladder>();

                if (!m_IsCelling && m_GroundCheck && !m_IsLaddered)
                {
                    m_Animator.SetBool("Climbing", true);

                    m_WhatIsGround = (-1) - ((1 << 9) | (1 << 8));
                    ladder.LadderRide();
                    ladder.LadderMove(-1);

                    m_IsLaddered = true;
                    m_IsGrounded = false;
                }
                else if (m_IsLaddered)
                {
                    if (!CheckOverlapCircle(m_GroundCheck, k_GroundedRadious, (-1) - (1 << 9)))
                        m_WhatIsGround = (-1) - (1 << 9);

                    m_Animator.speed = 1.2f;
                    ladder.LadderMove(-1);
                }
            }
        }

        //캐릭터가 땅에 닿을 시 사다리에서 내리기
        if (m_IsLaddered && m_IsGrounded)
        {
            m_Animator.SetBool("Climbing", false);

            Ladder ladder = m_TriggeredCollider.GetComponent<Ladder>();
ladder.LadderCancel();

            m_IsLaddered = false;
            m_WhatIsGround = (-1);
        }
        */