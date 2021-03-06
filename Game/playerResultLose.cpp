//--------------------------------------------------------------------------------------
//  プレイヤーステート( リザルト時の敗北 )   ( playerResultLose.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerResultLose.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerResultLose::Init( void )
{
	//  親の初期化
	PlayerState::Init( );
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerResultLose::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerResultLose::Update( void )
{
	//  カメラ制御
	ControlCamera( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerResultLose::Draw( void )
{

}