﻿//--------------------------------------------------------------------------------------
//  タイトルプログラム   ( title.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "mode.h"
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
class Title : public Mode
{
public: 
	Title( );											//  コンストラクタ
	~Title( );											//  デストラクタ

	void			Init( void );						//  初期化						
	void			Uninit( void );						//  終了
	void			Update( void );						//  更新
	void			Draw( void );						//  描画

private:
	Camera*			m_pCamera;
	static bool		m_bPush;
};

#endif
