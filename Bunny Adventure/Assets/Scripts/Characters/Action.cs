using System.Collections;
using System.Collections.Generic;
using UnityEngine;

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
            animator.SetBool(animation, true);
            rigidbody2D.velocity = new Vector2(direction * speed,
                                               rigidbody2D.velocity.y);
    }

    //점프 메서드(액션)
    public static void Jump(Rigidbody2D rigidbody2D, Animator animator, string animation, float speed)
    {
        animator.SetBool("Jumping", true);
        rigidbody2D.AddForce(new Vector2(0, speed));       
    }

    //낙하 메서드(액션)
    public static void Fall(Animator animator, string animation)
    {
        animator.SetBool(animation, true);
    }

    //벽타기 메서드(액션)
    public static void WallClimb(Rigidbody2D rigidbody2D, Animator animator, string animation, float speed)
    {
        animator.SetBool(animation, true);
        rigidbody2D.velocity = new Vector2(0, speed);           
    }
    
    //벽점프 메서드(액션)
    public static void WallJump(Rigidbody2D rigidbody2D, RaycastHit2D hit, int direction)
    {
        rigidbody2D.velocity = new Vector2(5f * hit.normal.x, 8f);
        direction *= -1;
        Action.Flip(rigidbody2D, direction);         
    }
}
