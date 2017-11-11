﻿//--------------------------------------------------------------------------------------
//  キャラクターAIクラス   ( characterAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _CHARACTER_AI_H_
#define _CHARACTER_AI_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  キャラクターAI階層クラスの定義
//--------------------------------------------------------------------------------------
class HerarchyCharacterAI
{
public:
	HerarchyCharacterAI( ){ };							//  コンストラクタ

private:

};

//--------------------------------------------------------------------------------------
//  キャラクターAIクラスの定義
//--------------------------------------------------------------------------------------
class CharacterAI
{
public: 
	CharacterAI( );									//  コンストラクタ

private:
	HerarchyCharacterAI*	m_pHerarchy;				//  AI階層クラスのポインタ
};

#endif
