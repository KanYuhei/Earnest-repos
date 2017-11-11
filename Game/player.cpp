//--------------------------------------------------------------------------------------
//  プレイヤー   ( player.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "player.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "scene2D.h"
#include "meshField.h"
#include "enemy.h"
#include "bullet.h"
#include "fade.h"
#include "bullet.h"
#include "particle.h"
#include "hitField.h"
#include "result.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"
#include "meshTracing.h"
#include "stencilShadow.h"
#include "magic.h"
#include "playerUI.h"
#include "shadow.h"
#include "life.h"
#include "brave.h"
#include "stateAnimator.h"
#include "utility.h"
#include <random>
#include "Wwise.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define PLAYER_TO_CAMERA_DISTANCE_Y				( 9.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Y2			( 10.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Z				( 20.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Z2			( 23.0f )
#define PLAYER_TO_CUBE_DISTANCE					( 7.0f )
#define PLAYER_TO_CUBE_PUT_DISTANCE				( 2.5f )
#define PLAYER_SIZE								( 2.5f )
#define PLAYER_GOAL_SIZE						( 0.8f )
#define PLAYER_HIT_SIZE							( 2.0f )
#define PLAYER_WAIT_TIME						( 20 )
#define PLAYER_GRAVITY							( 0.1f )
#define PLAYER_GRAVITY2							( 0.2f )
#define PLAYER_ENABLE_NUM_JUMP					( 2 )
#define PLAYER_TO_CAMERA_RATE					( 0.5f )
#define PLAYER_LOCKON_DASH_SPEED				( 3.0f )
#define PLAYER_HIT_SPHERE_LENGTH				( 6.0f )
#define PLAYER_HEAL_LIFE						( 20 )

#define PLAYER_AVOID_SPEED						( 1.5f )

#define PLAYER_HIT_SPHERE_POS_Y					( 3.0f )

#define PLAYER_LIFE_POS_X						( 389.0f )
#define PLAYER_LIFE_POS_Y						( 661.0f )
#define PLAYER_LIFE_SIZE_X						( 350.0f )
#define PLAYER_LIFE_SIZE_Y						( 49.0f )

#define PLAYER_VS_LIFE_POS_X					( 125.0f )
#define PLAYER_VS_LIFE_POS_Y					( 646.0f )
#define PLAYER_VS_LIFE_SIZE_X					( 442.0f )
#define PLAYER_VS_LIFE_SIZE_Y					( 38.0f )

#define PLAYER_BRAVE_POS_X						( 440.0f )
#define PLAYER_BRAVE_POS_Y						( 570.0f )
#define PLAYER_BRAVE_SIZE_X						( 60.0f )
#define PLAYER_BRAVE_SIZE_Y						( 60.0f )

#define PLAYER_UI_POS_X							( SCREEN_WIDTH * 0.25f )
#define PLAYER_UI_POS_Y							( SCREEN_HEIGHT * 0.88f )
#define PLAYER_UI_SIZE_X						( 640.0f )
#define PLAYER_UI_SIZE_Y						( 200.0f )

#define PLAYER2_LIFE_POS_X						( 975.0f )
#define PLAYER2_LIFE_POS_Y						( 605.0f )
#define PLAYER2_LIFE_SIZE_X						( 255.0f )
#define PLAYER2_LIFE_SIZE_Y						( 30.0f )

#define PLAYER2_VS_LIFE_POS_X					( 715.0f )
#define PLAYER2_VS_LIFE_POS_Y					( 646.0f )
#define PLAYER2_VS_LIFE_SIZE_X					( 442.0f )
#define PLAYER2_VS_LIFE_SIZE_Y					( 38.0f )

#define PLAYER2_BRAVE_POS_X						( 990.0f )
#define PLAYER2_BRAVE_POS_Y						( 565.0f )
#define PLAYER2_BRAVE_SIZE_X					( 35.0f )
#define PLAYER2_BRAVE_SIZE_Y					( 35.0f )

#define PLAYER2_UI_POS_X						( SCREEN_WIDTH * 0.75f )
#define PLAYER2_UI_POS_Y						( SCREEN_HEIGHT * 0.88f )
#define PLAYER2_UI_SIZE_X						( 640.0f )
#define PLAYER2_UI_SIZE_Y						( 200.0f )

#define PLAYER_JUMP_							( 0.4f )

#define PLAYER_FINISH_SPEED						( 3.0f )

#define PLAYER_HOMIMG_TIME						( 90.0f )
#define PLAYER_ENABLE_HP_ATTACK_DIST			( 150.0f )
#define PLAYER_HP_ATTACK_SPEED					( 1.0f )

static const int	PLAYER_BRAVE_DAMAGE			= 400;
static const int	PLAYER_HP_DAMAGE			= 150;
static const int	PLAYER_FINISHER_DAMAGE		= 250;

static const float	PLAYER_HP_RANGE				= 30.0f;
static const float	PLAYER_HP_RANGE_HEIGHT		= 12.0f;

static const float	PLAYER_BRAVE_BLOW_POWER		= 8.0f;
static const float	PLAYER_HP_BLOW_POWER		= 3.0f;
static const float	PLAYER_FINISHER_BLOW_POWER	= 7.0f;

static const int	PLAYER_GUARD_FRAME			= 5;

static const float	CAMERA_ROTATE_ANGLE			= 0.025f;

static const float	PLAYER_HEIGHT = 10.0f;

static const float	PLAYER_SHADOW_SCALE = 0.55f;

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  プレイヤークラスのコンストラクタ
//--------------------------------------------------------------------------------------
Player::Player( ) : SceneModelAnim( 5 )
{
	m_beforePos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_vecDirect = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	m_fSpeed = 0.0f;
	m_nWaitTime = 0;
	m_fCurrentJumpPower = 0.0f;
	m_nCntJump = 0;
	m_bLockOn = true;
	m_stencilShadow = nullptr;
	m_attackHitSphere.fLength = PLAYER_HIT_SPHERE_LENGTH;
	m_attackHitSphere.position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_hitSphere.fLength = PLAYER_HIT_SPHERE_LENGTH * 0.5f;
	m_hitSphere.position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_pLife = NULL;
	m_pBrave = NULL;
	m_pStateAnimator = NULL;
	m_pPlayerUI = NULL;
	m_pMagic = NULL;
	m_nLife = BASE_LIFE;
	m_nBrave = BASE_BRAVE;
	m_bVoiceStart = false;
	m_bJudgeHit = true;
	m_bHomingLeft = false;
	m_bSlashEffekseer = false;
	m_fHomingTime = 0.0f;
	m_fBlowPower = 0.0f;
	m_fMoveY = 0.0f;
	m_nPressTime = 0;
}

//--------------------------------------------------------------------------------------
//  プレイヤークラスのデストラクタ
//--------------------------------------------------------------------------------------
Player::~Player( )
{

}

//--------------------------------------------------------------------------------------
//  プレイヤーの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Player::Init( void )
{
	//  シーンモデル
	SceneModelAnim::Init( );

	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_PLAYER );

	//  ステンシルシャドウの生成
	m_stencilShadow = StencilShadow::Create( StencilShadow::TYPE::SPHERE ,
											 m_position , 
											 D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
											 D3DXVECTOR3( PLAYER_SHADOW_SCALE , PLAYER_SHADOW_SCALE , PLAYER_SHADOW_SCALE ) );

	if( m_nPlayerNo == 0 )
	{
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			//  体力の生成
			m_pLife = Life::Create( Life::TYPE_LIFE000 ,
									 D3DXVECTOR3( PLAYER_LIFE_POS_X , PLAYER_LIFE_POS_Y , 0.0f ) ,
									 D3DXVECTOR3( PLAYER_LIFE_SIZE_X , PLAYER_LIFE_SIZE_Y , 0.0f ) ,
									 D3DXVECTOR2( 0.0f , 0.0f ) ,
									 D3DXVECTOR2( 1.0f , 1.0f ) ,
									 m_nLife );
			////  ブレイブの生成
			//m_pBrave = Brave::Create( D3DXVECTOR3( PLAYER_BRAVE_POS_X , PLAYER_BRAVE_POS_Y , 0.0f ) ,
			//						   D3DXVECTOR3( PLAYER_BRAVE_SIZE_X , PLAYER_BRAVE_SIZE_Y , 0.0f ) ,
			//						   1000 , 4 );
		}
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			//  プレイヤーUIの生成
			m_pPlayerUI = PlayerUI::Create( PlayerUI::TYPE_PLAYER1 ,
											 D3DXVECTOR3( PLAYER_UI_POS_X , PLAYER_UI_POS_Y , 0.0f ) ,
											 D3DXVECTOR3( PLAYER_UI_SIZE_X , PLAYER_UI_SIZE_Y , 0.0f ) );

			//  体力の生成
			m_pLife = Life::Create( Life::TYPE_LIFE002 ,
									 D3DXVECTOR3( PLAYER_VS_LIFE_POS_X , PLAYER_VS_LIFE_POS_Y , 0.0f ) ,
									 D3DXVECTOR3( PLAYER_VS_LIFE_SIZE_X , PLAYER_VS_LIFE_SIZE_Y , 0.0f ) ,
									 D3DXVECTOR2( 0.0f , 0.0f ) ,
									 D3DXVECTOR2( 1.0f , 1.0f ) ,
									 m_nLife );

			////  ブレイブの生成
			//m_pBrave = Brave::Create( D3DXVECTOR3( PLAYER_BRAVE_POS_X , PLAYER_BRAVE_POS_Y , 0.0f ) ,
			//						   D3DXVECTOR3( PLAYER_BRAVE_SIZE_X , PLAYER_BRAVE_SIZE_Y , 0.0f ) ,
			//						   1000 , 4 );
		}

		//  体力の設定
		m_pLife->SetEndLife( m_nLife );										 
	}
	else if( m_nPlayerNo == 1 )
	{
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			m_pLife = Life::Create( Life::TYPE_LIFE001 ,
									 D3DXVECTOR3( PLAYER2_LIFE_POS_X , PLAYER2_LIFE_POS_Y , 0.0f ) ,
									 D3DXVECTOR3( PLAYER2_LIFE_SIZE_X , PLAYER2_LIFE_SIZE_Y , 0.0f ) ,
									 D3DXVECTOR2( 0.0f , 0.0f ) ,
									 D3DXVECTOR2( 1.0f , 1.0f ) ,
									 m_nLife );

			////  ブレイブの生成
			//m_pBrave = Brave::Create( D3DXVECTOR3( PLAYER2_BRAVE_POS_X , PLAYER2_BRAVE_POS_Y , 0.0f ) ,
			//						   D3DXVECTOR3( PLAYER2_BRAVE_SIZE_X , PLAYER2_BRAVE_SIZE_Y , 0.0f ) ,
			//						   1000 , 4 );
		}
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			//  プレイヤーUIの生成
			m_pPlayerUI = PlayerUI::Create( PlayerUI::TYPE_PLAYER2 ,
											 D3DXVECTOR3( PLAYER2_UI_POS_X , PLAYER2_UI_POS_Y , 0.0f ) ,
											 D3DXVECTOR3( PLAYER2_UI_SIZE_X , PLAYER2_UI_SIZE_Y , 0.0f ) );

			m_pLife = Life::Create( Life::TYPE_LIFE003 ,
									 D3DXVECTOR3( PLAYER2_VS_LIFE_POS_X , PLAYER2_VS_LIFE_POS_Y , 0.0f ) ,
									 D3DXVECTOR3( PLAYER2_VS_LIFE_SIZE_X , PLAYER2_VS_LIFE_SIZE_Y , 0.0f ) ,
									 D3DXVECTOR2( 0.0f , 0.0f ) ,
									 D3DXVECTOR2( 1.0f , 1.0f ) ,
									 m_nLife );

			////  ブレイブの生成
			//m_pBrave = Brave::Create( D3DXVECTOR3( PLAYER2_BRAVE_POS_X , PLAYER2_BRAVE_POS_Y , 0.0f ) ,
			//						   D3DXVECTOR3( PLAYER2_BRAVE_SIZE_X , PLAYER2_BRAVE_SIZE_Y , 0.0f ) ,
			//						   1000 , 4 );
		}

		//  体力の設定
		m_pLife->SetEndLife( m_nLife );
	}

	//  ステートマシン( アニメーター )の生成
	m_pStateAnimator = StateAnimator::Create( StateAnimator::MOTION_NEUTRAL );

	//  カメラの取得
	Camera* pCamera = Game::GetCamera( m_nPlayerNo );

	D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

	D3DXVECTOR3 workPosEye = m_position;

	//  CPU対戦の場合
	if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
	{
		workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y;
		workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
	}
	//  プレイヤー対戦の場合
	else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
	{
		workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
		workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z2;
	}

	//  カメラの視点設定
	pCamera->SetCameraPosEyeDirect( workPosEye );

	D3DXVECTOR3 workPosAt = m_position;
	workPosAt.z += 5.0f;

	//  カメラの注視点設定
	pCamera->SetCameraPosAtDirect( workPosAt );

	//  プレイヤー注視点の設定
	m_posAt = m_position + cameraVecDirect * 5.0f;

	//  方向ベクトルの代入
	D3DXVec3Normalize( &m_vecDirect , &( m_posAt - m_position ) );

	//D3DXVECTOR3 positionUp = m_position;
	//positionUp.y += PLAYER_HEIGHT;
	//D3DXVECTOR3 positionDown = m_position;

	////  軌跡メッシュの生成
	//m_pMeshTracing = MeshTracing::Create( D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ) , positionUp , positionDown );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  プレイヤーの終了処理
//--------------------------------------------------------------------------------------
void Player::Uninit( void )
{
	//  シーンモデル
	SceneModelAnim::Uninit( );

	if( m_pLife != NULL )
	{
		m_pLife = NULL;
	}

	if( m_pBrave != NULL )
	{
		m_pBrave = NULL;
	}

	if( m_pPlayerUI != NULL )
	{
		m_pPlayerUI = NULL;
	}

	if( m_pStateAnimator != NULL )
	{
		delete m_pStateAnimator;
		m_pStateAnimator = NULL;
	}

	if( m_stencilShadow != NULL )
	{
		m_stencilShadow = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーの更新処理
//--------------------------------------------------------------------------------------
void Player::Update( void )
{
	// キーボード情報の取得
	Keyboard*			pKeyboard = SceneManager::GetKeyboard( );

	//  Xboxコントローラー情報の取得
	XboxController*	pXInput = NULL;

	//  PS4コントローラー情報の取得
	PS4Controller*		pPS4Input = NULL;

	pPS4Input = SceneManager::GetPS4Input( );

	//  カメラの取得
	Camera* pCamera = Game::GetCamera( m_nPlayerNo );

	//  シーンクラスのポインタ
	Scene* pScene = NULL;			

	//  シーンクラスのポインタ
	Scene *pScene2 =NULL;										

	//  当たり判定用フィールドクラス
	HitField* pHitField = NULL;

	//  移動方向
	D3DXVECTOR3 move( 0.0f , 0.0f , 0.0f );

	D3DXVECTOR3 cameraVecDirect;

	//  押したかのフラグ
	bool bPush = false;

	//  ステートマシン( アニメーター )クラスのポインタがある場合
	if( m_pStateAnimator != NULL )
	{
		//  開始ボイスをまだ再生していない場合
		if( m_bVoiceStart == false )
		{
			m_bVoiceStart = true;
		}

		//  基本は当たり判定をする状態に
		m_bJudgeHit = true;

		//  ガード中ではない状態に
		m_bGuard = false;

		float fTargetDistance = 0.0f;

#pragma omp parallel for
		//  優先度の最大数分のループ
		for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
		{
			//  シーンの先頭アドレスを取得
			pScene = Scene::GetScene( nCntPriority );

			//  シーンが空ではない間ループ
			while( pScene != NULL )
			{
				Scene::OBJTYPE objType;						//  物体の種類

				//  物体の種類の取得
				objType = pScene->GetObjType( );

				//  CPU対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
				{
					//  種類が敵の場合
					if( objType == Scene::OBJTYPE_ENEMY )
					{
						//  ダウンキャスト
						Enemy* pEnemy = ( Enemy* )pScene;

						//  敵の座標の代入
						D3DXVECTOR3 posEnemy = pEnemy->GetPos( );

						//  距離の算出
						fTargetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
												 ( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
												 ( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );
					}
				}
				//  プレイヤー対戦モードの場合
				else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  ダウンキャスト
						Player* pPlayer = ( Player* )pScene;

						//  自分以外のプレイヤー番号の場合
						if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
						{
							//  敵の座標の代入
							D3DXVECTOR3 posEnemy = pPlayer->GetPos( );

							//  距離の算出
							fTargetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
													 ( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
													 ( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}

		////////////////////////////////////////////////////////
		//  状態毎の操作
		////////////////////////////////////////////////////////

		/*----------------------------------------------------------
		    左スティックやWASDでの移動入力情報の取得
		----------------------------------------------------------*/
#ifdef KEYBOARD_ENABLE

		//  移動量の初期化
		move = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		if( m_nPlayerNo == 0 )
		{
			if( pKeyboard->GetKeyboardPress( DIK_W ) )
			{
				move.z += 1.0f;
			}
			else if( pKeyboard->GetKeyboardPress( DIK_S ) )
			{
				move.z -= 1.0f;
			}

			if( pKeyboard->GetKeyboardPress( DIK_A ) )
			{
				move.x -= 1.0f;
			}
			else if( pKeyboard->GetKeyboardPress( DIK_D ) )
			{
				move.x += 1.0f;
			}
		}
		else if( m_nPlayerNo == 1 )
		{
			if( pKeyboard->GetKeyboardPress( DIK_UPARROW ) )
			{
				move.z += 1.0f;
			}
			else if( pKeyboard->GetKeyboardPress( DIK_DOWNARROW ) )
			{
				move.z -= 1.0f;
			}

			if( pKeyboard->GetKeyboardPress( DIK_LEFTARROW ) )
			{
				move.x -= 1.0f;
			}
			else if( pKeyboard->GetKeyboardPress( DIK_RIGHTARROW ) )
			{
				move.x += 1.0f;
			}
		}

#else

		//  左スティック情報の取得
		move.x = ( float )pPS4Input->GetLeftStickDisposition( m_nPlayerNo ).x;
		move.z = -( float )pPS4Input->GetLeftStickDisposition( m_nPlayerNo ).y;

#endif 

		/*----------------------------------------------------------
		    移動操作
		----------------------------------------------------------*/
		if( move.x != 0 ||
			move.z != 0 )
		{
			if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_MOVE ) )
			{
				SetAnimation( StateAnimator::MOTION_MOVE );
			}
		}

		int nKey = 0;

		/*----------------------------------------------------------
		    ジャンプ操作
		----------------------------------------------------------*/
		if( m_nCntJump < PLAYER_ENABLE_NUM_JUMP )
		{
			int nKey = 0;

			if( m_nPlayerNo == 0 )
			{
				nKey = DIK_SPACE;
			}
			else if( m_nPlayerNo == 1 )
			{
				nKey = DIK_RCONTROL;
			}

#ifdef KEYBOARD_ENABLE

			if( pKeyboard->GetKeyboardTrigger( nKey ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_JUMP ) )
				{
					//  ジャンプアニメーションに
					SetAnimation( StateAnimator::MOTION_JUMP );

					//  ジャンプ力の代入
					m_fCurrentJumpPower = m_fJumpPower;

					//  ジャンプのカウント
					m_nCntJump++;
				}
			}

#else

			if( pPS4Input->GetTrigger( m_nPlayerNo , PS4Controller::DIJ_CROSS ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_JUMP ) )
				{
					//  ジャンプアニメーションに
					SetAnimation( StateAnimator::MOTION_JUMP );

					//  ジャンプ力の代入
					m_fCurrentJumpPower = m_fJumpPower;

					//  ジャンプのカウント
					m_nCntJump++;
				}
			}

#endif

		}

		/*----------------------------------------------------------
		    ブレイブ攻撃000操作
		----------------------------------------------------------*/
		if( m_nPlayerNo == 0 )
		{
			nKey = DIK_B;
		}
		else if( m_nPlayerNo == 1 )
		{
			nKey = DIK_SEMICOLON;
		}

#ifdef KEYBOARD_ENABLE

		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_ATTACK_HP001 ) )
			{
				SetAnimation( StateAnimator::MOTION_ATTACK_HP001 );

				//  まだ攻撃エフェクトを出していない状態に
				m_bSlashEffekseer = false;
			}
		}

#else

		if( pPS4Input->GetTrigger( m_nPlayerNo , PS4Controller::DIJ_SQUARE ) )
		{
			if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_ATTACK_HP001 ) )
			{
				SetAnimation( StateAnimator::MOTION_ATTACK_HP001 );

				//  まだ攻撃エフェクトを出していない状態に
				m_bSlashEffekseer = false;
			}
		}

#endif

		/*----------------------------------------------------------
		    魔法攻撃操作
		----------------------------------------------------------*/
		if( m_pMagic == NULL )
		{
			if( m_nPlayerNo == 0 )
			{
				nKey = DIK_H;
			}
			else if( m_nPlayerNo == 1 )
			{
				nKey = DIK_AT;
			}

#ifdef KEYBOARD_ENABLE

			if( pKeyboard->GetKeyboardTrigger( nKey ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_MAGIC_READY ) )
				{
					SetAnimation( StateAnimator::MOTION_MAGIC_READY );

					float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

					//  エフェクトの生成
					m_handle[ EffekseerManager::TYPE_AURA ] = EffekseerManager::Create( EffekseerManager::TYPE_AURA , D3DXVECTOR3( m_position.x , m_position.y + 3.0f , m_position.z ) ,
																							D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.0f );

					//  エフェクトの生成
					m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] = EffekseerManager::Create( EffekseerManager::TYPE_MAGIC_CIRCLE , D3DXVECTOR3( m_position.x , m_position.y , m_position.z ) ,
																									D3DXVECTOR3( -D3DX_PI * 0.5f , fAngle , 0.0f ) , D3DXVECTOR3( 5.0f , 5.0f , 5.0f ) , 1.0f );

					D3DXVECTOR3 magicPos = m_position;
					magicPos.y += 3.0f;


					if( m_magic == MAGIC_FIRE )
					{
						m_pMagic = MagicFire::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
														D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , m_vecDirect , m_nPlayerNo );
					}
					else if( m_magic == MAGIC_LIGHTNING )
					{
						m_pMagic = MagicLightning::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
															D3DXVECTOR3( 0.6f , 0.6f , 0.6f ) , m_vecDirect , m_nPlayerNo );
					}
					else if( m_magic == MAGIC_TORNADE )
					{
						m_pMagic = MagicTornade::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
														  D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , m_vecDirect , m_nPlayerNo );
					}
				}
			}

#else

			if( pPS4Input->GetTrigger( m_nPlayerNo , PS4Controller::DIJ_R2 ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_MAGIC_READY ) )
				{
					SetAnimation( StateAnimator::MOTION_MAGIC_READY );

					float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

					//  エフェクトの生成
					m_handle[ EffekseerManager::TYPE_AURA ] = EffekseerManager::Create( EffekseerManager::TYPE_AURA , D3DXVECTOR3( m_position.x , m_position.y + 3.0f , m_position.z ) ,
																							D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.0f );

					//  エフェクトの生成
					m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] = EffekseerManager::Create( EffekseerManager::TYPE_MAGIC_CIRCLE , D3DXVECTOR3( m_position.x , m_position.y , m_position.z ) ,
																									D3DXVECTOR3( -D3DX_PI * 0.5f , fAngle , 0.0f ) , D3DXVECTOR3( 5.0f , 5.0f , 5.0f ) , 1.0f );

					D3DXVECTOR3 magicPos = m_position;
					magicPos.y += 3.0f;


					if( m_magic == MAGIC_FIRE )
					{
						m_pMagic = MagicFire::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
														D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , m_vecDirect , m_nPlayerNo );
					}
					else if( m_magic == MAGIC_LIGHTNING )
					{
						m_pMagic = MagicLightning::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
															D3DXVECTOR3( 0.6f , 0.6f , 0.6f ) , m_vecDirect , m_nPlayerNo );
					}
					else if( m_magic == MAGIC_TORNADE )
					{
						m_pMagic = MagicTornade::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
														  D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , m_vecDirect , m_nPlayerNo );
					}
				}
			}

#endif

		}

		/*----------------------------------------------------------
			攻撃000操作
		----------------------------------------------------------*/
		if( m_nPlayerNo == 0 )
		{
			nKey = DIK_C;
		}
		else if( m_nPlayerNo == 1 )
		{
			nKey = DIK_K;
		}

#ifdef KEYBOARD_ENABLE

		if( fTargetDistance <= PLAYER_ENABLE_HP_ATTACK_DIST )
		{
			if( pKeyboard->GetKeyboardTrigger( nKey ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_ATTACK_HP000_BEFORE ) )
				{
					SetAnimation( StateAnimator::MOTION_ATTACK_HP000_BEFORE );

					//  追尾時間の初期化
					m_fHomingTime = 0;

					//  速度の代入
					m_fSpeed = PLAYER_HP_ATTACK_SPEED;

					if( move.x >= 0.0f )
					{
						//  右回り追尾状態に
						m_bHomingLeft = false;
					}
					else
					{
						//  左回り追尾状態に
						m_bHomingLeft = true;
					}
				}
			}
		}
		else
		{
			if( pKeyboard->GetKeyboardTrigger( nKey ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_ATTACK_HP000_AFTER ) )
				{
					SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );

					float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

					//  攻撃側の当たり判定の代入
					m_attackHitSphere.position = m_position + m_vecDirect * 3.0f;

					//  速度の代入
					m_fSpeed = PLAYER_HP_ATTACK_SPEED;

					//  エフェクトの生成
					m_handle[ EffekseerManager::TYPE_SLASH000 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
																							  D3DXVECTOR3( m_position.x , m_attackHitSphere.position.y , m_position.z ) ,
																							  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
				}
			}
		}

#else

		if( fTargetDistance <= PLAYER_ENABLE_HP_ATTACK_DIST )
		{
			if( pPS4Input->GetTrigger( m_nPlayerNo , PS4Controller::DIJ_CIRCLE ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_ATTACK_HP000_BEFORE ) )
				{
					SetAnimation( StateAnimator::MOTION_ATTACK_HP000_BEFORE );

					//  追尾時間の初期化
					m_fHomingTime = 0;

					//  速度の代入
					m_fSpeed = PLAYER_HP_ATTACK_SPEED;

					if( move.x >= 0.0f )
					{
						//  右回り追尾状態に
						m_bHomingLeft = false;
					}
					else
					{
						//  左回り追尾状態に
						m_bHomingLeft = true;
					}
				}
			}
		}
		else
		{
			if( pPS4Input->GetTrigger( m_nPlayerNo , PS4Controller::DIJ_CIRCLE ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_ATTACK_HP000_AFTER ) )
				{
					SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );

					float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

					//  攻撃側の当たり判定の代入
					m_attackHitSphere.position = m_position + m_vecDirect * 3.0f;

					//  速度の代入
					m_fSpeed = PLAYER_HP_ATTACK_SPEED;

					//  エフェクトの生成
					m_handle[ EffekseerManager::TYPE_SLASH000 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
																								D3DXVECTOR3( m_position.x , m_attackHitSphere.position.y , m_position.z ) ,
																								D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
				}
			}
		}

#endif



		/*----------------------------------------------------------
		    回避操作
		----------------------------------------------------------*/
		if( m_nPlayerNo == 0 )
		{
			nKey = DIK_G;
		}
		else if( m_nPlayerNo == 1 )
		{
			nKey = DIK_P;
		}

#ifdef KEYBOARD_ENABLE

		if( pKeyboard->GetKeyboardTrigger( nKey ) && ( move.x != 0.0f || move.z != 0.0f ) )
		{
			if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_ESCAPE ) )
			{
				SetAnimation( StateAnimator::MOTION_ESCAPE );

				//  エフェクトの停止
				EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

				//  エフェクトの停止
				EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

				//  魔法を所持している場合
				if( m_pMagic != NULL )
				{
					//  魔法の削除
					m_pMagic->Delete( );
					m_pMagic = NULL;
				}

				D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

				//  速度の代入
				m_fSpeed = m_fBaseSpeed;

				if( pCamera != NULL )
				{
					float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

					D3DXMATRIX mtxRot;
					D3DXMatrixIdentity( &mtxRot );
					D3DXMatrixRotationY( &mtxRot , fAngle );

					D3DXVec3TransformNormal( &m_vecDirect , &move , &mtxRot );
					D3DXVec3Normalize( &m_vecDirect , &m_vecDirect );
				}
				else
				{
					D3DXVec3Normalize( &m_vecDirect , &move );
				}

				//  ジャンプ力の代入
				m_fCurrentJumpPower = m_fJumpPower;

				//  移動力の計算
				m_fMovePower = 1.0f;

				//  回避時移動量の代入
				m_fSpeed = PLAYER_AVOID_SPEED;
			}
		}
#else

		if( pPS4Input->GetPress( m_nPlayerNo , PS4Controller::DIJ_CROSS ) && pPS4Input->GetPress( m_nPlayerNo , PS4Controller::DIJ_R1 ) &&
			( ( float )pPS4Input->GetLeftStickDisposition( m_nPlayerNo ).x != 0.0f || ( float )pPS4Input->GetLeftStickDisposition( m_nPlayerNo ).y != 0.0f ) ||
			pKeyboard->GetKeyboardTrigger( nKey ) && ( move.x != 0.0f || move.y != 0.0f ) )
		{
			if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_ESCAPE ) )
			{
				SetAnimation( StateAnimator::MOTION_ESCAPE );

				//  エフェクトの停止
				EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

				//  エフェクトの停止
				EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

				//  魔法を所持している場合
				if( m_pMagic != NULL )
				{
					//  魔法の削除
					m_pMagic->Delete( );
					m_pMagic = NULL;
				}

				D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

				//  速度の代入
				m_fSpeed = m_fBaseSpeed;

				if( pCamera != NULL )
				{
					float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

					D3DXMATRIX mtxRot;
					D3DXMatrixIdentity( &mtxRot );
					D3DXMatrixRotationY( &mtxRot , fAngle );

					D3DXVec3TransformNormal( &m_vecDirect , &move , &mtxRot );
					D3DXVec3Normalize( &m_vecDirect , &m_vecDirect );
				}
				else
				{
					D3DXVec3Normalize( &m_vecDirect , &move );
				}

				//  ジャンプ力の代入
				m_fCurrentJumpPower = m_fJumpPower;

				float fStickX = fabsf( ( float )pPS4Input->GetLeftStickDisposition( m_nPlayerNo ).x );
				float fStickY = fabsf( ( float )pPS4Input->GetLeftStickDisposition( m_nPlayerNo ).y );

				if( fStickX >= fStickY )
				{
					//  移動力の計算
					m_fMovePower = fStickX / 1000.0f;
				}
				else
				{
					//  移動力の計算
					m_fMovePower = fStickY / 1000.0f;
				}

				//  回避時移動量の代入
				m_fSpeed = PLAYER_AVOID_SPEED;
			}
		}

#endif

		/*----------------------------------------------------------
		    ロックオンダッシュ操作
		----------------------------------------------------------*/
		//  ロックオン状態の場合
		if( m_bLockOn == true )
		{
			if( m_nPlayerNo == 0 )
			{
				nKey = DIK_V;
			}
			else if( m_nPlayerNo == 1 )
			{
				nKey = DIK_L;
			}

#ifdef KEYBOARD_ENABLE

			//  YとRBを長押ししている間
			if( pKeyboard->GetKeyboardTrigger( nKey ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_LOCKON_DASH ) )
				{
					SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

					//  ロックオンダッシュの速度の代入
					m_fSpeed = PLAYER_LOCKON_DASH_SPEED;
				}
			}

#else

			//  YとRBを長押ししている間
			if( ( pPS4Input->GetTrigger( m_nPlayerNo , PS4Controller::DIJ_TRIANGLE ) ) )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_LOCKON_DASH ) )
				{
					SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

					//  ロックオンダッシュの速度の代入
					m_fSpeed = PLAYER_LOCKON_DASH_SPEED;
				}
			}

#endif
		}

		/*----------------------------------------------------------
		    防御操作
		----------------------------------------------------------*/
		if( m_nPlayerNo == 0 )
		{
			nKey = DIK_F;
		}
		else if( m_nPlayerNo == 1 )
		{
			nKey = DIK_O;
		}

#ifdef KEYBOARD_ENABLE

		//  R1を押した場合間
		if( pKeyboard->GetKeyboardPress( nKey ) )
		{
			//  一定フレーム以上になった場合
			if( m_nPressTime >= PLAYER_GUARD_FRAME )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_GUARD ) )
				{
					SetAnimation( StateAnimator::MOTION_GUARD );

					float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

					//  攻撃側の当たり判定の代入
					D3DXVECTOR3 effekseerPos = m_position + m_vecDirect * 3.0f;
					effekseerPos.y += 3.0f;

					//  速度の代入
					m_fSpeed = PLAYER_HP_ATTACK_SPEED;

					//  エフェクトの生成
					m_handle[ EffekseerManager::TYPE_SHIELD ] = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
																							D3DXVECTOR3( effekseerPos.x , effekseerPos.y , effekseerPos.z ) ,
																							D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 3.0f , 3.0f , 3.0f ) , 1.0f );
				}
			}

			//  押す時間のカウント
			m_nPressTime++;
		}
		else
		{
			//  押す時間の初期化
			m_nPressTime = 0;
		}

#else

		//  R1を押した場合間
		if( ( pPS4Input->GetPress( m_nPlayerNo , PS4Controller::DIJ_R1 ) ) )
		{
			//  一定フレーム以上になった場合
			if( m_nPressTime >= PLAYER_GUARD_FRAME )
			{
				if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_GUARD ) )
				{
					SetAnimation( StateAnimator::MOTION_GUARD );

					float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

					//  攻撃側の当たり判定の代入
					D3DXVECTOR3 effekseerPos = m_position + m_vecDirect * 3.0f;
					effekseerPos.y += 3.0f;

					//  速度の代入
					m_fSpeed = PLAYER_HP_ATTACK_SPEED;

					//  エフェクトの生成
					m_handle[ EffekseerManager::TYPE_SHIELD ] = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
																							D3DXVECTOR3( effekseerPos.x , effekseerPos.y , effekseerPos.z ) ,
																							D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 3.0f , 3.0f , 3.0f ) , 1.0f );
				}
			}

			//  押す時間のカウント
			m_nPressTime++;
		}
		else
		{
			//  押す時間の初期化
			m_nPressTime = 0;
		}

#endif

		////////////////////////////////////////////////////////
		//  共通操作
		////////////////////////////////////////////////////////

		//  待ち時間がある場合
		if( m_nWaitTime > 0 )
		{
			//  待ち時間のカウント
			m_nWaitTime--;
		}
		else
		{
			m_nWaitTime = 0;
		}

		//  モーション情報の取得
		StateAnimator::MOTION motion = m_pStateAnimator->GetMotion( );

#pragma omp parallel for
		//  優先度の最大数分のループ
		for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
		{
			//  シーンの先頭アドレスを取得
			pScene = Scene::GetScene( nCntPriority );

			//  シーンが空ではない間ループ
			while( pScene != NULL )
			{
				Scene::OBJTYPE objType;						//  物体の種類

				//  物体の種類の取得
				objType = pScene->GetObjType( );

				//  種類がゴールの場合
				if( objType == Scene::OBJTYPE_HIT_FIELD )
				{
					//  当たり判定用フィールドクラスにダウンキャスト
					pHitField = ( HitField* )pScene;

					if( motion != StateAnimator::MOTION_LOCKON_DASH && motion != StateAnimator::MOTION_GUARD &&
						motion != StateAnimator::MOTION_BEND && motion != StateAnimator::MOTION_JUMP &&
						motion != StateAnimator::MOTION_ESCAPE && motion != StateAnimator::MOTION_ATTACK_HP000_BEFORE )
					{
						//  重力処理( フィールドより上にいるかどうかで変化 )
						if( m_position.y >= pHitField->GetHeight( m_position ) )
						{
							m_fMoveY -= PLAYER_GRAVITY2;

							m_position.y += m_fMoveY;
						}
					}
					else
					{
						m_fMoveY = 0.0f;
					}

					float fHeight = pHitField->GetHeight( m_position );

					if( fHeight == -100000.0f )
					{	

					}
					else
					{
						if( m_position.y <= fHeight )
						{
							//  Y座標フィールドに合わせる
							m_position.y = pHitField->GetHeight( m_position );
						}
					}

					//  ステンシルシャドウが存在している場合している場合
					if( m_stencilShadow != nullptr )
					{
						float fScale = PLAYER_SHADOW_SCALE + ( m_position.y - pHitField->GetHeight( m_position )  ) * 0.01f;

						D3DXVECTOR3 position = m_position;
						position.y = pHitField->GetHeight( m_position );

						m_stencilShadow->SetScale( D3DXVECTOR3( fScale , fScale , fScale ) );
						m_stencilShadow->SetPosition( position );
					}
				}

				//  CPU対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
				{
					//  種類が敵の場合
					if( objType == Scene::OBJTYPE_ENEMY )
					{
						//  ダウンキャスト
						Enemy* pEnemy = ( Enemy* )pScene;

						//  敵の座標の代入
						D3DXVECTOR3 posEnemy = pEnemy->GetPos( );

						//  距離の算出
						fTargetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
												 ( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
												 ( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );
					}
				}
				//  プレイヤー対戦モードの場合
				else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  ダウンキャスト
						Player* pPlayer = ( Player* )pScene;

						//  自分以外のプレイヤー番号の場合
						if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
						{
							//  敵の座標の代入
							D3DXVECTOR3 posEnemy = pPlayer->GetPos( );

							//  距離の算出
							fTargetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
													 ( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
													 ( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}

		////////////////////////////////////////////////////////
		//  共通操作終了
		////////////////////////////////////////////////////////

		//  アニメーションの種類によっての場合分け
		switch( motion )
		{
			case StateAnimator::MOTION_NEUTRAL:
			{
				break;
			}
			case StateAnimator::MOTION_MOVE:
			{
				cameraVecDirect = pCamera->GetCameraVecDirect( );

				if( move.x != 0 ||
					move.z != 0 )
				{
					//  速度の代入
					m_fSpeed = m_fBaseSpeed;

					if( pCamera != NULL )
					{
						float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

						D3DXMATRIX mtxRot;
						D3DXMatrixIdentity( &mtxRot );
						D3DXMatrixRotationY( &mtxRot , fAngle );

						D3DXVec3TransformNormal( &m_vecDirect , &move , &mtxRot );
						D3DXVec3Normalize( &m_vecDirect , &m_vecDirect );
					}
					else
					{
						D3DXVec3Normalize( &m_vecDirect , &move );
					}

					//  座標の更新( 進行方向 × 速度 分の移動 )
					m_position += m_vecDirect * m_fSpeed;
				}
				else
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}

				//  注視点を向いている方向に
				m_posAt = m_position + m_vecDirect * 5.0f;

				break;
			}
			case StateAnimator::MOTION_JUMP:
			{
				cameraVecDirect = pCamera->GetCameraVecDirect( );

				if( move.x != 0 ||
					move.z != 0 )
				{
					//  速度の代入
					m_fSpeed = m_fBaseSpeed;

					if( pCamera != NULL )
					{
						float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

						D3DXMATRIX mtxRot;
						D3DXMatrixIdentity( &mtxRot );
						D3DXMatrixRotationY( &mtxRot , fAngle );

						D3DXVec3TransformNormal( &m_vecDirect , &move , &mtxRot );
						D3DXVec3Normalize( &m_vecDirect , &m_vecDirect );
					}
					else
					{
						D3DXVec3Normalize( &m_vecDirect , &move );
					}

					//  座標の更新( 進行方向 × 速度 分の移動 )
					m_position += m_vecDirect * m_fSpeed * PLAYER_JUMP_;
				}

				//  注視点を向いている方向に
				m_posAt = m_position + m_vecDirect * 5.0f;

				//  ジャンプ力分上に上がるのとジャンプ力を重力分下げる
				m_position.y += m_fCurrentJumpPower;
				m_fCurrentJumpPower -= PLAYER_GRAVITY;

				//  地面についた場合
				if( m_position.y <= pHitField->GetHeight( m_position ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_LANDING_SHORT ) )
					{
						//  着地アニメーションに
						SetAnimation( StateAnimator::MOTION_LANDING_SHORT );
					}
				}

				break;
			}
			case StateAnimator::MOTION_BACKJUMP:
			{
				break;
			}
			case StateAnimator::MOTION_LANDING_SHORT:
			{
				//  アニメーションが終了している場合
				if( GetAnimationFinish( ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}

				break;
			}
			case StateAnimator::MOTION_LANDING_LONG:
			{
				//  アニメーションが終了している場合
				if( GetAnimationFinish( ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}

				break;
			}
			case StateAnimator::MOTION_ATTACK_BRAVE000:
			{
				//  攻撃側の当たり判定の代入
				m_attackHitSphere.position = m_position + m_vecDirect * 3.0f;
				m_attackHitSphere.position.y += 5.0f;

				//  優先度の最大数分のループ
				for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
				{
					//  シーンの先頭アドレスを取得
					pScene = Scene::GetScene( nCntPriority );

					//  シーンが空ではない間ループ
					while( pScene != NULL )
					{
						Scene::OBJTYPE objType;						//  物体の種類

						//  物体の種類の取得
						objType = pScene->GetObjType( );

						//  CPU対戦モードの場合
						if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
						{
							//  種類が敵の場合
							if( objType == Scene::OBJTYPE_ENEMY )
							{
								//  ダウンキャスト
								Enemy* pEnemy = ( Enemy* )pScene;

								if( Utility::HitSphere( m_attackHitSphere , pEnemy->GetHitSphere( ) ) )
								{
									pEnemy->Damage( PLAYER_BRAVE_DAMAGE );
								}
							}
						}
						//  プレイヤー対戦モードの場合
						else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
						{
							if( objType == Scene::OBJTYPE_PLAYER )
							{
								//  ダウンキャスト
								Player* pPlayer = ( Player* )pScene;

								//  自分以外のプレイヤー番号の場合
								if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
								{
									if( Utility::HitSphere( m_attackHitSphere , pPlayer->GetHitSphere( ) ) )
									{
										//  相手が防御状態である場合
										if( pPlayer->GetGuard( ) )
										{
											//  のけぞり状態に
											Bend( );
										}
										else
										{
											pPlayer->Damage( m_vecDirect , PLAYER_BRAVE_BLOW_POWER , PLAYER_BRAVE_DAMAGE );
										}
									}
								}
							}
						}

						//  種類が弾の場合
						if( objType == Scene::OBJTYPE_BULLET )
						{
							//  ダウンキャスト
							Bullet* pBullet = ( Bullet* )pScene;

							//  所有者が敵の場合
							if( pBullet->GetOwner( ) ==  Bullet::OWNER_ENEMY )
							{
								if( Utility::HitSphere( m_attackHitSphere , pBullet->GetHitSphere( ) ) )
								{
									//  ロックオン状態の場合
									if( m_bLockOn == true )
									{
										Scene* pScene2 = NULL;									//  シーンクラスのポインタ

										//  優先度の最大数分のループ
										for( int nCntPriority2 = 0; nCntPriority2 < MAX_NUM_PRIORITY; nCntPriority2++ )
										{
											//  シーンの先頭アドレスを取得
											pScene2 = Scene::GetScene( nCntPriority2 );

											//  シーンが空ではない間ループ
											while( pScene2 != NULL )
											{
												Scene::OBJTYPE objType2;						//  物体の種類

												//  物体の種類の取得
												objType2 = pScene2->GetObjType( );

												//  種類が敵の場合
												if( objType2 == Scene::OBJTYPE_ENEMY )
												{
													D3DXVECTOR3 workVecDirect;

													//  CPU対戦モードの場合
													if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
													{
														//  ダウンキャスト
														Enemy* pEnemy = ( Enemy* )pScene2;
														D3DXVECTOR3 enemyPos = pEnemy->GetPos( );

														D3DXVECTOR3	playerPos = m_position;

														D3DXVec3Normalize( &workVecDirect , &( enemyPos - m_position ) );

														pBullet->Hit( workVecDirect );
													}
													//  プレイヤー対戦モードの場合
													if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
													{
														//  ダウンキャスト
														Player* pPlayer = ( Player* )pScene2;

														if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
														{
															D3DXVECTOR3 enemyPos = pPlayer->GetPos( );

															D3DXVECTOR3	playerPos = m_position;

															D3DXVec3Normalize( &workVecDirect , &( enemyPos - m_position ) );

															pBullet->Hit( workVecDirect );
														}
													}
												}

												//  次のポインタを代入
												pScene2 = pScene2->GetNextScene( pScene2 );
											}
										}
									}
									else
									{
										pBullet->Hit( m_vecDirect );
									}

									//  弾の種類がワイドの場合
									if( pBullet->GetType( ) == Bullet::TYPE_WIDE )
									{
										//  体力の回復
										m_nLife += PLAYER_HEAL_LIFE;
									}
								}
							}
						}
						//  種類が炎魔法の場合
						if( objType == Scene::OBJTYPE_MAGIC_FIRE )
						{
							//  ダウンキャスト
							MagicFire* pMagicFire = ( MagicFire* )pScene;

							//  所有者が敵の場合
							if( pMagicFire->GetOwner( ) ==  Bullet::OWNER_ENEMY )
							{
								if( Utility::HitSphere( m_attackHitSphere , pMagicFire->GetHitSphere( ) ) )
								{
									//  体力の回復
									m_nLife += PLAYER_HEAL_LIFE;
								}
							}
						}

						//  次のポインタを代入
						pScene = pScene->GetNextScene( pScene );
					}
				}

				//  アニメーションが終了している場合
				if( GetAnimationFinish( ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}

				break;
			}
			case StateAnimator::MOTION_ATTACK_HP000_BEFORE:
			{
				//  相手に当たらない様に
				m_bJudgeHit = false;

				//  前回の座標を代入
				m_beforePos = m_position;

				//  結果座標
				D3DXVECTOR2 resultPos;
				resultPos.x = m_position.x;
				resultPos.y = m_position.z;

				D3DXVECTOR3 targetPos;

				float fDistance = 0.0f;

				//  優先度の最大数分のループ
				for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
				{
					//  シーンの先頭アドレスを取得
					pScene = Scene::GetScene( nCntPriority );

					//  シーンが空ではない間ループ
					while( pScene != NULL )
					{
						Scene::OBJTYPE objType;						//  物体の種類

						//  物体の種類の取得
						objType = pScene->GetObjType( );

						//  CPU対戦モードの場合
						if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
						{
							//  種類が敵の場合
							if( objType == Scene::OBJTYPE_ENEMY )
							{
								//  ダウンキャスト
								Enemy* pEnemy = ( Enemy* )pScene;

								//  目標の3D座標の取得
								targetPos = pEnemy->GetPos( );

								//  敵の座標の代入
								D3DXVECTOR2 posEnemy;
								posEnemy.x = pEnemy->GetPos( ).x;
								posEnemy.y = pEnemy->GetPos( ).z;

								//  距離の算出
								fDistance = sqrtf( ( posEnemy.x - resultPos.x ) * ( posEnemy.x - resultPos.x ) +
												   ( posEnemy.y - resultPos.y ) * ( posEnemy.y - resultPos.y ) );

								D3DXVECTOR2 vecDirect;
								D3DXVec2Normalize( &vecDirect , &( posEnemy - resultPos ) );

								D3DXVECTOR2 controllPoint;
								controllPoint = resultPos + vecDirect * fDistance * 0.2f;

								if( m_bHomingLeft == false )
								{					
									controllPoint.x += vecDirect.y * fDistance * 0.12f;
									controllPoint.y -= vecDirect.x * fDistance * 0.12f;
								}
								else
								{
									controllPoint.x -= vecDirect.y * fDistance * 0.12f;
									controllPoint.y += vecDirect.x * fDistance * 0.12f;
								}

								//  ベジェ曲線から結果座標の算出
								resultPos = Utility::BezierCurve2D( resultPos , posEnemy , controllPoint , m_fHomingTime );
							}
						}
						//  プレイヤー対戦モードの場合
						else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
						{
							if( objType == Scene::OBJTYPE_PLAYER )
							{
								//  ダウンキャスト
								Player* pPlayer = ( Player* )pScene;

								//  自分以外のプレイヤー番号の場合
								if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
								{
									//  目標の3D座標の取得
									targetPos = pPlayer->GetPos( );

									//  敵の座標の代入
									D3DXVECTOR2 posEnemy;
									posEnemy.x = pPlayer->GetPos( ).x;
									posEnemy.y = pPlayer->GetPos( ).z;

									//  距離の算出
									fDistance = sqrtf( ( posEnemy.x - resultPos.x ) * ( posEnemy.x - resultPos.x ) +
													   ( posEnemy.y - resultPos.y ) * ( posEnemy.y - resultPos.y ) );

									D3DXVECTOR2 vecDirect;
									D3DXVec2Normalize( &vecDirect , &( posEnemy - resultPos ) );

									D3DXVECTOR2 controllPoint;
									controllPoint = resultPos + vecDirect * fDistance * 0.2f;

									if( m_bHomingLeft == false )
									{					
										controllPoint.x += vecDirect.y * fDistance * 0.12f;
										controllPoint.y -= vecDirect.x * fDistance * 0.12f;
									}
									else
									{
										controllPoint.x -= vecDirect.y * fDistance * 0.12f;
										controllPoint.y += vecDirect.x * fDistance * 0.12f;
									}

									//  ベジェ曲線から結果座標の算出
									resultPos = Utility::BezierCurve2D( resultPos , posEnemy , controllPoint , m_fHomingTime );
								}
							}
						}

						//  次のポインタを代入
						pScene = pScene->GetNextScene( pScene );
					}
				}

				m_position.x = resultPos.x;
				m_position.y += ( targetPos.y - m_position.y ) * 0.1f;
				m_position.z = resultPos.y;

				D3DXVec3Normalize( &m_vecDirect , &( m_position - m_beforePos ) );

				m_posAt = m_position + m_vecDirect * 5.0f;

				//  総フレーム数から算出して足す
				m_fHomingTime += 1.0f / ( PLAYER_HOMIMG_TIME + fDistance * 0.8f );

				if( m_fHomingTime >= 0.8f || fDistance < 10.0f )
				{
					m_vecDirect = targetPos - m_position;
					m_vecDirect.y = 0.0f;

					//  目標に向かったベクトルの代入
					D3DXVec3Normalize( &m_vecDirect , &m_vecDirect );

					m_fHomingTime = 0.8f;

					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_ATTACK_HP000_AFTER ) )
					{
						SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );

						float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

						//  攻撃側の当たり判定の代入
						m_attackHitSphere.position = m_position + m_vecDirect * 3.0f;
						m_attackHitSphere.position.y += 2.0f;

						//  エフェクトの生成
						m_handle[ EffekseerManager::TYPE_SLASH000 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 , D3DXVECTOR3( m_position.x , m_attackHitSphere.position.y , m_position.z ) ,
																								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
					}
				}

				break;
			}
			case StateAnimator::MOTION_ATTACK_HP000_AFTER:
			{
				//  当たり判定をしない状態に
				m_bJudgeHit = false;

				m_position += m_vecDirect * m_fSpeed;

				m_posAt = m_position + m_vecDirect * 5.0f;

				//  攻撃側の当たり判定の代入
				m_attackHitSphere.position = m_position + m_vecDirect * 3.0f;
				m_attackHitSphere.position.y += 2.0f;

				//  優先度の最大数分のループ
				for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
				{
					//  シーンの先頭アドレスを取得
					pScene = Scene::GetScene( nCntPriority );

					//  シーンが空ではない間ループ
					while( pScene != NULL )
					{
						Scene::OBJTYPE objType;						//  物体の種類

						//  物体の種類の取得
						objType = pScene->GetObjType( );

						//  CPU対戦モードの場合
						if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
						{
							//  種類が敵の場合
							if( objType == Scene::OBJTYPE_ENEMY )
							{
								//  ダウンキャスト
								Enemy* pEnemy = ( Enemy* )pScene;

								if( Utility::HitSphere( m_attackHitSphere , pEnemy->GetHitSphere( ) ) )
								{
									pEnemy->Damage( PLAYER_HP_DAMAGE );
								}
							}
						}
						//  プレイヤー対戦モードの場合
						else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
						{
							if( objType == Scene::OBJTYPE_PLAYER )
							{
								//  ダウンキャスト
								Player* pPlayer = ( Player* )pScene;

								//  自分以外のプレイヤー番号の場合
								if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
								{
									if( Utility::HitSphere( m_attackHitSphere , pPlayer->GetHitSphere( ) ) )
									{
										//  相手が防御状態である場合
										if( pPlayer->GetGuard( ) )
										{
											//  のけぞり状態に
											Bend( );
										}
										else
										{
											pPlayer->Damage( m_vecDirect , PLAYER_HP_BLOW_POWER , PLAYER_HP_DAMAGE );
										}
									}
								}
							}
						}

						//  次のポインタを代入
						pScene = pScene->GetNextScene( pScene );
					}
				}

				float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

				EffekseerManager::SetPosition( m_handle[ EffekseerManager::TYPE_SLASH000 ] ,
										   D3DXVECTOR3( m_position.x , m_attackHitSphere.position.y , m_position.z ) );
				EffekseerManager::SetRot( m_handle[ EffekseerManager::TYPE_SLASH000 ] , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) );

				if( m_bAnimationFinish == true )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}

				break;
			}
			case StateAnimator::MOTION_ATTACK_HP001:
			{

				if( m_nKey == 2 && m_bSlashEffekseer == false )
				{
					//  攻撃側の当たり判定の代入
					m_attackHitSphere.position = m_position + m_vecDirect * 3.0f;

					float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

					//  エフェクトの生成
					EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( m_attackHitSphere.position.x , m_position.y , m_attackHitSphere.position.z ) ,
											   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

					//  エフェクトの生成
					EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( m_attackHitSphere.position.x , m_position.y , m_attackHitSphere.position.z ) ,
											   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

					//  エフェクトの生成
					EffekseerManager::Create( EffekseerManager::TYPE_FLAME , D3DXVECTOR3( m_attackHitSphere.position.x , m_position.y , m_attackHitSphere.position.z ) ,
											   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.0f , 2.0f , 2.0f ) , 1.0f );

					m_bSlashEffekseer = true;
				}

				//  攻撃側の当たり判定の代入
				m_attackHitSphere.position = m_position + m_vecDirect * 3.0f;

				if( m_nKey >= 3 && m_nKey <= 5 )
				{
					//  優先度の最大数分のループ
					for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
					{
						//  シーンの先頭アドレスを取得
						pScene = Scene::GetScene( nCntPriority );

						//  シーンが空ではない間ループ
						while( pScene != NULL )
						{
							Scene::OBJTYPE objType;						//  物体の種類

							//  物体の種類の取得
							objType = pScene->GetObjType( );

							//  CPU対戦モードの場合
							if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
							{
								//  種類が敵の場合
								if( objType == Scene::OBJTYPE_ENEMY )
								{
									//  ダウンキャスト
									Enemy* pEnemy = ( Enemy* )pScene;

									//  プレイヤーの座標の代入
									D3DXVECTOR2 enemyPos;
									enemyPos.x  = pEnemy->GetPos( ).x;
									enemyPos.y  = pEnemy->GetPos( ).z;

									Utility::HIT_CIRCLE hitCircle;
									hitCircle.position.x = m_position.x;
									hitCircle.position.y = m_position.z;
									hitCircle.fLength = PLAYER_HP_RANGE;

									if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
									{
										if( m_position.y + PLAYER_HP_RANGE_HEIGHT >= pEnemy->GetPos( ).y )
										{
											pEnemy->Damage( PLAYER_HP_DAMAGE );
										}
									}
								}
							}
							//  プレイヤー対戦モードの場合
							else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
							{
								if( objType == Scene::OBJTYPE_PLAYER )
								{
									//  ダウンキャスト
									Player* pPlayer = ( Player* )pScene;

									//  自分以外のプレイヤー番号の場合
									if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
									{
										//  プレイヤーの座標の代入
										D3DXVECTOR2 enemyPos;
										enemyPos.x  = pPlayer->GetPos( ).x;
										enemyPos.y  = pPlayer->GetPos( ).z;

										Utility::HIT_CIRCLE hitCircle;
										hitCircle.position.x = m_position.x;
										hitCircle.position.y = m_position.z;
										hitCircle.fLength = PLAYER_HP_RANGE;

										if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
										{
											if( m_position.y + PLAYER_HP_RANGE_HEIGHT >= pPlayer->GetPos( ).y )
											{
												//  相手が防御状態である場合
												if( pPlayer->GetGuard( ) )
												{
													//  のけぞり状態に
													Bend( );
												}
												else
												{
													pPlayer->Damage( m_vecDirect , PLAYER_BRAVE_BLOW_POWER , PLAYER_BRAVE_DAMAGE , false );
												}
											}
										}
									}
								}
							}

							//  次のポインタを代入
							pScene = pScene->GetNextScene( pScene );
						}
					}
				}

				if( m_bAnimationFinish == true )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}

				break;
			}
			case StateAnimator::MOTION_JUMP_ATTACK000:
			{

				break;
			}
			case StateAnimator::MOTION_LOCKON_DASH:
			{
				D3DXVECTOR3 posEnemy( 0.0f , 0.0f , 0.0f );

#pragma omp parallel for
				//  優先度の最大数分のループ
				for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
				{
					//  シーンの先頭アドレスを取得
					pScene2 = Scene::GetScene( nCntPriority );

					//  シーンが空ではない間ループ
					while( pScene2 != NULL )
					{
						Scene::OBJTYPE objType;						//  物体の種類

						//  物体の種類の取得
						objType = pScene2->GetObjType( );

						//  CPU対戦モードの場合
						if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
						{
							//  種類が敵の場合
							if( objType == Scene::OBJTYPE_ENEMY )
							{
								//  ダウンキャスト
								Enemy* pEnemy = ( Enemy* )pScene2;
							
								//  敵の座標代入
								posEnemy = pEnemy->GetPos( );
							}
						}
						//  プレイヤー対戦モードの場合
						if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
						{
							//  種類が敵の場合
							if( objType == Scene::OBJTYPE_PLAYER )
							{
								//  ダウンキャスト
								Player* pPlayer = ( Player* )pScene2;
							
								//  自分以外のプレイヤー番号の場合
								if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
								{
									//  敵の座標代入
									posEnemy = pPlayer->GetPos( );
								}
							}
						}

						//  次のポインタを代入
						pScene2 = pScene2->GetNextScene( pScene2 );
					}
				}

				//  敵からプレイヤーまでの方向ベクトルを求める
				m_vecDirect = posEnemy - m_position;
				D3DXVec3Normalize( &m_vecDirect , &m_vecDirect );

				//  座標の更新( 進行方向 × 速度 分の移動 )
				m_position += m_vecDirect * m_fSpeed;

				//  注視点をプレイヤーが向いている方向に変更
				m_posAt = m_position + m_vecDirect * 5.0f;

				//  速度を徐々に減衰させていく
				m_fSpeed += ( 0.0f - m_fSpeed ) * 0.03f;

				//  速度がある一定を下回った場合
				if( m_fSpeed <= 0.9f )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );

						//  速度をなくす
						m_fSpeed = 0.0f;	
					}
				}

				//  注視点座標の代入
				m_posAt = m_position + m_vecDirect * 5.0f;

				//  パーティクルの生成
				Particle::Create( Particle::TYPE_NORMAL , m_position , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
								   D3DXCOLOR( 0.2f , 0.5f , 0.8f , 1.0f ) , 4.0f , 0.1f , 0.01f , 0.0f , 0.0f , 20 );

				/*----------------------------------------------------------
					ダッシュ攻撃000操作
				----------------------------------------------------------*/
				if( m_nPlayerNo == 0 )
				{
					nKey = DIK_B;
				}
				else if( m_nPlayerNo == 1 )
				{
					nKey = DIK_SEMICOLON;
				}

#ifdef KEYBOARD_ENABLE

				if( pKeyboard->GetKeyboardTrigger( nKey ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_FINISHER ) )
					{
						SetAnimation( StateAnimator::MOTION_FINISHER );

						//  速度の代入
						m_fSpeed = PLAYER_FINISH_SPEED;

						float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

						//  エフェクトの生成
						m_handle[ EffekseerManager::TYPE_LANCE ] = EffekseerManager::Create( EffekseerManager::TYPE_LANCE , D3DXVECTOR3( m_position.x , m_position.y + PLAYER_HIT_SPHERE_POS_Y , m_position.z ) ,
																							   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.5f , 1.5f , 1.5f ) );
					}
				}

#else

				if( pKeyboard->GetKeyboardTrigger( nKey ) || pPS4Input->GetTrigger( m_nPlayerNo , PS4Controller::DIJ_SQUARE ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_FINISHER ) )
					{
						SetAnimation( StateAnimator::MOTION_FINISHER );

						//  速度の代入
						m_fSpeed = PLAYER_FINISH_SPEED;

						float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

						//  エフェクトの生成
						m_handle[ EffekseerManager::TYPE_LANCE ] = EffekseerManager::Create( EffekseerManager::TYPE_LANCE , D3DXVECTOR3( m_position.x , m_position.y + PLAYER_HIT_SPHERE_POS_Y , m_position.z ) ,
																							   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.5f , 1.5f , 1.5f ) );
					}
				}

#endif

				break;
			}
			case StateAnimator::MOTION_ESCAPE:
			{
				//  座標の更新( 進行方向 × 速度 分の移動 )
				m_position += m_vecDirect * m_fSpeed * m_fMovePower;

				//  移動量を下げていく
				m_fSpeed += ( 0.0f - m_fSpeed ) * 0.01f;

				//  ジャンプ力分上に上がるのとジャンプ力を重力分下げる
				m_position.y += m_fCurrentJumpPower;
				m_fCurrentJumpPower -= PLAYER_GRAVITY;

				//  注視点を向いている方向に
				m_posAt = m_position + m_vecDirect * 5.0f;

				//  無敵時間の代入
				m_nInvisibleTime = 2;

				//  地面についた場合
				if( m_position.y <= pHitField->GetHeight( m_position ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_LANDING_LONG ) )
					{
						//  着地アニメーションに
						SetAnimation( StateAnimator::MOTION_LANDING_LONG );

						//  ジャンプ数の初期化
						m_nCntJump = 0;
					}
				}

				break;
			}
			case StateAnimator::MOTION_VANISH:
			{

				break;
			}
			case StateAnimator::MOTION_WIN:
			{

				break;
			}
			case StateAnimator::MOTION_LOSE:
			{

				break;
			}
			case StateAnimator::MOTION_FINISHER:
			{
				//  当たり判定をしない状態に
				//m_bJudgeHit = false;

				m_position += m_vecDirect * m_fSpeed;

				//  注視点を向いている方向に
				m_posAt = m_position + m_vecDirect * 5.0f;

				D3DXVECTOR3 effekseerPos( m_position.x , m_position.y + PLAYER_HIT_SPHERE_POS_Y , m_position.z );
				effekseerPos += m_vecDirect * 8.0f;

				EffekseerManager::SetPosition( m_handle[ EffekseerManager::TYPE_LANCE ] , effekseerPos );

				if( m_nKey >= 11 )
				{
					//  速度の減衰
					m_fSpeed += ( 0.0f - m_fSpeed ) * 0.06f;

					//  エフェクトの停止
					EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_LANCE ] );
				}

				if( m_bAnimationFinish == true )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );

						//  速度をなくす
						m_fSpeed = 0.0f;
					}
				}

				//  攻撃側の当たり判定の代入
				m_attackHitSphere.position = m_position + m_vecDirect * 3.0f;
				m_attackHitSphere.position.y += 2.0f;

				if( m_nKey >= 1 && m_nKey <= 10 )
				{
					//  優先度の最大数分のループ
					for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
					{
						//  シーンの先頭アドレスを取得
						pScene = Scene::GetScene( nCntPriority );

						//  シーンが空ではない間ループ
						while( pScene != NULL )
						{
							Scene::OBJTYPE objType;						//  物体の種類

							//  物体の種類の取得
							objType = pScene->GetObjType( );

							//  CPU対戦モードの場合
							if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
							{
								//  種類が敵の場合
								if( objType == Scene::OBJTYPE_ENEMY )
								{
									//  ダウンキャスト
									Enemy* pEnemy = ( Enemy* )pScene;

									if( Utility::HitSphere( m_attackHitSphere , pEnemy->GetHitSphere( ) ) )
									{
										pEnemy->Damage( PLAYER_FINISHER_DAMAGE );
									}
								}
							}
							//  プレイヤー対戦モードの場合
							else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
							{
								if( objType == Scene::OBJTYPE_PLAYER )
								{
									//  ダウンキャスト
									Player* pPlayer = ( Player* )pScene;

									//  自分以外のプレイヤー番号の場合
									if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
									{
										if( Utility::HitSphere( m_attackHitSphere , pPlayer->GetHitSphere( ) ) )
										{
											D3DXVECTOR3 blowVecDirect = m_vecDirect;

											D3DXMATRIX mtxRot;
											D3DXMatrixIdentity( &mtxRot );
											D3DXMatrixRotationY( &mtxRot , D3DX_PI * 0.1f );

											D3DXVec3TransformNormal( &blowVecDirect , &blowVecDirect , &mtxRot );
											D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

											pPlayer->Damage( blowVecDirect , PLAYER_FINISHER_BLOW_POWER , PLAYER_FINISHER_DAMAGE );
										}
									}
								}
							}

							//  次のポインタを代入
							pScene = pScene->GetNextScene( pScene );
						}
					}
				}

				break;
			}
			case StateAnimator::MOTION_MAGIC_READY:
			{
				if( m_pMagic != NULL )
				{
					//  雷魔法を持っている場合
					if( m_magic == MAGIC_LIGHTNING )
					{
						D3DXVECTOR3 vecDirect( move.x , 0.0f , move.z );

						cameraVecDirect = pCamera->GetCameraVecDirect( );

						if( pCamera != NULL )
						{
							float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

							D3DXMATRIX mtxRot;
							D3DXMatrixIdentity( &mtxRot );
							D3DXMatrixRotationY( &mtxRot , fAngle );

							D3DXVec3TransformNormal( &vecDirect , &move , &mtxRot );
							D3DXVec3Normalize( &vecDirect , &vecDirect );
						}
						else
						{
							D3DXVec3Normalize( &vecDirect , &move );
						}

						//  魔法陣を動かす
						MagicLightning* pMagicLightning = ( MagicLightning* )m_pMagic;
						pMagicLightning->MoveMagicPos( vecDirect );
					}	

					//  竜巻魔法を持っている倍
					if( m_magic == MAGIC_TORNADE )
					{
						D3DXVECTOR3 vecDirect( move.x , 0.0f , move.z );

						cameraVecDirect = pCamera->GetCameraVecDirect( );

						if( pCamera != NULL )
						{
							float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

							D3DXMATRIX mtxRot;
							D3DXMatrixIdentity( &mtxRot );
							D3DXMatrixRotationY( &mtxRot , fAngle );

							D3DXVec3TransformNormal( &vecDirect , &move , &mtxRot );
							D3DXVec3Normalize( &vecDirect , &vecDirect );
						}
						else
						{
							D3DXVec3Normalize( &vecDirect , &move );
						}

						//  移動方向ベクトルの設定
						MagicTornade* pMagicToranade = ( MagicTornade* )m_pMagic;

						if( move.x == 0.0f && move.z == 0.0f )
						{
							vecDirect = m_vecDirect;
						}

						pMagicToranade->SetMoveVecDirect( vecDirect );
					}
				
					/*----------------------------------------------------------
						魔法攻撃発動操作
					----------------------------------------------------------*/
					if( m_nPlayerNo == 0 )
					{
						nKey = DIK_H;
					}
					else if( m_nPlayerNo == 1 )
					{
						nKey = DIK_AT;
					}

#ifdef KEYBOARD_ENABLE

					if( pKeyboard->GetKeyboardRelease( nKey ) )
					{
						if( m_magic == Player::MAGIC_FIRE )
						{
							if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_MAGIC_ACTIVE ) )
							{
								SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

								//  実行状態に
								m_pMagic->SetActive( );
							}
						}
						else if( m_magic == Player::MAGIC_LIGHTNING )
						{
							if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_MAGIC_ACTIVE2 ) )
							{
								SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE2 );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

								//  実行状態に
								m_pMagic->SetActive( );
							}
						}
						else if( m_magic == Player::MAGIC_TORNADE )
						{
							if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_MAGIC_ACTIVE3 ) )
							{
								SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE3 );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

								//  実行状態に
								m_pMagic->SetActive( );
							}
						}
					}

#else

					if( pPS4Input->GetRelease( m_nPlayerNo , PS4Controller::DIJ_R2 ) )
					{
						if( m_magic == Player::MAGIC_FIRE )
						{
							if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_MAGIC_ACTIVE ) )
							{
								SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

								//  実行状態に
								m_pMagic->SetActive( );
							}
						}
						else if( m_magic == Player::MAGIC_LIGHTNING )
						{
							if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_MAGIC_ACTIVE2 ) )
							{
								SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE2 );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

								//  実行状態に
								m_pMagic->SetActive( );
							}
						}
						else if( m_magic == Player::MAGIC_TORNADE )
						{
							if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_MAGIC_ACTIVE3 ) )
							{
								SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE3 );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

								//  エフェクトの停止
								EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

								//  実行状態に
								m_pMagic->SetActive( );
							}
						}
					}

#endif

				}

				break;
			}
			case StateAnimator::MOTION_MAGIC_ACTIVE:
			{
				//  魔法を所持している場合
				if( m_pMagic != NULL )
				{
					//  魔法を所持していない状態に
					m_pMagic = NULL;
				}

				//  アニメーションが終了している場合
				if( GetAnimationFinish( ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}

				break;
			}
			case StateAnimator::MOTION_MAGIC_ACTIVE2:
			{
				//  魔法を所持している場合
				if( m_pMagic != NULL )
				{
					//  魔法を所持していない状態に
					m_pMagic = NULL;
				}

				//  アニメーションが終了している場合
				if( GetAnimationFinish( ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}

				break;
			}
			case StateAnimator::MOTION_MAGIC_ACTIVE3:
			{
				//  魔法を所持している場合
				if( m_pMagic != NULL )
				{
					//  魔法を所持していない状態に
					m_pMagic = NULL;
				}

				//  アニメーションが終了している場合
				if( GetAnimationFinish( ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}

				break;
			}
			case StateAnimator::MOTION_GUARD:
			{
				//  キーが1か2の場合
				if( m_nKey <= 2 )
				{
					//  防御状態に
					m_bGuard = true;
				}

				//  押す時間の初期化
				m_nPressTime = 0;

				//  アニメーションが終了している場合
				if( GetAnimationFinish( ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );

						//  エフェクシアの再生ストップ
						EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_SHIELD ] );
					}
				}

				break;
			}
			case StateAnimator::MOTION_BEND:
			{
				//  エフェクトの再生ストップ
				StopAllEffekseer( );

				//  アニメーションが終了している場合
				if( GetAnimationFinish( ) )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );
					}
				}				

				break;
			}
			case StateAnimator::MOTION_BLOW:
			{
				//  エフェクトの再生ストップ
				StopAllEffekseer( );

				m_nInvisibleTime = 2;

				//  速度の減衰
				m_fBlowPower += ( 0.0f - m_fBlowPower ) * 0.04f;

				//  吹っ飛び力分の移動
				m_position += m_blowVecDirect * m_fBlowPower;
				m_posAt = m_position - m_blowVecDirect * 2.0f;

				D3DXVec3Normalize( &m_vecDirect , &( m_posAt - m_position ) );

				if( m_bAnimationFinish == true || m_fBlowPower < 1.0f )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_PASSIVE ) )
					{
						SetAnimation( StateAnimator::MOTION_PASSIVE );
					}
				}

				break;
			}
			case StateAnimator::MOTION_PASSIVE:
			{
				m_nInvisibleTime = 21;

				//  速度の減衰
				m_fBlowPower += ( 0.0f - m_fBlowPower ) * 0.04f;

				//  吹っ飛び力分の移動
				m_position += m_blowVecDirect * m_fBlowPower;
				m_posAt = m_position - m_blowVecDirect * 2.0f;

				D3DXVec3Normalize( &m_vecDirect , &( m_posAt - m_position ) );

				if( m_bAnimationFinish == true )
				{
					if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_NEUTRAL ) )
					{
						SetAnimation( StateAnimator::MOTION_NEUTRAL );

						//  速度をなくす
						m_fBlowPower = 0.0f;
					}
				}

				break;
			}
		}
	}

	if( m_pStateAnimator != NULL )
	{
		if( m_pStateAnimator->GetMotion( ) != StateAnimator::MOTION_LOCKON_DASH )
		{
			int nKey = 0;

			if( m_nPlayerNo == 0 )
			{
				nKey = DIK_R;
			}
			else if( m_nPlayerNo == 1 )
			{
				nKey = DIK_RETURN;
			}

#ifdef KEYBOARD_ENABLE

			//  ENTERキーまたは、L1ボタンを押した場合
			if( ( pKeyboard->GetKeyboardTrigger( nKey ) ) )
			{
				//  ロックオンの切り替え
				m_bLockOn = !m_bLockOn;
			}

#else

			//  ENTERキーまたは、L1ボタンを押した場合
			if( pPS4Input->GetTrigger( m_nPlayerNo , PS4Controller::DIJ_L1 ) )
			{
				//  ロックオンの切り替え
				m_bLockOn = !m_bLockOn;
			}

#endif

		}
	}

	////////////////////////////////////////////////////////
	//  状態毎の操作終了
	////////////////////////////////////////////////////////

	//  受ける側の当たり判定の代入
	m_hitSphere.position = m_position;
	m_hitSphere.position.y += PLAYER_HIT_SPHERE_POS_Y;

	//  地面についた場合
	if( m_position.y <= pHitField->GetHeight( m_position ) )
	{
		//  ジャンプ数の初期化
		m_nCntJump = 0;
	}

	////////////////////////////////////////////////////////
	//  敵との当たり判定
	////////////////////////////////////////////////////////

	//  モーション情報の取得
	StateAnimator::MOTION motion = m_pStateAnimator->GetMotion( );

	if( m_bJudgeHit )
	{
#pragma omp parallel for
		//  優先度の最大数分のループ
		for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
		{
			//  シーンの先頭アドレスを取得
			pScene2 = Scene::GetScene( nCntPriority );

			//  シーンが空ではない間ループ
			while( pScene2 != NULL )
			{
				Scene::OBJTYPE objType;						//  物体の種類

				//  物体の種類の取得
				objType = pScene2->GetObjType( );

				//  CPU対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
				{
					//  種類が敵の場合
					if( objType == Scene::OBJTYPE_ENEMY )
					{
						//  ダウンキャスト
						Enemy* pEnemy = ( Enemy* )pScene2;

						float fDist = 0.0f;

						if( Utility::HitSphere( m_hitSphere , pEnemy->GetHitSphere( ) , &fDist ) )
						{
							//  速度をなくす
							m_fSpeed = 0.0f;

							//  反射するベクトルを求める
							D3DXVECTOR3 reflectVec = m_hitSphere.position - pEnemy->GetHitSphere( ).position;
							reflectVec.y = 0.0f;

							D3DXVec3Normalize( &reflectVec , &reflectVec );

							//  めり込んだ分座標を戻す
							m_position += reflectVec * fDist;
						}
					}
				}
				//  プレイヤー対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					//  種類がプレイヤーの場合
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  ダウンキャスト
						Player* pPlayer = ( Player* )pScene2;

						//  自分以外のプレイヤーでかつ、当たり判定をする場合
						if( m_nPlayerNo != pPlayer->GetPlayerNo( ) && pPlayer->GetJudgeHit( ) )
						{
							float fDist = 0.0f;

							if( Utility::HitSphere( m_hitSphere , pPlayer->GetHitSphere( ) , &fDist ) )
							{
								//  速度をなくす
								m_fSpeed = 0.0f;

								//  反射するベクトルを求める
								D3DXVECTOR3 reflectVec = m_hitSphere.position - pPlayer->GetHitSphere( ).position;
								reflectVec.y = 0.0f;

								D3DXVec3Normalize( &reflectVec , &reflectVec );

								//  めり込んだ分座標を戻す
								m_position += reflectVec * fDist;
							}
						}
					}
				}

				//  次のポインタを代入
				pScene2 = pScene2->GetNextScene( pScene2 );
			}
		}
	}

	//  壁との当たり判定
	if( m_position.x > Game::GetFieldMax( ).x * 0.999f )
	{
		m_position.x = Game::GetFieldMax( ).x * 0.999f;

		m_fSpeed = 0.0f;
	}
	if( m_position.x < Game::GetFieldMin( ).x * 0.999f )
	{
		m_position.x = Game::GetFieldMin( ).x * 0.999f;

		m_fSpeed = 0.0f;
	}
	if( m_position.z > Game::GetFieldMax( ).z * 0.999f )
	{
		m_position.z = Game::GetFieldMax( ).z * 0.999f;

		m_fSpeed = 0.0f;
	}
	if( m_position.z < Game::GetFieldMin( ).z * 0.999f )
	{
		m_position.z = Game::GetFieldMin( ).z * 0.999f;

		m_fSpeed = 0.0f;
	}

	////////////////////////////////////////////////////////
	//  カメラの処理
	////////////////////////////////////////////////////////

	cameraVecDirect = pCamera->GetCameraVecDirect( );

	if( m_pStateAnimator->GetMotion( ) == StateAnimator::MOTION_FINISHER )
	{
		//  優先度の最大数分のループ
		for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
		{
			//  シーンの先頭アドレスを取得
			pScene = Scene::GetScene( nCntPriority );

			//  シーンが空ではない間ループ
			while( pScene != NULL )
			{
				Scene::OBJTYPE objType;						//  物体の種類

				//  物体の種類の取得
				objType = pScene->GetObjType( );
				
				//  CPU対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
				{
					//  種類が敵の場合
					if( objType == Scene::OBJTYPE_ENEMY )
					{
						D3DXVECTOR3 posEnemy;						//  敵の座標

						//  敵情報の取得
						posEnemy = pScene->GetPos( );

						float fDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
													( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

						//if( fDistance > 10.0f )
						{
							//  カメラの注視点設定
							pCamera->SetCameraPosAt( posEnemy , 0.1f );
						}

						D3DXVECTOR3 workPos;
						D3DXVECTOR3 workVec = posEnemy - m_position;
						workVec.y = 0.0f;

						D3DXVec3Normalize( &workVec , &workVec );

						//  CPU対戦の場合
						if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
						{
							workPos = m_position - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;
						}
						//  プレイヤー対戦の場合
						else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
						{
							workPos = m_position - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
						}

						//  カメラの視点設定
						pCamera->SetCameraPosEye( workPos , 0.1f );

						break;
					}
				}
				//  プレイヤー対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					//  種類がプレイヤーの場合
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  ダウンキャスト
						Player* pPlayer = ( Player* )pScene;

						if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
						{
							D3DXVECTOR3 posEnemy;						//  敵の座標

							//  敵情報の取得
							posEnemy = pScene->GetPos( );

							float fDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
													 ( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

							//if( fDistance > 8.0f )
							{
								//  カメラの注視点設定
								pCamera->SetCameraPosAt( posEnemy , 0.8f );
							}

							D3DXVECTOR3 workPos;
							D3DXVECTOR3 workVec = posEnemy - m_position;
							workVec.y = 0.0f;

							D3DXVec3Normalize( &workVec , &workVec );

							//  CPU対戦の場合
							if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
							{
								workPos = m_position - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
								workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;
							}
							//  プレイヤー対戦の場合
							else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
							{
								workPos = m_position - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
								workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
							}

							//  カメラの視点設定
							pCamera->SetCameraPosEye( workPos , 0.3f );

							break;
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}
	else if( m_bLockOn == false )
	{
		D3DXVECTOR3 workPosEye = m_position;

		//  CPU対戦の場合
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y;
			workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
		}
		//  プレイヤー対戦の場合
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
			workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z2;
		}

		//  カメラの視点設定
		pCamera->SetCameraPosEye( workPosEye , 1.0f );

		//  注視点座標の代入
		m_posAt = m_position + m_vecDirect * 5.0f;

		D3DXVECTOR3 workPosAt = m_position;

		//  CPU対戦の場合
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			workPosAt.y += PLAYER_TO_CAMERA_DISTANCE_Y * 0.7f;
			workPosAt += cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
		}
		//  プレイヤー対戦の場合
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			workPosAt.y += PLAYER_TO_CAMERA_DISTANCE_Y2 * 0.7f;
			workPosAt += cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z2;
		}

		//  カメラの注視点設定
		pCamera->SetCameraPosAt( workPosAt , 1.0f );
	}
	else
	{
		//  優先度の最大数分のループ
		for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
		{
			//  シーンの先頭アドレスを取得
			pScene = Scene::GetScene( nCntPriority );

			//  シーンが空ではない間ループ
			while( pScene != NULL )
			{
				Scene::OBJTYPE objType;						//  物体の種類

				//  物体の種類の取得
				objType = pScene->GetObjType( );
				
				//  CPU対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
				{
					//  種類が敵の場合
					if( objType == Scene::OBJTYPE_ENEMY )
					{
						D3DXVECTOR3 posEnemy;						//  敵の座標

						//  敵情報の取得
						posEnemy = pScene->GetPos( );

						float fDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
													( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

						//if( fDistance > 10.0f )
						{
							//  カメラの注視点設定
							pCamera->SetCameraPosAt( posEnemy , 0.1f );
						}

						D3DXVECTOR3 workPos;
						D3DXVECTOR3 workVec = posEnemy - m_position;
						workVec.y = 0.0f;

						D3DXVec3Normalize( &workVec , &workVec );

						//  CPU対戦の場合
						if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
						{
							workPos = m_position - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;
						}
						//  プレイヤー対戦の場合
						else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
						{
							workPos = m_position - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
						}

						//  カメラの視点設定
						pCamera->SetCameraPosEye( workPos , 0.1f );

						break;
					}
				}
				//  プレイヤー対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					//  種類がプレイヤーの場合
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  ダウンキャスト
						Player* pPlayer = ( Player* )pScene;

						if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
						{
							D3DXVECTOR3 posEnemy;						//  敵の座標

							//  敵情報の取得
							posEnemy = pScene->GetPos( );

							float fDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
													 ( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

							//if( fDistance > 8.0f )
							{
								//  カメラの注視点設定
								pCamera->SetCameraPosAt( posEnemy , 0.05f );
							}

							D3DXVECTOR3 workPos;
							D3DXVECTOR3 workVec = posEnemy - m_position;
							workVec.y = 0.0f;

							D3DXVec3Normalize( &workVec , &workVec );

							//  CPU対戦の場合
							if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
							{
								workPos = m_position - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
								workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;
							}
							//  プレイヤー対戦の場合
							else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
							{
								workPos = m_position - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
								workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
							}

							//  カメラの視点設定
							pCamera->SetCameraPosEye( workPos , 0.1f );

							break;
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}

	////////////////////////////////////////////////////////
	//  共通項目( 後ろ )開始
	////////////////////////////////////////////////////////

	//  ロックオン状態ではない場合
	if( m_bLockOn == false )
	{
		//  カメラ注視点切り替え処理
		if( pPS4Input->GetRightStickDisposition( m_nPlayerNo ).x > 300 ||
			pPS4Input->GetRightStickDisposition( m_nPlayerNo ).x < -300 ||
			pKeyboard->GetKeyboardPress( DIK_LEFTARROW ) || pKeyboard->GetKeyboardPress( DIK_RIGHTARROW ) )
		{
			if( pPS4Input->GetRightStickDisposition( m_nPlayerNo ).x < 0 ||
				pKeyboard->GetKeyboardPress( DIK_LEFTARROW ) )
			{
				D3DXVECTOR3 cameraPosAt = pCamera->GetCameraBasePosAt( );
				D3DXVECTOR3 cameraPosEye = pCamera->GetCameraBasePosEye( );
				D3DXMATRIX mtxRot;
				float fAngle = atan2f( cameraPosAt.z - cameraPosEye.z , cameraPosAt.x - cameraPosEye.x );
				float fLength = sqrtf( ( cameraPosAt.z - cameraPosEye.z ) * ( cameraPosAt.z - cameraPosEye.z ) +
										( cameraPosAt.x - cameraPosEye.x ) * ( cameraPosAt.x - cameraPosEye.x ) );

				fAngle += CAMERA_ROTATE_ANGLE;

				cameraPosAt.x = cosf( fAngle ) * fLength + cameraPosEye.x;
				cameraPosAt.z = sinf( fAngle ) * fLength + cameraPosEye.z;

				//  カメラの注視点設定
				pCamera->SetCameraPosAt( cameraPosAt , 1.0f );
			}
			else if( pPS4Input->GetRightStickDisposition( m_nPlayerNo ).x > 0 ||
						pKeyboard->GetKeyboardPress( DIK_RIGHTARROW ) )
			{
				D3DXVECTOR3 cameraPosAt = pCamera->GetCameraBasePosAt( );
				D3DXVECTOR3 cameraPosEye = pCamera->GetCameraBasePosEye( );
				D3DXMATRIX mtxRot;
				float fAngle = atan2f( cameraPosAt.z - cameraPosEye.z , cameraPosAt.x - cameraPosEye.x );
				float fLength = sqrtf( ( cameraPosAt.z - cameraPosEye.z ) * ( cameraPosAt.z - cameraPosEye.z ) +
										( cameraPosAt.x - cameraPosEye.x ) * ( cameraPosAt.x - cameraPosEye.x ) );

				fAngle -= CAMERA_ROTATE_ANGLE;

				cameraPosAt.x = cosf( fAngle ) * fLength + cameraPosEye.x;
				cameraPosAt.z = sinf( fAngle ) * fLength + cameraPosEye.z;

				//  カメラの注視点設定
				pCamera->SetCameraPosAt( cameraPosAt , 1.0f );
			}
		}
	}

	////////////////////////////////////////////////////////
	//  共通項目( 後ろ )終了
	////////////////////////////////////////////////////////

	//if( m_pMeshTracing != nullptr )
	//{
	//	D3DXVECTOR3 positionUp = m_position;
	//	positionUp.y += PLAYER_HEIGHT;
	//	D3DXVECTOR3 positionDown = m_position;

	//	m_pMeshTracing->SetNewVertexPositionUp( positionUp );
	//	m_pMeshTracing->SetNewVertexPositionDown( positionDown );
	//}

	//  シーンモデル
	SceneModelAnim::Update( );

	//  体力が上限を超えた場合
	if( m_nLife > BASE_LIFE )
	{
		m_nLife = BASE_LIFE;
	}

	//  体力ポインタがある場合
	if( m_pLife != NULL )
	{
		//  体力の設定
		m_pLife->SetLife( m_nLife );
	}

	//  無敵時間の場合
	if( m_nInvisibleTime > 0 )
	{
		//  無敵時間のカウント
		m_nInvisibleTime--;

		m_color.a = 0.5f;
	}
	else
	{
		m_color.a = 1.0f;
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーの描画処理
//--------------------------------------------------------------------------------------
void Player::Draw( void )
{
	//  シーンモデル
	SceneModelAnim::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Player* Player::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale , float fSpeed , MAGIC magic , int nPlayerNo )
{
	Player* pPlayer;

	//  インスタンス生成
	pPlayer = new Player;

	//  モデルの種類の代入
	pPlayer->m_type = SceneModelAnim::TYPE_PLAYER;

	//  座標の代入
	pPlayer->m_position = position;

	//  最初の座標の代入
	pPlayer->m_firstPos = position;

	//  注視点座標の代入
	pPlayer->m_posAt = position;

	//  回転角の代入
	pPlayer->m_rot = rot;

	//  大きさ倍率の代入
	pPlayer->m_scale = scale;

	//  速度の代入
	pPlayer->m_fBaseSpeed = fSpeed;

	//  所持魔法の代入
	pPlayer->m_magic = magic;

	//  プレイヤー番号の代入
	pPlayer->m_nPlayerNo = nPlayerNo;

	//  初期化
	pPlayer->Init( );

	return pPlayer;
}

//--------------------------------------------------------------------------------------
//  プレイヤーのダメージ処理
//--------------------------------------------------------------------------------------
void Player::Damage( D3DXVECTOR3 blowVecDirect , float fBlowPower , int nDamage , bool bBlow )
{
	if( m_nInvisibleTime != 0 )
	{
		return;
	}

	//  吹っ飛び方向と力の代入
	m_blowVecDirect = blowVecDirect;
	m_fBlowPower = fBlowPower;

	//  ダメージ処理
	m_nLife -= nDamage;

	//  無敵時間の設定
	m_nInvisibleTime = 60;

	//  乱数の宣言
	std::random_device rd;

	//  乱数の設定
	std::uniform_int_distribution< int > RandomSeekPos( 5 , 20 );

	float fSeekPosX = ( float )RandomSeekPos( rd ) * 0.1f;
	float fSeekPosZ = ( float )RandomSeekPos( rd ) * 0.1f;

	//  カメラの取得
	Camera* pCamera = Game::GetCamera( m_nPlayerNo );

	if( pCamera != NULL )
	{
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.0000006f;

		//  縦揺れ分の代入
		pCamera->SetSwayVertical( fSwayVertical );
	}

	if( m_pPlayerUI != NULL )
	{
		float fSwaySide = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00003f;
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00001f;

		if( m_nPlayerNo == 0 )
		{
			m_pPlayerUI->SetSwaySide( fSwaySide );
		}
		else if( m_nPlayerNo == 1 )
		{
			m_pPlayerUI->SetSwaySide( -fSwaySide );
		}
		
		m_pPlayerUI->SetSwayVertical( fSwayVertical );
	}

	if( m_pLife != NULL )
	{
		float fSwaySide = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00003f;
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00001f;

		m_pLife->SetSwaySide( fSwaySide );
		m_pLife->SetSwayVertical( fSwayVertical );
	}

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x + fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z + fSeekPosZ ) ,
							   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x - fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z - fSeekPosZ ) ,
							   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  エフェクトの停止
	EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

	//  エフェクトの停止
	EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

	//  魔法を所持している場合
	if( m_pMagic != NULL )
	{
		//  魔法の削除
		m_pMagic->Delete( );
		m_pMagic = NULL;
	}

	//  体力が0以下になった場合
	if( m_nLife <= 0 )
	{
		m_nLife = 0;

		m_pLife->SetLife( m_nLife );

		//  自分自身の削除
		Release( );

		//  CPU対戦の場合
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			//  ゲームオーバー画面へ
			Result::SetResult( Result::GAME_OVER );
		}
		//  プレイヤー対戦の場合
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			if( m_nPlayerNo == 0 )
			{
				//  プレイヤー2WIN画面へ
				Result::SetResult( Result::GAME_PLAYER2_WIN );
			}
			else if( m_nPlayerNo == 1 )
			{
				//  プレイヤー1WIN画面へ
				Result::SetResult( Result::GAME_PLAYER1_WIN );
			}
		}		

		//  結果画面への移行フラグをたてる
		Game::SetNextMode( Mode::MODE_RESULT );	
	}
	else
	{
		if( bBlow == true )
		{
			if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_BLOW ) )
			{
				SetAnimation( StateAnimator::MOTION_BLOW );
			}
		}
		else
		{
			if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_BLOW ) )
			{
				SetAnimation( StateAnimator::MOTION_BLOW );
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーのブレイブダメージ処理
//--------------------------------------------------------------------------------------
int Player::BraveDamage( int nDamage )
{
	if( m_nInvisibleTime != 0 )
	{
		return 0;
	}

	int nRealDamage = nDamage;	//  実際のダメージ格納
	int nOverDamage = 0;		//  オーバーした分のダメージ格納

	//  カメラの取得
	Camera* pCamera = Game::GetCamera( m_nPlayerNo );

	if( pCamera != NULL )
	{
		float fSwayVertical = ( float )nDamage * ( float )nDamage * 0.0006f;

		//  縦揺れ分の代入
		pCamera->SetSwayVertical( fSwayVertical );
	}

	//  ブレイブダメージ処理
	m_nBrave -= nDamage;

	//  乱数の宣言
	std::random_device rd;

	//  乱数の設定
	std::uniform_int_distribution< int > RandomSeekPos( 5 , 20 );

	float fSeekPosX = ( float )RandomSeekPos( rd ) * 0.1f;
	float fSeekPosZ = ( float )RandomSeekPos( rd ) * 0.1f;

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x + fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z + fSeekPosZ ) ,
							   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x - fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z - fSeekPosZ ) ,
							   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  体力が0以下になった場合
	if( m_nBrave <= 0 )
	{
		//  オーバーした分の代入
		nOverDamage = abs( 0 - m_nBrave );

		m_nBrave = 0;
	}

	//  オーバーした分を実際のダメージから引く
	nRealDamage -= nOverDamage;

	return nRealDamage;
}

//--------------------------------------------------------------------------------------
//  プレイヤー番号の取得処理
//--------------------------------------------------------------------------------------
int	Player::GetPlayerNo( void )
{
	return m_nPlayerNo;
}

//--------------------------------------------------------------------------------------
//  プレイヤーが当たり判定をするかどうかの取得処理
//--------------------------------------------------------------------------------------
bool Player::GetJudgeHit( void )
{
	return m_bJudgeHit;
}

//--------------------------------------------------------------------------------------
//  プレイヤーが防御しているかどうかの取得処理
//--------------------------------------------------------------------------------------
bool Player::GetGuard( void )
{
	return m_bGuard;
}

//--------------------------------------------------------------------------------------
//  プレイヤーをのけぞり状態にする処理
//--------------------------------------------------------------------------------------
void Player::Bend( void )
{
	if( m_pStateAnimator->SetMotion( StateAnimator::MOTION_BEND ) )
	{
		SetAnimation( StateAnimator::MOTION_BEND );
	}	
}

//--------------------------------------------------------------------------------------
//  プレイヤー全てのエフェクシアの再生ストップ処理
//--------------------------------------------------------------------------------------
void Player::StopAllEffekseer( void )
{
	for( int i = 0; i < EffekseerManager::TYPE_MAX; i++ )
	{
		if( m_handle[ i ] != -1 )
		{
			EffekseerManager::Stop( m_handle[ i ] );
		}
	}
}