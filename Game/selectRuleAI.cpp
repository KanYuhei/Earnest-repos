﻿//--------------------------------------------------------------------------------------
//  ビヘイビアツリーの選択ルール   ( selectRuleAI.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "selectRuleAI.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------------
SelectRuleAI::SelectRuleAI( )
{
	m_bActive = true;
}

//--------------------------------------------------------------------------------------
//  アクティブ状態に
//--------------------------------------------------------------------------------------
void SelectRuleAI::EnableActive( )
{
	m_bActive = true;
}

//--------------------------------------------------------------------------------------
//  非アクティブ状態に
//--------------------------------------------------------------------------------------
void SelectRuleAI::DisableActive( )
{
	m_bActive = false;
}