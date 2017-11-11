﻿//--------------------------------------------------------------------------------------
//  リザルトプログラム   ( result.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "mode.h"
#include "playerResult.h"
#include "camera.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class SceneBG;

//--------------------------------------------------------------------------------------
//  タイトルクラスの定義
//--------------------------------------------------------------------------------------
class Result : public Mode
{
public: 
	typedef enum
	{
		GAME_CLEAR = 0 ,
		GAME_OVER ,
		GAME_PLAYER1_WIN ,
		GAME_PLAYER2_WIN ,
	} RESULT;

	Result( );											//  コンストラクタ
	~Result( );										//  デストラクタ

	void			Init( void );						//  初期化						
	void			Uninit( void );						//  終了
	void			Update( void );						//  更新
	void			Draw( void );						//  描画

	static void		SetResult( RESULT result );			//  結果の設定

private:
	Camera*		m_pCamera;							//  カメラ
	PlayerResult*	m_pPlayer[ 2 ];						//  プレイヤー
	static RESULT	m_result;							//  結果
	static bool		m_bPush;							//  押したかどうか
};

#endif
