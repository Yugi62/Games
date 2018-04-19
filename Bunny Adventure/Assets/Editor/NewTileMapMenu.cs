using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class NewTileMapMenu
{
    //요약 : 매뉴를 생성하며 클릭 시 이 메서드를 호출
    [MenuItem("GameObject/Create Tile Map")]
    public static void CreateTileMap()
    {
        GameObject gameObject = new GameObject("Tile Map");
        gameObject.AddComponent<TileMap>();
    }
}
