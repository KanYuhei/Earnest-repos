﻿//--------------------------------------------------------------------------------------
//  数字   ( number.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include "number.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define NUMBER_TEXTURE_NAME000			"data/TEXTURE/UI/number.png"			//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  2DLポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Number::Number( )
{
	//  情報の代入
	m_position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_size = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	m_posUV = D3DXVECTOR2( 0.0f , 0.0f );
	m_divideUV = D3DXVECTOR2( 0.0f , 0.0f );
	m_nNumber = 0;
}

//--------------------------------------------------------------------------------------
//  2DLポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Number::Number( D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV , D3DXCOLOR color )
{
	//  情報の代入
	m_position = position;
	m_size = size;
	m_color = color;
	m_posUV = posUV;
	m_divideUV = divideUV;
	m_nNumber = 0;
}

//--------------------------------------------------------------------------------------
//  2DLポリゴンクラスのデストラクタ
//--------------------------------------------------------------------------------------
Number::~Number( )
{

}

//--------------------------------------------------------------------------------------
//  2DLポリゴンの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Number::Init( void )
{
	Sence2DL::Init( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  2DLポリゴンの終了処理
//--------------------------------------------------------------------------------------
void Number::Uninit( void )
{
	Sence2DL::Uninit( );
}

//--------------------------------------------------------------------------------------
//  2DLポリゴンの更新処理
//--------------------------------------------------------------------------------------
void Number::Update( void )
{
	m_posUV.x = m_nNumber * 0.2f;

	if( m_nNumber >= 5 )
	{
		m_posUV.y = 0.5f;
	}
	else
	{
		m_posUV.y = 0.0f;
	}

	Sence2DL::Update( );
}

//--------------------------------------------------------------------------------------
//  2DLポリゴンの描画処理
//--------------------------------------------------------------------------------------
void Number::Draw( void )
{
	Sence2DL::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Number* Number::Create( D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV , int nNumber )
{
	Number* pNumber;

	//  インスタンス生成
	pNumber = new Number;

	//  座標の代入
	pNumber->m_position = position;

	//  大きさの代入
	pNumber->m_size = size;

	//  UV座標開始地点の代入
	pNumber->m_posUV = posUV;

	//  テクスチャUV分割数の代入
	pNumber->m_divideUV.x = 5;
	pNumber->m_divideUV.y = 2;

	//  体力値の代入
	pNumber->m_nNumber = nNumber;

	//  テクスチャ名の設定
	pNumber->SetTextureName( NUMBER_TEXTURE_NAME000 );

	//  初期化
	pNumber->Init( );

	return pNumber;
}

//--------------------------------------------------------------------------------------
//  体力の設定をする関数
//--------------------------------------------------------------------------------------
void Number::SetNumber( int nNumber )
{
	m_nNumber = nNumber;
}