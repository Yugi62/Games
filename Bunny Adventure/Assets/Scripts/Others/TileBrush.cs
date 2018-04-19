using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TileBrush : MonoBehaviour
{
    public SpriteRenderer m_SpriteRenderer2D;
    public Vector2 m_BrushSize = Vector2.zero;

    public int m_TileID = 0;

    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireCube(transform.position, m_BrushSize);
    }

    public void UpdateBrush(Sprite sprite)
    {
        m_SpriteRenderer2D.sprite = sprite;
    }
}
