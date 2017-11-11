//--------------------------------------------------------------------------------------
//  カメラ   ( camera.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
const float CAMERA_TO_DISTANCE = 20.0f;

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  カメラクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Camera::Camera( )
{
	//  メンバ変数の初期化
	m_posEye = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	m_posAt = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	m_vecUp = D3DXVECTOR3( 0.0f, 1.0f , 0.0f );
	m_vecDirect = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	m_basePos = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	m_seekPos = D3DXVECTOR2( 0.0f, 0.0f );
	m_fFovY = 0.0f;
	m_fDistance = CAMERA_TO_DISTANCE;
	m_fReviseEye = 1.0f;
	m_fReviseAt = 1.0f;
	m_fRotY = 0.0f;
	m_fRotX = 0.0f;
	m_fSwaySide = 0.0f;
	m_fSwayVertical = 0.0f;
	m_fSwayAngle = 0.0f;
	m_nMode = 0;
	m_bFree = false;
	m_nCameraNo = 0;
}

//--------------------------------------------------------------------------------------
//  カメラクラスのデストラクタ
//--------------------------------------------------------------------------------------
Camera::~Camera( )
{

}

//--------------------------------------------------------------------------------------
//  カメラの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Camera::Init( D3DXVECTOR3 posEye , D3DXVECTOR3 posAt , float fFovY , float fNear , float fFar , int nCameraNo , bool bDivide )
{
	D3DXMATRIX			mtxView;						//  ビュー座標変換行列
	D3DXMATRIX			mtxProj;						//  プロジェクション座標行列

	LPDIRECT3DDEVICE9	pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  視点座標の代入
	m_posEye = posEye;

	//  注視点座標の代入
	m_posAt = posAt;

	//  画角の代入
	m_fFovY = fFovY;

	//  カメラ番号の代入
	m_nCameraNo = nCameraNo;

	if( bDivide == false )
	{
		//  プロジェクション行列の作成
		D3DXMatrixPerspectiveFovLH( &mtxProj ,											//  プロジェクション行列のアドレス
									m_fFovY ,											//  画角
									( float )SCREEN_WIDTH / SCREEN_HEIGHT ,				//  アスペクト比
									fNear ,												//  near( 0.0f < near )
									fFar );												//  far

		//  プロジェクション座標変換
		pDevice->SetTransform( D3DTS_PROJECTION , &mtxProj );
	}
	else
	{
		if( m_nCameraNo == 0 )
		{
			//  プロジェクション行列の作成
			D3DXMatrixPerspectiveFovLH( &mtxProj ,											//  プロジェクション行列のアドレス
										m_fFovY ,											//  画角
										( float )( SCREEN_WIDTH / 2 ) / ( SCREEN_HEIGHT ) ,	//  アスペクト比
										fNear ,												//  near( 0.0f < near )
										fFar );												//  far

			//  プロジェクション座標変換
			pDevice->SetTransform( D3DTS_PROJECTION , &mtxProj );
		}
	}

	//  ビュー座標変換行列の設定
	D3DXMatrixLookAtLH( &mtxView , &m_posEye , &m_posAt , &m_vecUp );

	//  ビュー座標変換
	pDevice->SetTransform( D3DTS_VIEW , &mtxView );

	//  基準値の代入
	m_basePosEye = posEye;
	m_basePosAt = posAt;

	if( m_nCameraNo == 0 )
	{
		m_vecDirect = D3DXVECTOR3( 0.0f, 0.0f , 1.0f );
	}
	else if( m_nCameraNo == 1 )
	{
		m_vecDirect = D3DXVECTOR3( 0.0f, 0.0f , -1.0f );
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  カメラの終了処理
//--------------------------------------------------------------------------------------
void Camera::Uninit( void )
{
	LPDIRECT3DDEVICE9	pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	D3DVIEWPORT9 vp;

	vp.X = 0;
	vp.Y = 0;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	//  ビューポート変換の設定
	pDevice->SetViewport( &vp );
}

//--------------------------------------------------------------------------------------
//  カメラの更新処理
//--------------------------------------------------------------------------------------
void Camera::Update( void )
{
	//  ローカル変数の宣言
	D3DXVECTOR3			work;						//  格納用
	D3DXVECTOR3			work2;						//  格納用2
	D3DXMATRIX			mtxView;					//  ビュー座標変換行列
	D3DXMATRIX			mtxProj;					//  プロジェクション座標行列

	LPDIRECT3DDEVICE9	pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  画角の代入
	m_fFovY += ( ( D3DX_PI / 3.0f ) - m_fFovY ) * 0.01f;

	//  フリーカメラモードではない場合
	if( m_bFree == false )
	{
		//  視点座標に補正をかける
		m_posEye += ( m_basePosEye - m_posEye ) * m_fReviseEye;

		//  注視点座標に補正をかける
		m_posAt += ( m_basePosAt - m_posAt ) * m_fReviseAt;

		//  注視点と視点座標の一時代入
		work = m_posAt;
		work.y = 0.0f;
		work2 = m_posEye;
		work2.y = 0.0f;

		//  カメラの方向ベクトルの算出( 視野角カリング用 )
		D3DXVec3Normalize( &m_vecDirect , &( work - work2 ) );

		//  ずらし量分注視点をずらす
		m_posAt.y = m_posAt.y + m_seekPos.y;

		if( m_fSwaySide != 0.0f || m_fSwayVertical != 0.0f )
		{
			m_seekPos.x = sinf( m_fSwayAngle ) * m_fSwaySide;
			m_seekPos.y = sinf( m_fSwayAngle ) * m_fSwayVertical;

			m_fSwayAngle += 0.8f;

			if( m_fSwayAngle > D3DX_PI )
			{
				m_fSwayAngle = -D3DX_PI;

				m_fSwaySide *= 0.3f;
				m_fSwayVertical *= 0.3f;

				if( m_fSwaySide < 0.01f )
				{
					m_fSwaySide = 0.0f;
				}
				if( m_fSwayVertical < 0.01f )
				{
					m_fSwayVertical = 0.0f;
				}

				m_nSwayTime = 0;
			}

			m_nSwayTime++;
		}
		else
		{
			m_fSwayAngle = 0.0f;
			m_nSwayTime = 0;
		}
	}
	else
	{
		// キーボード情報の取得
		Keyboard*			pKeyboard = SceneManager::GetKeyboard( );
		float fAxisX = 0.0f;
		float fAxisY = 0.0f;

		if( pKeyboard->GetKeyboardPress( DIK_A ) )
		{
			m_basePos.x -= 1.0f;
		}
		if( pKeyboard->GetKeyboardPress( DIK_D ) )
		{
			m_basePos.x += 1.0f;
		}	
		if( pKeyboard->GetKeyboardPress( DIK_W ) )
		{
			m_basePos.z += 1.0f;
		}
		if( pKeyboard->GetKeyboardPress( DIK_S ) )
		{
			m_basePos.z -= 1.0f;
		}
		if( pKeyboard->GetKeyboardPress( DIK_Z ) )
		{
			m_basePos.y += 1.0f;
		}
		if( pKeyboard->GetKeyboardPress( DIK_X ) )
		{
			m_basePos.y -= 1.0f;
		}

		if( pKeyboard->GetKeyboardPress( DIK_J ) )
		{
			fAxisX -= 0.1f;
		}
		if( pKeyboard->GetKeyboardPress( DIK_L ) )
		{
			fAxisX += 0.1f;
		}	
		if( pKeyboard->GetKeyboardPress( DIK_I ) )
		{
			fAxisY += 0.1f;
		}
		if( pKeyboard->GetKeyboardPress( DIK_K ) )
		{
			fAxisY -= 0.1f;
		}	

		m_fRotY -= fAxisX / ( float )SCREEN_WIDTH * D3DX_PI * 2.0f;
		m_fRotX -= fAxisY / ( float )SCREEN_HEIGHT * D3DX_PI * 2.0f;

		float fWhile = 0.0f;

		if( pKeyboard->GetKeyboardPress( DIK_UPARROW ) )
		{
			fWhile += 1.0f;
		}
		if( pKeyboard->GetKeyboardPress( DIK_DOWNARROW ) )
		{
			fWhile -= 1.0f;
		}

		//  物体までの距離の調整( ズームイン 、ズームアウト )
		m_fDistance -= fWhile * 0.002f;

		//  一時座標の格納
		D3DXVECTOR3 position( m_basePos.x , m_basePos.y , m_basePos.z - m_fDistance );

		//  Y軸回転行列の作成
		D3DXMATRIX mtxRotY;
		D3DXMatrixIdentity( &mtxRotY );
		D3DXMatrixRotationY( &mtxRotY , m_fRotY );

		//  X軸回転行列の作成
		D3DXMATRIX mtxRotX;
		D3DXMatrixIdentity( &mtxRotX );
		D3DXMatrixRotationX( &mtxRotX , m_fRotX );

		//  回転行列の合成
		D3DXMATRIX mtxRot;
		D3DXMatrixMultiply( &mtxRot , &mtxRotX , &mtxRotY );

		//  座標変換
		D3DXVec3TransformCoord( &m_posEye , &position , &mtxRot );

		//  座標変換
		D3DXVec3TransformCoord( &m_vecUp , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) , &mtxRot );

		m_posAt = m_basePos;
	}
}

//--------------------------------------------------------------------------------------
//  座標を設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetCameraPosEye( D3DXVECTOR3 posEye , float fRevice )
{
	m_basePosEye = posEye;

	m_fReviseEye = fRevice;
}

//--------------------------------------------------------------------------------------
//  注視点を設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetCameraPosAt( D3DXVECTOR3 posAt , float fRevice )
{
	m_basePosAt = posAt;

	m_fReviseAt = fRevice;
}

//--------------------------------------------------------------------------------------
//  座標を設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetCameraPosEyeDirect( D3DXVECTOR3 posEye )
{
	m_posEye = posEye;

	m_basePosEye = posEye;
}

//--------------------------------------------------------------------------------------
//  注視点を設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetCameraPosAtDirect( D3DXVECTOR3 posAt )
{
	m_posAt = posAt;

	m_basePosAt = posAt;
}

//--------------------------------------------------------------------------------------
//  横揺れを設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetSwaySide( float fSwaySide )
{
	m_fSwaySide = fSwaySide;
}

//--------------------------------------------------------------------------------------
//  縦揺れを設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetSwayVertical( float fSwayVertical )
{
	m_fSwayVertical = fSwayVertical;
}

//--------------------------------------------------------------------------------------
//  座標を移動する関数
//--------------------------------------------------------------------------------------
void Camera::MoveCameraPosEye( D3DXVECTOR3 moveEye , float fRevice )
{
	m_basePosEye = m_basePosEye + moveEye;

	m_fReviseEye = fRevice;
}

//--------------------------------------------------------------------------------------
//  注視点を移動する関数
//--------------------------------------------------------------------------------------
void Camera::MoveCameraPosAt( D3DXVECTOR3 moveAt , float fRevice )
{
	m_basePosAt = m_basePosAt + moveAt;

	m_fReviseAt = fRevice;
}

//--------------------------------------------------------------------------------------
//  視点座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraPosEye( void )
{
	return m_posEye;
}

//--------------------------------------------------------------------------------------
//  注視点座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraPosAt( void )
{
	return m_posAt;
}

//--------------------------------------------------------------------------------------
//  視点座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraBasePosEye( void )
{
	return m_basePosEye;
}

//--------------------------------------------------------------------------------------
//  注視点座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraBasePosAt( void )
{
	return m_basePosAt;
}

//--------------------------------------------------------------------------------------
//  方向ベクトルを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraVecDirect( void )
{
	return m_vecDirect;
}

//--------------------------------------------------------------------------------------
//  カメラと対峙する回転行列を取得する関数
//--------------------------------------------------------------------------------------
D3DXMATRIX Camera::GetInvRotateMat( D3DXVECTOR3 position )
{
	D3DXMATRIX mtxInv;								//  回転行列格納用

	//  単位行列に変換
	D3DXMatrixIdentity( &mtxInv );

	//  ビュー行列作成関数から回転行列の作成
	D3DXMatrixLookAtLH( &mtxInv , &D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , &( m_posEye - position ) , &m_vecUp );

	//  回転行列の逆行列を転置行列で求める
	D3DXMatrixTranspose( &mtxInv , &mtxInv );

	return mtxInv;
}

//--------------------------------------------------------------------------------------
//  カメラモードの切り替え関数
//--------------------------------------------------------------------------------------
void Camera::ChangeMode( void )
{
	m_bFree = !m_bFree;
}

//--------------------------------------------------------------------------------------
//  カメラ行列の設定をする関数
//--------------------------------------------------------------------------------------
void Camera::SetCamera( void )
{
	//  ローカル変数の宣言
	D3DXVECTOR3			work;						//  格納用
	D3DXVECTOR3			work2;						//  格納用2
	D3DXMATRIX			mtxView;					//  ビュー座標変換行列
	D3DXMATRIX			mtxProj;					//  プロジェクション座標行列
	LPDIRECT3DDEVICE9	pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
	{
		D3DVIEWPORT9 vp;

		vp.X = 0;
		vp.Y = 0;
		vp.Width = SCREEN_WIDTH;
		vp.Height = SCREEN_HEIGHT;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;

		//  ビューポート変換の設定
		pDevice->SetViewport( &vp );
	}
	if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
	{
		D3DVIEWPORT9 vp;

		vp.X = ( SCREEN_WIDTH / 2 ) * m_nCameraNo;
		vp.Y = 0;
		vp.Width = SCREEN_WIDTH / 2;
		vp.Height = SCREEN_HEIGHT;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;

		//  ビューポート変換の設定
		pDevice->SetViewport( &vp );
	}

	//  ビュー座標変換行列の設定
	D3DXMatrixLookAtLH( &mtxView , &m_posEye , &m_posAt , &m_vecUp );

	//  ビュー座標変換
	pDevice->SetTransform( D3DTS_VIEW , &mtxView );	
}

