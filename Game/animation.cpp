﻿//--------------------------------------------------------------------------------------
//  アニメーション   ( animation.cpp )
//
//  Author : SHUN YAMASHITA with YUHEI KAN
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "animation.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  アニメーションクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Animation::Animation( )
{
	m_nNumKey = 0;
	m_nLoop = 0;
}

//--------------------------------------------------------------------------------------
//  アニメーションクラスのデストラクタ
//--------------------------------------------------------------------------------------
Animation::~Animation( )
{

}

//--------------------------------------------------------------------------------------
//  アニメーションクラスのキーフレーム情報の設定
//--------------------------------------------------------------------------------------
void Animation::SetKeyFrame( int nKey , KEY_FRAME keyFrame )
{
	//  キーフレームの代入
	m_keyFrame[ nKey ] = keyFrame;
}

//--------------------------------------------------------------------------------------
//  アニメーションクラスのキーフレーム情報の取得
//--------------------------------------------------------------------------------------
Animation::KEY_FRAME Animation::GetKeyFrame( int nKey )
{
	return m_keyFrame[ nKey ];
}

//--------------------------------------------------------------------------------------
//  アニメーションクラスのキー総数を取得
//--------------------------------------------------------------------------------------
int Animation::GetNumKey( void )
{
	return m_nNumKey;
}

//--------------------------------------------------------------------------------------
//  アニメーションクラスのループ情報を取得
//--------------------------------------------------------------------------------------
int Animation::GetLoop( void )
{
	return m_nLoop;
}

//--------------------------------------------------------------------------------------
//  アニメーションクラスの生成
//--------------------------------------------------------------------------------------
Animation*	Animation::Create( int nNumKey ,
								int nLoop )
{
	Animation* pAnimation = new Animation;

	//  キー総数の代入
	pAnimation->m_nNumKey = nNumKey;

	//  ループ数の代入
	pAnimation->m_nLoop = nLoop;

	return pAnimation;
}