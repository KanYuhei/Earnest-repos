//--------------------------------------------------------------------------------------
//  ObjectFBX  ( objectFBX.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "objectFBX.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "test.h"
#include "utility.h"
#include "shaderManager.h"
#include "light.h"
#include <algorithm>

//--------------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------------
ObjectFBX::ObjectFBX( )
{
	m_startTime = 0;
	m_endTime = 0;
	m_currentFrame = 0;
	m_allTime = 0;
	m_scale = 1.0f;
}

//--------------------------------------------------------------------------------------
//  デスストラクタ
//--------------------------------------------------------------------------------------
ObjectFBX::~ObjectFBX( )
{

}

//--------------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------------
HRESULT ObjectFBX::Init( void )
{
	//  FBXモデル読み込み実験用プログラム
	FbxManager* lSdkManager = FbxManager::Create( ); 
 
	// Create the IO settings object. 
	FbxIOSettings *ios = FbxIOSettings::Create( lSdkManager , IOSROOT ); lSdkManager->SetIOSettings( ios ); 
 
	// Create an importer using the SDK manager. 
	FbxImporter* lImporter = FbxImporter::Create( lSdkManager ,"" ); 
 
	// Use the first argument as the filename for the importer. 
	if( !lImporter->Initialize( "data/FBX/player.fbx" , -1 , lSdkManager->GetIOSettings( ) ) ) 
	{      
		char buf[ 256 ];     
		sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
		MessageBox( NULL , buf , "error" , MB_OK );     
		return E_FAIL; 
	}

	// Create a new scene so that it can be populated by the imported file. 
	FbxScene* lScene = FbxScene::Create( lSdkManager , "myScene" ); 
 
	// Import the contents of the file into the scene. 
	lImporter->Import( lScene ); 
 
	FbxGeometryConverter converter( lSdkManager ); 
	
	// 三角ポリゴン化 
	converter.Triangulate( lScene , true );
	
	// 縮退メッシュを削除 
	converter.RemoveBadPolygonsFromMeshes( lScene ); 
	
	// マテリアルごとにメッシュ分離 
	converter.SplitMeshesPerMaterial( lScene , true ); 

	//  アニメーション個数の取得
	lImporter->GetAnimStackCount( );
 
	//  メッシュ数の取得
	int numberMesh = lScene->GetMemberCount< FbxMesh >( );

	//  マテリアル数の取得
	int numberMaterial = lScene->GetMaterialCount( );

	//  メッシュの数分のループ
	for( int countMesh = 0; countMesh < numberMesh; ++countMesh )
	{
		m_meshes.push_back( Mesh( ) );

		//  指定メッシュの情報の取得
		FbxMesh* mesh = lScene->GetSrcObject< FbxMesh >( countMesh );

		//  座標の取得
		AnalyzePosition( mesh );

		//  法線解析                 
		AnalyzeNormal( mesh );                 
				
		//  UV解析                 
		AnalyzeTexcoord( mesh , true );   

		//  マテリアル解析（参照情報の取得）                 
		AnalyzeMaterial( mesh ); 

		//  ボーン解析
		AnalyzeCluster( lScene , mesh );

		//  インデックス
		FbxLayerElementArrayTemplate< int >* index;

		//  
		if( mesh->GetMaterialIndices( &index ) && index->GetCount( ) > 0 )
		{
			// マテリアル情報の取得     
			FbxSurfaceMaterial* material = mesh->GetNode( )->GetMaterial( index->GetAt( 0 ) ); 

			//  マテリアル名の保存
			m_meshes.back( ).materialName = material->GetName( ); 
		}
	}

	//  テクスチャ情報の取得
	AnalyzeTexture( lScene , numberMaterial );
	
	//  頂点データの変換
	ConvertVertex( );

	//  メッシュ分の頂点バッファを生成
	CreateMeshVertex( );

	lImporter->Destroy( ); 
	lScene->Destroy( ); 
	lSdkManager->Destroy( ); 

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------------
void ObjectFBX::Uninit( void )
{
	Scene::Release( );
}

//--------------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------------
void ObjectFBX::Update( void )
{
	m_currentFrame += 1;
}

//--------------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------------
void ObjectFBX::Draw( void )
{  
	Camera* camera = SceneManager::GetCamera( );

	D3DXVECTOR3 cameraVectorDirect;
	D3DXVECTOR3 cameraToPlayer;

	if( camera != nullptr )
	{
		D3DXVECTOR3 cameraPosition = camera->GetCameraPosEye( );
		D3DXVECTOR3 playerPosition = m_position;
		cameraPosition.y = 0.0f;
		playerPosition.y = 0.0f;

		cameraToPlayer = playerPosition - cameraPosition;
		D3DXVec3Normalize( &cameraToPlayer , &cameraToPlayer );
		cameraVectorDirect = camera->GetCameraVecDirect( );
		cameraVectorDirect.y = 0.0f;
		D3DXVec3Normalize( &cameraVectorDirect , &cameraVectorDirect );
	}

	if( Utility::IntoView( cameraVectorDirect , cameraToPlayer , D3DX_PI * 0.5f ) )
	{
		//  再帰的に描画
		recursiveDraw( ); 
	}
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に描画処理
//--------------------------------------------------------------------------------------
void ObjectFBX::recursiveDraw( void )
{ 
	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  拡大行列
	D3DXMATRIX mtxRot;								//  ビュー座標変換行列

	int countLoop = 0;

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  シェーダー情報の取得
	Shader3D* shader3D = ( Shader3D* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D );

	if( shader3D != nullptr )
	{
		//  テクスチャクラスの取得
		Texture* pTexture = SceneManager::GetTexture( );

		for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
		{  
			if( itm->matrixes.empty( ) ) 
			{
				// 頂点の座標変換             
				std::vector< D3DXVECTOR3 > positions;
				int size = itm->points.size( );

				positions.reserve( size ); 
 
				D3DXMATRIX mtx;             
				
				for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
				{                
					ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

					D3DXVECTOR3 pos = it->positions;                              
					positions.push_back( pos );             
				} 

				VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

				if( m_pVtxBuffers.at( countLoop ) != NULL )
				{
					//  頂点バッファをロックして、仮想アドレスを取得する
					m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
														 ( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
														 0 );							//  ロックの種類

					int size = itm->positionIndices.size( );

					for( int countIndex = 0; countIndex < size; ++countIndex )
					{
						//  頂点座標の設定( 3D座標 ・ 右回り )
						pVtx[ countIndex ].position = positions[ itm->positionIndices[ countIndex ] ];

						//  法線の指定
						pVtx[ countIndex ].normal = itm->normals[ itm->normalIndices[ countIndex ] ];

						//  頂点色の設定( 0 ～ 255 の整数値 )
						pVtx[ countIndex ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

						//  UV座標の指定
						pVtx[ countIndex ].texcoord = itm->texcoords[ itm->texcoordIndices[ countIndex ] ];
					}

					//  頂点バッファのアンロック
					m_pVtxBuffers.at( countLoop )->Unlock( );     
				}

				D3DXMatrixIdentity( &mtxWorld );
				D3DXMatrixIdentity( &mtxScale );
				D3DXMatrixIdentity( &mtxRot );

				//  拡大行列の作成
				D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

				//  拡大行列の掛け算
				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

				//  平行移動行列の作成
				D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

				//  平行移動行列の掛け算
				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

				//  ライトの方向ベクトルを取得する
				D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( );
				D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );

				//  ローカル単位ベクトルに変更
				D3DXVECTOR3 lightDirectLocal;

				D3DXMATRIX worldInverseMatrix;
				D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
				D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
				D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );

				Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
				D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
				D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
				D3DXCOLOR lightDiffuseColor = SceneManager::GetLight( )->GetLight( 0 ).Diffuse;

				//  シェーダーに必要な情報の設定
				shader3D->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor );

				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource( 0 ,								//  パイプライン番号
										  m_pVtxBuffers.at( countLoop ) ,	//  頂点バッファのアドレス
										  0 ,								//  オフセット( byte )
										  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

				// テクスチャの設定
				pDevice->SetTexture( 0 , pTexture->GetTextureImage( "data/FBX/texture/Head.png" ) );

				//  シェーダー3Dの描画開始
				shader3D->DrawBegin( );

				// ポリゴンの描画
				pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
										0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
										itm->positionIndices.size( ) / 3 );		//  プリミティブ数  

				//  シェーダー3Dの描画終了
				ShaderManager::DrawEnd( );
			}      
			else 
			{
				// 骨あり（つまりワンスキンなど） 
 
				// 頂点の座標変換             
				std::vector< D3DXVECTOR3 > positions;
				int size = itm->points.size( );

				positions.reserve( size ); 
 
				D3DXMATRIX mtx;             
				
				for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
				{                
					ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

					for( auto itb = it->bornRefarences.begin( ); itb != it->bornRefarences.end( ); ++itb ) 
					{                    
						mtx += itm->matrixes[ itb->index ][ m_currentFrame % m_allTime ] * itb->weight;
					}

					D3DXVECTOR3 pos = it->positions;                 
					D3DXVec3TransformCoord( &pos , &it->positions , &mtx );                
					positions.push_back( pos );             
				} 

				VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

				if( m_pVtxBuffers.at( countLoop ) != NULL )
				{
					//  頂点バッファをロックして、仮想アドレスを取得する
					m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
														 ( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
														 0 );							//  ロックの種類

					for( size_t i = 0; i < itm->positionIndices.size( ); i++ ) 
					{             
						//  頂点座標の設定( 3D座標 ・ 右回り )
						pVtx[ i ].position = positions[ itm->positionIndices[ i ] ];

						//  法線の指定
						pVtx[ i ].normal = itm->normals[ itm->normalIndices[ i ] ];

						//  頂点色の設定( 0 ～ 255 の整数値 )
						pVtx[ i ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

						//  UV座標の指定
						pVtx[ i ].texcoord = itm->texcoords[ itm->texcoordIndices[ i ] ];
					}

					//  頂点バッファのアンロック
					m_pVtxBuffers.at( countLoop )->Unlock( );     
				}

				D3DXMatrixIdentity( &mtxWorld );
				D3DXMatrixIdentity( &mtxScale );
				D3DXMatrixIdentity( &mtxRot );

				//  拡大行列の作成
				D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

				//  拡大行列の掛け算
				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

				//  平行移動行列の作成
				D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

				//  平行移動行列の掛け算
				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

				//  頂点フォーマットの設定
				shader3D->SetVertexDeclaration( );

				//  ワールド行列の設定
				shader3D->SetWorldMatrix( mtxWorld );

				//  ライトの方向ベクトルを取得する
				D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( );
				D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );

				//  ローカル単位ベクトルに変更
				D3DXVECTOR3 lightDirectLocal;

				D3DXMATRIX worldInverseMatrix;
				D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
				D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
				D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );

				Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
				D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
				D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
				D3DXCOLOR lightDiffuseColor = SceneManager::GetLight( )->GetLight( 0 ).Diffuse;

				//  シェーダーに必要な情報の設定
				shader3D->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor );

				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource( 0 ,								//  パイプライン番号
										  m_pVtxBuffers.at( countLoop ) ,	//  頂点バッファのアドレス
										  0 ,								//  オフセット( byte )
										  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

				// テクスチャの設定
				pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_textures[ itm->materialIndex ].c_str( ) ) );

				//  シェーダー3Dの描画開始
				shader3D->DrawBegin( );

				// ポリゴンの描画
				pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
										0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
										itm->positionIndices.size( ) / 3 );		//  プリミティブ数

				//  シェーダー3Dの描画終了
				ShaderManager::DrawEnd( );
			}

			//  ループカウント
			countLoop++;
		}
	}
}

//--------------------------------------------------------------------------------------
//  FBXのデプス値書き込み処理
//--------------------------------------------------------------------------------------
void ObjectFBX::DrawDepth( void )
{
	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  拡大行列
	D3DXMATRIX mtxRot;								//  ビュー座標変換行列

	int countLoop = 0;

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
	{  
		if( itm->matrixes.empty( ) ) 
		{
			// 頂点の座標変換             
			std::vector< D3DXVECTOR3 > positions;
			int size = itm->points.size( );

			positions.reserve( size ); 
 
			D3DXMATRIX mtx;             
				
			for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
			{                
				ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

				D3DXVECTOR3 pos = it->positions;                              
				positions.push_back( pos );             
			} 

			VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

			if( m_pVtxBuffers.at( countLoop ) != NULL )
			{
				//  頂点バッファをロックして、仮想アドレスを取得する
				m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
														( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
														0 );							//  ロックの種類

				int size = itm->positionIndices.size( );

				for( int countIndex = 0; countIndex < size; ++countIndex )
				{
					//  頂点座標の設定( 3D座標 ・ 右回り )
					pVtx[ countIndex ].position = positions[ itm->positionIndices[ countIndex ] ];

					//  法線の指定
					pVtx[ countIndex ].normal = itm->normals[ itm->normalIndices[ countIndex ] ];

					//  頂点色の設定( 0 ～ 255 の整数値 )
					pVtx[ countIndex ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

					//  UV座標の指定
					pVtx[ countIndex ].texcoord = itm->texcoords[ itm->texcoordIndices[ countIndex ] ];
				}

				//  頂点バッファのアンロック
				m_pVtxBuffers.at( countLoop )->Unlock( );     
			}

			D3DXMatrixIdentity( &mtxWorld );
			D3DXMatrixIdentity( &mtxScale );
			D3DXMatrixIdentity( &mtxRot );

			//  拡大行列の作成
			D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

			//  拡大行列の掛け算
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

			//  平行移動行列の作成
			D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

			//  平行移動行列の掛け算
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

			//  シェーダー情報の取得
			Shader3DShadowMap* shader3DShadowMap = ( Shader3DShadowMap* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_SHADOW_MAP );

			//  カメラクラスの取得
			Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

			D3DXMATRIX viewMatrix = SceneManager::GetLight( )->GetViewMatrix( );
			D3DXMATRIX projectionMatrix = SceneManager::GetLight( )->GetProjectionMatrix( );

			shader3DShadowMap->SetShaderInfo( mtxWorld , viewMatrix * projectionMatrix );

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource( 0 ,								//  パイプライン番号
										m_pVtxBuffers.at( countLoop ) ,	//  頂点バッファのアドレス
										0 ,								//  オフセット( byte )
										sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

			// テクスチャの設定
			pDevice->SetTexture( 0 , nullptr );

			//  シェーダー描画開始
			shader3DShadowMap->DrawBegin( );

			// ポリゴンの描画
			pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
									0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
									itm->positionIndices.size( ) / 3 );		//  プリミティブ数  

			//  描画終了
			ShaderManager::DrawEnd( );
		}      
		else 
		{
			// 骨あり（つまりワンスキンなど） 
 
			// 頂点の座標変換             
			std::vector< D3DXVECTOR3 > positions;
			int size = itm->points.size( );

			positions.reserve( size ); 
 
			D3DXMATRIX mtx;             
				
			for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
			{                
				ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

				for( auto itb = it->bornRefarences.begin( ); itb != it->bornRefarences.end( ); ++itb ) 
				{                    
					mtx += itm->matrixes[ itb->index ][ m_currentFrame % m_allTime ] * itb->weight;
				}

				D3DXVECTOR3 pos = it->positions;                 
				D3DXVec3TransformCoord( &pos , &it->positions , &mtx );                
				positions.push_back( pos );             
			} 

			VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

			if( m_pVtxBuffers.at( countLoop ) != NULL )
			{
				//  頂点バッファをロックして、仮想アドレスを取得する
				m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
														( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
														0 );							//  ロックの種類

				for( size_t i = 0; i < itm->positionIndices.size( ); i++ ) 
				{             
					//  頂点座標の設定( 3D座標 ・ 右回り )
					pVtx[ i ].position = positions[ itm->positionIndices[ i ] ];

					//  法線の指定
					pVtx[ i ].normal = itm->normals[ itm->normalIndices[ i ] ];

					//  頂点色の設定( 0 ～ 255 の整数値 )
					pVtx[ i ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

					//  UV座標の指定
					pVtx[ i ].texcoord = itm->texcoords[ itm->texcoordIndices[ i ] ];
				}

				//  頂点バッファのアンロック
				m_pVtxBuffers.at( countLoop )->Unlock( );     
			}

			D3DXMatrixIdentity( &mtxWorld );
			D3DXMatrixIdentity( &mtxScale );
			D3DXMatrixIdentity( &mtxRot );

			//  拡大行列の作成
			D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

			//  拡大行列の掛け算
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

			//  平行移動行列の作成
			D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

			//  平行移動行列の掛け算
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource( 0 ,								//  パイプライン番号
										m_pVtxBuffers.at( countLoop ) ,	//  頂点バッファのアドレス
										0 ,								//  オフセット( byte )
										sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

			//  シェーダー情報の取得
			Shader3DShadowMap* shader3DShadowMap = ( Shader3DShadowMap* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_SHADOW_MAP );

			//  カメラクラスの取得
			Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

			D3DXMATRIX viewMatrix = SceneManager::GetLight( )->GetViewMatrix( );
			D3DXMATRIX projectionMatrix = SceneManager::GetLight( )->GetProjectionMatrix( );

			shader3DShadowMap->SetShaderInfo( mtxWorld , viewMatrix * projectionMatrix );

			// テクスチャの設定
			pDevice->SetTexture( 0 , nullptr );

			//  シェーダー描画開始
			shader3DShadowMap->DrawBegin( );

			// ポリゴンの描画
			pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
									0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
									itm->positionIndices.size( ) / 3 );		//  プリミティブ数

			//  描画終了
			ShaderManager::DrawEnd( );
		}

		//  ループカウント
		countLoop++;
	}
}

////--------------------------------------------------------------------------------------
////  ノードを辿って再帰的に描画処理
////--------------------------------------------------------------------------------------
//void ObjectFBX::recursiveDraw( void )
//{ 
//	D3DXMATRIX mtxWorld;							//  ワールド行列
//	D3DXMATRIX mtxTrans;							//  平行移動行列
//	D3DXMATRIX mtxScale;							//  拡大行列
//	D3DXMATRIX mtxRot;								//  ビュー座標変換行列
//
//	int countLoop = 0;
//
//	//  デバイス情報の取得
//	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );
//
//	//  シェーダー情報の取得
//	Shader3D* shader3D = ( Shader3D* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D );
//
//	if( shader3D != nullptr )
//	{
//		//  テクスチャクラスの取得
//		Texture* pTexture = SceneManager::GetTexture( );
//
//		for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
//		{  
//			//if( itm->matrixes.empty( ) ) 
//			//{
//			//	// 頂点の座標変換             
//			//	std::vector< D3DXVECTOR3 > positions;
//			//	int size = itm->points.size( );
//
//			//	positions.reserve( size ); 
// 
//			//	D3DXMATRIX mtx;             
//			//	
//			//	for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
//			//	{                
//			//		ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   
//
//			//		D3DXVECTOR3 pos = it->positions;                              
//			//		positions.push_back( pos );             
//			//	} 
//
//			//	VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ
//
//			//	if( m_pVtxBuffers.at( countLoop ) != NULL )
//			//	{
//			//		//  頂点バッファをロックして、仮想アドレスを取得する
//			//		m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
//			//											 ( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
//			//											 0 );							//  ロックの種類
//
//			//		int size = itm->positionIndices.size( );
//
//			//		for( int countIndex = 0; countIndex < size; ++countIndex )
//			//		{
//			//			//  頂点座標の設定( 3D座標 ・ 右回り )
//			//			pVtx[ countIndex ].position = positions[ itm->positionIndices[ countIndex ] ];
//
//			//			//  法線の指定
//			//			pVtx[ countIndex ].normal = itm->normals[ itm->normalIndices[ countIndex ] ];
//
//			//			//  頂点色の設定( 0 ～ 255 の整数値 )
//			//			pVtx[ countIndex ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
//
//			//			//  UV座標の指定
//			//			pVtx[ countIndex ].texcoord = itm->texcoords[ itm->texcoordIndices[ countIndex ] ];
//			//		}
//
//			//		//  頂点バッファのアンロック
//			//		m_pVtxBuffers.at( countLoop )->Unlock( );     
//			//	}
//
//			//	D3DXMatrixIdentity( &mtxWorld );
//			//	D3DXMatrixIdentity( &mtxScale );
//			//	D3DXMatrixIdentity( &mtxRot );
//
//			//	//  拡大行列の作成
//			//	D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );
//
//			//	//  拡大行列の掛け算
//			//	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );
//
//			//	//  平行移動行列の作成
//			//	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );
//
//			//	//  平行移動行列の掛け算
//			//	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );
//
//			//	//  ライトの方向ベクトルを取得する
//			//	D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( 0 );
//			//	D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );
//
//			//	//  ローカル単位ベクトルに変更
//			//	D3DXVECTOR3 lightDirectLocal;
//
//			//	D3DXMATRIX worldInverseMatrix;
//			//	D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
//			//	D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
//			//	D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );
//
//			//	//  シェーダーに必要な情報の設定
//			//	shader3D->SetShaderInfo( mtxWorld , lightDirectLocal );
//
//			//	// 頂点バッファをデータストリームに設定
//			//	pDevice->SetStreamSource( 0 ,								//  パイプライン番号
//			//							  m_pVtxBuffers.at( countLoop ) ,	//  頂点バッファのアドレス
//			//							  0 ,								//  オフセット( byte )
//			//							  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )
//
//			//	// テクスチャの設定
//			//	pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_textures[ itm->materialIndex ].c_str( ) ) );
//
//			//	//  シェーダー3Dの描画開始
//			//	shader3D->DrawBegin( );
//
//			//	// ポリゴンの描画
//			//	pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
//			//							0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
//			//							itm->positionIndices.size( ) / 3 );		//  プリミティブ数  
//
//			//	//  シェーダー3Dの描画終了
//			//	ShaderManager::DrawEnd( );
//			//}      
//			//else 
//			{
//				// 骨あり（つまりワンスキンなど） 
// 
//				//// 頂点の座標変換             
//				//std::vector< D3DXVECTOR3 > positions;
//				//int size = itm->points.size( );
//
//				//positions.reserve( size ); 
// 
//				//D3DXMATRIX mtx;            
//
//				//for( auto it = itm->vertexList.begin( ); it != itm->vertexList.end( ); ++it )
//				//{
//				//	ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   
//
//				//	for( auto itb = it->bornReference.begin( ); itb != it->bornReference.end( ); ++itb ) 
//				//	{                    
//				//		mtx += itm->matrixes[ itb->index ][ m_currentFrame % m_allTime ] * itb->weight;
//				//	}
//
//				//	D3DXVECTOR3 pos = it->vertex.position;                 
//				//	D3DXVec3TransformCoord( &pos , &pos , &mtx );                
//				//	positions.push_back( pos );
//				//}
//
//				VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ
//
//				if( m_pVtxBuffers.at( countLoop ) != NULL )
//				{
//					//  頂点バッファをロックして、仮想アドレスを取得する
//					m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
//														 ( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
//														 0 );							//  ロックの種類
//
//					int i = 0;
//
//					for( int index = 0; index < itm->indices.size( ); ++index )
//					{      
//						//  頂点座標の設定( 3D座標 ・ 右回り )
//						pVtx[ itm->indices[ index ] ].position = itm->vertexList[ itm->indices[ index ] ].vertex.position;
//
//						//  法線の指定
//						pVtx[ itm->indices[ index ] ].normal = itm->vertexList[ itm->indices[ index ] ].vertex.normal;
//
//						//  頂点色の設定( 0 ～ 255 の整数値 )
//						pVtx[ itm->indices[ index ] ].color = itm->vertexList[ itm->indices[ index ] ].vertex.color;
//
//						//  UV座標の指定
//						pVtx[ itm->indices[ index ] ].texcoord = itm->vertexList[ itm->indices[ index ] ].vertex.texcoord;
//					}
//
//					//  頂点バッファのアンロック
//					m_pVtxBuffers.at( countLoop )->Unlock( );     
//				}
//
//				D3DXMatrixIdentity( &mtxWorld );
//				D3DXMatrixIdentity( &mtxScale );
//				D3DXMatrixIdentity( &mtxRot );
//
//				//  拡大行列の作成
//				D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );
//
//				//  拡大行列の掛け算
//				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );
//
//				//  平行移動行列の作成
//				D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );
//
//				//  平行移動行列の掛け算
//				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );
//
//				//  頂点フォーマットの設定
//				shader3D->SetVertexDeclaration( );
//
//				//  ワールド行列の設定
//				shader3D->SetWorldMatrix( mtxWorld );
//
//				//  ライトの方向ベクトルを取得する
//				D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( 0 );
//				D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );
//
//				//  ローカル単位ベクトルに変更
//				D3DXVECTOR3 lightDirectLocal;
//
//				D3DXMATRIX worldInverseMatrix;
//				D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
//				D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
//				D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );
//
//				//  シェーダーに必要な情報の設定
//				shader3D->SetShaderInfo( mtxWorld , lightDirectLocal );
//
//				// 頂点バッファをデータストリームに設定
//				pDevice->SetStreamSource( 0 ,								//  パイプライン番号
//										  m_pVtxBuffers.at( countLoop ) ,	//  頂点バッファのアドレス
//										  0 ,								//  オフセット( byte )
//										  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )
//
//				// テクスチャの設定
//				pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_textures[ itm->materialIndex ].c_str( ) ) );
//
//				//  シェーダー3Dの描画開始
//				shader3D->DrawBegin( );
//
//				// ポリゴンの描画
//				pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
//										0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
//										itm->positionIndices.size( ) / 3 );		//  プリミティブ数
//
//				//  シェーダー3Dの描画終了
//				ShaderManager::DrawEnd( );
//			}
//
//			//  ループカウント
//			countLoop++;
//		}
//	}
//}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzePosition( FbxMesh* pMesh ) 
{     
	// コントロールポイント数の取得     
	int controlPointsCount = pMesh->GetControlPointsCount( ); 
 
    // 頂点座標用コンテナの領域予約     
	m_meshes.back( ).points.reserve( controlPointsCount ); 
 
    // 頂点データの取得     
	FbxVector4* pP = pMesh->GetControlPoints( );     
	
	for( int i = 0; i < pMesh->GetControlPointsCount( ); i++ ) 
	{         
		m_meshes.back( ).points.push_back( D3DXVECTOR3( static_cast< float >( pP[ i ][ 0 ] ) ,                          
													    static_cast< float >( pP[ i ][ 1 ] ) ,                          
													    static_cast< float >( pP[ i ][ 2 ] ) ) );     
	} 
 
    /* 頂点インデックスの取得 */ 
 
    // インデックス数を取得     
	int polygonVertexCount = pMesh->GetPolygonVertexCount( ); 
 
    // 頂点座標インデックス用コンテナの領域予約     
	m_meshes.back( ).positionIndices.reserve( polygonVertexCount );          
	
	// インデックスバッファの取得     
	for( int i = 0; i < polygonVertexCount; i++ )
	{         
		m_meshes.back( ).positionIndices.push_back( static_cast< unsigned short >( pMesh->GetPolygonVertices( )[ i ] ) );     
	} 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeNormal( FbxMesh* pMesh ) 
{     
	// レイヤー数の取得     
	int layerCount = pMesh->GetLayerCount( ); 
 
    for(int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // 法線の取得         
		FbxLayerElementNormal* pElementNormal = pMesh->GetElementNormal( layer ); 
 
        if( !pElementNormal ) 
		{ 
			continue; 
		} 
 
        // 法線数の取得         
		int normalCount = pElementNormal->GetDirectArray( ).GetCount( ); 
 
        // 法線格納用コンテナの領域予約         
		m_meshes.back( ).normals.reserve( normalCount );
 
        // 法線データの取得         
		for( int i = 0; i < normalCount; i++ ) 
		{             
			m_meshes.back( ).normals.push_back( D3DXVECTOR3( static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 0 ] ) ,                 
														     static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 1 ] ) ,                 
														     static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 2 ] ) ) );         
		} 
 
        // マッピングモード・リファレンスモード取得         
		FbxLayerElement::EMappingMode mappingMode = pElementNormal->GetMappingMode( );         
		FbxLayerElement::EReferenceMode referenceMode = pElementNormal->GetReferenceMode( ); 
 
        switch( mappingMode ) 
		{ 
			case FbxLayerElement::eNone:  
			{
				MessageBox( NULL , "Normal MappingMode = mappingMode" , "未実装" , MB_OK );      
				break; 
			}
			case FbxLayerElement::eByControlPoint: 
			{
				// 頂点バッファと同じインデックスを使用 
				if( referenceMode == FbxLayerElement::eDirect ) 
				{                 
					// 法線インデックス格納用コンテナの領域予約                 
					m_meshes.back( ).normalIndices.reserve( m_meshes.back( ).points.size( ) );                 
				
					// 頂点バッファと同じインデックスをコピー                 
					m_meshes.back( ).normalIndices.assign( m_meshes.back( ).positionIndices.begin( ) , m_meshes.back( ).positionIndices.end( ) );             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{                 
					MessageBox( NULL ,  "Normal ReferenceMode = eIndexToDirect or eIndex, MappingMode = eByControlPoint" ,  "未実装" , MB_OK );             
				}             
			
				break; 
			}
			case FbxLayerElement::eByPolygonVertex:
			{
				/* 法線独自のインデックスを使用 */ 
 
				if( referenceMode == FbxLayerElement::eDirect ) 
				{                                  
					// インデックス参照の必要なし => インデックスを作成 
 
					// 法線インデックス格納用コンテナの領域予約                 
					m_meshes.back( ).normalIndices.reserve( normalCount );                 
				
					// 法線インデックスの作成                 
					for( int i = 0; i < normalCount; i++ ) 
					{                     
						m_meshes.back( ).normalIndices.push_back( i );                 
					}             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{ 
					// 独自インデックスを所持 
 
					// インデックス数の取得                 
					int normalIndexCount = pElementNormal->GetIndexArray( ).GetCount( );                 
				
					// 法線インデックス格納用コンテナの領域予約                 
					m_meshes.back( ).normalIndices.reserve( normalIndexCount );                 
				
					// 法線インデックスの取得                 
					for( int i = 0; i < normalIndexCount; i++ ) 
					{                     
						m_meshes.back( ).normalIndices.push_back( pElementNormal->GetIndexArray( )[ i ] );                 
					}             
				}

				break; 
			}
			case FbxLayerElement::eByPolygon:       
			{
				MessageBox( NULL , "Normal MappingMode = eByPolygon", "未実装" , MB_OK );             
				break;       
			}
			case FbxLayerElement::eByEdge: 
			{
				MessageBox( NULL , "Normal MappingMode = eByEdge" , "未実装" , MB_OK );             
				break;         
			}
			case FbxLayerElement::eAllSame:  
			{
				MessageBox( NULL , "Normal MappingMode = eAllSame" , "未実装" , MB_OK );            
				break;   
			}
			default: 
			{
				MessageBox( NULL , "Normal ???" , "未実装" , MB_OK );             
				break;         
			}     
		}
	} 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeTexcoord( FbxMesh* pMesh , bool bRevers ) 
{ 
	int layerCount = pMesh->GetLayerCount( ); 
 
	if( !layerCount ) 
	{ 
		MessageBox(NULL, "レイヤーを持っていないメッシュを確認", "Analyze Texcoord", MB_OK); 
		return; 
	} 
 
	for( int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // UVの取得         
		FbxLayerElementUV* pElementUV = pMesh->GetLayer( layer )->GetUVs( ); 
 
        if( !pElementUV ) 
		{             
			MessageBox( NULL , "...UVのないメッシュレイヤーを確認" , "Analyze Texcoord" , MB_OK );             
			continue;         
		} 
 
        // UVセット名を取得         
		// = pElementUV->GetName(); 
 
        // マッピングモード・リファレンスモード取得         
		FbxLayerElement::EMappingMode mappingMode = pElementUV->GetMappingMode( );         
		FbxLayerElement::EReferenceMode referenceMode = pElementUV->GetReferenceMode( ); 
 
        if( mappingMode == FbxLayerElement::eByPolygonVertex ) 
		{             
			if( referenceMode == FbxLayerElement::eIndexToDirect || referenceMode == FbxLayerElement::eIndex ) 
			{                 
				int uvIndexCount = pElementUV->GetIndexArray( ).GetCount( );                
				m_meshes.back( ).texcoordIndices.reserve( uvIndexCount ); 
 
                for( int i = 0; i < uvIndexCount; i++ ) 
				{                     
					m_meshes.back( ).texcoordIndices.push_back( pElementUV->GetIndexArray( ).GetAt( i ) );                 
				} 
 
                int uvCount = pElementUV->GetDirectArray( ).GetCount( );      

				m_meshes.back().texcoords.reserve( uvCount ); 
 
                for( int i = 0; i < uvCount; i++ ) 
				{ 
                    m_meshes.back( ).texcoords.push_back( D3DXVECTOR2( static_cast< float >( pElementUV->GetDirectArray( ).GetAt( i )[ 0 ]),                         
																	   static_cast< float >( bRevers ? 1 - pElementUV->GetDirectArray( ).GetAt( i )[ 1]  : pElementUV->GetDirectArray( ).GetAt( i )[ 1] ) ) );                 
				}             																   
			}             
			else 
			{                 
				MessageBox(NULL, "Texcoord::未対応のリファレンスモードを取得", "FbxLayerElement::eByPolygonVertex", MB_OK);                 
				break;             
			}         
		}         
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{             
			MessageBox( NULL , "...未対応マッピングモード[eByControlPoint]を取得した", "Analyze Texcoord" , MB_OK );         
		}         
		else if( mappingMode == FbxLayerElement::eByPolygon) 
		{             
			MessageBox( NULL , "...未対応マッピングモード[eByPolygon]を取得した", "Analyze Texcoord" , MB_OK );         
		}         
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{             
			MessageBox( NULL , "...未対応マッピングモード[eByEdge]を取得した", "Analyze Texcoord" , MB_OK );         
		}         
		else 
		{             
			MessageBox( NULL , "...知らないマッピングモードを取得した", "Analyze Texcoord" , MB_OK );         
		} 
 
        break; // とりあえず１個めだけ     
	} 
}
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeMaterial( FbxMesh* pMesh ) 
{     
	int layerCount = pMesh->GetLayerCount( ); 
 
    for(int layer = 0; layer < layerCount; layer++) 
	{ 
		FbxLayerElementMaterial* pElementMaterial = pMesh->GetLayer( layer )->GetMaterials( );

		if( !pElementMaterial ) 
		{ 
			continue; 
		} 
 
		int materialIndexCount = pElementMaterial->GetIndexArray( ).GetCount( ); 
 
		if( materialIndexCount == 0 ) 
		{ 
			continue; 
		} 
 
		FbxLayerElement::EMappingMode mappingMode = pElementMaterial->GetMappingMode( );     
		FbxLayerElement::EReferenceMode referenceMode = pElementMaterial->GetReferenceMode( ); 
 
		if( mappingMode == FbxLayerElement::eAllSame ) 
		{         
			if( referenceMode == FbxLayerElement::eIndexToDirect ) 
			{        
				// メッシュ全部がこのマテリアルインデックス         
				m_meshes.back( ).materialIndex = pElementMaterial->GetIndexArray( ).GetAt( 0 );         
			}         
			else 
			{         
				MessageBox(NULL, "...未対応のリファレンスモードを取得した", "Material MappingMode = eAllSame", MB_OK);         
			}     
		}     
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{         
			MessageBox(NULL, "...未対応のマッピングモード[eByControlPoint]を取得した", "Material MappingMode", MB_OK);     
		}     
		else if( mappingMode == FbxLayerElement::eByPolygon ) 
		{         
			// マテリアル分割されているはずだから、一番はじめのだけでいい         
			m_meshes.back( ).materialIndex = pElementMaterial->GetIndexArray( ).GetAt( 0 );     
		}     
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{         
			MessageBox(NULL, "...未対応のマッピングモード[eByEdge]を取得した", "Material MappingMode", MB_OK);     
		}     
		else 
		{         
			MessageBox(NULL, "...未対応のマッピングモードを取得した", "Material MappingMode", MB_OK);     
		}     
	} 
}

//--------------------------------------------------------------------------------------
//  テクスチャ情報の取得をする関数
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeTexture( FbxScene* lScene , int numberMaterial )
{
	//  マテリアルの数分のループ
	for( int countMaterial = 0; countMaterial < numberMaterial; ++countMaterial )
	{
		// マテリアル情報の取得     
		FbxSurfaceMaterial* pMaterial = lScene->GetMaterial( countMaterial ); 

		//  マテリアル名の取得
		std::string materialName = pMaterial->GetName( );

		// ディフューズ情報の取得     
		FbxProperty diffuseProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sDiffuse ); 
 
		// プロパティが持っているレイヤードテクスチャの枚数をチェック     
		int layeredTextureCount = diffuseProperty.GetSrcObjectCount< FbxLayeredTexture >( ); 
 
		// レイヤードテクスチャが無ければ通常テクスチャ     
		if ( layeredTextureCount == 0 ) 
		{ 
			// 通常テクスチャの枚数をチェック         
			int textureCount = diffuseProperty.GetSrcObjectCount< FbxFileTexture >( ); 
   
			// 各テクスチャについてテクスチャ情報をゲット         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i番目のテクスチャオブジェクト取得         
				FbxFileTexture* pTexture = diffuseProperty.GetSrcObject< FbxFileTexture >( i ); 

				// テクスチャファイル名の取得         
				//std::string fileName = pTexture->GetFileName( );         
				std::string relFileName = pTexture->GetRelativeFileName( ); 
 
				// UVSet名の取得         
				std::string uvSetName = pTexture->UVSet.Get().Buffer( ); 

				char* fileName = ( char* )relFileName.c_str( );
				char aTextureFileName[ 128 ] = { };

				int index = 0;

				//  文字数分のループ
				for( int nCntLength = strlen( fileName ) - 1; nCntLength >= 0 ; nCntLength-- )
				{
					//  テクスチャ名の取り出し
					aTextureFileName[ index ] = fileName[ nCntLength ];

					index++;

					//  \があるとループを抜け出す
					if( fileName[ nCntLength ] == '\\' )
					{
						break;
					}
				}

				//  \0の格納
				aTextureFileName[ index ] = '\0';

				//  インデックス番号の初期化
				index = 0;

				char aWork;											//  １文字分の格納用
				int	 nLength = strlen( aTextureFileName );			//  文字の長さ

				//  文字数分のループ
				for( int nCntLength = 0; nCntLength < nLength / 2; nCntLength++ )
				{
					//  テクスチャ名の反転
					aWork = aTextureFileName[ nCntLength ];
					aTextureFileName[ nCntLength ] = aTextureFileName[ nLength - nCntLength - 1 ];
					aTextureFileName[ nLength - nCntLength - 1 ] = aWork;
				}

				std::string file = aTextureFileName;
				std::string sub = file.substr( 1 , file.size( ) - 1 );

				// 辞書登録            
				std::string strPathName = "data/FBX/texture/" + sub;        

				//  テクスチャクラスの取得
				Texture* pTexture2 = SceneManager::GetTexture( );

				// テクスチャの読み込みと管理     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// テクスチャの読み込みと管理         
				m_textures.push_back( strPathName );    
			}         
		}     
		else 
		{ 
			// レイヤードテクスチャあり         
			MessageBox( NULL , "レイヤードテクスチャ" , "マテリアルの取得" , MB_OK );     
		}     
	}
}

//--------------------------------------------------------------------------------------
//  メッシュ分の頂点バッファを生成をする関数
//--------------------------------------------------------------------------------------
void ObjectFBX::CreateMeshVertex( void )
{
	//  メッシュの生成
	for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
	{  
		m_pVtxBuff = nullptr;
		m_pVtxBuffers.push_back( m_pVtxBuff );

		//  骨あり( つまりワンスキンなど )
		MakeVertex( itm->indices.size( ) );
	}
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
// Get the geometry offset to a node. It is never inherited by the children. 
FbxAMatrix ObjectFBX::GetGeometry(FbxNode* pNode) 
{     
	const FbxVector4 lT = pNode->GetGeometricTranslation( FbxNode::eSourcePivot );     
	const FbxVector4 lR = pNode->GetGeometricRotation( FbxNode::eSourcePivot );     
	const FbxVector4 lS = pNode->GetGeometricScaling( FbxNode::eSourcePivot ); 
 
    return FbxAMatrix( lT , lR , lS ); 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeCluster( FbxScene* scene , FbxMesh* pMesh ) 
{     
	D3DXMATRIX mtxIdentitiy;     
	D3DXMatrixIdentity( &mtxIdentitiy ); 
 
    // スキンの数を取得     
	int skinCount = pMesh->GetDeformerCount( FbxDeformer::eSkin ); 
 
    for( int skinNum = 0; skinNum < skinCount; skinNum++ ) 
	{ 
        // スキンを取得         
		FbxSkin* pSkin = ( FbxSkin* )pMesh->GetDeformer( skinNum , FbxDeformer::eSkin ); 
 
        // クラスターの数を取得         
		int clusterCount = pSkin->GetClusterCount( ); 
 
        for( int clusterNum = 0; clusterNum < clusterCount; clusterNum++ ) 
		{ 
			// クラスタを取得             
			FbxCluster* pCluster = pSkin->GetCluster( clusterNum ); 
 
			// このクラスタが影響を及ぼす頂点インデックスの個数を取得             
			int pointIndexCount = pCluster->GetControlPointIndicesCount( ); 
 
			m_meshes.back( ).matrixes.emplace_back( 0 );

			if( !pointIndexCount ) 
			{                 
				// このメッシュにおいて、このクラスタは無視していいと思う...                 
				m_meshes.back( ).matrixes.back( ).push_back( mtxIdentitiy );     

				continue;             
			} 
 
			// 初期姿勢行列の取得             
			FbxAMatrix lReferenceGlobalInitPosition;             
			FbxAMatrix lReferenceGlobalCurrentPosition;             
			FbxAMatrix lReferenceGeometry;             
			FbxAMatrix lClusterGlobalInitPosition;             
			FbxAMatrix lClusterGlobalCurrentPosition;             
			FbxAMatrix lClusterRelativeInitPosition;             
			FbxAMatrix lClusterRelativeCurrentPositionInverse; 
 
            pCluster->GetTransformMatrix( lReferenceGlobalInitPosition );             
			
			// lReferenceGlobalCurrentPosition = pGlobalPosition; // <- たぶんワールド座標変換行列ではないかと                          
			
			// Multiply lReferenceGlobalInitPosition by Geometric Transformation             
			lReferenceGeometry = GetGeometry( pMesh->GetNode( ) );             
			lReferenceGlobalInitPosition *= lReferenceGeometry; 
 
            // Get the link initial global position and the link current global position.             
			pCluster->GetTransformLinkMatrix( lClusterGlobalInitPosition );             
			
			// lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose); // <- ポーズ行列の取り方？？？             
			
			FbxTime oneFrameTime;
			oneFrameTime.SetTime( 0 , 0 , 0 , 1 , 0 , 0 , FbxTime::eFrames60 );

			//  アニメーション一覧の取得
			FbxArray< FbxString* > animationNames;
			scene->FillAnimStackNameArray( animationNames );

			//  アニメーション情報の取得
			FbxTakeInfo* pTakeInfo = scene->GetTakeInfo( animationNames[ 0 ]->Buffer( ) );

			//  アニメーションの開始・終了時間の取得
			FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
			FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop( );

			//  総合フレーム数の取得
			m_allTime = ( int )( ( endTime - startTime ) / oneFrameTime ).Get( );

			for( FbxTime currentTime = m_startTime; currentTime < m_endTime; currentTime += oneFrameTime )
			{
				lClusterGlobalCurrentPosition = pCluster->GetLink( )->EvaluateGlobalTransform( currentTime ); 
 
				// Compute the initial position of the link relative to the reference.             
				lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse( ) * lReferenceGlobalInitPosition; 
 
				// Compute the current position of the link relative to the reference.             
				lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse( ) * lClusterGlobalCurrentPosition; 
 
				// Compute the shift of the link relative to the reference.            
				FbxAMatrix VertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition; 
				// ↑ 初期姿勢行列も考慮されたモーションボーン行列なので、これで頂点座標を変換するだけで良い 
 
				D3DXMATRIX d3dMtx; 
			
				for( int y = 0; y < 4; y++ ) 
				{                 
					for( int x = 0; x < 4; x++ ) 
					{ 
						d3dMtx( x , y ) = ( float )VertexTransformMatrix.Get( x , y );                 
					}  
				} 	
 
				m_meshes.back( ).matrixes.back( ).push_back( d3dMtx ); 
			}
 
			int* pointIndexArray = pCluster->GetControlPointIndices( );             
			double* weightArray = pCluster->GetControlPointWeights( ); 
 
			for( int i = 0 ; i < pointIndexCount; i++ ) 
			{                 
				m_meshes.back( ).points[ pointIndexArray[ i ] ].bornRefarences.push_back( BornRefarence( clusterNum ,
																										 static_cast< float >( weightArray[ i ] ) ) );            
			}     
		}		
	} 
}

//--------------------------------------------------------------------------------------
//  頂点データの変換関数
//--------------------------------------------------------------------------------------
void ObjectFBX::ConvertVertex( void )  
{
	for( unsigned int countMesh = 0; countMesh < m_meshes.size( ); ++countMesh )
	{
		for( unsigned int countIndex = 0; countIndex < m_meshes[ countMesh ].positionIndices.size( ); ++countIndex )
		{
			//  変換頂点の代入
			Vertex vertex;
			vertex.bornReference = m_meshes[ countMesh ].points[ m_meshes[ countMesh ].positionIndices[ countIndex ] ].bornRefarences;
			vertex.vertex.position = m_meshes[ countMesh ].points[ m_meshes[ countMesh ].positionIndices[ countIndex ] ].positions;
			vertex.vertex.normal = m_meshes[ countMesh ].normals[ m_meshes[ countMesh ].normalIndices[ countIndex ] ];
			vertex.vertex.color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
			vertex.vertex.texcoord = m_meshes[ countMesh ].texcoords[ m_meshes[ countMesh ].texcoordIndices[ countIndex ] ];

			int index = ChangeVertexIndex( vertex , m_meshes[ countMesh ].vertexList );

			if( index == -1 )
			{
				//  情報の格納
				m_meshes[ countMesh ].vertexList.push_back( vertex );
				m_meshes[ countMesh ].indices.push_back( m_meshes[ countMesh ].vertexList.size( ) );
			}
			else
			{
				//  情報の格納
				m_meshes[ countMesh ].indices.push_back( index );
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  頂点インデックス番号の変更
//--------------------------------------------------------------------------------------
int	ObjectFBX::ChangeVertexIndex( Vertex &vertex , std::vector< Vertex > &vertexList )
{
	int		countIndex = 0;
	bool	checkIndex = true;

	for( auto it = vertexList.begin( ); it != vertexList.end( ); ++it  )
	{
		if( vertex.vertex.position == it->vertex.position && 
			vertex.vertex.normal == it->vertex.normal &&
			vertex.vertex.texcoord == it->vertex.texcoord &&
			vertex.bornReference.size( ) == it->bornReference.size( ) )
		{
			for( unsigned int countBorn1 = 0; countBorn1 < vertex.bornReference.size( ); ++countBorn1 )
			{
				for( unsigned int countBorn2 = 0; countBorn2 < it->bornReference.size( ); ++countBorn2 )
				{
					if( vertex.bornReference[ countBorn1 ].index != it->bornReference[ countBorn2 ].index ||
						vertex.bornReference[ countBorn1 ].weight != it->bornReference[ countBorn2 ].weight )
					{
						checkIndex = false;
					}
				}
			}
		}
		else
		{
			checkIndex = false;
		}

		if( checkIndex == true )
		{
			return countIndex;
		}

		countIndex++;
	}

	return -1;
}

//--------------------------------------------------------------------------------------
//  現在のファイルポインタからある文字列の数をカウントする関数
//--------------------------------------------------------------------------------------
std::string ObjectFBX::GetAttributeTypeName(FbxNodeAttribute::EType type) 
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
//  頂点バッファを作成する関数
//--------------------------------------------------------------------------------------
void ObjectFBX::MakeVertex( int size )
{     
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * size ,				//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,				//  使用方法
											 FVF_VERTEX_3D ,					//  
											 D3DPOOL_MANAGED ,					//  メモリ管理方法( MANAGED → お任せ )
											 &m_pVtxBuffers.back( ) ,			//  バッファ
											 NULL ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return;
	}
}

//--------------------------------------------------------------------------------------
//  生成する関数
//--------------------------------------------------------------------------------------
ObjectFBX* ObjectFBX::Create( D3DXVECTOR3 position , float scale )
{
	ObjectFBX* objectFBX = new ObjectFBX;

	//  座標代入
	objectFBX->m_position = position;

	//  大きさ倍率代入
	objectFBX->m_scale = scale;

	//  初期化
	objectFBX->Init( );

	return objectFBX;
}