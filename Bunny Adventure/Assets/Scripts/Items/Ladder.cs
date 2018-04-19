using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ladder : MonoBehaviour, Item
{
    [SerializeField]
    private float m_MaxLadderSpeed = 0.08f;

    private Transform m_PlayerTransform;
    private PlayerCharacter m_PlayerCharacter;
    private Rigidbody2D m_PlayerRigidbody2D;

    public string GetId()
    {
        return "Ladder";
    }

    public void LadderMove(float destination)
    {
        destination *= m_MaxLadderSpeed;

        m_PlayerTransform.position = new Vector3(
            transform.position.x,
            m_PlayerTransform.position.y + destination,
            0);
    }

    public void LadderRide()
    {
        m_PlayerRigidbody2D.gravityScale = 0;
        m_PlayerRigidbody2D.velocity = new Vector2(0, 0);
        Physics2D.IgnoreLayerCollision(8, 10, true);
    }

    public void LadderCancel()
    {
        m_PlayerRigidbody2D.gravityScale = 3;
        Physics2D.IgnoreLayerCollision(8, 10, false);
    }

    private void OnTriggerEnter2D(Collider2D collider2D)
    {
        if (collider2D.GetComponent<PlayerController>() != null)
        {
            m_PlayerTransform = collider2D.transform;
            m_PlayerCharacter = collider2D.GetComponent<PlayerCharacter>();
            m_PlayerRigidbody2D = collider2D.GetComponent<Rigidbody2D>();
        }
    }

    private void OnTriggerExit2D(Collider2D collider2D)
    {
        if(m_PlayerCharacter)
        {
            LadderCancel();
        }
        m_PlayerCharacter = null;
    }
}
