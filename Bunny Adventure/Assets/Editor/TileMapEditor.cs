using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

//요약 : TileMap Component의 Inspector를 사용자 정의로 확장
[CustomEditor(typeof(TileMap))]
public class TileMapEditor : Editor
{
    public TileMap m_TileMap;

    private TileBrush brush;
    private Vector3 m_MouseHitPos;

    private bool MouseOnMap
    {
        get { return m_MouseHitPos.x > 0 && m_MouseHitPos.x < m_TileMap.m_GridSize.x && m_MouseHitPos.y < 0 && m_MouseHitPos.y > -m_TileMap.m_GridSize.y; }
    }

    //요약 : 오브젝트가 활성화 직후 한 번 호출
    private void OnEnable()
    {
        //요약 : target(inspected Object)를 TileMap으로 형변환
        m_TileMap = target as TileMap;
        //요약 : current tool를 View로 초기화
        Tools.current = Tool.View;

        //각 타일을 넣을 GameObject를 생성
        if (m_TileMap.m_Tiles == null)
        {
            var gameObject = new GameObject("Tiles");
            gameObject.transform.SetParent(m_TileMap.transform);
            gameObject.transform.position = Vector3.zero;

            m_TileMap.m_Tiles = gameObject;
        }

        if (m_TileMap.m_Texture2D != null)
        {
            //요약 : Pixels Per Unit/Tile Size/Grid Size를 계산
            UpdateCalculation();
            NewBrush();
        }
    }

    //요약 : 오브젝트가 비활성화 직후 호출
    private void OnDisable()
    {
        DestroyBrush();
    }

    /*요약 : OnScene에서 Editor에 접근 중일 때 자동으로 계속 호출
     *     : Editor에서 Scene에 접근해야 할 때 사용하는 메서드
     */
    private void OnSceneGUI()
    {
        if (brush != null)
        {
            UpdateHitPosition();
            MoveBrush();

            if (m_TileMap.m_Texture2D != null && MouseOnMap)
            {
                Event current = Event.current;
                if (current.shift)
                {
                    Draw();
                }
                else if (current.alt)
                {
                    RemoveTile();
                }
            }
        }
    }

    //요약 : OnInspector일때 실행 (Custom Inspector를 그려야할 때 자동으로 '계속' 호출)
    public override void OnInspectorGUI()
    {
        EditorGUILayout.BeginVertical();


        var oldTexture = m_TileMap.m_Texture2D;
        //요약 :
        //typeof(Texture2D) = Texture2D만을 Sort
        //false             = Asset 파일 안에서만 탐색
        m_TileMap.m_Texture2D = (Texture2D)EditorGUILayout.ObjectField("Texture2D", m_TileMap.m_Texture2D, typeof(Texture2D), false);

        if (oldTexture != m_TileMap.m_Texture2D)
        {
            UpdateCalculation();
            m_TileMap.m_TileID = 1;
            CreateBrush();
        }

        var oldSize = m_TileMap.m_MapSize;
        EditorGUILayout.BeginHorizontal();
        EditorGUILayout.LabelField("Map Size", GUILayout.Width(EditorGUIUtility.labelWidth));
        EditorGUILayout.LabelField("X", GUILayout.Width(15f));
        m_TileMap.m_MapSize.x = EditorGUILayout.IntField((int)m_TileMap.m_MapSize.x, GUILayout.Width(EditorGUIUtility.fieldWidth));
        EditorGUILayout.LabelField("Y", GUILayout.Width(15f));
        m_TileMap.m_MapSize.y = EditorGUILayout.IntField((int)m_TileMap.m_MapSize.y, GUILayout.Width(EditorGUIUtility.fieldWidth));
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.BeginHorizontal();
        EditorGUILayout.LabelField("TilePadding Size", GUILayout.Width(EditorGUIUtility.labelWidth));
        EditorGUILayout.LabelField("X", GUILayout.Width(15f));
        m_TileMap.m_TilePadding.x = EditorGUILayout.IntField((int)m_TileMap.m_TilePadding.x, GUILayout.Width(EditorGUIUtility.fieldWidth));
        EditorGUILayout.LabelField("Y", GUILayout.Width(15f));
        m_TileMap.m_TilePadding.y = EditorGUILayout.IntField((int)m_TileMap.m_TilePadding.y, GUILayout.Width(EditorGUIUtility.fieldWidth));
        EditorGUILayout.EndHorizontal();

        if (m_TileMap.m_MapSize != oldSize)
            UpdateCalculation();

        if (m_TileMap.m_Texture2D == null)
        {
            EditorGUILayout.HelpBox("Texture2D가 선택되지 않았습니다", MessageType.Warning);
        }
        else
        {
            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("Tile Size", GUILayout.Width(EditorGUIUtility.labelWidth));
            EditorGUILayout.LabelField("W", GUILayout.Width(15f));
            EditorGUILayout.SelectableLabel(m_TileMap.m_TileSize.x.ToString(), EditorStyles.textField, GUILayout.Width(EditorGUIUtility.fieldWidth), GUILayout.Height(EditorGUIUtility.singleLineHeight));
            EditorGUILayout.LabelField("H", GUILayout.Width(15f));
            EditorGUILayout.SelectableLabel(m_TileMap.m_TileSize.y.ToString(), EditorStyles.textField, GUILayout.Width(EditorGUIUtility.fieldWidth), GUILayout.Height(EditorGUIUtility.singleLineHeight));
            EditorGUILayout.EndHorizontal();

            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("Grid Size", GUILayout.Width(EditorGUIUtility.labelWidth));
            EditorGUILayout.LabelField("W", GUILayout.Width(15f));
            EditorGUILayout.SelectableLabel(m_TileMap.m_GridSize.x.ToString(), EditorStyles.textField, GUILayout.Width(EditorGUIUtility.fieldWidth), GUILayout.Height(EditorGUIUtility.singleLineHeight));
            EditorGUILayout.LabelField("H", GUILayout.Width(15f));
            EditorGUILayout.SelectableLabel(m_TileMap.m_GridSize.y.ToString(), EditorStyles.textField, GUILayout.Width(EditorGUIUtility.fieldWidth), GUILayout.Height(EditorGUIUtility.singleLineHeight));
            EditorGUILayout.EndHorizontal();

            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("Pixels Per Unit", GUILayout.Width(EditorGUIUtility.labelWidth));
            EditorGUILayout.SelectableLabel(m_TileMap.m_PixelsPerUnit.ToString(), EditorStyles.textField, GUILayout.Width(EditorGUIUtility.fieldWidth * 2 + 42f), GUILayout.Height(EditorGUIUtility.singleLineHeight));
            EditorGUILayout.EndHorizontal();

            //요약 : Update 함수 안에 있는 것과 동일하므로 이곳에 삽입
            UpdateBrush(m_TileMap.m_CurrentTileBursh);
        }

        if (GUILayout.Button("모든 타일 삭제"))
        {
            if (EditorUtility.DisplayDialog("모든 타일을 지웁니까?", "데이터를 복구하지 못할 수도 있습니다", "확인", "취소"))
            {
                ClearMap();
            }
                
        }

        EditorGUILayout.EndVertical();
    }

    public void UpdateBrush(Sprite sprite)
    {
        if(brush != null)
        {
            brush.UpdateBrush(sprite);
        }
    }

    private void UpdateCalculation()
    {
        //요약 : 에셋의 주소를 가져온다
        var path = AssetDatabase.GetAssetPath(m_TileMap.m_Texture2D);
        //요약 : 주소로부터 모든 에셋을 Object 타입으로 가져온다
        m_TileMap.m_SpriteReference = AssetDatabase.LoadAllAssetsAtPath(path);

        //첫번째 sprite로 타일 크기를 구한다
        var sprite = (Sprite)m_TileMap.m_SpriteReference[1];
        var height = sprite.textureRect.height;
        var width = sprite.textureRect.width;

        //요약 : Pixels Per Unit = 스프라이트 넓이 / 월드에서의 스프라이트 넓이
        m_TileMap.m_PixelsPerUnit = (int)(sprite.rect.width / sprite.bounds.size.x);
        m_TileMap.m_TileSize = new Vector2(width, height);
        m_TileMap.m_GridSize = new Vector2((width / m_TileMap.m_PixelsPerUnit) * m_TileMap.m_MapSize.x, (height / m_TileMap.m_PixelsPerUnit) * m_TileMap.m_MapSize.y);
    }

    private void CreateBrush()
    {
        var sprite = m_TileMap.m_CurrentTileBursh;
        if(sprite != null)
        {
            GameObject go = new GameObject("Brush");
            go.transform.SetParent(m_TileMap.transform);

            brush = go.AddComponent<TileBrush>();
            brush.m_SpriteRenderer2D = go.AddComponent<SpriteRenderer>();
            brush.m_SpriteRenderer2D.sortingOrder = 1000;

            brush.m_BrushSize = new Vector2(sprite.bounds.size.x,
                                            sprite.bounds.size.y);

            brush.UpdateBrush(sprite);
        }
    }

    private void NewBrush()
    {
        if(brush == null)
        {
            CreateBrush();
        }
    }

    private void DestroyBrush()
    {
        if(brush != null)
        {
            //지연된 평가를 막기 위한 Destroy (editor에서만 사용 가능)
            DestroyImmediate(brush.gameObject);
        }
    }

    private void UpdateHitPosition()
    {
        var plane = new Plane(m_TileMap.transform.TransformDirection(Vector3.forward), Vector3.zero);
        var ray = HandleUtility.GUIPointToWorldRay(Event.current.mousePosition);
        var hit = Vector3.zero;
        var distance = 0f;

        //ray -> plane으로 레이캐스팅을 하여 거리 값을 출력
        if(plane.Raycast(ray, out distance))
        {
            //ray 위치 + ray 방향(정규화) * 거리 = 맞은 오브젝트의 위치
            hit = ray.origin + ray.direction.normalized * distance;

            //월드 좌표를 로컬 좌표로 변경
            //m_TileMap의 왼쪽 위가 0,0으로 취급된다
            m_MouseHitPos = m_TileMap.transform.InverseTransformPoint(hit);
        }
    }

    private void MoveBrush()
    {
        //Scene에서의 타일 사이즈
        var tileSize = m_TileMap.m_TileSize.x / m_TileMap.m_PixelsPerUnit;        

        //Get Local Position
        var x = Mathf.Floor(m_MouseHitPos.x / tileSize) * tileSize;
        var y = Mathf.Floor(m_MouseHitPos.y / tileSize) * tileSize;

        //가로 위치
        var row = x / tileSize;
        //세로 위치
        var column = Mathf.Abs(y / tileSize) - 1;

        if (!MouseOnMap)
            return;

        var id = (int)((column * m_TileMap.m_MapSize.x) + row);
        brush.m_TileID = id;

        x += m_TileMap.transform.position.x + tileSize / 2;
        y += m_TileMap.transform.position.y + tileSize / 2;
        brush.transform.position = new Vector3(x, y, m_TileMap.transform.position.z);
    }

    private void Draw()
    {
        var id = brush.m_TileID.ToString();

        var posX = brush.transform.position.x;
        var posY = brush.transform.position.y;

        GameObject tile = GameObject.Find(m_TileMap.name + "/Tiles/Tile_" + id);

        if(tile == null)
        {
            tile = new GameObject("Tile_" + id);
            tile.transform.SetParent(m_TileMap.m_Tiles.transform);
            tile.transform.position = new Vector3(posX, posY, 0);
            tile.AddComponent<SpriteRenderer>();
        }
        tile.GetComponent<SpriteRenderer>().sprite = brush.m_SpriteRenderer2D.sprite;
    }

    private void RemoveTile()
    {
        var id = brush.m_TileID.ToString();
        GameObject tile = GameObject.Find(m_TileMap.name + "/Tiles/Tile_" + id);

        if(tile != null)
        {
            DestroyImmediate(tile);
        }
    }

    private void ClearMap()
    {
        for(var i = 0; i < m_TileMap.m_Tiles.transform.childCount; i++)
        {
            Transform t = m_TileMap.m_Tiles.transform.GetChild(i);
            DestroyImmediate(t.gameObject);
            i--;
        }
    }
}
