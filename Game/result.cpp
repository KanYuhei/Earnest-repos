﻿//--------------------------------------------------------------------------------------
//  リザルトプログラム   ( title.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "result.h"
#include "manager.h"
#include "sceneBG.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "fade.h"
#include "renderer.h"
#include "effekseerManager.h"
#include "meshDome.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float THIEF_SIZE = 1.0f; 

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
CResult::RESULT	CResult::m_result = CResult::GAME_CLEAR;	//  結果
bool	CResult::m_bPush = false;							//  ボタンを押したかどうか

//--------------------------------------------------------------------------------------
//  リザルトクラスのコンストラクタ
//--------------------------------------------------------------------------------------
CResult::CResult( )
{

}

//--------------------------------------------------------------------------------------
//  リザルトクラスのデストラクタ
//--------------------------------------------------------------------------------------
CResult::~CResult( )
{

}

//--------------------------------------------------------------------------------------
//  リザルトクラスの初期化処理
//--------------------------------------------------------------------------------------
void CResult::Init( void )
{
	m_bPush = false;

	//  カメラクラスポインタが空の場合
	if( m_pCamera == NULL )
	{
		//  カメラクラスの生成
		m_pCamera = new Camera;
		m_pCamera->Init( D3DXVECTOR3( 0.0f , 3.0f , -15.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
						 D3DX_PI / 3.0f , 1.0f , 10000.0f );
	}

	//  メッシュドームの生成
	MeshDome::Create( MeshDome::TYPE_SKY ,
					   D3DXVECTOR3( 0.0f , -1.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
					   D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( 0.00008f , 0.0f ) ,
					   32 , 32 );

	//  メッシュドームの生成
	MeshDome::Create( MeshDome::TYPE_SKY ,
					   D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) , D3DXVECTOR3( D3DX_PI , 0.0f , 0.0f ) ,
					   D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( -0.00003f , 0.0f ) ,
					   32 , 32 );

	//  ゲームクリアの場合
	if( m_result == CResult::GAME_CLEAR )
	{
		//  背景
		SenceBG::Create( SenceBG::TYPE_RESULT , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
						  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
						  D3DXVECTOR2( 1.0f , 1.0f ) );
	}
	//  ゲームオーバーの場合
	else if( m_result == CResult::GAME_OVER )
	{
		//  背景
		SenceBG::Create( SenceBG::TYPE_RESULT2 , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
						  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
						  D3DXVECTOR2( 1.0f , 1.0f ) );
	}
	//  ゲームオーバーの場合
	else if( m_result == CResult::GAME_PLAYER1_WIN )
	{
		//  背景
		SenceBG::Create( SenceBG::TYPE_RESULT3 , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
						  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
						  D3DXVECTOR2( 1.0f , 1.0f ) );

		if( m_pPlayer[ 0 ] == NULL )
		{
			//  プレイヤーの生成
			m_pPlayer[ 0 ] = PlayerResult::Create( D3DXVECTOR3( -7.0f , -5.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , -15.0f ) ,
													D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
													D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );

			m_pPlayer[ 0 ]->Win( );
		}

		if( m_pPlayer[ 1 ] == NULL )
		{
			//  プレイヤーの生成
			m_pPlayer[ 1 ] = PlayerResult::Create( D3DXVECTOR3( 7.0f , -5.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , -15.0f ) , 
													D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
													D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );

			m_pPlayer[ 1 ]->Lose( );
		}
	}
	//  ゲームオーバーの場合
	else if( m_result == CResult::GAME_PLAYER2_WIN )
	{
		//  背景
		SenceBG::Create( SenceBG::TYPE_RESULT4 , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
						  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
						  D3DXVECTOR2( 1.0f , 1.0f ) );

		if( m_pPlayer[ 0 ] == NULL )
		{
			//  プレイヤーの生成
			m_pPlayer[ 0 ] = PlayerResult::Create( D3DXVECTOR3( -7.0f , -5.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , -15.0f ) ,
													D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
													D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );

			m_pPlayer[ 0 ]->Lose( );
		}

		if( m_pPlayer[ 1 ] == NULL )
		{
			//  プレイヤーの生成
			m_pPlayer[ 1 ] = PlayerResult::Create( D3DXVECTOR3( 7.0f , -5.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , -15.0f ) , 
													D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
													D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );

			m_pPlayer[ 1 ]->Win( );
		}
	}

}

//--------------------------------------------------------------------------------------
//  リザルトクラスの終了処理
//--------------------------------------------------------------------------------------
void CResult::Uninit( void )
{
	//  オブジェクトクラスの全解放
	Sence::ReleaseAll( );

	//  カメラクラスポインタが空ではない場合
	if( m_pCamera != NULL )
	{
		//  カメラクラスの破棄
		m_pCamera->Uninit( );
		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  リザルトクラスの更新処理
//--------------------------------------------------------------------------------------
void CResult::Update( void )
{
	// キーボード情報の取得
	Keyboard*			pKeyboard = SceneManager::GetKeyboard( );

	//  Xboxコントローラー情報の取得
	XboxController*	pXInput = SceneManager::GetXInput( );

	//  PS4コントローラー情報の取得
	PS4Controller*		pPS4Input = SceneManager::GetPS4Input( );

	if( Fade::GetFade( ) == Fade::FADE_NONE )
	{
		if( ( pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_CIRCLE ) || pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_OPTIONS ) ) ||
			 pKeyboard->GetKeyboardTrigger( DIK_RETURN ) || pKeyboard->GetKeyboardTrigger( DIK_SPACE ) )
		{
			//  フェードの設定
			Fade::SetFade( Fade::FADE_OUT , Mode::MODE_TITLE , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
		}
	}
}

//--------------------------------------------------------------------------------------
//  リザルトクラスの描画処理
//--------------------------------------------------------------------------------------
void CResult::Draw( void )
{
	Sence::DrawAll( );
}

//--------------------------------------------------------------------------------------
//  結果の代入
//--------------------------------------------------------------------------------------
void CResult::SetResult( CResult::RESULT result )
{
	m_result = result;
}