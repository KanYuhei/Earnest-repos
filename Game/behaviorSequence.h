﻿//--------------------------------------------------------------------------------------
//  ビヘイビアツリーの選択ルール( シークエンス )   ( behaviorSequence.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _BEHAVIOR_SEQUENCE_H_
#define _BEHAVIOR_SEQUENCE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "selectRuleAI.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ビヘイビアツリーの選択ルール( シークエンス )クラスの定義
//--------------------------------------------------------------------------------------
class BehaviorSequence : SelectRuleAI
{
public: 
	BehaviorSequence( );								//  コンストラクタ

	void		Init( void ) override;					//  初期化						
	void		Uninit( void ) override;				//  終了
	void		Update( void ) override;				//  更新

private:

};

#endif
