using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class TilePickerWindow : EditorWindow
{
    private float m_Scale = 1f;                        //스케일 값
    public Vector2 scrollPosition = Vector2.zero;      //스크롤 한만큼의 값
    private Vector2 currentSelection = Vector2.zero;   //마우스가 있는 위치(전환 값)

    [MenuItem("Window/Tile Picker")]
    public static void OpenTilePickerWindow()
    {
        //요약 : 새로운 윈도우 생성
        var window = EditorWindow.GetWindow(typeof(TilePickerWindow));

        var title = new GUIContent();
        title.text = "Tile Picker";
        window.titleContent = title;
    }

    //요약 : OnGUI일때 실행 (GUI를 그려야할 때 호출)
    private void OnGUI()
    {
        //요약 : 게임 오브젝트가 선택되었는 지 확인
        if (Selection.activeGameObject == null)
            return;

        //요약 : 게임 오브젝트에 'TileMap' Component가 있는 지 확인
        var selection = Selection.activeGameObject.GetComponent<TileMap>();
        if (selection != null)
        {
            //요약 : TileMap Component에서 Texture2D가 있는 지 확인
            var texture2D = selection.m_Texture2D;
            if(texture2D != null)
            {         
                //요약 : scale(0~3) 조절 Sldier 초기화
                EditorGUILayout.BeginHorizontal();
                EditorGUILayout.LabelField("Scale", GUILayout.Width(EditorGUIUtility.fieldWidth));
                m_Scale = EditorGUILayout.Slider(m_Scale, 0f, 3f, GUILayout.Height(EditorGUIUtility.singleLineHeight));
                EditorGUILayout.EndHorizontal();
                
                //요약 : 조절된 sclae만큼 사이즈 수정
                var newTextureSize = new Vector2(texture2D.width, texture2D.height) * m_Scale;
                //요약 : 설치된 슬라이더를 피하기 위한 offset 초기화
                var offset = new Vector2(0, EditorGUIUtility.singleLineHeight);                

                //요약 : 윈도우 크기
                var viewPort = new Rect(0, 0, position.width, position.height);
                //요약 : 윈도우에서의 teture2D 크기
                var contentSize = new Rect(0, 0, newTextureSize.x + offset.x, newTextureSize.y + offset.y);

                //요약 : contentSize보다 viewPort가 작으면 스크롤바 생성
                //요약 : BeginScrollView에서 EndScrollView까지 스크롤과 같이 이동
                scrollPosition = GUI.BeginScrollView(viewPort, scrollPosition, contentSize);
                GUI.DrawTexture(new Rect(offset.x, offset.y, newTextureSize.x, newTextureSize.y), texture2D);

                //요약 : 변경된 크기만큼의 타일 사이즈
                var tile = selection.m_TileSize * m_Scale;

                tile.x += selection.m_TilePadding.x * m_Scale;
                tile.y += selection.m_TilePadding.y * m_Scale;

                //요약 : grid 갯수
                var grid = new Vector2(newTextureSize.x / tile.x, newTextureSize.y / tile.y);

                var selectionPos = new Vector2(tile.x * currentSelection.x + offset.x,
                                               tile.y * currentSelection.y + offset.y);

                var boxTexture = new Texture2D(1, 1);
                boxTexture.SetPixel(0, 0, new Color(0, 0.5f, 1f, 0.4f));
                boxTexture.Apply();

                var style = new GUIStyle(GUI.skin.customStyles[0]);
                style.normal.background = boxTexture;

                GUI.Box(new Rect(selectionPos.x, selectionPos.y, tile.x, tile.y), "", style);

                var cEvent = Event.current;
                Vector2 mousePos = new Vector2(cEvent.mousePosition.x, cEvent.mousePosition.y);

                if(cEvent.type == EventType.mouseDown && cEvent.button == 0)
                {
                    //Math.Floor = 내림

                    //요약 : 현재 위치(정수 값) = (마우스 위치 + 스크롤한 위치) / 타일 사이즈
                    currentSelection.x = (int)((mousePos.x + scrollPosition.x) / tile.x);
                    currentSelection.y = (int)((mousePos.y + scrollPosition.y) / tile.y);

                    /*요약 : m_TileID는 m_SpriteReference에서 값을 빼올 때의 키 값이므로
                     *       일차원 배열일 때의 키 값을 건내줄 필요가 있다 = x값 + (y값 * 가로 총 갯수) + 1 
                     *       마지막에 1을 더하는 것은 첫번째 값에는 에셋 주소가 저장되어 있기 때문이다
                    */       

                    selection.m_TileID = (int)(currentSelection.x + (currentSelection.y * grid.x) + 1);
                    Repaint();
                }

                GUI.EndScrollView();

            }
        }
    }
}
