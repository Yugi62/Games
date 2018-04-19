using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraFollow : MonoBehaviour
{
    [SerializeField] private Transform m_Target;
    [SerializeField] private float m_Damping = 1.0f;
    [SerializeField] private float m_OffsetX = 0.0f;
    [SerializeField] private float m_OffsetY = 0.0f;
    [SerializeField] private float m_OffsetZ = -10.0f;

    private Vector3 m_CurrentVelocity;

    private Transform m_LeftUp;
    private Transform m_RightDown;

    private void Awake()
    {
        m_LeftUp = GameObject.Find("LeftUp").transform;
        m_RightDown = GameObject.Find("RightDown").transform;
    }

    private void FixedUpdate()
    {   
        var revisedTarget = m_Target.position + (new Vector3(m_OffsetX, m_OffsetY, m_OffsetZ));
        var widthSize = Camera.main.orthographicSize * Screen.width / Screen.height;
        var heightSize = Camera.main.orthographicSize;

        if (revisedTarget.x < m_LeftUp.position.x + widthSize)
        {    
            revisedTarget.x = m_LeftUp.position.x + widthSize;
        }
        else if (revisedTarget.x > m_RightDown.position.x - widthSize)
        {
            revisedTarget.x = m_RightDown.position.x - widthSize;
        }

        if (revisedTarget.y < m_RightDown.position.y + heightSize)
        {
            revisedTarget.y = m_RightDown.position.y + heightSize;
        }
        else if(revisedTarget.y > m_LeftUp.position.y - heightSize)
        {
            revisedTarget.y = m_LeftUp.position.y - heightSize;
        }


        transform.position = Vector3.SmoothDamp(transform.position, revisedTarget, ref m_CurrentVelocity, m_Damping);
    }
}
