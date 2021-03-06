﻿//--------------------------------------------------------------------------------------
//  ステートマシン( AI用 )警戒時クラス   ( stateAICaution.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _STATE_AI_CAUTION_H_
#define _STATE_AI_CAUTION_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "stateAIManager.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  感覚( 警戒 )クラスの定義
//--------------------------------------------------------------------------------------
class SenceCaution : Sence
{
public: 
	typedef enum
	{
		STATE_SEARCH_ENEMY = 0 ,
		STATE_ESCAPE ,
	} STATE;

	SenceCaution( );								//  コンストラクタ

private:
	STATE	m_state;								//  状態
};

#endif
