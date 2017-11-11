﻿//--------------------------------------------------------------------------------------
//  チームロゴプログラム   ( title.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "teamLogo.h"
#include "manager.h"
#include "sceneBG.h"
#include "keyboard.h"
#include "xboxController.h"
#include "fade.h"
#include "renderer.h"
#include "effekseerManager.h"

//--------------------------------------------------------------------------------------
//  チームロゴクラスのコンストラクタ
//--------------------------------------------------------------------------------------
TeamLogo::TeamLogo( )
{

}

//--------------------------------------------------------------------------------------
//  チームロゴクラスのデストラクタ
//--------------------------------------------------------------------------------------
TeamLogo::~TeamLogo( )
{

}

//--------------------------------------------------------------------------------------
//  チームロゴクラスの初期化処理
//--------------------------------------------------------------------------------------
void TeamLogo::Init( void )
{
	m_nTime = TeamLogo::FADE_TIME;

	//  背景
	SenceBG::Create( SenceBG::TYPE_TEAM_LOGO , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
					  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
					  D3DXVECTOR2( 1.0f , 1.0f ) );
}

//--------------------------------------------------------------------------------------
//  チームロゴクラスの終了処理
//--------------------------------------------------------------------------------------
void TeamLogo::Uninit( void )
{
	//  オブジェクトクラスの全解放
	Sence::ReleaseAll( );
}

//--------------------------------------------------------------------------------------
//  チームロゴクラスの更新処理
//--------------------------------------------------------------------------------------
void TeamLogo::Update( void )
{
	if( Fade::GetFade( ) == Fade::FADE_NONE )
	{
		if( m_nTime > 0 )
		{
			m_nTime--;
		}
		else if( m_nTime == 0 )
		{
			//  フェードの設定
			Fade::SetFade( Fade::FADE_OUT , Mode::MODE_TITLE , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
		}
	}

	//  レンダラークラスの取得
	Renderer* pRenderer = SceneManager::GetRenderer( );

	//  レンダラークラスポインタが空ではない場合
	if( pRenderer != NULL )
	{
		pRenderer->Update( true );
	}
}

//--------------------------------------------------------------------------------------
//  チームロゴクラスの描画処理
//--------------------------------------------------------------------------------------
void TeamLogo::Draw( void )
{
	Sence::DrawAll( );
}