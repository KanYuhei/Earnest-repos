//--------------------------------------------------------------------------------------
//  管理プログラム   ( manager.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "input.h"
#include "keyboard.h"
#include "xboxController.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "light.h"
#include "teamLogo.h"
#include "title.h"
#include "game.h"
#include "stageSelect.h"
#include "result.h"
#include "joystick.h"
#include "sound.h"
#include "effekseerManager.h"
#include "fade.h"
#include "tutorial.h"
#include "test.h"
#include "Wwise.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
Renderer*				SceneManager::m_pRenderer = NULL;					//  シーンクラスのポインタ
Keyboard*				SceneManager::m_pKeyboard = NULL;					//  キーボードクラスのポインタ
XboxController*			SceneManager::m_pXboxInput = NULL;					//  Xboxコントローラークラスのポインタ
PS4Controller*			SceneManager::m_pPS4Input = NULL;					//  PS4コントローラークラスのポインタ
Texture*				SceneManager::m_pTexture = NULL;					//  テクスチャクラスのポインタ
Light*					SceneManager::m_pLight = NULL;						//  ライトクラスのポインタ
Sound*					SceneManager::m_pSound = NULL;						//  サウンドクラスのポインタ
Wwise*					SceneManager::m_pWwise = NULL;						//  Wwiseクラスのポインタ
EffekseerManager*		SceneManager::m_pEffekseer = NULL;					//  エフェクシア管理クラスのポインタ
Fade*					SceneManager::m_pFade = NULL;						//  フェードクラスのポインタ
Mode*					SceneManager::m_pMode = NULL;						//  モードクラスのポインタ
Mode::MODE				SceneManager::m_mode = Mode::MODE_TEAM_LOGO;		//  モード
SceneManager::STAGE		SceneManager::m_stage = STAGE_MAX;					//  ステージの種類
int						SceneManager::m_nLoop = 0;							//  ループ数

//--------------------------------------------------------------------------------------
//  管理クラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneManager::SceneManager( )
{

}

//--------------------------------------------------------------------------------------
//  管理クラスのデストラクタ
//--------------------------------------------------------------------------------------
SceneManager::~SceneManager( )
{

}

//--------------------------------------------------------------------------------------
//  管理クラスの初期化処理
//--------------------------------------------------------------------------------------
HRESULT SceneManager::Init( HINSTANCE hInstance , HWND hWnd, bool bWindow )
{
	//  クラスポインタの初期化
	m_pRenderer = NULL;
	m_pTexture = NULL;
	m_pKeyboard = NULL;
	m_pPS4Input = NULL;
	m_pLight = NULL;
	m_pSound = NULL;
	m_pWwise = NULL;
	m_pEffekseer = NULL;
	m_pFade = NULL;
	m_nLoop = 0;

	//  テクスチャクラスポインタが空の場合
	if( m_pTexture == NULL )
	{
		//  テクスチャクラスの生成
		m_pTexture = new Texture;
		m_pTexture->Init( );
	}

	//  メッセージの表示
	UINT nID = MessageBox( NULL , "フルスクリーンモードで起動しますか?" , "Full Screen" , MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );

	//  はいと答えた場合
	if( nID == IDYES )
	{
		bWindow = FALSE;
	}

	//	ポインタが空の場合
	if( m_pRenderer == NULL )
	{
		//  レンダラーの生成
		m_pRenderer = new Renderer;

		//  レンダラーの初期化
		if( FAILED( m_pRenderer->Init( hWnd , bWindow ) ) )
		{
			return E_FAIL;
		}
	}

	//  キーボードクラスポインタが空の場合
	if( m_pKeyboard == NULL )
	{
		//  キーボードクラスの生成
		m_pKeyboard = new Keyboard;

		//  キーボードの初期化
		if( FAILED( m_pKeyboard->Init( hInstance , hWnd ) ) )
		{
			return E_FAIL;
		}
	}

	//  Xboxコントローラークラスポインタが空の場合
	if( m_pXboxInput == NULL )
	{
		//  Xboxコントローラークラスの生成
		m_pXboxInput = new XboxController;

		//  Xboxコントローラーの初期化
		if( FAILED( m_pXboxInput->Init( ) ) )
		{

		}
	}

	//  PS4コントローラークラスポインタが空の場合
	if( m_pPS4Input == NULL )
	{
		//  PS4コントローラークラスの生成
		m_pPS4Input = new PS4Controller;

		//  PS4コントローラーの初期化
		if( FAILED( m_pPS4Input->Init( hInstance , hWnd ) ) )
		{
			//return E_FAIL;
		}
	}

	//  ライトクラスポインタが空の場合
	if( m_pLight == NULL )
	{
		//  ライトクラスの生成
		m_pLight = new Light;
		m_pLight->Init( );
	}

	////  サウンドクラスポインタが空の場合
	//if( m_pSound == NULL )
	//{
	//	//  サウンドクラスの生成
	//	m_pSound = new Sound;

	//	//  サウンドの初期化
	//	if( FAILED( m_pSound->Init( hWnd ) ) )
	//	{
	//		return E_FAIL;
	//	}
	//}

	////  Wwiseクラスポインタが空の場合
	//if( m_pWwise == NULL )
	//{
	//	//  Wwiseクラスの生成
	//	m_pWwise = new Wwise;

	//	//  Wwiseの初期化
	//	m_pWwise->Init( );
	//}

	//  フェードクラスポインタが空の場合
	if( m_pFade == NULL )
	{
		//  フェードの生成
		m_pFade = Fade::Create( );
	}

	//  エフェクシアの初期化
	EffekseerManager::Init( );

	//  モードの設定
	SetMode( m_mode );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  管理クラスの終了処理
//--------------------------------------------------------------------------------------
void SceneManager::Uninit( void )
{
	//  エフェクシアの終了
	EffekseerManager::Uninit( );

	//  フェードクラスポインタが空ではない場合
	if( m_pFade != NULL )
	{
		//  フェードの終了処理
		m_pFade->Uninit( );
		delete m_pFade;
		m_pFade = NULL;
	}

	//  モードクラスの終了処理
	if( m_pMode != NULL )
	{
		m_pMode->Uninit( );
		delete m_pMode;
		m_pMode = NULL;
	}

	//  レンダラークラスの終了処理
	if( m_pRenderer != NULL )
	{
		m_pRenderer->Uninit( );
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//  テクスチャクラスポインタが空ではない場合
	if( m_pTexture != NULL )
	{
		//  テクスチャクラスの破棄
		m_pTexture->Uninit( );
		delete m_pTexture;
		m_pTexture = NULL;
	}

	//  キーボードクラスポインタが空ではない場合
	if( m_pKeyboard != NULL )
	{
		//  キーボードクラスの破棄
		m_pKeyboard->Uninit( );
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	//  Xboxコントローラークラスポインタが空ではない場合
	if( m_pXboxInput != NULL )
	{
		//  Xboxコントローラークラスの破棄
		m_pXboxInput->Uninit( );
		delete m_pXboxInput;
		m_pXboxInput = NULL;
	}

	//  PS4コントローラークラスポインタが空ではない場合
	if( m_pPS4Input != NULL )
	{
		//  PS4コントローラークラスの破棄
		m_pPS4Input->Uninit( );
		delete m_pPS4Input;
		m_pPS4Input = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  管理クラスの更新処理
//--------------------------------------------------------------------------------------
void SceneManager::Update( void )
{
	//  キーボードクラスポインタが空ではない場合
	if( m_pKeyboard != NULL )
	{
		m_pKeyboard->Update( );
	}

	//  Xboxコントローラークラスポインタが空ではない場合
	if( m_pXboxInput != NULL )
	{
		m_pXboxInput->Update( );
	}

	//  PS4コントローラークラスポインタが空ではない場合
	if( m_pPS4Input != NULL )
	{
		m_pPS4Input->Update( );
	}

	//  レンダラークラスポインタが空ではない場合
	if( m_pRenderer != NULL )
	{
		m_pRenderer->Update( true );
	}

	//  モードクラスポインタが空ではない場合
	if( m_pMode != NULL )
	{
		m_pMode->Update( );
	}

	//  エフェクシアの更新
	EffekseerManager::Update( );

	//  フェードクラスポインタが空ではない場合
	if( m_pFade != NULL )
	{
		//  フェードの更新
		m_pFade->Update( );
	}
}

//--------------------------------------------------------------------------------------
//  管理クラスの描画処理
//--------------------------------------------------------------------------------------
void SceneManager::Draw( void )
{
	//  レンダラークラスが空ではない場合
	if( m_pRenderer != NULL )
	{
		//  ループ数の初期化
		m_nLoop = 0;

		//  モードゲーム以外または、CPU対戦の場合
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU || m_mode != Mode::MODE_GAME )
		{
			Camera* pCamera = Game::GetCamera( 0 );

			//  描画開始
			m_pRenderer->DrawBegin( );

			if( pCamera != NULL )
			{
				//  カメラ行列の設定
				pCamera->SetCamera( );
			}

			//  エフェクシアの行列を設定する関数
			EffekseerManager::SetMatrix( 0 );

			// バックバッファ＆Ｚバッファのクリア
			m_pRenderer->DrawClearBuffer( );

			//  モードクラスポインタが空ではない場合
			if( m_pMode != NULL )
			{
				m_pMode->Draw( );
			}

			//  フェードクラスポインタが空ではない場合
			if( m_pFade != NULL )
			{
				//  フェードの描画
				m_pFade->Draw( );
			}

			//  描画終了
			m_pRenderer->DrawEnd( );
		}
		//  プレイヤー対戦の場合
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			//  描画開始
			m_pRenderer->DrawBegin( );

			for( int i = 0; i < 2; i++ )
			{
				Camera* pCamera = Game::GetCamera( i );

				if( pCamera != NULL )
				{
					//  カメラ行列の設定
					pCamera->SetCamera( );
				}

				//  エフェクシアの行列を設定する関数
				EffekseerManager::SetMatrix( i );

				// バックバッファ＆Ｚバッファのクリア
				m_pRenderer->DrawClearBuffer( );

				//  モードクラスポインタが空ではない場合
				if( m_pMode != NULL )
				{
					m_pMode->Draw( );
				}

				//  フェードクラスポインタが空ではない場合
				if( m_pFade != NULL )
				{
					//  フェードの描画
					m_pFade->Draw( );
				}

				//  ループ数のカウント
				m_nLoop++;
			}

			//  描画終了
			m_pRenderer->DrawEnd( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  管理クラスの描画処理
//--------------------------------------------------------------------------------------
void SceneManager::SetMode( Mode::MODE mode )
{
	/* 終了処理  */

	//  ゲームの終了
	if( NULL != m_pMode )
	{
		//  エフェクシア全ての停止
		EffekseerManager::StopAll( );

		m_pMode->Uninit( );
		m_pMode = NULL;
	}

	/*  初期化処理  */

	//  次のモードによっての場合分け
	switch( mode )
	{
		case Mode::MODE_TEAM_LOGO:
		{
			//  タイトルの初期化
			if( NULL == m_pMode )
			{
				m_pMode = new TeamLogo;
				m_pMode->Init( );
			}

			break;
		}
		case Mode::MODE_TITLE:
		{
			//  タイトルの初期化
			if( NULL == m_pMode )
			{
				m_pMode = new Title;
				m_pMode->Init( );
			}

			break;
		}
		case Mode::MODE_TUTORIAL:
		{
			//  チュートリアルの初期化
			m_pMode = new Tutorial;
			m_pMode->Init( );

			break;
		}
		case Mode::MODE_STAGE_SELECT:
		{
			//  ステージセレクトの初期化
			m_pMode = new CStageSelect;
			m_pMode->Init( );

			break;
		}
		case Mode::MODE_GAME:
		{
			//  ゲームの初期化
			m_pMode = new Game( );
			m_pMode->Init( );

			break;
		}
		case Mode::MODE_RESULT:
		{
			//  結果の初期化
			m_pMode = new Result;
			m_pMode->Init( );

			break;
		}
		case Mode::MODE_TEST:
		{
			//  結果の初期化
			m_pMode = new Test;
			m_pMode->Init( );

			break;
		}
		default :
		{
			MessageBox( NULL , "正常な値が入っていません。" , "エラー" , MB_OK );

			break;
		}
	}

	//  現在のモードに次のモードを代入
	m_mode = mode;
}

//--------------------------------------------------------------------------------------
//  レンダラークラスを取得をする関数
//--------------------------------------------------------------------------------------
Renderer* SceneManager::GetRenderer( void )
{
	if( m_pRenderer == NULL )
	{
		return NULL;
	}

	return m_pRenderer;
}

//--------------------------------------------------------------------------------------
//  テクスチャクラスを取得をする関数
//--------------------------------------------------------------------------------------
Texture* SceneManager::GetTexture( void )
{
	if( m_pTexture == NULL )
	{
		return NULL;
	}

	return m_pTexture;
}

//--------------------------------------------------------------------------------------
//  キーボードクラスを取得をする関数
//--------------------------------------------------------------------------------------
Keyboard* SceneManager::GetKeyboard( void )
{
	if( m_pKeyboard == NULL )
	{
		return NULL;
	}

	return m_pKeyboard;
}

//--------------------------------------------------------------------------------------
//  Xboxコントローラークラスを取得をする関数
//--------------------------------------------------------------------------------------
XboxController* SceneManager::GetXInput( void )
{
	if( m_pXboxInput == NULL )
	{
		return NULL;
	}

	return m_pXboxInput;
}

//--------------------------------------------------------------------------------------
//  PS4コントローラークラスを取得をする関数
//--------------------------------------------------------------------------------------
PS4Controller* SceneManager::GetPS4Input( void )
{
	if( m_pPS4Input == NULL )
	{
		return NULL;
	}

	return m_pPS4Input;
}

//--------------------------------------------------------------------------------------
//  ライトクラスを取得をする関数
//--------------------------------------------------------------------------------------
Light* SceneManager::GetLight( void )
{
	if( m_pLight == NULL )
	{
		return NULL;
	}

	return m_pLight;
}

//--------------------------------------------------------------------------------------
//  モード情報の取得をする関数
//--------------------------------------------------------------------------------------
Mode::MODE SceneManager::GetMode( void )
{
	return m_mode;
}

//--------------------------------------------------------------------------------------
//  モード情報の取得をする関数
//--------------------------------------------------------------------------------------
Mode* SceneManager::GetModeClass( void )
{
	return m_pMode;
}

//--------------------------------------------------------------------------------------
//  ステージの種類を設定する関数
//--------------------------------------------------------------------------------------
void SceneManager::SetStage( SceneManager::STAGE stage )
{
	m_stage = stage;
}

//--------------------------------------------------------------------------------------
//  ステージの種類を取得をする関数
//--------------------------------------------------------------------------------------
SceneManager::STAGE SceneManager::GetStage( void )
{
	return m_stage;
}

//--------------------------------------------------------------------------------------
//  サウンド情報の取得をする関数
//--------------------------------------------------------------------------------------
Sound* SceneManager::GetSound( void )
{
	return m_pSound;
}

//--------------------------------------------------------------------------------------
//  Wwise情報の取得をする関数
//--------------------------------------------------------------------------------------
Wwise* SceneManager::GetWwise( void )
{
	return m_pWwise;
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理クラスの取得をする関数
//--------------------------------------------------------------------------------------
EffekseerManager* SceneManager::GetEffekseer( void )
{
	return m_pEffekseer;
}

//--------------------------------------------------------------------------------------
//  描画ループ数を取得をする関数
//--------------------------------------------------------------------------------------
int SceneManager::GetLoop( void )
{
	return m_nLoop;
}