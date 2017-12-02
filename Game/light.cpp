//--------------------------------------------------------------------------------------
//  ライト   ( light.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "shaderManager.h"
#include "imgui.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ライトクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Light::Light( )
{

}

//--------------------------------------------------------------------------------------
//  ライトクラスのデストラクタ
//--------------------------------------------------------------------------------------
Light::~Light( )
{

}

//--------------------------------------------------------------------------------------
//  ライトの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Light::Init( void )
{
	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ライト最大数分のループ
	for( int countLight = 0; countLight < MAX_LIGHT; ++countLight )
	{
		//  光源クラスの初期化
		ZeroMemory( &m_light , sizeof( D3DLIGHT9 ) );
	}

	m_light[ 0 ].Type = D3DLIGHT_DIRECTIONAL;									//  光源の種類を平行光源に		
	m_light[ 0 ].Diffuse = D3DXCOLOR( 1.0f , 0.9f , 0.3f , 1.0f );				//  拡散光

	D3DXVECTOR3 vecDir = D3DXVECTOR3( 0.1f , -0.5f , -1.0f );					//  法線の向き
	D3DXVec3Normalize( &vecDir , &vecDir );
	m_vectorDirection = vecDir;
	m_distance = 100.0f;

	//  光源の座標を代入
	m_position = -m_vectorDirection * m_distance;
	m_positionAt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	//  ビュー行列の設定
	D3DXMatrixIdentity( &m_viewMatrix );
	D3DXMatrixLookAtLH( &m_viewMatrix , 
						&m_position , 
						&m_positionAt , 
						&D3DXVECTOR3( 0.0f , 1.0f, 0.0f ) );

	//  法線を1.0fに変換
	D3DXVec3Normalize( ( D3DXVECTOR3* )&m_light[ 0 ].Direction , &vecDir );	

	//  デバイスにライトの設定
	//pDevice->SetLight( 0 , &m_light[ 0 ] );

	m_light[ 1 ].Type = D3DLIGHT_DIRECTIONAL;									//  光源の種類を平行光源に		
	m_light[ 1 ].Diffuse = D3DXCOLOR( 0.9f , 0.9f , 0.2f , 1.0f );				//  拡散光

	vecDir = D3DXVECTOR3( 1.0f , -0.5f , -1.0f );								//  法線の向き

	//  法線を1.0fに変換
	D3DXVec3Normalize( ( D3DXVECTOR3* )&m_light[ 1 ].Direction , &vecDir );	

	m_light[ 2 ].Type = D3DLIGHT_DIRECTIONAL;									//  光源の種類を平行光源に		
	m_light[ 2 ].Diffuse = D3DXCOLOR( 0.3f , 0.9f , 0.3f , 1.0f );				//  拡散光

	vecDir = D3DXVECTOR3( 0.0f , 0.5f , 1.0f );									//  法線の向き

	//  法線を1.0fに変換
	D3DXVec3Normalize( ( D3DXVECTOR3* )&m_light[ 2 ].Direction , &vecDir );	

	m_fov = D3DX_PI / 8.0f;
	m_near = 1.0f;
	m_far = 2000.0f;

	//  プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH( &m_projectionMatrix ,							//  プロジェクション行列のアドレス
								m_fov ,											//  画角
								( float )SCREEN_WIDTH / SCREEN_HEIGHT ,			//  アスペクト比
								m_near ,										//  near( 0.0f < near )
								m_far );

	//D3DXMatrixOrthoLH( &m_projectionMatrix , 
	//				   10000.0f , 10000.0f , 
	//				   m_near , m_far );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  ライトの終了処理
//--------------------------------------------------------------------------------------
void Light::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  ライトの更新処理
//--------------------------------------------------------------------------------------
void Light::Update( void )
{

}

//--------------------------------------------------------------------------------------
//  光を点ける関数
//--------------------------------------------------------------------------------------
void Light::SetDiffuseColor( D3DXCOLOR color , int nNo )
{
	D3DLIGHT9 light;													//  光源

	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  光源クラスの初期化
	ZeroMemory( &light , sizeof( D3DLIGHT9 ) );

	light.Type = D3DLIGHT_DIRECTIONAL;									//  光源の種類を平行光源に		
	light.Diffuse = color;												//  拡散光

	D3DXVECTOR3 vecDir = D3DXVECTOR3( 1.0f , -0.4f , -1.0f );			//  法線の向き

	//  法線を1.0fに変換
	D3DXVec3Normalize( ( D3DXVECTOR3* )&light.Direction , &vecDir );
}

//--------------------------------------------------------------------------------------
//  光を点ける関数
//--------------------------------------------------------------------------------------
void Light::LightOn( void )
{
	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ライトを点ける
	pDevice->SetRenderState( D3DRS_LIGHTING , true );
}

//--------------------------------------------------------------------------------------
//  光を消す関数
//--------------------------------------------------------------------------------------
void Light::LightOff( void )
{
	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ライトを消す
	pDevice->SetRenderState( D3DRS_LIGHTING , false );
}

//--------------------------------------------------------------------------------------
//	ビュー行列の設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetViewMatrix( D3DXVECTOR3 position , D3DXVECTOR3 positionAt )
{
	//  情報の代入
	m_position = position;
	m_positionAt = positionAt;

	D3DXVECTOR3 vecDir = m_positionAt - m_position;    //  光源の向き
	D3DXVec3Normalize( &vecDir , &vecDir );
	m_vectorDirection = vecDir;

	//  ビュー行列の設定
	D3DXMatrixIdentity( &m_viewMatrix );
	D3DXMatrixLookAtLH( &m_viewMatrix , 
						&m_position , 
						&m_positionAt , 
						&D3DXVECTOR3( 0.0f , 1.0f, 0.0f ) );
}

//--------------------------------------------------------------------------------------
//	ビュー行列の設定をする関数
//--------------------------------------------------------------------------------------
const D3DXMATRIX& Light::GetDirectionalViewMatrix( D3DXVECTOR3 position )
{
	//  情報の代入
	m_positionAt = position;

	//  座標の算出
	m_position = position - m_vectorDirection * m_distance;
	
	//  ビュー行列の設定
	D3DXMatrixIdentity( &m_viewMatrix );
	D3DXMatrixLookAtLH( &m_viewMatrix , 
						&m_position , 
						&m_positionAt , 
						&D3DXVECTOR3( 0.0f , 1.0f, 0.0f ) );

	return m_viewMatrix;
}

//--------------------------------------------------------------------------------------
//  プロジェクション行列の設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetProjectionMatrix( float fov , float fNear , float fFar )
{
	m_fov = fov;
	m_near = fNear;
	m_far = fFar;

	//  プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH( &m_projectionMatrix ,							//  プロジェクション行列のアドレス
								m_fov ,											//  画角
								( float )SCREEN_WIDTH / SCREEN_HEIGHT ,			//  アスペクト比
								m_near ,										//  near( 0.0f < near )
								m_far );										//  far

	//D3DXMatrixOrthoLH( &m_projectionMatrix , 
	//				   10000.0f , 10000.0f , 
	//				   m_near , m_far );
}

//--------------------------------------------------------------------------------------
//  距離の設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetDistance( float distance )
{
	m_distance = distance;
}

//--------------------------------------------------------------------------------------
//  進行方向ベクトルの設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetVectorDirection( D3DXVECTOR3 direction )
{
	m_vectorDirection = direction;
	D3DXVec3Normalize( &m_vectorDirection , &m_vectorDirection );
}

//--------------------------------------------------------------------------------------
//  進行方向Xベクトルの設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetVectorDirectionX( float directionX )
{
	m_vectorDirection.x = directionX;
	D3DXVec3Normalize( &m_vectorDirection , &m_vectorDirection );
}

//--------------------------------------------------------------------------------------
//  進行方向Yベクトルの設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetVectorDirectionY( float directionY )
{
	m_vectorDirection.y = directionY;
	D3DXVec3Normalize( &m_vectorDirection , &m_vectorDirection );
}

//--------------------------------------------------------------------------------------
//  進行方向Yベクトルの設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetVectorDirectionZ( float directionZ )
{
	m_vectorDirection.z = directionZ;
	D3DXVec3Normalize( &m_vectorDirection , &m_vectorDirection );
}

//--------------------------------------------------------------------------------------
//  指定番号のライト情報の取得する関数
//--------------------------------------------------------------------------------------
const D3DLIGHT9& Light::GetLight( int index )
{
	return m_light[ index ];
}

//--------------------------------------------------------------------------------------
//  指定番号のライト方向ベクトルの取得する関数
//--------------------------------------------------------------------------------------
const D3DXVECTOR3& Light::GetDirection( void )
{
	return m_vectorDirection;
}

//--------------------------------------------------------------------------------------
//  指定番号の座標の取得する関数
//--------------------------------------------------------------------------------------
const D3DXVECTOR3&	Light::GetPosition( void )
{
	return m_position;
}

//--------------------------------------------------------------------------------------
//  指定番号の注視点の取得をする関数
//--------------------------------------------------------------------------------------
const D3DXVECTOR3& Light::GetPositionAt( void )
{
	return m_positionAt;
}

//--------------------------------------------------------------------------------------
//  ビュー行列の取得する関数
//--------------------------------------------------------------------------------------
const D3DXMATRIX& Light::GetViewMatrix( void )
{
	return m_viewMatrix;
}

//--------------------------------------------------------------------------------------
//  プロジェクション行列の取得する関数
//--------------------------------------------------------------------------------------
const D3DXMATRIX& Light::GetProjectionMatrix( void )
{
	return m_projectionMatrix;
}

//--------------------------------------------------------------------------------------
//  画角の取得する関数
//--------------------------------------------------------------------------------------
const float& Light::GetFov( void )
{
	return m_fov;
}

//--------------------------------------------------------------------------------------
//  ニアの取得する関数
//--------------------------------------------------------------------------------------
const float& Light::GetNear( void )
{
	return m_near;
}

//--------------------------------------------------------------------------------------
//  ファーの取得する関数
//--------------------------------------------------------------------------------------
const float& Light::GetFar( void )
{
	return m_far;
}

//--------------------------------------------------------------------------------------
//  距離の取得する関数
//--------------------------------------------------------------------------------------
const float& Light::GetDistance( void )
{
	return m_distance;
}