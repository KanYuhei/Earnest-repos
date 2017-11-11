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
#include "sceneModel.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "keyboard.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define	MODEL_FILENAME		"data/MODEL/tree.x"
#define	MODEL_FILENAME2		"data/MODEL/castle.x"
#define	MODEL_FILENAME3		"data/MODEL/field2.x"
#define	MODEL_FILENAME4		"data/MODEL/hitField2.x"
#define	MODEL_FILENAME5		"data/MODEL/field.x"
#define	MODEL_FILENAME6		"data/MODEL/hitField.x"

#define TEXTURE_FILEPATH	"data\\TEXTURE"						//  テクスチャへのファイルパス

#define MAX_MODEL_PARTS		( 20 )								//  マテリアル最大数

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  xモデルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SenceModel::SenceModel( )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_posAt = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_scale = D3DXVECTOR3( 1.0f, 1.0f , 1.0f );
	m_rot = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );

	D3DXQuaternionIdentity( &m_quaternion );

	m_type = TYPE_TREE;
}

//--------------------------------------------------------------------------------------
//  xモデルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SenceModel::SenceModel( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_position = position;
	m_rot = rot;
	m_scale = scale;

	m_type = TYPE_TREE;
}

//--------------------------------------------------------------------------------------
//  xモデルのデストラクタ
//--------------------------------------------------------------------------------------
SenceModel::~SenceModel( )
{

}

//--------------------------------------------------------------------------------------
//  xモデルの初期化処理
//--------------------------------------------------------------------------------------
HRESULT SenceModel::Init( void )
{
	D3DXMATERIAL*	pMat = NULL;									//  マテリアル
	char			aTextureFileName[ 128 ] = { };
	int				nIndex = 0;

	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	if( m_type == TYPE_TREE )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME ,				//  ファイル名
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
	}
	else if( m_type == TYPE_CASTLE )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME2 ,				//  ファイル名
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
	}
	else if( m_type == TYPE_FIELD000 )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME3 ,				//  ファイル名
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
	}
	else if( m_type == TYPE_HIT_FIELD000 )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME4 ,				//  ファイル名
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
	}
	else if( m_type == TYPE_FIELD001 )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME5 ,				//  ファイル名
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
	}
	else if( m_type == TYPE_HIT_FIELD001 )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME6 ,				//  ファイル名
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
void SenceModel::Uninit( void )
{
	//  メッシュ情報の解放
	SAFE_RELEASE( m_pMeshModel );

	//  マテリアル情報の解放
	SAFE_RELEASE( m_pBuffMatModel );
}

//--------------------------------------------------------------------------------------
//  xモデルの更新処理
//--------------------------------------------------------------------------------------
void SenceModel::Update( void )
{
	D3DXQUATERNION quaternion;

	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	/*  ボール用 

	if( pKeyboard->GetKeyboardPress( DIK_W ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &m_quaternion , &quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_S ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( -1.0f , 0.0f , 0.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &m_quaternion , &quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_D ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 0.0f , 0.0f , -1.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &m_quaternion , &quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_A ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &m_quaternion , &quaternion );
	}

	*/

	/*  戦闘機用

	if( pKeyboard->GetKeyboardPress( DIK_W ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , -0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &quaternion , &m_quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_S ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &quaternion , &m_quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_D ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) , -0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &quaternion , &m_quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_A ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &quaternion , &m_quaternion );
	}

	*/
}

//--------------------------------------------------------------------------------------
//  xモデルの描画処理
//--------------------------------------------------------------------------------------
void SenceModel::Draw( void )
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
	D3DXMatrixScaling( &mtxScale , m_scale.x , m_scale.y , m_scale.z );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

	if( m_type == TYPE_TREE )
	{
		D3DXMatrixRotationQuaternion( &mtxRot , &m_quaternion );
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll( &mtxRot , m_rot.y , m_rot.x , m_rot.z );
	}

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y + 1.0f , m_position.z );

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
		pMat[ nCntMaterial ].MatD3D.Diffuse = m_color;

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
void SenceModel::SetScale( D3DXVECTOR3 scale )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  xポリゴンの大きさを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 SenceModel::GetScale( void )
{
	return m_scale;
}

//--------------------------------------------------------------------------------------
//  xモデルの注視点設定をする関数
//--------------------------------------------------------------------------------------
void SenceModel::SetPosAt( D3DXVECTOR3 posAt )
{
	m_posAt = posAt;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
SenceModel* SenceModel::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	SenceModel *pSceneModel;

	//  インスタンス生成
	pSceneModel = new SenceModel;

	//  種類の代入
	pSceneModel->m_type = type;

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

//--------------------------------------------------------------------------------------
//  外積によって求められる垂直ベクトルから行列を作る関数
//--------------------------------------------------------------------------------------
D3DXMATRIX* SenceModel::GetFixedLookAtMatrix( D3DXMATRIX* pMtx , D3DXVECTOR3* pPos , D3DXVECTOR3* pPosAt , D3DXVECTOR3* pVecUp )
{
	//  ローカル変数の宣言
	D3DXVECTOR3 X , Y , Z , D;

	//  Z軸方向のベクトルを求める
	D = *pPosAt - *pPos;

	//  Zを単位ベクトル化
	D3DXVec3Normalize( &D , &D );

	//  上方向ベクトルとZ軸ベクトルの外積よりX軸ベクトルを求める
	D3DXVec3Cross( &X , D3DXVec3Normalize( &Y , pVecUp ) , &D );

	//  Xを単位ベクトル化
	D3DXVec3Normalize( &X , &X );

	//  Yを単位ベクトル化
	D3DXVec3Normalize( &Z , D3DXVec3Cross( &Z , &X , &Y ) );

	//  1行目の代入
	pMtx->_11 = X.x;
	pMtx->_12 = X.y;
	pMtx->_13 = X.z;
	pMtx->_14 = 0;

	//  2行目の代入
	pMtx->_21 = Y.x;
	pMtx->_22 = Y.y;
	pMtx->_23 = Y.z;
	pMtx->_24 = 0;

	//  3行目の代入
	pMtx->_31 = Z.x;
	pMtx->_32 = Z.y;
	pMtx->_33 = Z.z;
	pMtx->_34 = 0;

	//  4行目の代入
	pMtx->_41 = 0.0f;
	pMtx->_42 = 0.0f;
	pMtx->_43 = 0.0f;
	pMtx->_44 = 1.0f;

	return pMtx;
}

//--------------------------------------------------------------------------------------
//  視界に入ってるかどうか判定する関数
//--------------------------------------------------------------------------------------
bool SenceModel::IntoView( D3DXVECTOR3 vec1 , D3DXVECTOR3 vec2 , float fViewAngle )
{
	//  ローカル変数の宣言
	float fAngle = 0.0f;

	//  二つのベクトルの内積をとる
	fAngle = D3DXVec3Dot( &vec1 , &vec2 );

	//  その結果から二つのベクトルのなすラジアン角を求める
	fAngle = acosf( fAngle );

	if( fAngle > fViewAngle || fAngle < -fViewAngle )
	{
		return false;
	}

	return true;
}