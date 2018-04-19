using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TileMap : MonoBehaviour
{
    public Vector2 m_MapSize = new Vector2(2,2);
    public Vector2 m_TilePadding = new Vector2();
    public Vector2 m_TileSize = new Vector2();
    public Vector2 m_GridSize = new Vector2();
    public Texture2D m_Texture2D;
    public Object[] m_SpriteReference;

    public int m_PixelsPerUnit;
    public int m_TileID = 0;

    public GameObject m_Tiles;

    public Sprite m_CurrentTileBursh
    {
        get { return m_SpriteReference[m_TileID] as Sprite; }
    }

    //오브젝트 선택 시 기즈모를 그리는 함수
    private void OnDrawGizmosSelected()
    {
        var pos = transform.position;

        if(m_Texture2D != null)
        {
            //기즈모 색깔
            Gizmos.color = Color.gray;

            var row = 0;
            var maxColumns = m_MapSize.x;
            //타일의 총 개수
            var total = m_MapSize.x * m_MapSize.y;
            //월드에서의 타일 1개 사이즈
            var tile = new Vector3(m_TileSize.x / m_PixelsPerUnit, m_TileSize.y / m_PixelsPerUnit);
            var offset = new Vector2(tile.x / 2, tile.y / 2);

            for(int i = 0; i< total; i++)
            {
                var column = i % maxColumns;

                var newX = (column * tile.x) + offset.x + pos.x;
                var newY = -(row * tile.y) - offset.y  + pos.y;

                Gizmos.DrawWireCube(new Vector3(newX, newY), tile);

                if(column == maxColumns - 1)
                {
                    row++;
                }
            }

            //m_GridSize를 기준으로 중앙 값 지정
            var centerX = pos.x + (m_GridSize.x / 2);
            var centerY = pos.y - (m_GridSize.y / 2);

            //기즈모 생성
            Gizmos.DrawWireCube(new Vector2(centerX, centerY), m_GridSize);
        }
    }
}
