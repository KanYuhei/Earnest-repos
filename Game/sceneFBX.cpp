//--------------------------------------------------------------------------------------
//  レンダリング処理   ( sceneFBX.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "sceneFBX.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------------
SenceFBX::SenceFBX( )
{
	//  メンバ変数の初期化
	m_position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_scale = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_fScale = 1.0f;
	m_fRot = 0.0f;
	m_fRot2 = 0.0f;
	m_nTexID = 0;
	numTabs = 0;
}

//--------------------------------------------------------------------------------------
//  デスストラクタ
//--------------------------------------------------------------------------------------
SenceFBX::~SenceFBX( )
{

}

//--------------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------------
HRESULT SenceFBX::Init( void )
{
	numTabs = 0;

	m_nCntV = 0;
	m_nCntVn = 0;
	m_nCntVt = 0;
	m_nCntF = 0;

	//  FBXモデル読み込み実験用プログラム

	//  FBXの作成
	FbxManager* lSdkManager = FbxManager::Create( );

	// Create the IO settings object.
    FbxIOSettings *ios = FbxIOSettings::Create( lSdkManager, IOSROOT );
    lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create( lSdkManager , "" );
    
    // Use the first argument as the filename for the importer.
    if( !lImporter->Initialize( "data/pronama/Pronama-chan.fbx" , -1 , lSdkManager->GetIOSettings( ) ) ) 
	{ 
		char aBuf[ 256 ];
		sprintf( aBuf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus().GetErrorString( ) );
		MessageBox( NULL , aBuf , "Error Message" , MB_OK );

		//  インポーターの削除
		lImporter->Destroy( );

		//  FBXの削除
		lSdkManager->Destroy( );

		return E_FAIL;
	}

	// Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create( lSdkManager, "myScene" );

    // Import the contents of the file into the scene.
    lImporter->Import( lScene );

    // The file is imported, so get rid of the importer.
    lImporter->Destroy( );

	//  三角分割
	FbxGeometryConverter lCoverter( lSdkManager );
	lCoverter.Triangulate( lScene , true );

    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node because it should
    // not contain any attributes.
    FbxNode* lRootNode = lScene->GetRootNode( );

    if( lRootNode ) 
	{
		myNode* pNode = RecursiveNode( lRootNode );
    }

	//  FBXの削除
	lSdkManager->Destroy( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  テクスチャの読み込み
	pTexture->SetTextureImage( "data/pronama/textures/Tex_skin.png" );

	// 頂点バッファの生成
	MakeVertex( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------------
void SenceFBX::Uninit( void )
{
	Sence::Release( );

	//  メモリの破棄
	delete[ ] m_pV; 
	delete[ ] m_pVt;
	delete[ ] m_pVn;
	delete[ ] m_pFace;

	// 頂点バッファの破棄
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release( );
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if( m_pIndexBuff != NULL )
	{
		m_pIndexBuff->Release( );
		m_pIndexBuff = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------------
void SenceFBX::Update( void )
{
	//  頂点の設定
	//SetVertex( );
}

//--------------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------------
void SenceFBX::Draw( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  拡大行列
	D3DXMATRIX mtxRot;								//  ビュー座標変換行列

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  拡大行列の作成
	D3DXMatrixScaling( &mtxScale , m_scale.x , m_scale.y , m_scale.z );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  ワールド座標変換
	pDevice->SetTransform( D3DTS_WORLD , &mtxWorld );

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource( 0 ,								//  パイプライン番号
							  m_pVtxBuff ,						//  頂点バッファのアドレス
							  0 ,								//  オフセット( byte )
							  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_3D );

	// テクスチャの設定
	pDevice->SetTexture( 0 , pTexture->GetTextureImage( "data/pronama/textures/Tex_skin.png" ) ); 

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,				//  プリミティブの種類
							0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
							m_nCntF / 3 );						//  プリミティブ数
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
SenceFBX* SenceFBX::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	SenceFBX *pSceneFBX;

	//  インスタンス生成
	pSceneFBX = new SenceFBX;

	//  座標の代入
	pSceneFBX->m_position = position;

	//  回転角の代入
	pSceneFBX->m_rot = rot;

	//  大きさ倍率の代入
	pSceneFBX->m_scale = scale;

	//  初期化
	pSceneFBX->Init( );

	return pSceneFBX;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
void SenceFBX::MakeVertex( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * m_nCntF ,					//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,								//  使用方法
											 FVF_VERTEX_3D ,									//  
											 D3DPOOL_MANAGED ,									//  メモリ管理方法( MANAGED → お任せ )
											 &m_pVtxBuff ,										//  バッファ
											 NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return;
	}

	VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,							//  アドレスが書かれたメモ帳のアドレス
						  0 );										//  ロックの種類

		//  頂点の数分のループ
		for( int i = 0; i < m_nCntF; i++ )
		{
			//  頂点座標の設定( 3D座標 ・ 右回り )
			pVtx[ 0 ].position = D3DXVECTOR3( m_pV[ m_pFace[ i ].v ].x , m_pV[ m_pFace[ i ].v ].y , m_pV[ m_pFace[ i ].v ].z );;

			//  法線の指定
			pVtx[ 0 ].normal = D3DXVECTOR3( m_pVn[ m_pFace[ i ].vn ].x , m_pVn[ m_pFace[ i ].vn ].y , m_pVn[ m_pFace[ i ].vn ].z );

			//  頂点色の設定( 0 ～ 255 の整数値 )
			pVtx[ 0 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

			//  UV座標の指定
			pVtx[ 0 ].tex = D3DXVECTOR2( m_uvSet.uvBuffer.at( i ).x , m_uvSet.uvBuffer.at( i ).y );

			//  次の頂点へ
			pVtx++;
		}

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}

	return;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
void SenceFBX::SetVertex( void )
{
	VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,							//  アドレスが書かれたメモ帳のアドレス
						  0 );										//  ロックの種類

		//  頂点の数分のループ
		for( int i = 0; i < m_nCntF; i++ )
		{
			//  頂点座標の設定( 3D座標 ・ 右回り )
			pVtx[ 0 ].position = D3DXVECTOR3( m_pV[ m_pFace[ i ].v ].x , m_pV[ m_pFace[ i ].v ].y , m_pV[ m_pFace[ i ].v ].z );;

			//  法線の指定
			pVtx[ 0 ].normal = D3DXVECTOR3( m_pVn[ m_pFace[ i ].vn ].x , m_pVn[ m_pFace[ i ].vn ].y , m_pVn[ m_pFace[ i ].vn ].z );

			//  頂点色の設定( 0 ～ 255 の整数値 )
			pVtx[ 0 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

			//  UV座標の指定
			pVtx[ 0 ].tex = D3DXVECTOR2( m_uvSet.uvBuffer.at( i ).x , m_uvSet.uvBuffer.at( i ).y );

			//  次の頂点へ
			pVtx++;
		}

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}
}

//--------------------------------------------------------------------------------------
//  座標の代入
//--------------------------------------------------------------------------------------
void SenceFBX::SetPos( D3DXVECTOR3 position )
{
	m_position = position;
}

//--------------------------------------------------------------------------------------
//  大きさの代入
//--------------------------------------------------------------------------------------
void SenceFBX::SetScale( D3DXVECTOR3 scale )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  回転角の代入
//--------------------------------------------------------------------------------------
void SenceFBX::SetRot( float fRot )
{
	m_fRot = fRot;
}

//--------------------------------------------------------------------------------------
//  大きさ倍率の代入
//--------------------------------------------------------------------------------------
void SenceFBX::SetScale( float fScale )
{
	m_fScale = fScale;
}

//--------------------------------------------------------------------------------------
//  座標の移動
//--------------------------------------------------------------------------------------
void SenceFBX::MovePos( D3DXVECTOR3 movePos )
{
	m_position += movePos;
}

//--------------------------------------------------------------------------------------
//  回転角の変化
//--------------------------------------------------------------------------------------
void SenceFBX::ChangeRot( float fChangeRot )
{
	m_fRot += fChangeRot;
}

//--------------------------------------------------------------------------------------
//  大きさ倍率の変化
//--------------------------------------------------------------------------------------
void SenceFBX::ChangeScale( float fChangeScale )
{
	m_fScale += fChangeScale;
}

//--------------------------------------------------------------------------------------
//  現在のファイルポインタからある文字列まで検索する関数
//--------------------------------------------------------------------------------------
int SenceFBX::GetStrToken( FILE* pFile , const char* pToken , char* pBuf )
{
	int		nCntStr = 0;
	char	buf;

	//  1文字分の文字を格納
	while( ( buf = ( char )fgetc( pFile ) ) != EOF )
	{
		//  検索文字列の長さ分の検索
		for( unsigned int nCntBuf = 0; nCntBuf < strlen( pToken ); nCntBuf++ )
		{
			if( buf == pToken[ nCntBuf ] )
			{
				pBuf[ nCntStr ] = 0;

				return nCntStr;
			}
		}

		//  文字列の代入
		pBuf[ nCntStr++ ] = buf;
	}

	return -1;
}

//--------------------------------------------------------------------------------------
//  現在のファイルポインタからある文字列の数をカウントする関数
//--------------------------------------------------------------------------------------
int SenceFBX::GetStrCount( FILE* pFile , const char* pToken , char* pStr )
{
	char	aBuf[ 256 ];							//  文字列格納用
	int		nCntStr = 0;							//  数カウント用

	//  文字列を区切りごとに検索
	while( GetStrToken( pFile , pToken , aBuf ) != -1 )
	{
		//  文字列を比べる
		if( strcmp( pStr , aBuf ) == 0 )
		{
			nCntStr++;
		}
	}

	return nCntStr;
}

/* Tab character ("\t") counter */


//--------------------------------------------------------------------------------------
//  現在のファイルポインタからある文字列の数をカウントする関数
//--------------------------------------------------------------------------------------
void SenceFBX::PrintTabs( ) 
{
    for(int i = 0; i < numTabs; i++)
        printf("\t");
}

//--------------------------------------------------------------------------------------
//  現在のファイルポインタからある文字列の数をカウントする関数
//--------------------------------------------------------------------------------------
FbxString SenceFBX::GetAttributeTypeName(FbxNodeAttribute::EType type) 
{ 
    switch(type) { 
        case FbxNodeAttribute::eUnknown: return "unidentified"; 
        case FbxNodeAttribute::eNull: return "null"; 
        case FbxNodeAttribute::eMarker: return "marker"; 
        case FbxNodeAttribute::eSkeleton: return "skeleton"; 
        case FbxNodeAttribute::eMesh: return "mesh"; 
        case FbxNodeAttribute::eNurbs: return "nurbs"; 
        case FbxNodeAttribute::ePatch: return "patch"; 
        case FbxNodeAttribute::eCamera: return "camera"; 
        case FbxNodeAttribute::eCameraStereo: return "stereo"; 
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher"; 
        case FbxNodeAttribute::eLight: return "light"; 
        case FbxNodeAttribute::eOpticalReference: return "optical reference"; 
        case FbxNodeAttribute::eOpticalMarker: return "marker"; 
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve"; 
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface"; 
        case FbxNodeAttribute::eBoundary: return "boundary"; 
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface"; 
        case FbxNodeAttribute::eShape: return "shape"; 
        case FbxNodeAttribute::eLODGroup: return "lodgroup"; 
        case FbxNodeAttribute::eSubDiv: return "subdiv"; 
        default: return "unknown"; 
    } 
}

//--------------------------------------------------------------------------------------
//  現在のファイルポインタからある文字列の数をカウントする関数
//--------------------------------------------------------------------------------------
void SenceFBX::PrintAttribute(FbxNodeAttribute* pAttribute) 
{
    if(!pAttribute) return;
 
    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

//--------------------------------------------------------------------------------------
//  現在のファイルポインタからある文字列の数をカウントする関数
//--------------------------------------------------------------------------------------
void SenceFBX::PrintNode( FILE* pFile , FbxNode* pNode ) 
{
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get(); 
    FbxDouble3 rotation = pNode->LclRotation.Get(); 
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    fprintf( pFile , "<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n", 
        nodeName, 
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
        );
    numTabs++;

    // Print the node's attributes.
    for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for(int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode( pFile , pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}

//--------------------------------------------------------------------------------------
//  再帰的に必要なデータを取得する関数
//--------------------------------------------------------------------------------------
SenceFBX::myNode* SenceFBX::RecursiveNode( FbxNode* pNode ) 
{
	myNode* p = NULL;

	if( pNode != NULL )
	{
		p = new myNode;
		p->name = pNode->GetName( );
		p->translation = pNode->LclTranslation.Get( );
		p->rotation = pNode->LclRotation.Get( );
		p->scaling = pNode->LclScaling.Get( );

		for( int i = 0; i < pNode->GetNodeAttributeCount( ); i++ )
		{
			FbxNodeAttribute::EType type = pNode->GetNodeAttributeByIndex( i )->GetAttributeType( );
			p->attributeNames.push_back( GetAttributeTypeName( type ) );

			//FbxString str = pNode->GetNodeAttributeByIndex( i )->GetName( );
		 
			if( type == FbxNodeAttribute::eMesh )
			{
				//  メッシュ情報の取得
				FbxMesh* pMesh = pNode->GetMesh( );

				//  座標の取得
				AnalizePosition( pMesh );

				//  法線の取得
				AnalizeNormalize( pMesh );

				//  マテリアル情報の取得
				AnalizeMaterial( pMesh );

				//  UV座標の取得
				AnalizeUV( pMesh );

				//  テクスチャ名の取得
				AnalizeTextureName( pMesh );

				//  
				AnalizeCluster( pMesh );
			}
		}

		for( int i = 0; i < pNode->GetChildCount( ); i++ )
		{
			//  子供のデータも取得
			p->childlen.push_back( RecursiveNode( pNode->GetChild( i ) ) );
		}

		return p;
	}

	return NULL;
}

//--------------------------------------------------------------------------------------
//  再帰的に必要なデータを取得する関数
//--------------------------------------------------------------------------------------
void SenceFBX::AnalizePosition( FbxMesh* pMesh ) 
{
	if( pMesh != NULL )
	{
		//  頂点情報の取得
		m_nCntV = pMesh->GetControlPointsCount( );

		//  頂点バッファの作成
		m_pV = new Vector3D[ m_nCntV ];

		//  頂点情報の取得
		FbxVector4* pFbxV = pMesh->GetControlPoints( );

		for( int i = 0; i < m_nCntV; i++ )
		{
			m_pV[ i ].x = static_cast< float >( pFbxV[ i ][ 0 ] );
			m_pV[ i ].y = static_cast< float >( pFbxV[ i ][ 1 ] );
			m_pV[ i ].z = static_cast< float >( pFbxV[ i ][ 2 ] );
		}

		//  頂点インデックス番号の取得
		m_nCntF = pMesh->GetPolygonVertexCount( );
		m_pFace = new Face[ m_nCntF ];
		int* pFbxIndex = pMesh->GetPolygonVertices( );

		for( int i = 0; i < m_nCntF; i++ )
		{
			m_pFace[ i ].v = static_cast< unsigned short >( pFbxIndex[ i ] );
		}
	}
}

//--------------------------------------------------------------------------------------
//  法線データを取得する関数
//--------------------------------------------------------------------------------------
void SenceFBX::AnalizeNormalize( FbxMesh* pMesh ) 
{
	if( pMesh != NULL )
	{
		//  レイヤー数の取得
		int layerCount = pMesh->GetLayerCount( );

		if( layerCount > 1 )
		{
			//MessageBox( NULL , "法線を持たないモデルです( sceneModel.cpp )" , "<注意メッセージ>" , MB_OK );
		}
		else if( layerCount == 0 )
		{
			MessageBox( NULL , "法線を持たないモデルです( sceneModel.cpp )" , "<注意メッセージ>" , MB_OK );
		}

		//--- レイヤー数だけ回る ---//
		for ( int i = 0; i < layerCount; i++ ) 
		{
			//--- 法線セットを取得 ---//
			FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal( i );

			if( pNormal == NULL )
			{
				//MessageBox( NULL , "法線のないメッシュレイヤーです( sceneModel.cpp )" , "<注意メッセージ>" , MB_OK );
			}

			//--- マッピングモードの取得
			FbxGeometryElement::EMappingMode mapping = pNormal->GetMappingMode( );

			//--- リファレンスモードの取得 ---//
			FbxGeometryElement::EReferenceMode reference = pNormal->GetReferenceMode( );

			//--- マッピングモードの判別 ---//
			switch( mapping ) 
			{
				case FbxGeometryElement::eByControlPoint:
				{		  
					break;
				}
				case FbxGeometryElement::eByPolygonVertex:
				{
					//  リファレンスモードの判別 
					switch( reference ) 
					{
						case FbxGeometryElement::eDirect:
						{
							//--- 法線数を取得 ---//
							m_nCntVn = pNormal->GetDirectArray( ).GetCount( );

							//  頂点バッファの作成
							m_pVn = new Vector3D[ m_nCntVn ];
		          
							//-----------------------------------------------------------------------
							// eDirect の場合データは順番に格納されているのでそのまま保持
							//-----------------------------------------------------------------------
							for(int i = 0; i < m_nCntVn; i++) 
							{
								//--- 法線の取得 ---//
								m_pVn[ i ].x = static_cast< float >( pNormal->GetDirectArray().GetAt( i )[ 0 ] );
								m_pVn[ i ].y = static_cast< float >( pNormal->GetDirectArray().GetAt( i )[ 1 ] );
								m_pVn[ i ].z = static_cast< float >( pNormal->GetDirectArray().GetAt( i )[ 2 ] );

								m_pFace[ i ].vn = i;
							}

							break;
						}					
						case FbxGeometryElement::eIndexToDirect: 
						{
							//--- 法線数を取得 ---//
							m_nCntVn = pNormal->GetDirectArray( ).GetCount( );

							//  頂点バッファの作成
							m_pVn = new Vector3D[ m_nCntVn ];
		          
							//-----------------------------------------------------------------------
							// eDirect の場合データは順番に格納されているのでそのまま保持
							//-----------------------------------------------------------------------
							for(int i = 0; i < m_nCntVn; i++) 
							{
								//--- 法線の取得 ---//
								m_pVn[ i ].x = static_cast< float >( pNormal->GetDirectArray().GetAt( i )[ 0 ] );
								m_pVn[ i ].y = static_cast< float >( pNormal->GetDirectArray().GetAt( i )[ 1 ] );
								m_pVn[ i ].z = static_cast< float >( pNormal->GetDirectArray().GetAt( i )[ 2 ] );
							}

							//--- 法線数を取得 ---//
							int normalIdxCount = pNormal->GetIndexArray( ).GetCount( );
		          
							//-----------------------------------------------------------------------
							// eDirect の場合データは順番に格納されているのでそのまま保持
							//-----------------------------------------------------------------------
							for(int i = 0; i < normalIdxCount; i++) 
							{
								//--- 法線の取得 ---//
								m_pFace[ i ].vn = static_cast< unsigned short >( pNormal->GetIndexArray().GetAt( i ) );
							}

							break;
						}
						default:
						{
							break;
						}
					}

					break;
				}
				default:
				{
					break;
				}
			}

			break;
		}
	}
}

//--------------------------------------------------------------------------------------
//  マテリアルデータを取得する関数
//--------------------------------------------------------------------------------------
void SenceFBX::AnalizeMaterial( FbxMesh* pMesh ) 
{
	int nNumLayer = pMesh->GetLayerCount( );

	if( nNumLayer > 0 )
	{

	}
	else if( !nNumLayer )
	{
		return;
	}

	//for( int nCntLayer = 0; nCntLayer < nNumLayer; ++nCntLayer )
	//{
	//	FbxLayerElementMaterial* pElementMaterial = pMesh->GetLayer( nCntLayer )->GetMaterials( );

	//	int nNumMaterialIndex = pElementMaterial->GetIndexArray( ).GetCount( );
	//}

	//if( mappinMode == FbxLayerElement::eAllSame )
	//{
	//	if( referenceMode == FbxLayerElement::eIndexToDirect )
	//	{

	//	}
	//}
	//else if( mappinMode == FbxLayerElement::eByPolygon )
	//{
	//	if( referenceMode == FbxLayerElement::eIndexToDirect )
	//	{
	//		//  ポリゴン毎にマテリアルインデックス
	//	}
	//}

	FbxNode* pNode = pMesh->GetNode();

	//  マテリアル数の取得
	const int nNumMaterial = pNode->GetMaterialCount( );

	for( int nCntMaterial = 0; nCntMaterial < nNumMaterial; ++nCntMaterial )
	{
		//  マテリアル取得。 
		FbxSurfaceMaterial* pMat = pNode->GetMaterial( nCntMaterial ); 

		//  ディフューズプロパティ
		FbxProperty diffuseProperty = pMat->FindProperty( FbxSurfaceMaterial::sDiffuse );

		//  プロパティが持っているレイヤードテクスチャ
		int nNumLayeredTexture = diffuseProperty.GetSrcObjectCount<FbxLayeredTexture>();

		//if( nNumLayeredTexture == 0 )
		{
			//  テクスチャ数の取得
			int nNumTexture = diffuseProperty.GetSrcObjectCount< FbxFileTexture >( );

			//  テクスチャの数分のテクスチャ情報取得のループ
			for( int nCntTexture = 0; nCntTexture < nNumTexture; nCntTexture++ )
			{
				FbxFileTexture* pTexture = diffuseProperty.GetSrcObject< FbxFileTexture >( nCntTexture );

				if( pTexture != NULL ) 
				{
					//std::string textureName = pTexture->GetFileName();
					std::string textureName = pTexture->GetRelativeFileName( );

					//MessageBox( NULL , textureName.c_str( ) , "test" , MB_OK );
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  クラスターデータを取得する関数
//--------------------------------------------------------------------------------------
void SenceFBX::AnalizeCluster( FbxMesh* pMesh ) 
{
	//  スキンの数の取得
	int nSkinCount = pMesh->GetDeformerCount( FbxDeformer::eSkin );

	//  スキンの数分のループ
	for( int nCntSkin = 0; nCntSkin < nSkinCount; ++nCntSkin )
	{
		//  スキンを取得
		FbxSkin* pSkin = FbxCast< FbxSkin >( pMesh->GetDeformer( nCntSkin , FbxDeformer::eSkin ) );

		//  クラスターの数を取得
		int nClusterCount = pSkin->GetClusterCount( );

		//  スキンの数分のループ
		for( int nCntCluster = 0; nCntCluster < nClusterCount; ++nCntCluster )
		{
			//  クラスターを取得
			FbxCluster* pCluster = pSkin->GetCluster( nCntCluster );

			//  このクラスタが影響を及ぼす頂点インデックスの取得
			int nPointIndexCount = pCluster->GetControlPointIndicesCount( );

			if( !nPointIndexCount )
			{
				continue;
			}

			//  初期姿勢行列の取得
			FbxAMatrix initMatrix;
			pCluster->GetTransformLinkMatrix( initMatrix );

			//  戻すの時の行列
			FbxAMatrix lReferenceGlobalInitPosition;
			pCluster->GetTransformMatrix( lReferenceGlobalInitPosition );

			FbxAMatrix mtxGeometry;
			mtxGeometry.SetT( pMesh->GetNode( )->GetGeometricTranslation( FbxNode::eSourcePivot ) );
			mtxGeometry.SetR( pMesh->GetNode( )->GetGeometricRotation( FbxNode::eSourcePivot ) );
			mtxGeometry.SetS( pMesh->GetNode( )->GetGeometricScaling( FbxNode::eSourcePivot ) );

			lReferenceGlobalInitPosition *= mtxGeometry;

			//  姿勢マトリクスを取得してみる
			FbxAMatrix matrix = pCluster->GetLink( )->GetScene( )->GetAnimationEvaluator( )->GetNodeGlobalTransform( pCluster->GetLink( ) , 0 );

			int	pointNum = pCluster->GetControlPointIndicesCount( );

			//  そのボーンの影響を受ける頂点全ての情報
			int* nPointIndexArray = pCluster->GetControlPointIndices( );

			//  その頂点毎の影響度合いを代入
			double* dwWeightArray = pCluster->GetControlPointWeights( ); 

			for ( int i = 0; i < pointNum; ++i ) 
			{
			   // 頂点インデックスとウェイトを取得
			   int   index  = nPointIndexArray[ i ];
			   float weight = ( float )dwWeightArray[ i ];
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  UVデータを取得する関数
//--------------------------------------------------------------------------------------
void SenceFBX::AnalizeUV( FbxMesh* pMesh ) 
{
	//  UVセット数を取得
	int UVLayerCount = pMesh->GetElementUVCount( );

	for ( int i = 0; i < UVLayerCount; i++ ) 
	{
		//  UVバッファを取得
		FbxGeometryElementUV* UV = pMesh->GetElementUV(i);

		//  マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();

		//  リファレンスモードの取得
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();

		//  UV数を取得
		int uvCount = UV->GetDirectArray().GetCount();
    
		//  マッピングモードの判別
		switch( mapping ) 
		{
			case FbxGeometryElement::eByControlPoint:    
			{
				break;
			}
			case FbxGeometryElement::eByPolygonVertex:
			{
				//  リファレンスモードの判別
				switch( reference ) 
				{
					case FbxGeometryElement::eDirect:
					{
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						FbxLayerElementArray* uvIndex = &UV->GetIndexArray( );
						int uvIndexCount = uvIndex->GetCount( );
            
						//--- UVを保持 ---// 
						point2 temp;

						for(int i = 0; i < uvIndexCount; i++) 
						{
							temp.x = ( float )UV->GetDirectArray( ).GetAt( i )[ 0 ];

							temp.y = 1.0f - ( float )UV->GetDirectArray( ).GetAt( i )[ 1 ];

							m_uvSet.uvBuffer.push_back(temp);
						}
            
						//--- UVSet名を取得 ---//
						m_uvSet.uvSetName = UV->GetName();
					
						break;
					}
					default:
					{
						break;
					}
				}

				break;
			}
			case FbxGeometryElement::eByEdge:
			{
				break;
			}
			case FbxGeometryElement::eByPolygon:
			{
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  テクスチャ名データを取得する関数
//--------------------------------------------------------------------------------------
void SenceFBX::AnalizeTextureName( FbxMesh* pMesh ) 
{
  //--- メッシュからノードを取得 ---//
  FbxNode* node = pMesh->GetNode();

  //--- マテリアルの数を取得 ---//
  int materialCount = node->GetMaterialCount();

	//--- マテリアルの数だけ繰り返す ---//
	for (int i = 0; materialCount > i; i++) 
	{ 
		//--- マテリアルを取得 ---//
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

		////--- FbxLayeredTexture の数を取得 ---//
		//int layeredTextureCount = prop.GetSrcSceneCount();

		////--- アタッチされたテクスチャが FbxLayeredTexture の場合 ---//
		//if(0 < layeredTextureCount) 
		//{
		//	//--- アタッチされたテクスチャの数だけ繰り返す ---//
		//	for(int j = 0; layeredTextureCount > j; j++) 
		//	{
		//		//--- テクスチャを取得 ---//
		//		FbxLayeredTexture* layeredTexture = prop.GetSrcScene<FbxLayeredTexture>(j);

		//		//--- レイヤー数を取得 ---//
		//		int textureCount = layeredTexture->GetSrcSceneCount<FbxFileTexture>();

		//		//--- レイヤー数だけ繰り返す ---//
		//		for(int k = 0; textureCount > k; k++) 
		//		{
		//			//--- テクスチャを取得 ---//
		//			FbxFileTexture* texture = prop.GetSrcScene<FbxFileTexture>(k);
  //  
		//			if(texture) 
		//			{
		//				//--- テクスチャ名を取得 ---//
		//				//std::string textureName = texture->GetName();
		//				std::string textureName = texture->GetRelativeFileName();

		//				//--- UVSet名を取得 ---//
		//				std::string UVSetName = texture->UVSet.Get().Buffer();

		//				//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
		//				if(m_uvSet.uvSetName == UVSetName) 
		//				{
		//					m_uvSet.textures.push_back(textureName);
		//				}
		//			}
		//		}
		//	}
		//}
		//else 
		{
			//--- テクスチャ数を取得 ---//
			int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			if(0 < fileTextureCount) 
			{
				//--- テクスチャの数だけ繰り返す ---//
				for(int j = 0; fileTextureCount > j; j++) 
				{
					//--- テクスチャを取得 ---//
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(j);

					if(texture) 
					{
						//--- テクスチャ名を取得 ---//
						//std::string textureName = texture->GetName();
						std::string textureName = texture->GetRelativeFileName();

						//--- UVSet名を取得 ---//
						std::string UVSetName = texture->UVSet.Get().Buffer();

						//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
						if(m_uvSet.uvSetName == UVSetName) 
						{
							m_uvSet.textures.push_back(textureName);
						}
					}
				}
			}
		}
	}
}