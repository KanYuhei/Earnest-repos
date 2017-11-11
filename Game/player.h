//--------------------------------------------------------------------------------------
//  プレイヤー  ( player.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModelAnim.h"
#include "effekseerManager.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class MeshTracing;
class StencilShadow;
class StateAnimator;
class PlayerUI;
class Life;
class Shadow;
class Brave;
class Magic;

//--------------------------------------------------------------------------------------
//  プレイヤークラスの定義
//--------------------------------------------------------------------------------------
class Player : public SceneModelAnim
{
public:
	typedef enum
	{
		MAGIC_FIRE = 0 ,
		MAGIC_LIGHTNING ,
		MAGIC_TORNADE ,
	} MAGIC;

	Player( );														//  コンストラクタ
	~Player( );													//  デストラクタ

	HRESULT				Init( void );								//  初期化
	void				Uninit( void );								//  終了
	void				Update( void );								//  更新
	void				Draw( void );								//  描画

	static Player*		Create( D3DXVECTOR3 position ,					//  生成
								D3DXVECTOR3 rot ,
								D3DXVECTOR3 scale ,
								float fSpeed ,
								MAGIC magic , 
								int nPlayerNo = 0 );

	void				Damage( D3DXVECTOR3 blowVecDirect ,
								float fBlowPower = 0.0f ,			
								int nDamage = 250 ,
								bool bBlow = true );				//  ダメージ処理
	void				Bend( void );								//  のけぞり状態にする
	int					BraveDamage( int nDamage = 100 );			//  ブレイブダメージ処理
	int					GetPlayerNo( void );						//  プレイヤー番号の取得
	bool				GetJudgeHit( void );						//  当たり判定をするかどうかの取得
	bool				GetGuard( void );							//  防御しているかどうかの取得
	void				StopAllEffekseer( void );					//  全てのエフェクシアの再生ストップ

private:
	D3DXVECTOR3			m_firstPos;									//  最初の座標
	D3DXVECTOR3			m_beforePos;								//  前回の座標
	float				m_fDistance;								//  距離
	D3DXVECTOR3			m_vecDirect;								//  進行方向
	D3DXVECTOR3			m_blowVecDirect;							//  吹っ飛び進行方向
	float				m_fSpeed;									//  速度
	float				m_fBaseSpeed;								//  速度
	float				m_fCurrentJumpPower;						//  現在のジャンプ力
	float				m_fRotateY;									//  回転Y軸
	float				m_fMovePower;								//  移動力
	float				m_fHomingTime;								//  追尾時間
	float				m_fBlowPower;								//  吹っ飛び力
	float				m_fMoveY;									//  下に移動する力
	int					m_nWaitTime;								//  待ち時間
	int					m_nCntJump;									//  ジャンプ回数カウント用
	int					m_nLife;									//  体力
	int					m_nBrave;									//  ブレイブ
	int					m_nInvisibleTime;							//  無敵時間
	int					m_nPlayerNo;								//  プレイヤー番号
	int					m_nPressTime;								//  プレス時間
	bool				m_bLockOn;									//  ロックオンフラグ
	bool				m_bVoiceStart;								//  開始ボイスを喋ったかどうか
	bool				m_bJudgeHit;								//  当たり判定をするかどうか
	bool				m_bHomingLeft;								//  追尾時左が入力されていたかどうか
	bool				m_bGuard;									//  防御中かどうか
	bool				m_bSlashEffekseer;							//  攻撃エフェクト
	MAGIC				m_magic;									//  魔法

	Shadow*				m_pShadow;									//  影のクラスのポインタ
	Life*				m_pLife;									//  体力クラスのポインタ
	Brave*				m_pBrave;									//  ブレイブクラスのポインタ
	PlayerUI*			m_pPlayerUI;								//  プレイヤーUIクラスのポインタ
	Magic*				m_pMagic;									//  魔法クラスのポインタ
	StateAnimator*		m_pStateAnimator;							//  ステートマシン( アニメーター )クラスのポインタ
	MeshTracing*		m_pMeshTracing;								//  軌跡メッシュクラスのポインタ
	StencilShadow*		m_stencilShadow;							//  ステンシルシャドウクラスのポインタ

	::Effekseer::Handle m_handle[ EffekseerManager::TYPE_MAX ];		//  エフェクシアハンドル

	const int			BASE_LIFE = 3000;							//  体力初期値
	const int			BASE_BRAVE = 1000;							//  ブレイブ初期値
};

#endif

