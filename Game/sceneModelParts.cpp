//--------------------------------------------------------------------------------------
//  モデルレンダリング処理   ( sceneModel.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "sceneModelParts.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sceneModel.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define TEXTURE_FILEPATH		"data\\TEXTURE"					//  テクスチャへのファイルパス

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  xモデルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneModelParts::SceneModelParts( )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_posAt = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_scale = D3DXVECTOR3( 1.0f, 1.0f , 1.0f );
	m_rot = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  xモデルのデストラクタ
//--------------------------------------------------------------------------------------
SceneModelParts::~SceneModelParts( )
{

}

//--------------------------------------------------------------------------------------
//  xモデルの初期化処理
//--------------------------------------------------------------------------------------
HRESULT SceneModelParts::Init( void )
{
	D3DXMATERIAL*	pMat = NULL;									//  マテリアル
	char			aTextureFileName[ 128 ] = { };
	int				nIndex = 0;

	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  xファイルの読み込み
	if( FAILED( D3DXLoadMeshFromX( m_aModelName ,				//  ファイル名
		 						   D3DXMESH_MANAGED ,			//  オプション
								   pDevice ,					//  デバイス
								   NULL ,						//  
								   &m_pBuffMatModel ,			//  マテリアル情報
								   NULL ,						//  
								   &m_nNumMatModel ,			//  マテリアル数
								   &m_pMeshModel ) ) )			//  メッシュ情報
	{
		MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return E_FAIL;
	}

	//  マテリアル情報の先頭アドレスを取得
	pMat = ( D3DXMATERIAL* )m_pBuffMatModel->GetBufferPointer( );

	//  マテリアルの数分のループ
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  テクスチャポインタがある場合
		if( pMat[ nCntMaterial ].pTextureFilename != NULL )
		{
			//  テクスチャファイルパス用の文字列
			char aTextureFilePath[ 128 ] = { TEXTURE_FILEPATH };

			//  文字数分のループ
			for( int nCntLength = strlen( pMat[ nCntMaterial ].pTextureFilename ) - 1; nCntLength >= 0 ; nCntLength-- )
			{
				//  テクスチャ名の取り出し
				aTextureFileName[ nIndex ] = pMat[ nCntMaterial ].pTextureFilename[ nCntLength ];

				nIndex++;

				//  \があるとループを抜け出す
				if( pMat[ nCntMaterial ].pTextureFilename[ nCntLength ] == '\\' )
				{
					break;
				}
			}

			//  \0の格納
			aTextureFileName[ nIndex ] = '\0';

			//  インデックス番号の初期化
			nIndex = 0;

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

			//  テクスチャ名をテクスチャパス名に
			strcat( aTextureFilePath , aTextureFileName );

			//  テクスチャ名のコピー
			strcpy( m_aTextureName[ nCntMaterial ] , aTextureFilePath );

			//  テクスチャの登録
			pTexture->SetTextureImage( aTextureFilePath );
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  xモデルの終了処理
//--------------------------------------------------------------------------------------
void SceneModelParts::Uninit( void )
{
	//  メッシュ情報の解放
	SAFE_RELEASE( m_pMeshModel );

	//  マテリアル情報の解放
	SAFE_RELEASE( m_pBuffMatModel );
}

//--------------------------------------------------------------------------------------
//  xモデルの更新処理
//--------------------------------------------------------------------------------------
void SceneModelParts::Update( D3DXMATRIX mtxParent )
{
	D3DXMATRIX		mtxWorld;						//  ワールド行列
	D3DXMATRIX		mtxWorld2;						//  ワールド行列2
	D3DXMATRIX		mtxScale;						//  拡大行列
	D3DXMATRIX		mtxRot;							//  回転行列
	D3DXMATRIX		mtxRot2;						//  回転行列
	D3DXMATRIX		mtxTrans;						//  平行移動行列	
	D3DXVECTOR3		workRot( 0.0f, 0.0f , 0.0f );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &m_mtxWorld );

	//  回転行列の作成
	D3DXMatrixRotationYawPitchRoll( &mtxRot ,
									m_rot.y ,
									m_rot.x ,
									m_rot.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &m_mtxWorld ,
						&m_mtxWorld ,
						&mtxRot );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans ,
						   m_position.x + m_seekPos.x ,
						   m_position.y + m_seekPos.y ,
						   m_position.z + m_seekPos.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &m_mtxWorld ,
						&m_mtxWorld ,
						&mtxTrans );

	if( m_nParent != -1 )
	{
		//  平行移動行列の掛け算
		D3DXMatrixMultiply( &m_mtxWorld , &m_mtxWorld , &mtxParent );
	}
}

//--------------------------------------------------------------------------------------
//  xモデルの描画処理
//--------------------------------------------------------------------------------------
void SceneModelParts::Draw( D3DXVECTOR3 position , D3DXVECTOR3 posAt , D3DXCOLOR color )
{
	D3DMATERIAL9	matDef;							//  マテリアルの初期情報
	D3DXMATERIAL*	pMat = NULL;					//  マテリアル

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  スケール行列
	D3DXMATRIX mtxRot;								//  回転行列

	D3DXMATRIX mtxView;								//  ビュー座標変換行列
	D3DXMATRIX mtxProj;								//  プロジェクション座標行列

	LPDIRECT3DDEVICE9 pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  ライトを消す
	pDevice->SetRenderState( D3DRS_LIGHTING , true );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  拡大行列の作成
	D3DXMatrixScaling( &mtxScale ,
					   m_scale.x ,
					   m_scale.y ,
					   m_scale.z );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &m_mtxWorld ,
						&m_mtxWorld ,
						&mtxScale );

	//  回転行列の作成
	SceneModel::GetFixedLookAtMatrix( &mtxRot , &position , &posAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , position.x , position.y , position.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &m_mtxWorld , &mtxRot );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  ワールド座標変換
	pDevice->SetTransform( D3DTS_WORLD , &mtxWorld );

	//  マテリアルの初期情報の取得
	pDevice->GetMaterial( &matDef );

	//  マテリアル情報の先頭アドレスを取得
	pMat = ( D3DXMATERIAL* )m_pBuffMatModel->GetBufferPointer( );

	//  マテリアルの数分のループ
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  拡散光の設定
		pMat[ nCntMaterial ].MatD3D.Diffuse = color;

		//  環境光の設定
		pMat[ nCntMaterial ].MatD3D.Ambient = D3DXCOLOR( 0.6f , 0.6f , 0.6f , 1.0f );

		//  マテリアルの設定
		pDevice->SetMaterial( &pMat[ nCntMaterial ].MatD3D );

		if( pMat[ nCntMaterial ].pTextureFilename == NULL )
		{
			//  テクスチャの設定
			pDevice->SetTexture( 0 , NULL );
		}
		else
		{
			//  テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_aTextureName[ nCntMaterial ] ) );
		}

		//  メッシュの描画
		m_pMeshModel->DrawSubset( nCntMaterial );

		//  材質の設定
		pDevice->SetMaterial( &matDef );
	}
}

//--------------------------------------------------------------------------------------
//  xモデルの大きさ設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelParts::SetScale( D3DXVECTOR3 scale )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  xポリゴンの大きさを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 SceneModelParts::GetScale( void )
{
	return m_scale;
}

//--------------------------------------------------------------------------------------
//  xモデルの回転角を設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelParts::SetRot( D3DXVECTOR3 rot )
{
	m_rot = rot;
}

//--------------------------------------------------------------------------------------
//  xポリゴンの回転角を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 SceneModelParts::GetRot( void )
{
	return m_rot;
}

//--------------------------------------------------------------------------------------
//  xモデルのずらす座標を設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelParts::SetSeekPos( D3DXVECTOR3 seekPos )
{
	m_seekPos = seekPos;
}

//--------------------------------------------------------------------------------------
//  xモデルの注視点設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelParts::SetPositionAt( D3DXVECTOR3 posAt )
{
	m_posAt = posAt;
}

//--------------------------------------------------------------------------------------
//  モデル名の設定
//--------------------------------------------------------------------------------------
void SceneModelParts::SetModelName( char* pFileName )
{
	strcpy( m_aModelName , pFileName );
}

//--------------------------------------------------------------------------------------
//  識別番号の設定
//--------------------------------------------------------------------------------------
void SceneModelParts::SetIndex( int nIndex )
{
	m_nIndex = nIndex;
}

//--------------------------------------------------------------------------------------
//  識別番号を取得する関数
//--------------------------------------------------------------------------------------
int SceneModelParts::GetIndex( void )
{
	return m_nIndex;
}

//--------------------------------------------------------------------------------------
//  親番号の設定
//--------------------------------------------------------------------------------------
void SceneModelParts::SetParent( int nParent )
{
	m_nParent = nParent;
}

//--------------------------------------------------------------------------------------
//  親番号を取得する関数
//--------------------------------------------------------------------------------------
int SceneModelParts::GetParent( void )
{
	return m_nParent;
}

//--------------------------------------------------------------------------------------
//  親行列を設定する関数
//--------------------------------------------------------------------------------------
D3DXMATRIX SceneModelParts::GetMtxWorld( void )
{
	return m_mtxWorld;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
SceneModelParts* SceneModelParts::Create( int nIndex , int nParent , char* pFileName , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	SceneModelParts *pSceneModel;

	//  インスタンス生成
	pSceneModel = new SceneModelParts;

	//  識別番号の代入
	pSceneModel->m_nIndex = nIndex;

	//  親番号の代入
	pSceneModel->m_nParent = nParent;

	//  モデル名の設定
	pSceneModel->SetModelName( pFileName);

	//  座標の代入
	pSceneModel->m_position = position;

	//  回転角の代入
	pSceneModel->m_rot = rot;

	//  大きさ倍率の代入
	pSceneModel->m_scale = scale;

	//  初期化
	pSceneModel->Init( );

	return pSceneModel;
}