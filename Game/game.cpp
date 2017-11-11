﻿//--------------------------------------------------------------------------------------
//  ゲームプログラム   ( game.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "game.h"
#include "sceneModel.h"
#include "meshField.h"
#include "meshWall.h"
#include "meshDome.h"
#include "player.h"
#include "camera.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "renderer.h"
#include "enemy.h"
#include "sceneBG.h"
#include "sceneFBX.h"
#include "field.h"
#include "effekseerManager.h"
#include "imgui_impl_dx9.h"
#include "light.h"
#include <stdio.h>
#include <random>
#include "Wwise.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define PLAYER_SPEED			( 0.8f )
#define WALL_DIVIDE				( 1 )

#define STAGE1_FILENAME			"data/STAGE/stage1.txt"
#define STAGE2_FILENAME			"data/STAGE/stage2.txt"
#define STAGE3_FILENAME			"data/STAGE/stage3.txt"

static const float	ATMOSPHERE_SIZE = 15.0f; 
static const int	ATMOSPHERE_NUM = 1; 

static const float	THIEF_SIZE = 1.0f; 

static const float	STAGE1_DISTACE= 300.0f; 

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
Game::MODE_VS	Game::m_modeVS = Game::MODE_VS_CPU;				//  対戦モード
Game::CRYSTAL	Game::m_crystal[ 2 ] = { Game::CRYSTAL_FIRE };	//  魔法クリスタル
Camera*		Game::m_pCamera[ 2 ] = { NULL };					//  カメラクラスのポインタ
int				Game::m_nNumDivide = MESH_FIELD_DIVIDE;			//  フィールド分割数
int				Game::m_nNumField = 0;								//  フィールド数
int				Game::m_nFocusField = 0;							//  フォーカスがあっているフィールド数
float			Game::m_fSize = ONE_CUBE_SIZE;						//  フォーカスがあっているフィールド数
bool			Game::m_bClear = false;							//  クリアしたかどうか
D3DXVECTOR3		Game::m_fieldPos( 0.0f , 0.0f , 0.0f );			//  フォーカスがあっているフィールド数
D3DXVECTOR3		Game::m_fieldMin( 0.0f , 0.0f , 0.0f );			//  フィールド最小座標
D3DXVECTOR3		Game::m_fieldMax( 0.0f , 0.0f , 0.0f );			//  フィールド最大座標
Mode::MODE		Game::m_nextMode = Mode::MODE_RESULT;				//  次の場面

//--------------------------------------------------------------------------------------
//  ゲームクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Game::Game( )
{

}

//--------------------------------------------------------------------------------------
//  ゲームクラスのデストラクタ
//--------------------------------------------------------------------------------------
Game::~Game( )
{

}

//--------------------------------------------------------------------------------------
//  ゲームクラスの初期化処理
//--------------------------------------------------------------------------------------
void Game::Init( void )
{
	m_bClear = false;

	//SenceFBX::Create( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , 
	//				   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
	//				   D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) );

	for( int i = 0; i < 2; i++ )
	{
		m_pCamera[ i ] = NULL;
	}

	//  ステージの生成
	SetStage( );

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );
	
    // Setup ImGui binding
    ImGui_ImplDX9_Init( GetWindow( ) , pDevice );
}

//--------------------------------------------------------------------------------------
//  ゲームクラスの終了処理
//--------------------------------------------------------------------------------------
void Game::Uninit( void )
{
	//  ImGuiの終了
	ImGui_ImplDX9_Shutdown( );

	//  オブジェクトクラスの全解放
	Sence::ReleaseAll( );

	for( int i = 0; i < 2; i++ )
	{
		//  カメラクラスポインタが空ではない場合
		if( m_pCamera[ i ] != NULL )
		{
			//  カメラクラスの破棄
			m_pCamera[ i ]->Uninit( );
			delete m_pCamera[ i ];
			m_pCamera[ i ] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------
//  ゲームクラスの更新処理
//--------------------------------------------------------------------------------------
void Game::Update( void )
{
	// キーボード情報の取得
	Keyboard*			pKeyboard = SceneManager::GetKeyboard( );

	for( int i = 0; i < 2; i++ )
	{
		//  カメラクラスポインタが空ではない場合
		if( m_pCamera[ i ] != NULL )
		{
			if( pKeyboard->GetKeyboardTrigger( DIK_1 ) )
			{
				//  モードの切り替え
				//m_pCamera[ i ]->ChangeMode( );
			}

			m_pCamera[ i ]->Update( );
		}
	}

	ImGui_ImplDX9_NewFrame( );

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_col = ImColor(114, 144, 154);

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
		ImGui::Begin( "Test Window", &show_test_window );
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_col);
        if (ImGui::Button("Test Window")) show_test_window ^= 1;
        if (ImGui::Button("Another Window")) show_another_window ^= 1;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End( );
    }

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        ImGui::End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (show_test_window)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowTestWindow(&show_test_window);
    }

	if( Fade::GetFade( ) == Fade::FADE_NONE )
	{
		if(  pKeyboard->GetKeyboardTrigger( DIK_F1 ) )
		{
			//  フェードの設定
			Fade::SetFade( Fade::FADE_OUT , Mode::MODE_TITLE , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
		}
		else if(  pKeyboard->GetKeyboardTrigger( DIK_F2 ) )
		{
			//  フェードの設定
			Fade::SetFade( Fade::FADE_OUT , Mode::MODE_STAGE_SELECT , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
		}
	}

	if( m_bClear )
	{
		//  フェードの設定
		Fade::SetFade( Fade::FADE_OUT , m_nextMode , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
	}
}

//--------------------------------------------------------------------------------------
//  ゲームクラスの描画処理
//--------------------------------------------------------------------------------------
void Game::Draw( void )
{
	//  シーン全ての描画
	Sence::DrawAll( );

	//  GUIの描画
	//ImGui::Render( );
}

//--------------------------------------------------------------------------------------
//  カメラクラスを取得をする関数
//--------------------------------------------------------------------------------------
Camera* Game::GetCamera( int nCameraNo )
{
	if( nCameraNo >= 2 )
	{
		return NULL;
	}

	return m_pCamera[ nCameraNo ];
}

//--------------------------------------------------------------------------------------
//  フィールド分割数の取得
//--------------------------------------------------------------------------------------
int Game::GetNumDivide( void )
{
	return m_nNumDivide;
}

//--------------------------------------------------------------------------------------
//  フィールド数の取得
//--------------------------------------------------------------------------------------
int Game::GetNumField( void )
{
	return m_nNumField;
}

//--------------------------------------------------------------------------------------
//  フォーカスがあっているフィールド番号
//--------------------------------------------------------------------------------------
int Game::GetFocusField( void )
{
	return m_nFocusField;
}

//--------------------------------------------------------------------------------------
//  ゴールクラスを取得をする関数
//--------------------------------------------------------------------------------------
void Game::SetStage( void )
{
	char		aFileName[ SceneManager::STAGE_MAX ][ 128 ] = {
																STAGE1_FILENAME ,
																STAGE2_FILENAME ,
																STAGE3_FILENAME ,
														  };

	int			nNumBlock = 0;						//  ブロック数の格納
	int			nType = -1;							//  ブロックの種類
	int			nNumCube = 0;						//  キューブ数の格納
	D3DXVECTOR3 workPos( 0.0f , 0.0f , 0.0f );		//  座標の格納
	int			nBaseCube = -1;						//  基準キューブ数の格納
	int			nCubeDirect = -1;					//  追加進行方向の代入

	m_nNumDivide = MESH_FIELD_DIVIDE;

	Light* pLight = SceneManager::GetLight( );

	if( SceneManager::GetStage( ) == SceneManager::STAGE1 )
	{
		if( pLight != NULL )
		{
			pLight->SetDiffuseColor( D3DXCOLOR( 1.0f , 0.9f , 0.3f , 1.0f ) );
		}

		//  メッシュドームの生成
		MeshDome::Create( MeshDome::TYPE_SKY,
						   D3DXVECTOR3( 0.0f , -15.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
						   D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( 0.00001f , 0.0f ) ,
						   32 , 4 );

		//  CPU対戦の場合
		if( m_modeVS == Game::MODE_VS_CPU )
		{
			//  背景
			SenceBG::Create( SenceBG::TYPE_GAME , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
							  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
							  D3DXVECTOR2( 1.0f , 1.0f ) );

			//  カメラクラスポインタが空の場合
			if( m_pCamera[ 0 ] == NULL )
			{
				//  カメラクラスの生成
				m_pCamera[ 0 ] = new Camera;
				m_pCamera[ 0 ]->Init( D3DXVECTOR3( 0.0f , 10.0f , -( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
									  D3DX_PI / 3.0f , 1.0f , 10000.0f );
			}

			Player::MAGIC magic;

			switch( m_crystal[ 0 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic = Player::MAGIC_FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic = Player::MAGIC_TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic = Player::MAGIC_LIGHTNING;

					break;
				}
			}

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic );

			//  敵の生成
			Enemy::Create( D3DXVECTOR3( 0.0f , 0.0f , STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED );
		}
		//  プレイヤー対戦の場合
		else if( m_modeVS == Game::MODE_VS_PLAYER )
		{
			//  カメラクラスポインタが空の場合
			if( m_pCamera[ 0 ] == NULL )
			{
				//  カメラクラスの生成
				m_pCamera[ 0 ] = new Camera;
				m_pCamera[ 0 ]->Init( D3DXVECTOR3( 0.0f , 10.0f , -( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
									  D3DX_PI / 3.0f , 1.0f , 10000.0f , 0 , true );
			}

			//  カメラクラスポインタが空の場合
			if( m_pCamera[ 1 ] == NULL )
			{
				//  カメラクラスの生成
				m_pCamera[ 1 ] = new Camera;
				m_pCamera[ 1 ]->Init( D3DXVECTOR3( 0.0f , 10.0f , ( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
									  D3DX_PI / 3.0f , 1.0f , 10000.0f , 1 , true );
			}

			Player::MAGIC magic;

			switch( m_crystal[ 0 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic = Player::MAGIC_FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic = Player::MAGIC_TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic = Player::MAGIC_LIGHTNING;

					break;
				}
			}

			Player::MAGIC magic2;

			switch( m_crystal[ 1 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic2 = Player::MAGIC_FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic2 = Player::MAGIC_TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic2 = Player::MAGIC_LIGHTNING;

					break;
				}
			}

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic , 0 );

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic2 , 1 );
		}

		//  フィールドの生成
		Field::Create( Field::TYPE_STAGE000 , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

		//  メッシュフィールドの生成
		MeshField::Create( MeshField::TYPE_SEA , 
							D3DXVECTOR3( 0.0f , -9.0f , 0.0f ) , 
							D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							D3DXVECTOR3( 20000.0f , 0.0f , 20000.0f ) , 
							D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) , 
							16 , 
							16 ,
							D3DXVECTOR2( 0.0001f , 0.0001f ) );
	}
	else if( SceneManager::GetStage( ) == SceneManager::STAGE2 )
	{
		if( pLight != NULL )
		{
			pLight->SetDiffuseColor( D3DXCOLOR( 1.0f , 0.3f , 0.3f , 1.0f ) );
		}

		//  メッシュドームの生成
		MeshDome::Create( MeshDome::TYPE_COSMO ,
						   D3DXVECTOR3( 0.0f , -15.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
						   D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( 0.00001f , 0.0f ) ,
						   32 , 4 );

		//  CPU対戦の場合
		if( m_modeVS == Game::MODE_VS_CPU )
		{
			//  背景
			SenceBG::Create( SenceBG::TYPE_GAME , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
							  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
							  D3DXVECTOR2( 1.0f , 1.0f ) );

			//  カメラクラスポインタが空の場合
			if( m_pCamera[ 0 ] == NULL )
			{
				//  カメラクラスの生成
				m_pCamera[ 0 ] = new Camera;
				m_pCamera[ 0 ]->Init( D3DXVECTOR3( 0.0f , 10.0f , -( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
									  D3DX_PI / 3.0f , 1.0f , 10000.0f );
			}

			Player::MAGIC magic;

			switch( m_crystal[ 0 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic = Player::MAGIC_FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic = Player::MAGIC_TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic = Player::MAGIC_LIGHTNING;

					break;
				}
			}

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic );

			//  敵の生成
			Enemy::Create( D3DXVECTOR3( 0.0f , 0.0f , STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED );
		}
		//  プレイヤー対戦の場合
		else if( m_modeVS == Game::MODE_VS_PLAYER )
		{
			//  カメラクラスポインタが空の場合
			if( m_pCamera[ 0 ] == NULL )
			{
				//  カメラクラスの生成
				m_pCamera[ 0 ] = new Camera;
				m_pCamera[ 0 ]->Init( D3DXVECTOR3( 0.0f , 10.0f , -( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
									  D3DX_PI / 3.0f , 1.0f , 10000.0f , 0 , true );
			}

			//  カメラクラスポインタが空の場合
			if( m_pCamera[ 1 ] == NULL )
			{
				//  カメラクラスの生成
				m_pCamera[ 1 ] = new Camera;
				m_pCamera[ 1 ]->Init( D3DXVECTOR3( 0.0f , 10.0f , ( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
									  D3DX_PI / 3.0f , 1.0f , 10000.0f , 1 , true );
			}

			Player::MAGIC magic;

			switch( m_crystal[ 0 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic = Player::MAGIC_FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic = Player::MAGIC_TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic = Player::MAGIC_LIGHTNING;

					break;
				}
			}

			Player::MAGIC magic2;

			switch( m_crystal[ 1 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic2 = Player::MAGIC_FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic2 = Player::MAGIC_TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic2 = Player::MAGIC_LIGHTNING;

					break;
				}
			}

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic, 0 );

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic2 , 1 );
		}

		//  フィールドの生成
		Field::Create( Field::TYPE_STAGE000 , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

		//  メッシュフィールドの生成
		MeshField::Create( MeshField::TYPE_SEA , 
							D3DXVECTOR3( 0.0f , -9.0f , 0.0f ) , 
							D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							D3DXVECTOR3( 20000.0f , 0.0f , 20000.0f ) , 
							D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) , 
							16 , 
							16 ,
							D3DXVECTOR2( 0.0001f , 0.0001f ) );
	}

	for( int i = 0; i < ATMOSPHERE_NUM; i++ )
	{
		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		/////////////////////////////////////////////////////
		//  第1ライン
		/////////////////////////////////////////////////////

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE , 
								   D3DXVECTOR3( 0.0f , 0.0f , -500.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( 0.0f , 0.0f , 500.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( -500.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( 500.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( -500.0f , 0.0f , -500.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( -500.0f , 0.0f , 500.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( 500.0f , 0.0f , -500.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( 500.0f , 0.0f , 500.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		/////////////////////////////////////////////////////
		//  第2ライン
		/////////////////////////////////////////////////////

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE , 
								   D3DXVECTOR3( 0.0f , 0.0f , -250.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( 0.0f , 0.0f , 250.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( -250.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( 250.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( -250.0f , 0.0f , -250.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( -250.0f , 0.0f , 250.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( 250.0f , 0.0f , -250.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								   D3DXVECTOR3( 250.0f , 0.0f , 250.0f ) ,
								   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								   D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								   1.0f );
	}
}

//--------------------------------------------------------------------------------------
//  次のモード情報の代入
//--------------------------------------------------------------------------------------
void Game::SetNextMode( Mode::MODE mode )
{
	//  クリア状態にして次のモードの設定
	m_bClear = true;
	m_nextMode = mode;
}

//--------------------------------------------------------------------------------------
//  対戦モードを設定
//--------------------------------------------------------------------------------------
void Game::SetModeVS( MODE_VS modeVS )
{
	m_modeVS = modeVS;
}

//--------------------------------------------------------------------------------------
//  対戦モードを取得
//--------------------------------------------------------------------------------------
Game::MODE_VS Game::GetModeVS( void )
{
	return m_modeVS;
}

//--------------------------------------------------------------------------------------
//  対戦モードを設定
//--------------------------------------------------------------------------------------
void Game::SetCrystal( int nIndex , CRYSTAL crystal )
{
	m_crystal[ nIndex ] = crystal;
}

//--------------------------------------------------------------------------------------
//  フィールド最小座標を設定
//--------------------------------------------------------------------------------------
void Game::SetFieldMin( D3DXVECTOR3 min )
{
	m_fieldMin = min;
}

//--------------------------------------------------------------------------------------
//  フィールド最大座標を設定
//--------------------------------------------------------------------------------------
void Game::SetFieldMax( D3DXVECTOR3 max )
{
	m_fieldMax = max;
}

//--------------------------------------------------------------------------------------
//  フィールド最小座標を取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Game::GetFieldMin( void )
{
	return m_fieldMin;
}

//--------------------------------------------------------------------------------------
//  フィールド最大座標を取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Game::GetFieldMax( void )
{
	return m_fieldMax;
}